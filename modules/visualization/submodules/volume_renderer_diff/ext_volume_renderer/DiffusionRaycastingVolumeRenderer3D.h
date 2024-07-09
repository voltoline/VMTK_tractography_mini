/**
  * @file DiffusionRaycastigVolumeRenderer3D.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H
#define DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H

#include "../../volume_renderer/3d_renderer/RaycastingVolumeRenderer3D.h"

class DiffusionRaycastingShader;

/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Render the scalar values of diffusion volume and a buffer of visible voxels on the GPU.
 *
 */
class DiffusionRaycastingVolumeRenderer3D: public RaycastingVolumeRenderer3D
{
public:
    /**
     * @brief Constructor of an object of the class DiffusionRayCastingVolumeRenderer3D
     *
     * @param volume_dim[] x-, y- and z-length in number of voxels
     * @param scale_factors[] x-, y- and z-scale factor relative to each voxel
     * @param opengl_volume_rendering_resource opengl rendering settings
     */
    DiffusionRaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[], OpenGLVolumeRenderingResource::sPtr opengl_volume_rendering_resource);

    /**
     * @brief Enable grayscale rendering mode
     *
     * @param state mode switch  //Ting - unnecessary (20/04/2020)
     * @param use_transfer_function grayscale transfer function
     */
    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);

    void changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader) override;
    void restoreRaycastingShader() override;

private:
    BaseRaycastingShader* createRaycastingShader() override;
    BaseRaycastingShader* createBackRaycastingShader() override;

};

#endif // DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H
