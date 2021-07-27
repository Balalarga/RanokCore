#ifndef ISPACECALCULATOR_H
#define ISPACECALCULATOR_H

#include "Color.h"
#include "../SpaceBuilder.h"
#include "Language/Program.h"
#include "LinearGradientModel.h"
#include <functional>


enum class CalculatorMode
{
    Model, Mimage
};

class ISpaceCalculator
{
public:
    ISpaceCalculator(std::function<void(CalculatorMode, int, int)> callback);
    virtual ~ISpaceCalculator(){}

    virtual void CalcModel(SpaceData* space, int start = 0, int end = 0) = 0;
    virtual void CalcMImage(SpaceData* space, int start = 0, int end = 0) = 0;

    void SetCalculatorMode(CalculatorMode mode);
    CalculatorMode GetCalculatorMode();

    Color GetMImageColor(double value);
    void SetMImageColorGradiend(const std::vector<Color>& colors);

    Color GetModelColor();
    void SetModelColor(const Color &newDefaultColor);

    void SetProgram(Program *program);
    Program* GetProgram();

    void SetBatchSize(int size);
    int GetBatchSize();

    void SetDoneCallback(std::function<void(CalculatorMode, int, int)> func);

    void Run();

protected:
    void Complete(int start, int end);


private:
    std::function<void(CalculatorMode, int, int)> _finishFunction;

    CalculatorMode _mode;

    Color _modelColor;
    LinearGradientModel _mimageColorModel;

    Program* _program;

    int _batchSize;
};

#endif // ISPACECALCULATOR_H
