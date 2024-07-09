#ifndef MODEL_DIFFUSION_UI_H
#define MODEL_DIFFUSION_UI_H

#include "model/ModelSubject.h"
#include <modules/file_reader/dicom/vmtkDiffAcquisition.h>

enum class ModelDiffusionUIEvent
{
    DWI_INDEX_CHANGED,
    DWI_RENDERING_STATE_CHANGED
};

class ModelDiffusionUI: public ModelSubject<ModelDiffusionUI, ModelDiffusionUIEvent>
{
public:
    ModelDiffusionUI(vmtkDiffAcquisition *acq);
    ~ModelDiffusionUI() { }

    void setCurrentDWIIndex(unsigned short index);
    unsigned short getCurrentDWIIndex();
    unsigned short getNumberOfDwiVolumes();
    void setDWIRenderingActivated(bool state);
    bool isDWIRenderingActivated();

    vmtkDiffAcquisition *getAcquisition() const;

private:
    bool m_isDWIRenderingActivated;
    unsigned short m_currentDWIIndex; /**< TODO: describe */
    vmtkDiffAcquisition *m_acq;
    unsigned short m_numDWIVolumes;
};

#endif // MODEL_DIFFUSION_UI_H
