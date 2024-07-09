#ifndef REGISTER_RAYCASTING_VOLUME_RENDERER3D_H
#define REGISTER_RAYCASTING_VOLUME_RENDERER3D_H

#include "../../volume_renderer/3d_renderer/RaycastingPreRenderShader.h"
#include "RegisterRaycastingShader.h"
#include <modules/visualization/submodules/volume_renderer/OpenGLVolumeRenderingResource.h>
#include <modules/visualization/submodules/volume_renderer/3d_renderer/BaseRaycastingVolumeRenderer3D.h>

class RegisterRaycastingVolumeRenderer3D: public BaseRaycastingVolumeRenderer3D
{
public:
    RegisterRaycastingVolumeRenderer3D(unsigned short ref_volume_dim[], unsigned short float_volume_dim[], float ref_scale_factors[],
                                       OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource);

    void changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader) override;
    void restoreRaycastingShader() override;

    void setBlendingFactor(float blending);

    void setRegistrationMatrixInv(const glm::mat4 &registration_matrix_inv);

    void setReferenceScaleFactors(float ref_scale_factors[]);
    void setReferenceVolumeDim(float ref_volume_dim[]);
    void setFloatingVolumeDim(float float_volume_dim[]);

    void setReferenceOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr referenceOpenGLVolumeRenderingResource);
    void setFloatingOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr floatingOpenGLVolumeRenderingResource);

protected:

    OpenGLVolumeRenderingResource::sPtr m_referenceOpenGLVolumeRenderingResource;
    OpenGLVolumeRenderingResource::sPtr m_floatingOpenGLVolumeRenderingResource;

    float m_refScaleFactors[4];
    float m_refPhyDimensions[4];
    float m_floatPhyDimensions[4];   /**< scale factors of the volume data */

private:
    BaseRaycastingShader* createRaycastingShader() override;
    BaseRaycastingShader* createBackRaycastingShader() override;
};

#endif // REGISTER_RAYCASTING_VOLUME_RENDERER3D_H
