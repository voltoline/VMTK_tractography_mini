#include "ToolSliceOrientation.h"

ToolSliceOrientation::ToolSliceOrientation():
    Tool("ToolSliceOrientation")
{
    m_ptrController = nullptr;
}

void ToolSliceOrientation::updateController(ControllerToolSliceOrientation *controller)
{
    m_ptrController = controller;
}

void ToolSliceOrientation::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateComponentsEnabled();

    if(m_ptrController == nullptr)
        return;

    if(m_ptrController->isOrientationEnabled())
        updateSliceOrientation();
}

ControllerToolSliceOrientation *ToolSliceOrientation::getController() const
{
    return m_ptrController;
}
