#include "VEC2RGBAbsoluteColorMap.h"

#include <cmath>

VEC2RGBAbsoluteColorMap::VEC2RGBAbsoluteColorMap()
{

}

VEC2RGBAbsoluteColorMap::~VEC2RGBAbsoluteColorMap()
{

}

void VEC2RGBAbsoluteColorMap::map(float in_comp_a, float in_comp_b, float in_comp_c, float &out_comp_a, float &out_comp_b, float &out_comp_c)
{
    out_comp_a = fabs(in_comp_a);
    out_comp_b = fabs(in_comp_b);
    out_comp_c = fabs(in_comp_c);
}
