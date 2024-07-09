/**
  * @file RegisteredDiffusionRaycastingVolumeRenderer3D.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef REGISTERED_DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H
#define REGISTERED_DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H

#include <modules/visualization/submodules/volume_renderer_reg/ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.h>

class DiffusionRegisterRaycastingShader;

/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Render co-registered MRI-T1 and DWI volumes.
 *
 * Two framebuffer objects are rendered: the raycasted volume and the coordinates of visible voxels.
 *
 * Specific fragment shader: castregister_with_planar_clipping_diff.frag
 *
 */
class RegisteredDiffusionRaycastingVolumeRenderer3D: public RegisterRaycastingVolumeRenderer3D
{
public:
    /**
     * @brief Constructor of an object of the class RegisteredDiffusionRaycastingVolumeRenderer3D.
     * @param ref_volume_dim dimensions in voxels of the reference volume.
     * @param float_volume_dim dimensions in voxels of the floating volume.
     * @param ref_scale_factors scale factors of the voxel in the reference volume.
     * @param ref_opengl_volume_rendering_resource OpenGL graphics recource of the reference volume.
     * @param float_opengl_volume_rendering_resource OpenGL graphics resource of the floating volume.
     */
    RegisteredDiffusionRaycastingVolumeRenderer3D(unsigned short ref_volume_dim[], unsigned short float_volume_dim[], float ref_scale_factors[],
                                                  OpenGLVolumeRenderingResource::sPtr ref_opengl_volume_rendering_resource, OpenGLVolumeRenderingResource::sPtr float_opengl_volume_rendering_resource);


    /**
     * @brief Set the rendering mode in grayscale.
     *
     * @param state enable (1) or disable (0).
     * @param use_transfer_function using transfer function (1) or not (0).
     */
    void setEnabledGrayscaleMode(bool state, bool use_transfer_function);

    /**
     * @brief Update the GPU raycasting program.
     *
     * @param new_raycasting_shader GPU rendering program.
     */
    void changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader) override;

    /**
     * @brief Restore the GPU raycasting program.
     *
     */
    void restoreRaycastingShader() override;

private:
    BaseRaycastingShader* createRaycastingShader() override;
    BaseRaycastingShader *createBackRaycastingShader() override;
};

#endif // REGISTERED_DIFFUSION_RAYCASTING_VOLUME_RENDERER_3D_H
