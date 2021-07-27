#ifndef BUFFEREDCALCULATOR_H
#define BUFFEREDCALCULATOR_H

#include "ISpaceCalculator.h"

class BufferedCalculator : public ISpaceCalculator
{
public:
    explicit BufferedCalculator(ISpaceCalculator* baseCalc, QObject *parent = nullptr);

    void CalcModel();
    void CalcMImage();


protected:
    ISpaceCalculator* _baseCalculator;

};

#endif // BUFFEREDCALCULATOR_H
