#include "DiffusionSliceRenderer.h"

DiffusionSliceRenderer::DiffusionSliceRenderer(OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource):
    SliceRenderer(opengl_volume_rendering_resource)
{
    setFragmentShaderPath(":/slicing_frag_diff");

    m_isGrayscale = true;
}

void DiffusionSliceRenderer::initialize()
{
    SliceRenderer::initialize();

    getSliceShaderProgram().binding();
    {
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"is_grayscale"), m_isGrayscale);
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"use_transfer_function"), 1);
    }
    getSliceShaderProgram().releasing();
}

void DiffusionSliceRenderer::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    m_isGrayscale = state;

    if(isInitialized())
    {
        getSliceShaderProgram().binding();
        {
            glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"is_grayscale"), state);

            glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"use_transfer_function"), use_transfer_function);
        }
        getSliceShaderProgram().releasing();
    }
}
