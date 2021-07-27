#ifndef LINEARGRADIENTMODEL_H
#define LINEARGRADIENTMODEL_H

#include <vector>
#include "Color.h"

class LinearGradientModel
{
public:
    LinearGradientModel(std::vector<Color> colors = {});
    Color GetColor(unsigned value);
    void SetColors(const std::vector<Color>& colors);

private:
    std::vector<Color> _stops;
    Color interpolate(Color c1, Color c2, float normalized_value);
};

#endif // LINEARGRADIENTMODEL_H
