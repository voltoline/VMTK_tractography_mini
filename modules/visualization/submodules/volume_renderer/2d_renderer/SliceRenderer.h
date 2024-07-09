#ifndef SLICE_RENDERER_H
#define SLICE_RENDERER_H

#include <modules/visualization/submodules/volume_renderer/OpenGLVolumeRenderingResource.h>
#include "BaseSliceRenderer.h"

class SliceRenderer: public BaseSliceRenderer
{
public:
    SliceRenderer(OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource);

    void configureTextureUnits();

private:
    OpenGLVolumeRenderingResource::sPtr m_openGLVolumeRenderingResource;
};

#endif // SLICE_RENDERER_H
