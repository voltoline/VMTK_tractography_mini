/*  
 *  transferFunction.h
 *
 *  Copyright (C) 2013  Wu Shin-Ting, FEEC, Unicamp
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef VMTK_GRAY_SCALE_H
#define VMTK_GRAY_SCALE_H

#include <vector>
/**
 * @brief The vmtkGrayScale class
 */
class vmtkGrayScale
{
public:
    void GetGrayScaleTF (int tag, int min, int max, int *elemSize, unsigned char **ptr);
    void GetGrayScaleTF (int tag, int min, int max, int *elemSize, unsigned char **ptr, unsigned short nbitsalloc);
    /**
     * @brief Build the color map LUT from the range [0,umax] to [0,entries].
     * @param tag type of LUT.
     * @param min center-width/2
     * @param max center+width/2-1
     * @param LUT correpondence between raw data and full-range data.
     * @param nbitsalloc number of bits allocated to voxel raw data.
     * @param entries number of elements in LUT
     * @param ptr pointer to the color map LUT.
     */
    void GetGrayScaleTF (int tag, int min, int max, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr);
    /**
     * @brief Build the color map LUT, piecewise linear, from the range [0,umax] to [0,entries] on the basis of a vector of points.
     * @param tag type of LUT.
     * @param nodes sequence of numb/2 pairs of (x,y), where the first element x is a value in the domain of raw data and the second element y is a value in [0,1.].
     * @param LUT correpondence between raw data and full-range data.
     * @param nbitsalloc number of bits allocated to voxel raw data.
     * @param entries number of elements in LUT.
     * @param ptr pointer to the color map LUT.
     */
    void GetGrayScaleTF (int tag, std::vector<float>& nodes, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr);

    void GetRGBTF (int tag, int min, int max, int *elemSize, unsigned char **ptr);
private:
    void GetGrayScaleTFE (int min, int max, int *elemSize, unsigned char **ptr);
    void GetGrayScaleTFL (int min, int max, int *elemSize, unsigned char **ptr);
    void GetGrayScaleTFS (int min, int max, int *elemSize, unsigned char **ptr);
    void GetGrayScaleTFC (int min, int max, int *elemSize, unsigned char **ptr);
    /**
     * @brief Build color map LUT with 2^{nbitsalloc} with windowing function in the interval [min,max] of the full-range domain.
     * @param min the minimum of the window in the transformed domain.
     * @param max the maximum of the window in the transformed domain.
     * @param elemSize number of entries in LUT.
     * @param ptr pointer to the color map LUT.
     * @param nbitsalloc number of bits allocated to voxel raw data.
     */
    void GetGrayScaleTFD (int min, int max, int *elemSize, unsigned char **ptr, unsigned short nbitsalloc);

    /**
     * @brief Build color map LUT with 2^{nbitsalloc} from the windowing function in the interval [min,max] of the raw data domain.
     * @param min center-width/2.
     * @param max center+width/2-1.
     * @param LUT pointer to the correspondence between raw data and equalized/normalized data.
     * @param nbitsalloc number of bits allocated to voxel raw data.
     * @param entries  number of elements in the color map LUT.
     * @param ptr pointer to the color map LUT.
     */
    void GetGrayScaleTFN (int min, int max, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr);

    /**
     * @brief Build color map LUT with 2^{nbitsalloc} from a vector of points of the raw data domain.
     * @param nodes numb/2 pairs of (x,y), where the ascending x is the value in the domain of the raw data and y is the color factor in the range of [0,1].
     * @param LUT  pointer to the table of the correspondences between raw data and equalized/normalized data.
     * @param nbitsalloc number of bits allocated to voxel raw data.
     * @param entries number of elements in the color map LUT.
     * @param ptr pointer to the color map LUT.
     * @return error code (0: no error).
     */
    unsigned short GetGrayScaleTFNPoints (std::vector<float>& nodes, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr);
};
#endif

