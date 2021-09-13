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
        <<" (Optional){ Buffer size power } "<<endl;
    exit(-1);
}
string CharToLower(char* rawSource)
{
    string source(rawSource);
    std::transform(source.begin(), source.end(), source.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return source;
}


void CompleteFunc(CalculatorMode mode, int batchStart, int count)
{
    SpaceManager& space = SpaceManager::Self();
    if(mode == CalculatorMode::Model)
    {
        for(int i = batchStart; i < count; ++i)
        {
            if(space.GetZone(i) == 0)
                ++space.metadata.zeroCount;
            else if(space.GetZone(i) == 1)
                ++space.metadata.positiveCount;
            else
                ++space.metadata.negativeCount;
        }
        space.SaveZoneRange(resultFile, count);
    }
    else
        space.SaveMimageRange(resultFile, count);

    cout<<"Written "<<(batchStart+count)*100.f/space.GetSpaceSize()<<"% points"<<endl;
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
    if(argc >= 7)
        batchSize = pow(2, stoi(argv[6]));

    Parser parser;
    if(!parser.SetFile(sourcePath))
    {
        cout<<"Couldn't open source file "<<sourcePath<<"\n";
        return -2;
    }

    unique_ptr<Program> program(parser.GetProgram());

    if(device == "cpu")
    {
        calculator.reset(new CommonCalculator(&CompleteFunc));
    }
    else if(device == "gpu")
    {
        calculator.reset(new OpenclCalculator(&CompleteFunc));
    }
    else
        ArgsError();

    if(mode == "model")
    {
        resultPath += ".mbin";
        calculator->SetCalculatorMode(CalculatorMode::Model);
    }
    else if(mode == "mimage")
    {
        resultPath += ".ibin";
        calculator->SetCalculatorMode(CalculatorMode::Mimage);
    }
    else
        ArgsError();

    resultFile.open(resultPath, ios_base::binary);
    if(!resultFile)
    {
        cout<<"Couldn't create result file: "<<resultPath<<"\n";
        return -2;
    }

    calculator->SetProgram(program.get());
    SpaceManager& space = SpaceManager::Self();
    auto args = program->GetSymbolTable().GetAllArgs();
    space.InitSpace(args[0]->limits,
                                   args[1]->limits,
                                   args[2]->limits,
                                   depth);
    space.ResetBufferSize(batchSize);
    auto startPoint = space.GetStartPoint();
    auto pointSize = space.GetPointSize();
    auto spaceUnits = space.GetSpaceUnits();

    space.metadata.commonData.startPointX = startPoint.x;
    space.metadata.commonData.startPointY = startPoint.y;
    space.metadata.commonData.startPointZ = startPoint.z;
    space.metadata.commonData.pointSizeX = pointSize.x;
    space.metadata.commonData.pointSizeY = pointSize.y;
    space.metadata.commonData.pointSizeZ = pointSize.z;
    space.metadata.commonData.spaceUnitsX = spaceUnits.x;
    space.metadata.commonData.spaceUnitsY = spaceUnits.y;
    space.metadata.commonData.spaceUnitsZ = spaceUnits.z;

    resultFile.write((char*)&space.metadata, sizeof(SpaceManager::ModelMetadata));

    auto start = std::chrono::system_clock::now();
    calculator->Run();
    auto end = std::chrono::system_clock::now();

    resultFile.flush();
    resultFile.seekp(0);
    resultFile.write((char*)&space.metadata, sizeof(SpaceManager::ModelMetadata));

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout<<"Calculating finished in "<<elapsed.count()/1000.f<<" seconds\n";

    resultFile.close();
    return 0;
}
