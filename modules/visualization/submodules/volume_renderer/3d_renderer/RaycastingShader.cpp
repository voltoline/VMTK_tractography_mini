#include "RaycastingShader.h"

#include "renderer/vmtkManagementShader.h"
#include "RaycastingPreRenderShader.h"

RaycastingShader::RaycastingShader()
{
    m_openGLVolumeRenderingResource = nullptr;

    setVertexShaderPath(":/raytrace_vert");
    setFragmentShaderPath(":/raytrace_with_planar_clipping_depth_front_frag");
}

RaycastingShader::RaycastingShader(RaycastingShader *raycasting_shader):
    BaseRaycastingShader(raycasting_shader)
{
    setVolumeDim(raycasting_shader->m_volumeDimension);
    setScaleFactors(raycasting_shader->m_scaleFactors);
    setOpenGLVolumeRenderingResource(raycasting_shader->m_openGLVolumeRenderingResource);
}

void RaycastingShader::setVolumeDim(unsigned short volume_dim[])
{
    m_volumeDimension[0] = volume_dim[0];
    m_volumeDimension[1] = volume_dim[1];
    m_volumeDimension[2] = volume_dim[2];
}

void RaycastingShader::setScaleFactors(float scale_factors[])
{
    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];
}

void RaycastingShader::initialize()
{
    BaseRaycastingShader::initialize();

    getShader().binding();
    {
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"width"),  m_volumeDimension[0]);
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"height"), m_volumeDimension[1]);
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"depth"),  m_volumeDimension[2]);

        glUniform1i(glGetUniformLocation(getShader().getProgram(), "transferfunction"), 0);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "volumetexture"), 1);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "backface_fbo"), 3);
        glUniform1i(glGetUniformLocation(getShader().getProgram(), "frontface_fbo"), 4);

        m_scaleFactorsUniformLoc = glGetUniformLocation(getShader().getProgram(), "scaleFactors");

        glUniform4fv(m_scaleFactorsUniformLoc, 1, m_scaleFactors);
    }
    getShader().releasing();
}

void RaycastingShader::updateTextureUnits(RaycastingPreRenderShader *pre_render_shader)
{
    GLuint texID = m_openGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, texID);

    texID = m_openGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, texID);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, pre_render_shader->getBackFBO().getTexture());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, pre_render_shader->getFrontFBO().getTexture());
}

void RaycastingShader::setOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr openGLVolumeRenderingResource)
{
    m_openGLVolumeRenderingResource = openGLVolumeRenderingResource;
}

void RaycastingShader::copyData(RaycastingShader *raycasting_shader)
{
    setOpenGLVolumeRenderingResource(raycasting_shader->m_openGLVolumeRenderingResource);
}
