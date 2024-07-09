#ifndef TOOL_DIFFUSION_H
#define TOOL_DIFFUSION_H

#include "core/Tool.h"
#include "../controllers/ControllerDiffusionTool.h"

class ToolDiffusion: public Tool<ControllerDiffusionTool>
{
public:
    ToolDiffusion();
    ~ToolDiffusion() { }

    void updateController(ControllerDiffusionTool *controller);
    void updateComponents();

    virtual void updateDiffusion() = 0;
    virtual void updateDiffusionEnabled() = 0;

protected:
    ControllerDiffusionTool *getController() const;

private:
    ControllerDiffusionTool *m_ptrControllerDiffusion;
};

#endif // TOOL_DIFFUSION_H
