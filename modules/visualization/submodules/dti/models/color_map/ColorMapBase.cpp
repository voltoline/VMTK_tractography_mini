#include "ColorMapBase.h"

#include <cmath>

ColorMapBase::ColorMapBase()
{

}

ColorMapBase::~ColorMapBase()
{

}

void ColorMapBase::map(float in_comp_a, float in_comp_b, float in_comp_c, float &out_comp_a, float &out_comp_b, float &out_comp_c)
{
    out_comp_a = in_comp_a;
    out_comp_b = in_comp_b;
    out_comp_c = in_comp_c;
}

float ColorMapBase::mod(float a, float b)
{
    return (a - b * (floor(a / b)));
}
