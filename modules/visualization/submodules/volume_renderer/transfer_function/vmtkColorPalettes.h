#ifndef VMTK_COLOR_PALETTES_H
#define VMTK_COLOR_PALETTES_H

#include <iostream>
#include <string.h>

//http://medical.nema.org/dicom/2013/output/chtml/part06/chapter_B.html
/**
 * @brief The vmtkColorPalettes class
 */
class vmtkColorPalettes
{
public:
    void GetRGBTF (int tag, int min, int max, int *elemSize, unsigned char **ptr);
private:
};
#endif

