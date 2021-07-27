#ifndef ISPACECALCULATOR_H
#define ISPACECALCULATOR_H

#include <QThread>
#include <QtGui/qcolor.h>
#include "../SpaceBuilder.h"
#include "Language/Program.h"
#include "LinearGradientModel.h"


enum class CalculatorMode
{
    Model, Mimage
};

class ISpaceCalculator : public QThread
{
    Q_OBJECT
public:
    explicit ISpaceCalculator(QObject *parent = nullptr);
    virtual ~ISpaceCalculator(){ quit(); }

    virtual void CalcModel(SpaceData* space, int start = 0, int end = 0) = 0;
    virtual void CalcMImage(SpaceData* space, int start = 0, int end = 0) = 0;

    void SetCalculatorMode(CalculatorMode mode);
    CalculatorMode GetCalculatorMode();

    QColor GetMImageColor(double value);
    void SetMImageColorGradiend(const QVector<QColor>& colors);

    QColor GetModelColor();
    void SetModelColor(const QColor &newDefaultColor);

    void SetProgram(Program *program);
    Program* GetProgram();

    void SetBatchSize(int size);
    int GetBatchSize();

signals:
    void ComputedModel(int start, int end);
    void ComputedMimage(int start, int end);


protected:
    void run() override;


private:
    CalculatorMode _mode;

    QColor _modelColor;
    LinearGradientModel _mimageColorModel;

    Program* _program;

    int _batchSize;
};

#endif // ISPACECALCULATOR_H
