#ifndef VEC_2_RGB_COLOR_MAP_H
#define VEC_2_RGB_COLOR_MAP_H

#include "ColorMapBase.h"

class VEC2RGBAbsoluteColorMap: public ColorMapBase
{
public:
    VEC2RGBAbsoluteColorMap();
    ~VEC2RGBAbsoluteColorMap();

    void map(float in_comp_a, float in_comp_b, float in_comp_c, float &out_comp_a, float &out_comp_b, float &out_comp_c);
};

#endif // VEC_2_RGB_COLOR_MAP_H
