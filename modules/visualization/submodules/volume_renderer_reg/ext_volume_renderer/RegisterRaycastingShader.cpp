#include "RegisterRaycastingShader.h"

#include "renderer/vmtkManagementShader.h"
#include "../../volume_renderer/3d_renderer/RaycastingPreRenderShader.h"
#include <glm/gtc/type_ptr.hpp>

RegisterRaycastingShader::RegisterRaycastingShader()
{
    setVertexShaderPath(":/raytrace_vert");
    setFragmentShaderPath(":/castregister_with_planar_clipping_frag");

    m_blendingFactor = 50;                /**< default blending factor */
    m_registration_matrix_inv = glm::mat4(1.f);

    m_referenceOpenGLVolumeRenderingResource = nullptr;
    m_floatingOpenGLVolumeRenderingResource = nullptr;
}

void RegisterRaycastingShader::setReferenceVolumeDim(float ref_volume_dim[])
{
    m_refPhyDimensions[0] = ref_volume_dim[0];
    m_refPhyDimensions[1] = ref_volume_dim[1];
    m_refPhyDimensions[2] = ref_volume_dim[2];
    m_refPhyDimensions[3] = ref_volume_dim[3];

    m_volumeDimension[0] = ref_volume_dim[0];
    m_volumeDimension[1] = ref_volume_dim[1];
    m_volumeDimension[2] = ref_volume_dim[2];
    m_volumeDimension[3] = ref_volume_dim[3];

    if(isInitialized())
    {
        getShader().binding();
        {
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"width"),  m_refPhyDimensions[0]);
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"height"), m_refPhyDimensions[1]);
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"depth"),  m_refPhyDimensions[2]);

            glUniform4fv(glGetUniformLocation(getShader().getProgram(),"ref_phyDimensions"), 1, m_refPhyDimensions);

        }
        getShader().releasing();
    }
}

void RegisterRaycastingShader::setReferenceScaleFactors(float ref_scale_factors[])
{
    m_refScaleFactors[0] = ref_scale_factors[0];
    m_refScaleFactors[1] = ref_scale_factors[1];
    m_refScaleFactors[2] = ref_scale_factors[2];
    m_refScaleFactors[3] = ref_scale_factors[3];

    if(isInitialized())
    {
        getShader().binding();
        {
            glUniform4fv(m_scaleFactorsUniformLoc, 1, m_refScaleFactors);
        }
        getShader().releasing();
    }
}

void RegisterRaycastingShader::setFloatingVolumeDim(float float_volume_dim[])
{
    m_floatPhyDimensions[0] = float_volume_dim[0];
    m_floatPhyDimensions[1] = float_volume_dim[1];
    m_floatPhyDimensions[2] = float_volume_dim[2];
    m_floatPhyDimensions[3] = float_volume_dim[3];

    if(isInitialized())
    {
        getShader().binding();
        {
            glUniform4fv(glGetUniformLocation(getShader().getProgram(),"float_phyDimensions"), 1, m_floatPhyDimensions);
        }
        getShader().releasing();
    }
}

void RegisterRaycastingShader::initialize()
{
    BaseRaycastingShader::initialize();

    getShader().binding();
    {
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"width"),  m_refPhyDimensions[0]);
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"height"), m_refPhyDimensions[1]);
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"depth"),  m_refPhyDimensions[2]);

        glUniform1i(glGetUniformLocation(getShader().getProgram(), "ref_transferfunction"), 0);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "ref_volumetexture"), 1);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "float_transferfunction"), 3);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "float_volumetexture"), 4);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "backface_fbo"), 6);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "frontface_fbo"), 7);

        glUniformMatrix4fv( glGetUniformLocation(getShader().getProgram(),"inv_registration_matrix"), 1, GL_TRUE , glm::value_ptr(m_registration_matrix_inv));
        glUniform4fv(glGetUniformLocation(getShader().getProgram(),"ref_phyDimensions"), 1, m_refPhyDimensions);
        glUniform4fv(glGetUniformLocation(getShader().getProgram(),"float_phyDimensions"), 1, m_floatPhyDimensions);
        glUniform1f(glGetUniformLocation(getShader().getProgram(),"blending_factor"), m_blendingFactor / 100.f);

        m_scaleFactorsUniformLoc = glGetUniformLocation(getShader().getProgram(), "scaleFactors");

        glUniform4fv(m_scaleFactorsUniformLoc, 1, m_refScaleFactors);
    }
    getShader().releasing();
}

void RegisterRaycastingShader::updateTextureUnits(RaycastingPreRenderShader *pre_render_shader)
{
    GLuint texID = m_referenceOpenGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, texID);

    texID = m_referenceOpenGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, texID);

    texID = m_floatingOpenGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, texID);

    texID = m_floatingOpenGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_3D, texID);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, pre_render_shader->getBackFBO().getTexture());

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, pre_render_shader->getFrontFBO().getTexture());
}

void RegisterRaycastingShader::setFloatingOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr floatingOpenGLVolumeRenderingResource)
{
    m_floatingOpenGLVolumeRenderingResource = floatingOpenGLVolumeRenderingResource;
}

void RegisterRaycastingShader::copyData(RegisterRaycastingShader *raycasting_shader)
{
    m_referenceOpenGLVolumeRenderingResource = raycasting_shader->m_referenceOpenGLVolumeRenderingResource;
    m_floatingOpenGLVolumeRenderingResource = raycasting_shader->m_floatingOpenGLVolumeRenderingResource;

    setRegistrationMatrixInv(raycasting_shader->m_registration_matrix_inv);
    setBlendingFactor(raycasting_shader->m_blendingFactor);
}

void RegisterRaycastingShader::setReferenceOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr referenceOpenGLVolumeRenderingResource)
{
    m_referenceOpenGLVolumeRenderingResource = referenceOpenGLVolumeRenderingResource;
}

float RegisterRaycastingShader::getBlendingFactor() const
{
    return m_blendingFactor;
}

void RegisterRaycastingShader::setBlendingFactor(float blendingFactor)
{
    m_blendingFactor = blendingFactor;
    
    if(isInitialized())
    {
        getShader().binding();
        {
            glUniform1f(glGetUniformLocation(getShader().getProgram(),"blending_factor"), m_blendingFactor / 100.f);
        }
        getShader().releasing();
    }
}

void RegisterRaycastingShader::setRegistrationMatrixInv(const glm::mat4 &registration_matrix_inv)
{
    m_registration_matrix_inv = registration_matrix_inv;

    if(isInitialized())
    {
        getShader().binding();
        {
            glUniformMatrix4fv( glGetUniformLocation(getShader().getProgram(),"inv_registration_matrix"), 1, GL_TRUE , glm::value_ptr(m_registration_matrix_inv));
        }
        getShader().releasing();
    }
}
