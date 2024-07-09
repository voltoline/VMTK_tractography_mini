#include "ToolTractography.h"

ToolTractography::ToolTractography():
    Tool("ToolTractography")
{
    m_ptrControllerTractography = nullptr;
    m_controllerROIToolTab = nullptr;
}

void ToolTractography::updateController(ControllerDTITractographyToolTab *controller)
{
    m_ptrControllerTractography = controller;
}

void ToolTractography::updateController(ControllerROIToolTab *controller)
{
    m_controllerROIToolTab = controller;
}

void ToolTractography::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateTractographyEnabled();

    if(m_ptrControllerTractography != nullptr && m_ptrControllerTractography->isTractographyEnabled())
        updateTractography();
}

ControllerDTITractographyToolTab *ToolTractography::getControllerTractography() const
{
    return m_ptrControllerTractography;
}

ControllerROIToolTab *ToolTractography::getControllerROI() const
{
    return m_controllerROIToolTab;
}
