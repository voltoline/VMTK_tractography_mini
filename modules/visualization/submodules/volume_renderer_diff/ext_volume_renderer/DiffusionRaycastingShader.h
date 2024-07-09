#ifndef DIFFUSION_RAYCASTING_SHADER_H
#define DIFFUSION_RAYCASTING_SHADER_H

#include "modules/visualization/submodules/volume_renderer/3d_renderer/RaycastingShader.h"

class DiffusionRaycastingShader: public RaycastingShader
{
public:
    DiffusionRaycastingShader();
    ~DiffusionRaycastingShader() { }

    void initialize() override;

    void resize(int width, int height) override;

    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);
};

#endif // DIFFUSIONRAYCASTINGSHADER_H
