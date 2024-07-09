#ifndef TOOL_SLICE_ORIENTATION_H
#define TOOL_SLICE_ORIENTATION_H

#include "core/Tool.h"
#include "../controller/ControllerToolSliceOrientation.h"

class ToolSliceOrientation: public Tool<ControllerToolSliceOrientation>
{
public:
    ToolSliceOrientation();
    ~ToolSliceOrientation() { }

    void updateController(ControllerToolSliceOrientation *controller);
    void updateComponents();

    virtual void updateComponentsEnabled() = 0;
    virtual void updateSliceOrientation() = 0;

protected:
    ControllerToolSliceOrientation *getController() const;

private:
    ControllerToolSliceOrientation *m_ptrController;
};

#endif // TOOL_SLICE_ORIENTATION_H
