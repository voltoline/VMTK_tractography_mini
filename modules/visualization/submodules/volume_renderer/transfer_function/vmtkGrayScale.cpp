/*
 *  transferFunction.cpp: constrói diferentes funcoes de transferencia 
 *                 que mapeiam os valores escalares da imagem 3D em 
 *                 propriedades opticas (niveis de cinza/cores e 
 *                 e opacidade)
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
#include <iostream>
#include <string.h>
#include "vmtkGrayScale.h"
#include <cmath>

#ifndef SQR
#define SQR(a) ((a) * (a))
#endif
#ifndef CUB
#define CUB(a) ((a) * (a) * (a))
#endif

/// Estimate logarithmic transfer function from image char data values
/// entry 0 is reserved to background intensity 0
void vmtkGrayScale::GetGrayScaleTFE (int min, int max, int *elemSize, unsigned char **ptr) {
  double aux;

  *elemSize = max-min+2;

  *ptr = new unsigned char[*elemSize];
  aux = 1./(max-min+1)*9;

  (*ptr)[0] = 0;
  for (int i = 1 ; i < *elemSize; i++) 
    (*ptr)[i] = (unsigned char)(log(i*aux+1.)*255);
}

/// Estimate linear transfer function from image char data values
void vmtkGrayScale::GetGrayScaleTFL (int min, int max, int *elemSize, unsigned char **ptr) {
  double aux;

  *elemSize = max-min+2;

  *ptr = new unsigned char[*elemSize];
  aux = 1./(max-min+1);

  (*ptr)[0] = 0;
  for (int i = 1 ; i < *elemSize; i++) 
    (*ptr)[i] = (unsigned char)(i*aux*255);
}

void vmtkGrayScale::GetGrayScaleTFS (int min, int max, int *elemSize, unsigned char **ptr) {
  double aux;

  *elemSize = max-min+2;

  *ptr = new unsigned char[*elemSize];
  aux = 1./(max-min+1);

  (*ptr)[0] = 0;
  for (int i = 1 ; i < *elemSize; i++) 
    (*ptr)[i] = (unsigned char)(SQR(i*aux)*255);
}

void vmtkGrayScale::GetGrayScaleTFC (int min, int max, int *elemSize, unsigned char **ptr) {
  double aux;

  *elemSize = max-min+2;

  *ptr = new unsigned char[*elemSize];
  aux = 1./(max-min+1);

  (*ptr)[0] = 0;
  for (int i = 1 ; i < *elemSize; i++) 
    (*ptr)[i] = (unsigned char)(CUB(i*aux)*255);
}

// Constroi uma funcao de transferencia que mapeia em [0,255] os valores
// do intervalo [umin,umax] do dominio [0,2^{nbitsalloc}]
void vmtkGrayScale::GetGrayScaleTFD (int min, int max, int *elemSize, unsigned char **ptr, unsigned short nbitsalloc) {
  double aux;
  int node1, node2;

  *elemSize = 256;

  *ptr = new unsigned char[*elemSize];

  aux = (1.*pow(2,8))/pow(2,nbitsalloc);

  node1 = (int)(min*aux);
  node2 = (int)(max*aux);

  std::cout << "node1 = " << node1 << ", node2 = " << node2 << std::endl;

  for (int i = 0 ; i < node1; i++)
    (*ptr)[i] = 0;
  for (int i = node1 ; i < node2; i++)
    (*ptr)[i] = (unsigned char)((1.0*(i-node1))/(node2-node1)*255);
  for (int i = node2; i < *elemSize; i++)
    (*ptr)[i] = 0;

}

void vmtkGrayScale::GetGrayScaleTFN (int min, int max, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr) {
    double aux, aux_opac;
    int node1, node2;

    //Check allowed (hard-coded) texel elements (Ting, 07/01/2022)
    int elem;
    if (entries < pow(2,14)-1) {
        elem = entries;
    } else {
        elem = pow(2,14);
    }

    *ptr = new unsigned char[elem*4];

    //Get the normalized values
    node1 = LUT[min];
    node2 = LUT[max];

    //Get the correspondent entries in the color map LUT (revised by Ting - 08/01/2022)
    node1 = (int) (node1 *(elem-1.))/(pow(2,nbitsalloc)-1)+1;
    node2 = (int) (node2 *(elem-1.))/(pow(2,nbitsalloc)-1)+1;

    aux = (pow(2,8)-1)/(node2-node1);
    aux_opac = (pow(2,8)-1)/entries;

    for (int i = 0, jj=0 ; i < node1; i++) {
        (*ptr)[i*4] = (*ptr)[i*4+1] = (*ptr)[i*4+2] = 0;
        (*ptr)[i*4+3] = 0;
    }

    int j=0;
    for (int i = node1, j=1; i < node2; i++, j++) {
        (*ptr)[i*4] = (*ptr)[i*4+1] = (*ptr)[i*4+2] = (*ptr)[i*4+3] = (unsigned char)(aux*j);
    }
    for (int i = node2; i < elem; i++) {
        (*ptr)[i*4] = (*ptr)[i*4+1] = (*ptr)[i*4+2] = (*ptr)[i*4+3] = pow(2,8)-1;
    }
}

unsigned short vmtkGrayScale::GetGrayScaleTFNPoints (std::vector<float>& nodes, unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr) {
    //Format of vector nodes: x (entry), y in [0,1.0] of 255.
    double aux;

    *ptr = nullptr;

    int N = nodes.size()/2; //number of pairs

    if (N > entries) return 1;  // Error code 1

    // Verify whether the indices are in ascending order
    std::cout << "vmtkGrayScale::GetGrayScaleTFNPoints: " << std::endl;
    for (int i=0; i < (N-1); i++) {
        std::cout << "(" << nodes[2*i] << "," << nodes[(2*i)+1] << ")";
    }
    std::cout << std::endl;

    for (int i=0; i < (N-1); i++) {
        if ((nodes[2*i] - nodes[(2*i)+2]) > pow(10,-5)) return 1; // Error code 1
    }

    int elemSize = entries*4;

    int *norm_nodes = new int [N];

    *ptr = new unsigned char[elemSize];

    //Get the correspondent entries in the color map LUT
    for (int i = 0; i < N ; i++) {
        norm_nodes[i] = (LUT[(int)(nodes[2*i])] *(entries-1.))/(pow(2,nbitsalloc)-1);
    }

    for (int i = 0 ; i < (int)(norm_nodes[0]); i++) {
        (*ptr)[i*4] = (*ptr)[i*4+1] = (*ptr)[i*4+2] = 0;
        (*ptr)[i*4+3] = 0;
    }

    for (int i=0; i < (N-1); i++) {
        aux = (((nodes[2*(i+1)+1]-nodes[(2*i)+1]))*255.)/((norm_nodes[i+1]-norm_nodes[i]));
        int k=0;
        for (int j=norm_nodes[i]; j < norm_nodes[i+1]; j++,k++) {
            (*ptr)[j*4] = (*ptr)[j*4+1] = (*ptr)[j*4+2] = (*ptr)[j*4+3] = (unsigned char)((aux*k)+((norm_nodes[(2*i)+1])*255));
        }
    }

    (*ptr)[(norm_nodes[N-1]*4)] = (*ptr)[norm_nodes[N-1]*4+1] = (*ptr)[norm_nodes[N-1]*4+2] = (*ptr)[norm_nodes[N-1]*4+3] = (unsigned char)(nodes[(N-1)*2+1]*255);

    for (int i = norm_nodes[N-1]+1; i < entries; i++) {
        (*ptr)[i*4] = (*ptr)[i*4+1] = (*ptr)[i*4+2] = (*ptr)[i*4+3] = 255;
    }

    return 0; //No error
}

void vmtkGrayScale::GetGrayScaleTF (int tag, int min, int max,
  int *elemSize, unsigned char **ptr) {
//  double aux;
 switch (tag) {
 case 0:
   GetGrayScaleTFE (min, max, elemSize, ptr);
   break;
 case 1:
   GetGrayScaleTFL (min, max, elemSize, ptr);
   break;
 case 2:
   GetGrayScaleTFS (min, max, elemSize, ptr);
   break;
 case 3:
   GetGrayScaleTFC (min, max, elemSize, ptr);
   break;
 }
}

void vmtkGrayScale::GetGrayScaleTF (int tag, int min, int max,
  int *elemSize, unsigned char **ptr, unsigned short nbitsalloc) {
//  double aux;

 switch (tag) {
 case 4:
   GetGrayScaleTFD (min, max, elemSize, ptr, nbitsalloc);
   break;
 }
}

void vmtkGrayScale::GetGrayScaleTF (int tag, int min, int max,
  unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr) {
 switch (tag) {
 case 5:
   GetGrayScaleTFN (min, max, LUT, nbitsalloc, entries, ptr);
   break;
 }
}

void vmtkGrayScale::GetGrayScaleTF (int tag, std::vector<float>& nodes,
  unsigned short *LUT, unsigned short nbitsalloc, unsigned short entries, unsigned char **ptr) {
 switch (tag) {
 case 6:
   GetGrayScaleTFNPoints (nodes, LUT, nbitsalloc, entries, ptr);
   break;
 }
}
