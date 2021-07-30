#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include <chrono>
using namespace std;

#include "SpaceCalculators.h"
#include "Language/Parser.h"


unique_ptr<ISpaceCalculator> calculator;
ofstream resultFile;
string arg0;

// Utils
void ArgsError()
{
    cout<<"Usage: "<<arg0<<" { Source filepath } "
        <<" { Result filepath } "
        <<" { Cpu|Gpu } "<<" { Model|MImage } "
        <<" { Recursion depth } "
        <<" (Optional){ Batch size pow } "<<endl;
    exit(-1);
}
string CharToLower(char* rawSource)
{
    string source(rawSource);
    std::transform(source.begin(), source.end(), source.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return source;
}


void CompleteFunc(CalculatorMode mode, int start, int batchStart, int end)
{
    SpaceManager& space = SpaceManager::Self();
    if(mode == CalculatorMode::Model)
        space.SaveZoneRange(resultFile, start+batchStart, start+end);
    else
        space.SaveMimageRange(resultFile, start+batchStart, start+end);

    cout<<"Written "<<(start + end)*100.f/space.GetSpaceSize()<<"% points"<<endl;
}

int main(int argc, char** argv)
{
    arg0 = argv[0];
    if(argc != 6 && argc != 7)
          ArgsError();
    string sourcePath = argv[1];
    string resultPath = argv[2];
    string device     = CharToLower(argv[3]);
    string mode       = CharToLower(argv[4]);
    int    depth      = stoi(argv[5]);
    int    batchSize  = 0;
    if(argc == 7)
        batchSize = pow(2, stoi(argv[6]));

    Parser parser;
    if(!parser.SetFile(sourcePath))
    {
        cout<<"Couldn't open source file "<<sourcePath<<"\n";
        return -2;
    }

    unique_ptr<Program> program(parser.GetProgram());

    resultFile.open(resultPath, ios_base::out|ios_base::binary);
    if(!resultFile)
    {
        cout<<"Couldn't create result file: "<<resultPath<<"\n";
        return -2;
    }


    if(device == "cpu")
        calculator.reset(new CommonCalculator(&CompleteFunc));
    else if(device == "gpu")
        calculator.reset(new OpenclCalculator(&CompleteFunc));
    else
        ArgsError();


    if(mode == "model")
        calculator->SetCalculatorMode(CalculatorMode::Model);
    else if(mode == "mimage")
        calculator->SetCalculatorMode(CalculatorMode::Mimage);
    else
        ArgsError();

    calculator->SetProgram(program.get());
    calculator->SetBatchSize(batchSize);

    auto args = program->GetSymbolTable().GetAllArgs();
    SpaceManager::Self().InitSpace(args[0]->limits,
                                   args[1]->limits,
                                   args[2]->limits,
                                   depth);

    SpaceManager::Self().ResetBufferSize(pow(2, 10));
    auto startPoint = SpaceManager::Self().GetStartPoint();
    auto pointSize = SpaceManager::Self().GetPointSize();
    auto spaceUnits = SpaceManager::Self().GetSpaceUnits();
    resultFile << startPoint.x;
    resultFile << startPoint.y;
    resultFile << startPoint.z;
    resultFile << pointSize.x;
    resultFile << pointSize.y;
    resultFile << pointSize.z;
    resultFile << spaceUnits.x;
    resultFile << spaceUnits.y;
    resultFile << spaceUnits.z;


    auto start = std::chrono::system_clock::now();
    calculator->Run();
    auto end = std::chrono::system_clock::now();


    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout<<"Calculating finished in "<<elapsed.count()/1000.f<<" seconds\n";

    resultFile.close();
    return 0;
}
