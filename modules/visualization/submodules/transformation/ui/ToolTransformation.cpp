#include "ToolTransformation.h"

ToolTransformation::ToolTransformation():
    Tool("ToolTransformation")
{
    m_ptrControllerToolTransformation = nullptr;
}

void ToolTransformation::updateController(ControllerToolTransformation *controller)
{
    m_ptrControllerToolTransformation = controller;
}

void ToolTransformation::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateTransformationEnabled();

    if(m_ptrControllerToolTransformation != nullptr)
        updateMouseMode();
}

ControllerToolTransformation *ToolTransformation::getController() const
{
    return m_ptrControllerToolTransformation;
}
