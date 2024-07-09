#ifndef DIFFUSION_SLICE_RENDERER_H
#define DIFFUSION_SLICE_RENDERER_H

#include "../../volume_renderer/2d_renderer/SliceRenderer.h"

class DiffusionSliceRenderer: public SliceRenderer
{
public:
    DiffusionSliceRenderer(OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource);

    void initialize();

    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);

private:
    bool m_isGrayscale;
};

#endif // DIFFUSION_SLICE_RENDERER_H
