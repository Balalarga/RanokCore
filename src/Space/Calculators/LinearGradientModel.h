#ifndef LINEARGRADIENTMODEL_H
#define LINEARGRADIENTMODEL_H

#include <QObject>
#include <QVector>
#include <QtGui/qcolor.h>

class LinearGradientModel : public QObject
{
    Q_OBJECT
public:
    explicit LinearGradientModel(QVector<QColor> colors = {},
                                 QObject *parent = nullptr);
    QColor GetColor(unsigned value);
    void SetColors(const QVector<QColor>& colors);

private:
    QVector<QColor> _stops;
    QColor interpolate(QColor c1, QColor c2, float normalized_value);
};

#endif // LINEARGRADIENTMODEL_H
