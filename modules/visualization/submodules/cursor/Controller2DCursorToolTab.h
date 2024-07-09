#ifndef CONTROLLER_2D_CURSOR_TOOLTAB_H
#define CONTROLLER_2D_CURSOR_TOOLTAB_H

#include "ModelCursor.h"
#include "core/ToolBase.h"

class Model2DTransformation;

class Controller2DCursorToolTab:
        public ControllerTool
{
public:
    Controller2DCursorToolTab();

    void handleCursorStateChangedEvent(ModelCursor *, ModelCursorEvent event);
    void handleCursorModeChangedEvent(ModelCursor *, ModelCursorEvent event);
    void handleCursorDrawTypeChangeEvent(ModelCursor *, ModelCursorEvent event);

    bool isCursorEnabled();

    bool isCursorActivated();
    void setCursorActivated(bool state);

    QMap<DrawCursorType, QString> getMapDrawCursorType();

    DrawCursorType getDrawCursorType();
    void setDrawCursorType(DrawCursorType newDrawCursorType);

    void setCursorLocked(bool is_locked);

    bool isCursorLocked();

    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
    void setModelCursor(ModelCursor *modelCursor);

private:

    ModelCursor *m_modelCursor;
    int m_cursorStateChangedHandlerSlotID;
    int m_cursorModeChangedHandlerSlotID;
    int m_cursorDrawTypeChangedHandlerSlotID;
};

#endif // CONTROLLER_2D_CURSOR_TOOLTAB_H
