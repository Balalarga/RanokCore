#include "ISpaceCalculator.h"

ISpaceCalculator::ISpaceCalculator(QObject *parent):
    QThread(parent),
    _mode(CalculatorMode::Model),
    _modelColor({255, 255, 255, 20}),
    _program(0),
    _batchSize(9)
{
    QVector<QColor> gradColors;
    gradColors.push_back(QColor(255, 255, 0,   20));
    gradColors.push_back(QColor(0,   255, 162, 20));
    gradColors.push_back(QColor(0,   0,   255, 20));
    gradColors.push_back(QColor(255, 145, 0,   20));
    gradColors.push_back(QColor(214, 0,   255, 20));
    SetMImageColorGradiend(gradColors);

    QThread::setTerminationEnabled(true);
}

void ISpaceCalculator::run()
{
    if(_program)
    {
        auto space = SpaceBuilder::Instance().GetSpace();
        if(!space)
            return;

        auto theRun = [this](SpaceData* space, int start, int end){

            if(_mode == CalculatorMode::Model)
            {
                space->CreateZoneData();
                CalcModel(space, start, end);
            }
            else
            {
                space->CreateMimageData();
                CalcMImage(space, start, end);
            }
        };

        if(_batchSize != 0)
        {
            int start = 0;
            while(start + _batchSize < space->GetSize())
            {
                theRun(space, start, start+_batchSize);
                start += _batchSize;
            }
            theRun(space, start, space->GetSize());
        }
        else
            theRun(space, 0, 0);
        qDebug()<<"Complete";
    }
    else
        qDebug()<<"[ISpaceCalculator] Program is null";
}

void ISpaceCalculator::SetCalculatorMode(CalculatorMode mode)
{
    _mode = mode;
}

CalculatorMode ISpaceCalculator::GetCalculatorMode()
{
    return _mode;
}

QColor ISpaceCalculator::GetMImageColor(double value)
{
    value = (1. + value)/2.;
    unsigned uValue = UINT_MAX*value;
    return _mimageColorModel.GetColor(uValue);
}

void ISpaceCalculator::SetMImageColorGradiend(const QVector<QColor> &colors)
{
    _mimageColorModel.SetColors(colors);
}

QColor ISpaceCalculator::GetModelColor()
{
    return _modelColor;
}

void ISpaceCalculator::SetModelColor(const QColor &newDefaultColor)
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

void ISpaceCalculator::SetBatchSize(int size)
{
    _batchSize = size;
}

int ISpaceCalculator::GetBatchSize()
{
    return _batchSize;
}
