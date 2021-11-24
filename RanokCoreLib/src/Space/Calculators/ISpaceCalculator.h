#ifndef ISPACECALCULATOR_H
#define ISPACECALCULATOR_H

#include <functional>

#include "Color.h"
#include "Space/SpaceManager.h"
#include "Language/Program.h"
#include "LinearGradientModel.h"


enum class CalculatorMode
{
    Model, Mimage
};


class ISpaceCalculator
{
public:
    ISpaceCalculator(std::function<void(CalculatorMode, int, int)> callback);
    virtual ~ISpaceCalculator() = default;

    virtual void CalcModel(int spaceOffset, int count) = 0;
    virtual void CalcMImage(int spaceOffset, int count) = 0;

    static Color GetMImageColor(double value);
    static void SetMImageColorGradiend(const std::vector<Color>& colors);

    static Color GetModelColor();
    static void SetModelColor(const Color &newDefaultColor);

    void SetCalculatorMode(CalculatorMode mode);
    CalculatorMode GetCalculatorMode();

    void SetProgram(Program *program);
    Program* GetProgram();

    void SetDoneCallback(std::function<void(CalculatorMode, int, int)> func);

    void Run();


protected:
    void Complete(int bufferStart, int end);


private:
    Program* _program;
    CalculatorMode _mode;

    std::function<void(CalculatorMode, int, int)> _finishFunction;

    static Color _modelColor;
    static LinearGradientModel _mimageColorModel;
};

#endif // ISPACECALCULATOR_H
