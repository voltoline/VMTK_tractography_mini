#ifndef DIFFUSION_REGISTER_RAYCASTING_SHADER_H
#define DIFFUSION_REGISTER_RAYCASTING_SHADER_H

#include "modules/visualization/submodules/volume_renderer_reg/ext_volume_renderer/RegisterRaycastingShader.h"

class DiffusionRegisterRaycastingShader: public RegisterRaycastingShader
{
public:
    DiffusionRegisterRaycastingShader();
    ~DiffusionRegisterRaycastingShader() { }

    void initialize() override;
    void resize(int width, int height) override;

    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);

    void copyData(DiffusionRegisterRaycastingShader *raycasting_shader);

private:
    bool m_isGrayscaleActivated; /**< enable state of the grayscale rendering mode */
    bool m_useTransferFunction; /**< usage state of a transfer function */
};

#endif // DIFFUSIONREGISTERRAYCASTINGSHADER_H
