#include "DiffusionRegisterSliceRenderer.h"

DiffusionRegisterSliceRenderer::DiffusionRegisterSliceRenderer(float ref_scale_factors[], float float_scale_factors[], float ref_volume_dim[], float float_volume_dim[], OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource):
    RegisterSliceRenderer(ref_scale_factors, float_scale_factors, ref_volume_dim, float_volume_dim, ref_opengl_volume_rendering_resource, float_opengl_volume_rendering_resource)
{
    setFragmentShaderPath(":/castregister2d_diff_frag");

    m_isGrayscaleActivated = true;
    m_useTransferFunction = true;
}

void DiffusionRegisterSliceRenderer::initialize()
{
    RegisterSliceRenderer::initialize();

    getSliceShaderProgram().binding();
    {
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"is_grayscale"), m_isGrayscaleActivated);
        glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"use_transfer_function"), m_useTransferFunction);
    }
    getSliceShaderProgram().releasing();
}

void DiffusionRegisterSliceRenderer::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    m_isGrayscaleActivated = state;
    m_useTransferFunction = use_transfer_function;

    if(isInitialized())
    {
        getSliceShaderProgram().binding();
        {
            if(state)
                glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"is_grayscale"), 1);
            else
                glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"is_grayscale"), 0);

            if(use_transfer_function)
                glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"use_transfer_function"), 1);
            else
                glUniform1i(glGetUniformLocation(getSliceShaderProgram().getProgram(),"use_transfer_function"), 0);
        }
        getSliceShaderProgram().releasing();

    } else
        throw std::runtime_error("DiffusionRegisterSliceRenderer::setEnabledGrayscaleMode -->> Object not initialized!");
}
