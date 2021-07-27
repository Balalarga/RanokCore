#include "LinearGradientModel.h"


LinearGradientModel::LinearGradientModel(QVector<QColor> colors,
                                         QObject *parent):
    QObject(parent),
    _stops(colors)
{

}

QColor LinearGradientModel::GetColor(unsigned value)
{
    // Find the "bin" that value falls in
    unsigned range = UINT_MAX;
    float step = range / (float)(_stops.size()-1);
    int bin = (int)(value / step);

    // Normalize value in the interval (0,1]
    float normalized_v = (value - bin*step) / step;

    return interpolate(_stops[bin], _stops[bin+1], normalized_v);
}

void LinearGradientModel::SetColors(const QVector<QColor> &colors)
{
    _stops = colors;
}

QColor LinearGradientModel::interpolate(QColor c1, QColor c2,
                                        float normalized_value)
{
    if( normalized_value <= 0.0 ){ return c1; }
    if( normalized_value >= 1.0 ){ return c2; }

    QColor result;
    result.setRedF((1.0-normalized_value)*c1.redF() +
                            normalized_value*c2.redF());
    result.setGreenF((1.0-normalized_value)*c1.greenF() +
                              normalized_value*c2.greenF());
    result.setBlueF((1.0-normalized_value)*c1.blueF() +
                             normalized_value*c2.blueF());
    result.setAlphaF((1.0-normalized_value)*c1.alphaF() +
                             normalized_value*c2.alphaF());
    return result;
}
