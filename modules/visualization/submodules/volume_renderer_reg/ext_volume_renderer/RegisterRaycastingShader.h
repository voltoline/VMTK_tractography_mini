/**
  * @file RegisterRaycastingShader.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef REGISTER_RAYCASTING_SHADER_H
#define REGISTER_RAYCASTING_SHADER_H

#include "utilities/QCheckingOpenGL.h"
#include <glm/glm.hpp>
#include <modules/visualization/submodules/volume_renderer/OpenGLVolumeRenderingResource.h>
#include <modules/visualization/submodules/volume_renderer/3d_renderer/BaseRaycastingShader.h>

class RaycastingPreRenderShader;

/**
 * @brief Configure the shaders and the rendering parameters to a GPU program that renders two
 * co-registered scalar volumes.
 *
 * By default, two shaders are set:
 * Vertex shader: raytrace.vert
 * Fragment shader: castregister_with_planar_clipping.frag
 *
 */
class RegisterRaycastingShader: public BaseRaycastingShader
{
public:
    /**
     * @brief Constructor of an object of the class RegisterRaycastingShader.
     *
     */
    RegisterRaycastingShader();

    /**
     * @brief Initialize the rendering parameters.
     *
     */
    void initialize();

    /**
     * @brief Set the dimensions of the reference volume.
     *
     * @param volume_dim[] x-, y- and z-dimensions in voxels.
     */
    void setReferenceVolumeDim(float volume_dim[]);

    /**
     * @brief Set the scale factors of the voxels in x-, y- and z-directions to get physical dimensions of the reference volume.
     *
     * @param scale_factors[] scale factor in x-, y- and z- directions.
     */
    void setReferenceScaleFactors(float scale_factors[]);

    /**
     * @brief  Set the dimensions of the reference volume.
     *
     * @param volume_dim[] x-, y- and z-dimensions in voxels.
     */
    void setFloatingVolumeDim(float volume_dim[]);

    /**
     * @brief Set the inverse of the co-registration matrix.
     *
     * @param registration_matrix_inv matrix.
     */
    void setRegistrationMatrixInv(const glm::mat4 &registration_matrix_inv);

    /**
     * @brief Se the blendig factor between the reference and the floating volumes.
     *
     * @param blendingFactor factor between 0.0 and 1.0.
     */
    virtual void setBlendingFactor(float blendingFactor);

    /**
     * @brief Get the blending facto between the reference and the floating volumes.
     *
     * @return float factor between 0.0 and 1.0.
     */
    float getBlendingFactor() const;

    /**
     * @brief Set the OpenGL resources to render the reference volume.
     *
     * @param referenceOpenGLVolumeRenderingResource resource.
     */
    void setReferenceOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr referenceOpenGLVolumeRenderingResource);

    /**
     * @brief Set the OpenGL resource to render the floating volume.
     *
     * @param floatingOpenGLVolumeRenderingResource resource.
     */
    void setFloatingOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr floatingOpenGLVolumeRenderingResource);

    void copyData(RegisterRaycastingShader *raycasting_shader);

protected:

    /**
     * @brief
     *
     * @param pre_render_shader
     */
    void updateTextureUnits(RaycastingPreRenderShader *pre_render_shader);

    GLshort m_scaleFactorsUniformLoc; /**< location of scale factor variable on the GPU */

    float m_refScaleFactors[4]; /**< TODO: describe */
    float m_refPhyDimensions[4]; /**< TODO: describe */
    float m_floatPhyDimensions[4];   /**< scale factors of the volume data */

    unsigned short m_volumeDimension[4];

    glm::mat4 m_registration_matrix_inv; /**< TODO: describe */

    float m_blendingFactor;                   /**< blending weight of co-registered volumes */

    OpenGLVolumeRenderingResource::sPtr m_referenceOpenGLVolumeRenderingResource;
    OpenGLVolumeRenderingResource::sPtr m_floatingOpenGLVolumeRenderingResource;
};

#endif // REGISTER_RAYCASTING_SHADER_H
