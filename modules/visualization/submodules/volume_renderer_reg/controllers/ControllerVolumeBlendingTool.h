#ifndef CONTROLLER_VOLUME_BLENDING_TOOL_H
#define CONTROLLER_VOLUME_BLENDING_TOOL_H

#include <string>

#include <core/ToolBase.h>

class RenderingSurfaceSet;
class RegisterRaycastingVolumeRenderer3D;
class RegisterSliceRenderer;

class ControllerVolumeBlendingTool:
        public ControllerTool
{
public:
    ControllerVolumeBlendingTool();

    int getBlendingFactor();

    void setBlendingFactor(int value);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setRenderer3D(RegisterRaycastingVolumeRenderer3D *renderer);
    void setRendererAxial(RegisterSliceRenderer *rendererAxial);
    void setRendererCoronal(RegisterSliceRenderer *rendererCoronal);
    void setRendererSagittal(RegisterSliceRenderer *rendererSagittal);

private:

    RegisterRaycastingVolumeRenderer3D *m_renderer3D;
    RegisterSliceRenderer *m_rendererAxial;
    RegisterSliceRenderer *m_rendererCoronal;
    RegisterSliceRenderer *m_rendererSagittal;

    RenderingSurfaceSet *m_renderingSurfaceSet;

    int m_blendingFactor;
};

#endif // CONTROLLER_VOLUME_BLENDING_TOOL_H
