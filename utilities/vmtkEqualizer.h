/*  
 *  equaliza.h
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

#ifndef VMTK_EQUALIZER_H
#define VMTK_EQUALIZER_H

class vmtkEqualizer
{
public:
    static void EqualizeHistogram (int dimx, int dimy, int dimz, const unsigned short *volume,
                            int nbitsalloc, unsigned int umax, unsigned short **histogram);

    static void EqualizedLogNormalizedHistogram (int dimx, int dimy, int dimz,
                                          const unsigned short *volume, int nbitsalloc,
                                          unsigned int umax, float **norm_hist, unsigned int *hist_size);

};

#endif

