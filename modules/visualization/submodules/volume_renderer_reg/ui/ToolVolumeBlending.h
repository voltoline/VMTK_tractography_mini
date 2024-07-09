#ifndef TOOL_VOLUME_BLENDING_H
#define TOOL_VOLUME_BLENDING_H

#include "core/Tool.h"
#include "../controllers/ControllerVolumeBlendingTool.h"

class ToolVolumeBlending: public Tool<ControllerVolumeBlendingTool>
{
public:
    ToolVolumeBlending();
    ~ToolVolumeBlending() { }

    void updateController(ControllerVolumeBlendingTool *controller);
    void updateComponents();

    virtual void updateVolumeBlending() = 0;
    virtual void updateVolumeBlendingEnabled() = 0;

protected:
    ControllerVolumeBlendingTool *getController() const;

private:
    ControllerVolumeBlendingTool *m_ptrController;
};

#endif // TOOL_VOLUME_BLENDING_H
