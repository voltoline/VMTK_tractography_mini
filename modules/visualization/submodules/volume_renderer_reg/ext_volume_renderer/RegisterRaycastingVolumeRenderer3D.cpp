#include "RegisterRaycastingVolumeRenderer3D.h"

RegisterRaycastingVolumeRenderer3D::RegisterRaycastingVolumeRenderer3D(unsigned short ref_volume_dim[], unsigned short float_volume_dim[], float ref_scale_factors[], /* float float_scale_factors[],*/
                                                                       OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource):
    BaseRaycastingVolumeRenderer3D(ref_volume_dim, ref_scale_factors)
{
    m_refScaleFactors[0] = ref_scale_factors[0];
    m_refScaleFactors[1] = ref_scale_factors[1];
    m_refScaleFactors[2] = ref_scale_factors[2];
    m_refScaleFactors[3] = ref_scale_factors[3];

    m_refPhyDimensions[0] = ref_volume_dim[0];
    m_refPhyDimensions[1] = ref_volume_dim[1];
    m_refPhyDimensions[2] = ref_volume_dim[2];
    m_refPhyDimensions[3] = ref_volume_dim[3];

    m_floatPhyDimensions[0] = float_volume_dim[0];
    m_floatPhyDimensions[1] = float_volume_dim[1];
    m_floatPhyDimensions[2] = float_volume_dim[2];
    m_floatPhyDimensions[3] = float_volume_dim[3];

    m_referenceOpenGLVolumeRenderingResource = ref_opengl_volume_rendering_resource;
    m_floatingOpenGLVolumeRenderingResource = float_opengl_volume_rendering_resource;

}

BaseRaycastingShader *RegisterRaycastingVolumeRenderer3D::createRaycastingShader()
{
    RegisterRaycastingShader *raycasting_shader = new RegisterRaycastingShader();
    raycasting_shader->setReferenceVolumeDim(m_refPhyDimensions);
    raycasting_shader->setReferenceScaleFactors(m_refScaleFactors);
    raycasting_shader->setFloatingVolumeDim(m_floatPhyDimensions);
    raycasting_shader->setReferenceOpenGLVolumeRenderingResource(m_referenceOpenGLVolumeRenderingResource);
    raycasting_shader->setFloatingOpenGLVolumeRenderingResource(m_floatingOpenGLVolumeRenderingResource);

    return raycasting_shader;
}

BaseRaycastingShader *RegisterRaycastingVolumeRenderer3D::createBackRaycastingShader()
{
    std::cerr << "RegisterRaycastingVolumeRenderer3D::createBackRaycastingShader -->> Not implemented!" << std::endl;

    return nullptr;
}

void RegisterRaycastingVolumeRenderer3D::setReferenceScaleFactors(float ref_scale_factors[])
{
    if(!isInitialized())
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::setReferenceScaleFactors -->> Object not initialized!");

    m_refScaleFactors[0] = ref_scale_factors[0];
    m_refScaleFactors[1] = ref_scale_factors[1];
    m_refScaleFactors[2] = ref_scale_factors[2];
    m_refScaleFactors[3] = ref_scale_factors[3];

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setReferenceScaleFactors(m_refScaleFactors);

    BaseRaycastingVolumeRenderer3D::setScaleFactors(ref_scale_factors);

}

void RegisterRaycastingVolumeRenderer3D::setReferenceVolumeDim(float ref_volume_dim[])
{
    if(!isInitialized())
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::setReferenceVolumeDim -->> Object not initialized!");

    m_refPhyDimensions[0] = ref_volume_dim[0];
    m_refPhyDimensions[1] = ref_volume_dim[1];
    m_refPhyDimensions[2] = ref_volume_dim[2];
    m_refPhyDimensions[3] = ref_volume_dim[3];

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setReferenceVolumeDim(m_refPhyDimensions);
}

void RegisterRaycastingVolumeRenderer3D::setFloatingVolumeDim(float float_volume_dim[])
{
    if(!isInitialized())
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::setFloatingVolumeDim -->> Object not initialized!");

    m_floatPhyDimensions[0] = float_volume_dim[0];
    m_floatPhyDimensions[1] = float_volume_dim[1];
    m_floatPhyDimensions[2] = float_volume_dim[2];
    m_floatPhyDimensions[3] = float_volume_dim[3];

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setFloatingVolumeDim(m_floatPhyDimensions);
}

void RegisterRaycastingVolumeRenderer3D::setReferenceOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr referenceOpenGLVolumeRenderingResource)
{
    m_referenceOpenGLVolumeRenderingResource = referenceOpenGLVolumeRenderingResource;

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setReferenceOpenGLVolumeRenderingResource(m_referenceOpenGLVolumeRenderingResource);
}

void RegisterRaycastingVolumeRenderer3D::setFloatingOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr floatingOpenGLVolumeRenderingResource)
{
    m_floatingOpenGLVolumeRenderingResource = floatingOpenGLVolumeRenderingResource;

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setFloatingOpenGLVolumeRenderingResource(m_floatingOpenGLVolumeRenderingResource);
}

void RegisterRaycastingVolumeRenderer3D::setRegistrationMatrixInv(const glm::mat4 &registration_matrix_inv)
{
    if(!isInitialized())
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::setRegistrationMatrixInv -->> Object not initialized!");
    
    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setRegistrationMatrixInv(registration_matrix_inv);
}

void RegisterRaycastingVolumeRenderer3D::setBlendingFactor(float blending)
{
    if(!isInitialized())
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::setBlendingFactor -->> Object not initialized!");

    static_cast<RegisterRaycastingShader*>(getRaycastingShader())->setBlendingFactor(blending);
}

void RegisterRaycastingVolumeRenderer3D::changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader)
{
    RegisterRaycastingShader *raycasting_shader = dynamic_cast<RegisterRaycastingShader*>(new_raycasting_shader);

    if(raycasting_shader)
    {
        raycasting_shader->copyData(static_cast<RegisterRaycastingShader*>(getRaycastingShader()));
        BaseRaycastingVolumeRenderer3D::changeRaycastingShader(raycasting_shader);

    } else
        throw std::runtime_error("RegisterRaycastingVolumeRenderer3D::changeRaycastingShader -->> Object is not of the RegisterRaycastingShader class!");
}

void RegisterRaycastingVolumeRenderer3D::restoreRaycastingShader()
{
    if(getPreviousRaycastingShader())
    {
        static_cast<RegisterRaycastingShader*>(getPreviousRaycastingShader())->copyData(static_cast<RegisterRaycastingShader*>(getRaycastingShader()));
        BaseRaycastingVolumeRenderer3D::restoreRaycastingShader();

    } else
        std::cerr << "RegisterRaycastingVolumeRenderer3D::restoreRaycastingShader -->> Previous raycasting shader is NULL!" << std::endl;
}
