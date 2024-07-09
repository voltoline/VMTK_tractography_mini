#ifndef VMTK_REORIENT_IMAGE_VOLUME_TO_LPS_H
#define VMTK_REORIENT_IMAGE_VOLUME_TO_LPS_H

#include "vmtkAcquisition.h"

class vmtkReorientImageVolumeToLPS
{
public:
    static bool reorient(vmtkAcquisition *);

private:
    vmtkReorientImageVolumeToLPS();
};

#endif // VMTK_REORIENT_IMAGE_VOLUME_TO_LPS_H
