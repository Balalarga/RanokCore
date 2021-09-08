#ifndef COMMONCALCULATOR_H
#define COMMONCALCULATOR_H

#include "ISpaceCalculator.h"
#include <vector>

class CommonCalculator : public ISpaceCalculator
{
public:
    CommonCalculator(std::function<void (CalculatorMode, int, int)> func);

    void CalcModel(int spaceOffset, int count) override;
    void CalcMImage(int spaceOffset, int count) override;

protected:
    double DeterminantOfMatrix(std::vector<std::vector<double> > &mat, int n);
    void GetCofactor(std::vector<std::vector<double>>& mat,
                     std::vector<std::vector<double>>& temp,
                     int p, int q, int n);
    int GetZone(const std::vector<double>& values);
};

#endif // COMMONCALCULATOR_H
