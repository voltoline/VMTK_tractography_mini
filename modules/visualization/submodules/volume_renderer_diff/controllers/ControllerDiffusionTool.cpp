#include "ControllerDiffusionTool.h"

#include "../ui/ToolDiffusion.h"

ControllerDiffusionTool::ControllerDiffusionTool():
    ControllerTool("ToolDiffusion")
{
    m_modelDiffusion = nullptr;
}

void ControllerDiffusionTool::update(ModelDiffusionUI *, ModelDiffusionUIEvent event)
{    
    switch(event)
    {
    case ModelDiffusionUIEvent::DWI_RENDERING_STATE_CHANGED:
    case ModelDiffusionUIEvent::DWI_INDEX_CHANGED:
    {
        if(getTool())
            static_cast<ToolDiffusion*>(getTool())->updateDiffusion();

        break;
    }

    default:
        break;
    }
}

void ControllerDiffusionTool::setDWIVolumeNumber(unsigned short dwi_number)
{
    m_modelDiffusion->setCurrentDWIIndex(dwi_number - 1);
    m_modelDiffusion->notify();
}

unsigned short ControllerDiffusionTool::getDWIVolumeNumber()
{
    return m_modelDiffusion->getCurrentDWIIndex() + 1;
}

unsigned short ControllerDiffusionTool::getDWIVolumeNumberMax()
{
    return m_modelDiffusion->getNumberOfDwiVolumes();
}

void ControllerDiffusionTool::setDWIRenderingActivated(bool state)
{
    m_modelDiffusion->setDWIRenderingActivated(state);
    m_modelDiffusion->notify(this);
}

bool ControllerDiffusionTool::isDWIRenderingActivated()
{
    return m_modelDiffusion->isDWIRenderingActivated();
}

bool ControllerDiffusionTool::isDiffusionEnabled()
{
    return true;
}

void ControllerDiffusionTool::setModelDiffusionUI(ModelDiffusionUI *model_diffusion)
{
    m_modelDiffusion = model_diffusion;
    m_modelDiffusion->subscribe(this, ModelDiffusionUIEvent::DWI_INDEX_CHANGED);
    m_modelDiffusion->subscribe(this, ModelDiffusionUIEvent::DWI_RENDERING_STATE_CHANGED);
}
