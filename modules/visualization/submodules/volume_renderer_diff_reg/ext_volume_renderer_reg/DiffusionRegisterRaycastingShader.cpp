#include "DiffusionRegisterRaycastingShader.h"

#include "renderer/vmtkManagementShader.h"

DiffusionRegisterRaycastingShader::DiffusionRegisterRaycastingShader()
{
    setFragmentShaderPath(":/castregister_with_planar_clipping_diff_frag");

    m_isGrayscaleActivated = true;
    m_useTransferFunction = true;
}

void DiffusionRegisterRaycastingShader::initialize()
{
    RegisterRaycastingShader::initialize();

    setRayTracedFBO(new vmtkFrameBufferObject(400,400, 2));
}

void DiffusionRegisterRaycastingShader::resize(int width, int height)
{
    RegisterRaycastingShader::resize(width, height);

    setRayTracedFBO(new vmtkFrameBufferObject(width * getFBOSizeFactor(), height * getFBOSizeFactor(), 2));
}

void DiffusionRegisterRaycastingShader::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    if(!isInitialized())
        throw std::runtime_error("DiffusionRegisterRaycastingShader::setEnabledGrayscaleMode -->> Object not initialized!");

    m_isGrayscaleActivated = state;
    m_useTransferFunction = use_transfer_function;

    getShader().binding();
    {
        if(state)
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"is_grayscale"), 1);
        else
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"is_grayscale"), 0);

        if(use_transfer_function)
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"use_transfer_function"), 1);
        else
            glUniform1i(glGetUniformLocation(getShader().getProgram(),"use_transfer_function"), 0);
    }
    getShader().releasing();
}

void DiffusionRegisterRaycastingShader::copyData(DiffusionRegisterRaycastingShader *raycasting_shader)
{
    setEnabledGrayscaleMode(raycasting_shader->m_isGrayscaleActivated, raycasting_shader->m_useTransferFunction);
}
