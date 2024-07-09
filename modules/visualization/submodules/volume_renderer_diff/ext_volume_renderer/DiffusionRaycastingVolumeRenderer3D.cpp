#include "DiffusionRaycastingVolumeRenderer3D.h"

#include "DiffusionRaycastingShader.h"

DiffusionRaycastingVolumeRenderer3D::DiffusionRaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[], OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource):
    RaycastingVolumeRenderer3D(volume_dim, scale_factors, opengl_volume_rendering_resource)
{

}

BaseRaycastingShader *DiffusionRaycastingVolumeRenderer3D::createRaycastingShader()
{
    DiffusionRaycastingShader *raycasting_shader = new DiffusionRaycastingShader();
    raycasting_shader->setVolumeDim(m_volumeDimension);
    raycasting_shader->setScaleFactors(m_scaleFactors);
    raycasting_shader->setOpenGLVolumeRenderingResource(m_openGLVolumeRenderingResource);

    return raycasting_shader;
}

BaseRaycastingShader *DiffusionRaycastingVolumeRenderer3D::createBackRaycastingShader()
{
    std::cerr << "DiffusionRaycastingVolumeRenderer3D::createBackRaycastingShader -->> Not implemented!" << std::endl;

    return nullptr;
}

void DiffusionRaycastingVolumeRenderer3D::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    if(!isInitialized())
        throw std::runtime_error("DiffusionRaycastingVolumeRenderer3D::setEnabledGrayscaleMode -->> Object not initialized!");

    static_cast<DiffusionRaycastingShader*>(getRaycastingShader())->setEnabledGrayscaleMode(state, use_transfer_function);
}

void DiffusionRaycastingVolumeRenderer3D::changeRaycastingShader(BaseRaycastingShader */*new_raycasting_shader*/)
{
    std::cerr << "DiffusionRaycastingVolumeRenderer3D::changeRaycastingShader -->> Not implemented!" << std::endl;
}

void DiffusionRaycastingVolumeRenderer3D::restoreRaycastingShader()
{
    std::cerr << "DiffusionRaycastingVolumeRenderer3D::restoreRaycastingShader -->> Not implemented!" << std::endl;
}
