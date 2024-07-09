#ifndef TOOL_2D_3D_SLICING_H
#define TOOL_2D_3D_SLICING_H

#include "core/Tool.h"
#include "..//controllers/Controller2D3DSlicingTool.h"

class Tool2D3DSlicing: public Tool<Controller2D3DSlicingTool>
{
public:
    Tool2D3DSlicing();
    ~Tool2D3DSlicing() { }

    void updateController(Controller2D3DSlicingTool *controller);
    void updateComponents();

    virtual void updateComponentsEnabled() = 0;
    virtual void updateIntervals() = 0;
    virtual void updateClipping() = 0;

protected:
    Controller2D3DSlicingTool *getController() const;

private:
    Controller2D3DSlicingTool *m_ptrController2D3DSlicing;
};

#endif // TOOL_2D_3D_SLICING_H
