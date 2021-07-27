#include "LinearGradientModel.h"
#include <climits>

LinearGradientModel::LinearGradientModel(std::vector<Color> colors):
    _stops(colors)
{

}

Color LinearGradientModel::GetColor(unsigned value)
{
    // Find the "bin" that value falls in
    unsigned range = UINT_MAX;
    float step = range / (float)(_stops.size()-1);
    int bin = (int)(value / step);

    // Normalize value in the interval (0,1]
    float normalized_v = (value - bin*step) / step;

    return interpolate(_stops[bin], _stops[bin+1], normalized_v);
}

void LinearGradientModel::SetColors(const std::vector<Color> &colors)
{
    _stops = colors;
}

Color LinearGradientModel::interpolate(Color c1, Color c2,
                                        float normalized_value)
{
    if( normalized_value <= 0.0 ){ return c1; }
    if( normalized_value >= 1.0 ){ return c2; }

    Color result;
    result.red = (1.0-normalized_value)*c1.red + normalized_value*c2.red;
    result.green = (1.0-normalized_value)*c1.green + normalized_value*c2.green;
    result.blue = (1.0-normalized_value)*c1.blue + normalized_value*c2.blue;
    result.alpha = (1.0-normalized_value)*c1.alpha + normalized_value*c2.alpha;
    return result;
}
