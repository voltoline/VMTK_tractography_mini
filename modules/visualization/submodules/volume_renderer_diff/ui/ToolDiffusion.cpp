#include "ToolDiffusion.h"

ToolDiffusion::ToolDiffusion():
    Tool("ToolDiffusion")
{
    m_ptrControllerDiffusion = nullptr;
}

void ToolDiffusion::updateController(ControllerDiffusionTool *controller)
{
    m_ptrControllerDiffusion = controller;
}

void ToolDiffusion::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateDiffusionEnabled();

    if(m_ptrControllerDiffusion != nullptr && m_ptrControllerDiffusion->isDiffusionEnabled())
        updateDiffusion();
}

ControllerDiffusionTool *ToolDiffusion::getController() const
{
    return m_ptrControllerDiffusion;
}
