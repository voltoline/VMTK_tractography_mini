#ifndef CONTROLLER_DIFFUSION_TOOL_H
#define CONTROLLER_DIFFUSION_TOOL_H

#include "../models/ModelDiffusionUI.h"
#include <core/ToolBase.h>

class ControllerDiffusionTool:
        public ControllerTool,
        public ModelObserver<ModelDiffusionUI, ModelDiffusionUIEvent>
{
public:
    ControllerDiffusionTool();
    ~ControllerDiffusionTool() {}

    void update(ModelDiffusionUI *subject, ModelDiffusionUIEvent event);

    void setDWIVolumeNumber(unsigned short dwi_number);
    unsigned short getDWIVolumeNumber();
    unsigned short getDWIVolumeNumberMax();

    void setDWIRenderingActivated(bool state);
    bool isDWIRenderingActivated();

    bool isDiffusionEnabled();

    void setModelDiffusionUI(ModelDiffusionUI *model_diffusion);

private:
    ModelDiffusionUI *m_modelDiffusion;
};

#endif // CONTROLLER_DIFFUSION_TOOL_TAB_H
