#include "SliceRenderer.h"

SliceRenderer::SliceRenderer(OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource)
{
    m_openGLVolumeRenderingResource = opengl_volume_rendering_resource;

    setVertexShaderPath(":/simple_vert");
    setFragmentShaderPath(":/slicing_frag");
}

void SliceRenderer::configureTextureUnits()
{
    GLuint texID = m_openGLVolumeRenderingResource->getTransferFunctionTextureID();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, texID);
    glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "transferfunction"), 0);

    texID = m_openGLVolumeRenderingResource->getVolumeTextureID();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, texID);
    glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(), "volumetexture"), 1);
}
