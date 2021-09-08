#ifndef ISPACECALCULATOR_H
#define ISPACECALCULATOR_H

#include "Color.h"
#include "../SpaceManager.h"
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

    virtual void CalcModel(int spaceOffset, int count) = 0;
    virtual void CalcMImage(int spaceOffset, int count) = 0;

    void SetCalculatorMode(CalculatorMode mode);
    CalculatorMode GetCalculatorMode();

    Color GetMImageColor(double value);
    void SetMImageColorGradiend(const std::vector<Color>& colors);

    Color GetModelColor();
    void SetModelColor(const Color &newDefaultColor);

    void SetProgram(Program *program);
    Program* GetProgram();

    void SetDoneCallback(std::function<void(CalculatorMode, int, int)> func);

    void Run();


protected:
    void Complete(int bufferStart, int end);


private:
    std::function<void(CalculatorMode, int, int)> _finishFunction;

    Color _modelColor;
    Program* _program;
    CalculatorMode _mode;
    LinearGradientModel _mimageColorModel;
};

#endif // ISPACECALCULATOR_H
