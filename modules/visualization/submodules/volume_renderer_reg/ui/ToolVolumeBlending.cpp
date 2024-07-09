#include "ToolVolumeBlending.h"

ToolVolumeBlending::ToolVolumeBlending():
    Tool("ToolVolumeBlending")
{
    m_ptrController = nullptr;
}

void ToolVolumeBlending::updateController(ControllerVolumeBlendingTool *controller)
{
    m_ptrController = controller;
}

void ToolVolumeBlending::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateVolumeBlendingEnabled();

    if(m_ptrController != nullptr/* && m_ptrController->isEnabled()*/)
        updateVolumeBlending();
}

ControllerVolumeBlendingTool *ToolVolumeBlending::getController() const
{
    return m_ptrController;
}
