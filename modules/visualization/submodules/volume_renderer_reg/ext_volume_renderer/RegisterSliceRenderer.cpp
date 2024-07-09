#include "RegisterSliceRenderer.h"

#include <glm/gtc/type_ptr.hpp>

RegisterSliceRenderer::RegisterSliceRenderer(float ref_scale_factors[4], float float_scale_factors[4], float ref_volume_dim[4], float float_volume_dim[4], OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource)
{
    m_refScaleFactors[0] = ref_scale_factors[0];
    m_refScaleFactors[1] = ref_scale_factors[1];
    m_refScaleFactors[2] = ref_scale_factors[2];
    m_refScaleFactors[3] = ref_scale_factors[3];

    m_floatScaleFactors[0] = float_scale_factors[0];
    m_floatScaleFactors[1] = float_scale_factors[1];
    m_floatScaleFactors[2] = float_scale_factors[2];
    m_floatScaleFactors[3] = float_scale_factors[3];

    m_refPhyDimensions[0] = ref_volume_dim[0];
    m_refPhyDimensions[1] = ref_volume_dim[1];
    m_refPhyDimensions[2] = ref_volume_dim[2];
    m_refPhyDimensions[3] = ref_volume_dim[3];

    m_floatPhyDimensions[0] = float_volume_dim[0];
    m_floatPhyDimensions[1] = float_volume_dim[1];
    m_floatPhyDimensions[2] = float_volume_dim[2];
    m_floatPhyDimensions[3] = float_volume_dim[3];

    m_blendingFactor = 0.5f;

    mRegistrationMatrixInv = glm::mat4(1.0);

    m_referenceOpenGLVolumeRenderingResource = ref_opengl_volume_rendering_resource;
    m_floatingOpenGLVolumeRenderingResource = float_opengl_volume_rendering_resource;

    setVertexShaderPath(":/simple_vert");
    setFragmentShaderPath(":/castregister2d_frag");
}

void RegisterSliceRenderer::initialize()
{
    BaseSliceRenderer::initialize();

    getSliceShaderProgram().binding();
    {
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "ref_transferfunction"), 0);
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "ref_volumetexture"), 1);
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "float_transferfunction"), 3);
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "float_volumetexture"), 4);

        glUniform4fv(glGetUniformLocation(getSliceShaderProgram().getProgram(), "ref_phyDimensions"), 1, m_refPhyDimensions);
        glUniform4fv(glGetUniformLocation(getSliceShaderProgram().getProgram(), "float_phyDimensions"), 1, m_floatPhyDimensions);
        glUniform4fv(glGetUniformLocation(getSliceShaderProgram().getProgram(), "scaleFactors"), 1, m_refScaleFactors);

        glUniform1f(glGetUniformLocation(getSliceShaderProgram().getProgram(), "blending_factor"), m_blendingFactor);

        glUniformMatrix4fv(glGetUniformLocation(getSliceShaderProgram().getProgram(), "inv_registration_matrix"), 1, GL_TRUE , glm::value_ptr(mRegistrationMatrixInv));
    }
    getSliceShaderProgram().releasing();
}

void RegisterSliceRenderer::configureTextureUnits()
{
    //Reference volume data
    GLuint texID = m_referenceOpenGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, texID);
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    texID = m_referenceOpenGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, texID);
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    //Floating volume data
    texID = m_floatingOpenGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, texID);
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    texID = m_floatingOpenGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_3D, texID);
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void RegisterSliceRenderer::setBlendingFactor(float blendingFactor)
{
    m_blendingFactor = blendingFactor / 100.f;

    if(isInitialized())
    {
        getSliceShaderProgram().binding();
        {
            glUniform1f(glGetUniformLocation(getSliceShaderProgram().getProgram(), "blending_factor"), m_blendingFactor);
        }
        getSliceShaderProgram().releasing();

    } else
        throw std::runtime_error("RegisterSliceRenderer::setBlendingFactor -->> Object not initialized!");
}

void RegisterSliceRenderer::setRegistrationMatrixInv(const glm::mat4 &registrationMatrixInv)
{
    mRegistrationMatrixInv = registrationMatrixInv;

    if(isInitialized())
    {
        getSliceShaderProgram().binding();
        {
            glUniformMatrix4fv(glGetUniformLocation(getSliceShaderProgram().getProgram(), "inv_registration_matrix"), 1, GL_TRUE , glm::value_ptr(mRegistrationMatrixInv));
        }
        getSliceShaderProgram().releasing();

    } else
        throw std::runtime_error("RegisterSliceRenderer::setRegistrationMatrixInv -->> Object not initialized!");
}
