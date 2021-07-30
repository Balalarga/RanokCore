#include "ISpaceCalculator.h"
#include <climits>
using namespace std;

ISpaceCalculator::ISpaceCalculator(std::function<void (CalculatorMode, int, int)> callback):
    _finishFunction(callback),
    _mode(CalculatorMode::Model),
    _modelColor({255, 255, 255, 20}),
    _program(0),
    _batchSize(0)
{
    std::vector<Color> gradColors;
    gradColors.push_back(Color::fromUint(255, 255, 0,   20));
    gradColors.push_back(Color::fromUint(0,   255, 162, 20));
    gradColors.push_back(Color::fromUint(0,   0,   255, 20));
    gradColors.push_back(Color::fromUint(255, 145, 0,   20));
    gradColors.push_back(Color::fromUint(214, 0,   255, 20));
    SetMImageColorGradiend(gradColors);
}

void ISpaceCalculator::Run()
{
    if(_program)
    {
        SpaceManager& space = SpaceManager::Self();

        if(_mode == CalculatorMode::Model)
            space.ActivateBuffer(SpaceManager::BufferType::ZoneBuffer);
        else
            space.ActivateBuffer(SpaceManager::BufferType::MimageBuffer);

        auto theRun = [this, &space](int start, int end){

            if(_mode == CalculatorMode::Model)
                CalcModel(start, end);
            else
                CalcMImage(start, end);
        };

        int start = 0;
        int spaceSize = space.GetSpaceSize();
        int bufferSize = space.GetBufferSize();
        if(bufferSize > spaceSize)
            bufferSize = spaceSize;
        int batchSize = _batchSize;
        if(!_batchSize)
            batchSize = spaceSize;
        else if(batchSize > bufferSize)
            batchSize = bufferSize;

        for(; start < spaceSize; start += bufferSize)
        {
            for(int batchStart = 0; batchStart < bufferSize; batchStart += batchSize)
            {
                if(batchStart + batchSize < bufferSize)
                    theRun(start+batchStart, start+batchStart + batchSize);
                else
                    theRun(start+batchStart, start+bufferSize);
            }
            Complete(0, bufferSize);
        }
    }
    else
        cout<<"[ISpaceCalculator] Program is null"<<endl;
}

void ISpaceCalculator::Complete(int start, int end)
{
    _finishFunction(_mode, start, end);
}

void ISpaceCalculator::SetCalculatorMode(CalculatorMode mode)
{
    _mode = mode;
}

CalculatorMode ISpaceCalculator::GetCalculatorMode()
{
    return _mode;
}

Color ISpaceCalculator::GetMImageColor(double value)
{
    value = (1. + value)/2.;
    unsigned uValue = UINT_MAX*value;
    return _mimageColorModel.GetColor(uValue);
}

void ISpaceCalculator::SetMImageColorGradiend(const std::vector<Color> &colors)
{
    _mimageColorModel.SetColors(colors);
}

Color ISpaceCalculator::GetModelColor()
{
    return _modelColor;
}

void ISpaceCalculator::SetModelColor(const Color &newDefaultColor)
{
    _modelColor = newDefaultColor;
}

void ISpaceCalculator::SetProgram(Program *program)
{
    _program = program;
}

Program *ISpaceCalculator::GetProgram()
{
    return _program;
}

int ISpaceCalculator::GetBatchSize()
{
    return _batchSize;
}

void ISpaceCalculator::SetBatchSize(int size)
{
    _batchSize = size;
}
