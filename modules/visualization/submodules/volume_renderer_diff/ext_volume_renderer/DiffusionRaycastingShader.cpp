#include "DiffusionRaycastingShader.h"

#include <renderer/vmtkManagementShader.h>

DiffusionRaycastingShader::DiffusionRaycastingShader()
{

}

void DiffusionRaycastingShader::initialize()
{
    setFragmentShaderPath(":/raytrace_with_planar_clipping_frag_diff");

    RaycastingShader::initialize();

    setRayTracedFBO(new vmtkFrameBufferObject(400,400, 2));
}

void DiffusionRaycastingShader::resize(int width, int height)
{
    RaycastingShader::resize(width, height);

    setRayTracedFBO(new vmtkFrameBufferObject(width * getFBOSizeFactor(), height * getFBOSizeFactor(), 2));
}

void DiffusionRaycastingShader::setEnabledGrayscaleMode(bool state, bool use_transfer_function)
{
    getShader().binding();
    {
        glUniform1i(glGetUniformLocation(getShader().getProgram(),"is_grayscale"), state);

        glUniform1i(glGetUniformLocation(getShader().getProgram(),"use_transfer_function"), use_transfer_function);
    }
    getShader().releasing();
}
