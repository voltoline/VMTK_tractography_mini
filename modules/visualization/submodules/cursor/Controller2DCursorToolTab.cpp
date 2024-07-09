#include "Controller2DCursorToolTab.h"

#include "ui/ToolCursor.h"

Controller2DCursorToolTab::Controller2DCursorToolTab():
    ControllerTool("ToolCursor")
{
    m_cursorStateChangedHandlerSlotID = -1;
    m_cursorModeChangedHandlerSlotID = -1;
    m_cursorDrawTypeChangedHandlerSlotID = -1;

    m_modelCursor = nullptr;
}

void Controller2DCursorToolTab::handleCursorStateChangedEvent(ModelCursor *, ModelCursorEvent event)
{
    if(getTool() == nullptr)
        return;

    if(event == ModelCursorEvent::CURSOR_STATE_CHANGED)
    {
        static_cast<ToolCursor*>(getTool())->updateComponentsEnabled();
        static_cast<ToolCursor*>(getTool())->updateCursor();
    }
}

void Controller2DCursorToolTab::handleCursorModeChangedEvent(ModelCursor *, ModelCursorEvent event)
{
    if(event == ModelCursorEvent::CURSOR_MODE_CHANGED)
        if(getTool())
            static_cast<ToolCursor*>(getTool())->updateCursor();
}

void Controller2DCursorToolTab::handleCursorDrawTypeChangeEvent(ModelCursor *, ModelCursorEvent event){
    if(getTool() == nullptr){ return; }
    if(event == ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED){
        static_cast<ToolCursor*>(getTool())->updateDrawType();
    }
}

bool Controller2DCursorToolTab::isCursorEnabled()
{
    return m_modelCursor->isEnabled();
}

bool Controller2DCursorToolTab::isCursorActivated()
{
    return m_modelCursor->isActivated();
}

void Controller2DCursorToolTab::setCursorActivated(bool state)
{

    ModelCursorEvent event = m_modelCursor->setActivated(state);
    m_modelCursor->notify(event, m_cursorStateChangedHandlerSlotID, m_modelCursor, event);
}

QMap<DrawCursorType, QString> Controller2DCursorToolTab::getMapDrawCursorType()
{
    return m_modelCursor->getMapDrawCursorType();
}

DrawCursorType Controller2DCursorToolTab::getDrawCursorType(){
    return m_modelCursor->getDrawCursorType();
}

void Controller2DCursorToolTab::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    ModelCursorEvent event = m_modelCursor->setDrawCursorType(newDrawCursorType);
    m_modelCursor->notify(event, m_cursorDrawTypeChangedHandlerSlotID, m_modelCursor, event);
}

void Controller2DCursorToolTab::setCursorLocked(bool is_locked)
{
    if(is_locked)
    {
        ModelCursorEvent event = m_modelCursor->setCursorMode(ModelCursor::Mode::LOCK);
        m_modelCursor->notify(event, m_cursorModeChangedHandlerSlotID, m_modelCursor, event);

    } else {

        ModelCursorEvent event = m_modelCursor->setCursorMode(ModelCursor::Mode::MOVE);
        m_modelCursor->notify(event, m_cursorModeChangedHandlerSlotID, m_modelCursor, event);
    }
}

bool Controller2DCursorToolTab::isCursorLocked()
{
    return m_modelCursor->isLocked();
}

void Controller2DCursorToolTab::setModelCursor(ModelCursor *modelCursor)
{
    m_modelCursor = modelCursor;
    m_cursorStateChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_STATE_CHANGED, CREATE_SLOT(this, &Controller2DCursorToolTab::handleCursorStateChangedEvent));
    m_cursorModeChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_MODE_CHANGED, CREATE_SLOT(this, &Controller2DCursorToolTab::handleCursorModeChangedEvent));
    m_cursorDrawTypeChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED, CREATE_SLOT(this, &Controller2DCursorToolTab::handleCursorDrawTypeChangeEvent));
}
