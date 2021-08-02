#include "ISpaceCalculator.h"
#include <climits>
using namespace std;

ISpaceCalculator::ISpaceCalculator(std::function<void (CalculatorMode, int, int, int)> callback):
    _finishFunction(callback),
    _mode(CalculatorMode::Model),
    _modelColor(Color::fromUint(255, 255, 255, 20)),
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

        int spaceSize = space.GetSpaceSize();
        int bufferSize = space.GetBufferSize();
        if(bufferSize > spaceSize)
            bufferSize = spaceSize;
        int batchSize = _batchSize;
        if(!_batchSize)
            batchSize = spaceSize;
        if(batchSize > bufferSize)
            batchSize = bufferSize;

        int buffSize = bufferSize;
        for(int dataOffset = 0; dataOffset < spaceSize;)
        {
            if(dataOffset + buffSize > spaceSize)
                buffSize = spaceSize - dataOffset;
            for(int bufferOffset = 0; bufferOffset < buffSize;)
            {
                int batchEnd = bufferOffset+batchSize;
                if(batchEnd > buffSize)
                    batchEnd = buffSize;

                if(_mode == CalculatorMode::Model)
                    CalcModel(dataOffset, bufferOffset, batchEnd);
                else
                    CalcMImage(dataOffset, bufferOffset, batchEnd);

                Complete(dataOffset, bufferOffset, batchEnd);
                bufferOffset = batchEnd;
            }
            dataOffset += buffSize;
        }

    }
    else
        cout<<"[ISpaceCalculator] Program is null"<<endl;
}

void ISpaceCalculator::Complete(int bufferStart, int start, int end)
{
    _finishFunction(_mode, bufferStart, start, end);
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

void ISpaceCalculator::SetDoneCallback(std::function<void (CalculatorMode, int, int, int)> func)
{

}

int ISpaceCalculator::GetBatchSize()
{
    return _batchSize;
}

void ISpaceCalculator::SetBatchSize(int size)
{
    _batchSize = size;
}
