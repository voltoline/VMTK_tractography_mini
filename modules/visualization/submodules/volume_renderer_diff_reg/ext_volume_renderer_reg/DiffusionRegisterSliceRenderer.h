#ifndef DIFFUSION_REGISTER_SLICE_RENDERER_H
#define DIFFUSION_REGISTER_SLICE_RENDERER_H

#include "../../volume_renderer_reg/ext_volume_renderer/RegisterSliceRenderer.h"

class DiffusionRegisterSliceRenderer: public RegisterSliceRenderer
{
public:
    DiffusionRegisterSliceRenderer(float ref_scale_factors[4], float float_scale_factors[4], float ref_volume_dim[4], float float_volume_dim[4],
    OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource);

    ~DiffusionRegisterSliceRenderer() { }

    void initialize();

    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);

private:
    bool m_isGrayscaleActivated;
    bool m_useTransferFunction;
};

#endif // DIFFUSION_REGISTER_SLICE_RENDERER_H
