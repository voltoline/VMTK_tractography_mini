
#include "vmtkColorPalettes.h"

void vmtkColorPalettes::GetRGBTF (int tag, int min, int max,
  int *elemSize, unsigned char **ptr) {

  // Variavel tag reservado para extensoes futuras

  unsigned char tabelaCor [] = {
    0, 0, 0,
    51, 0, 0,
    102, 0, 0,
    153, 0, 0,
    204, 0, 0,
    255, 0 , 0,
    255, 51, 0,
    255, 102, 0,
    255, 153, 0,
    255, 204, 0,
    255, 255, 0,
    204, 255, 0,
    153, 255, 0,
    102, 255, 0,
    51, 255, 0,
    0, 255, 0,
    0, 255, 51,
    0, 255, 102,
    0, 255, 153,
    0, 255, 204,
    0, 255, 255,
    0, 204, 255,
    0, 153, 255,
    0, 102, 255,
    0, 51, 255,
    0, 0, 255,
    51, 0, 255,
    102, 0, 255,
    153, 0, 255,
    204, 0, 255,
    255, 0, 255,
    255, 51, 255,
    255, 102, 255,
    255, 153, 255,
    255, 204, 244,
    255, 255, 255
  };

  *elemSize = sizeof(tabelaCor);

  *ptr = new unsigned char[*elemSize];

  memcpy (*ptr, tabelaCor, *elemSize);
}

