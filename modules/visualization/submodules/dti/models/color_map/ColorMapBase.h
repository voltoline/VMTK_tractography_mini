#ifndef COLOR_MAP_BASE_H
#define COLOR_MAP_BASE_H

class ColorMapBase
{
public:
    ColorMapBase();
    virtual ~ColorMapBase();

    virtual void map(float in_comp_a, float in_comp_b, float in_comp_c, float &out_comp_a, float &out_comp_b, float &out_comp_c);

protected:
    float mod(float a, float b);
};

#endif // COLOR_MAP_BASE_H
