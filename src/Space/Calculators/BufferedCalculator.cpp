#include "BufferedCalculator.h"

BufferedCalculator::BufferedCalculator(ISpaceCalculator *baseCalc, QObject *parent):
    ISpaceCalculator(parent),
    _baseCalculator(baseCalc)
{

}

void BufferedCalculator::CalcModel()
{

}

void BufferedCalculator::CalcMImage()
{

}
