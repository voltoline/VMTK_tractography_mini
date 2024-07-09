#ifndef TOOL_TRANSFORMATION_H
#define TOOL_TRANSFORMATION_H

#include "core/Tool.h"
#include "../controllers/ControllerToolTransformation.h"

class ToolTransformation: public Tool<ControllerToolTransformation>
{
public:
    ToolTransformation();
    ~ToolTransformation() { }

    void updateController(ControllerToolTransformation *controller);
    void updateComponents();

    virtual void updateMouseMode() = 0;
    virtual void updateTransformationEnabled() = 0;

protected:
    ControllerToolTransformation *getController() const;

private:
    ControllerToolTransformation *m_ptrControllerToolTransformation;
};

#endif // TOOL_TRANSFORMATION_H
