#include "ToolCursor.h"

ToolCursor::ToolCursor():
    Tool("ToolCursor")
{
    m_ptrControllerCursor = nullptr;
}

void ToolCursor::updateController(Controller2DCursorToolTab *controller)
{
    m_ptrControllerCursor = controller;
}

void ToolCursor::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateComponentsEnabled();

    if(m_ptrControllerCursor == nullptr)
        return;


    if(m_ptrControllerCursor->isCursorEnabled()){
        updateCursor();
    }
    updateDrawType();
}

Controller2DCursorToolTab *ToolCursor::getController() const
{
    return m_ptrControllerCursor;
}
