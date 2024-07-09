#ifndef REGISTER_SLICE_RENDERER_H
#define REGISTER_SLICE_RENDERER_H

#include "utilities/QCheckingOpenGL.h"
#include <modules/visualization/submodules/volume_renderer/2d_renderer/BaseSliceRenderer.h>
#include <modules/visualization/submodules/volume_renderer/OpenGLVolumeRenderingResource.h>

class RegisterSliceRenderer: public BaseSliceRenderer
{
public:
    RegisterSliceRenderer(float ref_scale_factors[4], float float_scale_factors[4], float ref_volume_dim[4], float float_volume_dim[4],
    OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource);

    void initialize();
    void configureTextureUnits();

    void setBlendingFactor(float blendingFactor);
    void setRegistrationMatrixInv(const glm::mat4 &registrationMatrixInv);

private:
    OpenGLVolumeRenderingResource::sPtr m_referenceOpenGLVolumeRenderingResource;
    OpenGLVolumeRenderingResource::sPtr m_floatingOpenGLVolumeRenderingResource;

    float m_blendingFactor;                   /**< blending weight of co-registered volumes */

    glm::mat4 mRegistrationMatrixInv;

    float m_refScaleFactors[4];
    float m_floatScaleFactors[4];
    float m_refPhyDimensions[4];
    float m_floatPhyDimensions[4];   /**< scale factors of the volume data */
};

#endif // REGISTER_SLICE_RENDERER_H
