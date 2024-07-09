/*  
 *  equaliza.cpp: equaliza o histograma para aumentar a escala dinamica
 *                das intensidades
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
#include <cmath>
#include <string.h>
#include "vmtkEqualizer.h"

#include <iostream>

// Sugestão de leitura:
//http://en.wikipedia.org/wiki/Histogram_equalization

void vmtkEqualizer::EqualizeHistogram (int dimx, int dimy, int dimz,
                                       const unsigned short *volume, int nbitsalloc,
                                       unsigned int umax, unsigned short **histogram)
{
    // Computa o histograma do <volume>:
    // frequencia de ocorrência de cada intensidade
    unsigned int i;
    unsigned int L = pow(2,nbitsalloc);
    int *tmp = new int[L];
    int *cdf = new int[L];

    // Initializa os vetores
    memset(tmp, 0x00, L*sizeof(int));

    // Determina a frequencia de ocorrencia de cada intensidade
    int intensidade, iz, iy, ix;
    for (iz = 0; iz < dimz; iz++) {
        for (iy =0; iy < dimy; iy++) {
            for (ix =0; ix < dimx; ix++) {
                intensidade = (int)(volume[iz*dimx*dimy+iy*dimx+ix]);
                tmp[intensidade]++;  // histogram of the original volume data
            }
        }
    }

    int volSize = dimx*dimy*dimz;

    // Determina a funcao de distribuicao acumulada
    // https://pt.wikipedia.org/wiki/Fun%C3%A7%C3%A3o_distribui%C3%A7%C3%A3o_acumulada
    cdf[0] = tmp[0];
    int cdfmin = -1;

    if (cdf[0]) cdfmin = cdf[0];
    for(i = 1; i < L; i++) {
        cdf[i] = cdf[i-1] + tmp[i];
        if (cdfmin == -1 && cdf[i]) cdfmin = cdf[i];
    }

    delete [] tmp;

    // Mapeamento de intensidades
    *histogram = new unsigned short[umax+1];
    memset(*histogram, 0x00, (umax+1)*sizeof(unsigned short));

    for (i=0; i<umax+1; i++) {
        if (cdf[i])
            (*histogram)[i] = (unsigned short)(((1.0*(cdf[i]-cdfmin))/(volSize-cdfmin))*(L-1));
        else
            (*histogram)[i] = 0;
    }

    delete [] cdf;
}

void vmtkEqualizer::EqualizedLogNormalizedHistogram (int dimx, int dimy, int dimz,
                                                     const unsigned short *volume, int nbitsalloc,
                                                     unsigned int umax, float **norm_hist, unsigned int *hist_size)
{
    // Computa o histograma do <volume>:
    // frequencia de ocorrência de cada intensidade
    unsigned int i;
    unsigned int L = pow(2,nbitsalloc);
    int *tmp = new int[L];
    int *cdf = new int[L];

    // Initializa os vetores
    memset(tmp, 0x00, L*sizeof(int));

    // Determina a frequencia de ocorrencia de cada intensidade
    int intensidade, iz, iy, ix;
    for (iz = 0; iz < dimz; iz++) {
        for (iy =0; iy < dimy; iy++) {
            for (ix =0; ix < dimx; ix++) {
                intensidade = (int)(volume[iz*dimx*dimy+iy*dimx+ix]);
                tmp[intensidade]++;  // histogram of the original volume data
            }
        }
    }

    int volSize = dimx*dimy*dimz;

    // Determina a funcao de distribuicao acumulada
    // https://pt.wikipedia.org/wiki/Fun%C3%A7%C3%A3o_distribui%C3%A7%C3%A3o_acumulada
    cdf[0] = tmp[0];
    int cdfmin = -1;

    if (cdf[0]) cdfmin = cdf[0];
    for(i = 1; i < L; i++) {
        cdf[i] = cdf[i-1] + tmp[i];
        if (cdfmin == -1 && cdf[i]) cdfmin = cdf[i];
    }

    // Mapeamento de intensidades
    unsigned int *histogram = new unsigned int[umax];
    memset(histogram, 0x00, umax*sizeof(unsigned int));

    for (i=0; i<umax; i++) {
        histogram[i] = (unsigned int)(((1.0*(cdf[i]-cdfmin))/(volSize-cdfmin))*(L-1));
    }
    delete [] cdf;

    // apply logarithm on histodata
    float *data = new float[L];
    int eq_density;

    memset(data, 0.0, L*sizeof(float));

    for (unsigned int i=0; i <umax; i++)
    {
        eq_density = histogram[i];

        if (tmp[i] > 0)
            data[eq_density] = log( (float) (tmp[i] ));
        else
            data[eq_density] = 0;
    }

    // find the maximum
    static int max2=0;
    for (unsigned int i=0; i < L; i++)
        if (data[i] > max2)
            max2 = data[i];

    // normalize
    for (unsigned int i=0; i < L; i++)
        data[i] = data[i] / (float) max2;

    delete[] tmp;

    *norm_hist = data;
    *hist_size = L;
}

