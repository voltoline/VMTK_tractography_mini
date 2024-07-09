#ifndef TOOL_CURSOR_H
#define TOOL_CURSOR_H

#include "core/Tool.h"
#include "../Controller2DCursorToolTab.h"

class ToolCursor: public Tool<Controller2DCursorToolTab>
{
public:
    ToolCursor();
    ~ToolCursor() { }

    void updateController(Controller2DCursorToolTab *controller);
    void updateComponents();

    virtual void updateComponentsEnabled() = 0;
    virtual void updateCursor() = 0;

    virtual void updateDrawType() = 0;

protected:
    Controller2DCursorToolTab *getController() const;

private:
    Controller2DCursorToolTab *m_ptrControllerCursor;
};

#endif // TOOL_CURSOR_H
