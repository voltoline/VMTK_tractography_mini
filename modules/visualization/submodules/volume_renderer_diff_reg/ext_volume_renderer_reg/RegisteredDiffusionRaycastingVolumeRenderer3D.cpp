#include "RegisteredDiffusionRaycastingVolumeRenderer3D.h"

#include <renderer/vmtkManagementShader.h>
#include "DiffusionRegisterRaycastingShader.h"

RegisteredDiffusionRaycastingVolumeRenderer3D::RegisteredDiffusionRaycastingVolumeRenderer3D(unsigned short ref_volume_dim[], unsigned short float_volume_dim[], float ref_scale_factors[], OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource):
    RegisterRaycastingVolumeRenderer3D(ref_volume_dim, float_volume_dim, ref_scale_factors, ref_opengl_volume_rendering_resource, float_opengl_volume_rendering_resource)
{

}

BaseRaycastingShader *RegisteredDiffusionRaycastingVolumeRenderer3D::createRaycastingShader()
{
    DiffusionRegisterRaycastingShader *raycasting_shader = new DiffusionRegisterRaycastingShader();
    raycasting_shader->setReferenceVolumeDim(m_refPhyDimensions);
    raycasting_shader->setReferenceScaleFactors(m_refScaleFactors);
    raycasting_shader->setFloatingVolumeDim(m_floatPhyDimensions);
    raycasting_shader->setReferenceOpenGLVolumeRenderingResource(m_referenceOpenGLVolumeRenderingResource);
    raycasting_shader->setFloatingOpenGLVolumeRenderingResource(m_floatingOpenGLVolumeRenderingResource);

    return raycasting_shader;
}

BaseRaycastingShader *RegisteredDiffusionRaycastingVolumeRenderer3D::createBackRaycastingShader()
{
    std::cerr << "RegisteredDiffusionRaycastingVolumeRenderer3D::createBackRaycastingShader -->> Not implemented!" << std::endl;

    return nullptr;
}

void RegisteredDiffusionRaycastingVolumeRenderer3D::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    if(!isInitialized())
        throw std::runtime_error("RegisteredDiffusionRaycastingVolumeRenderer3D::setEnabledGrayscaleMode -->> Object not initialized!");

    static_cast<DiffusionRegisterRaycastingShader*>(getRaycastingShader())->setEnabledGrayscaleMode(state, use_transfer_function);
}

void RegisteredDiffusionRaycastingVolumeRenderer3D::changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader)
{
    DiffusionRegisterRaycastingShader *raycasting_shader = dynamic_cast<DiffusionRegisterRaycastingShader*>(new_raycasting_shader);

    if(raycasting_shader)
    {
        raycasting_shader->copyData(static_cast<DiffusionRegisterRaycastingShader*>(getRaycastingShader()));
        RegisterRaycastingVolumeRenderer3D::changeRaycastingShader(raycasting_shader);

    } else
        throw std::runtime_error("RegisteredDiffusionRaycastingVolumeRenderer3D::changeRaycastingShader -->> Object is not of the DiffusionRegisterRaycastingShader class!");
}

void RegisteredDiffusionRaycastingVolumeRenderer3D::restoreRaycastingShader()
{
    if(getPreviousRaycastingShader())
    {
        static_cast<DiffusionRegisterRaycastingShader*>(getPreviousRaycastingShader())->copyData(static_cast<DiffusionRegisterRaycastingShader*>(getRaycastingShader()));
        RegisterRaycastingVolumeRenderer3D::restoreRaycastingShader();

    } else
        std::cerr << "RegisteredDiffusionRaycastingVolumeRenderer3D::restoreRaycastingShader -->> Previous raycasting shader is NULL!" << std::endl;
}
