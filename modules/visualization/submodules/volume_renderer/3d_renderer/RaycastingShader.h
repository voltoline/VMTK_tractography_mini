#ifndef RAYCASTING_SHADER_H
#define RAYCASTING_SHADER_H

#include "utilities/QCheckingOpenGL.h"
#include "BaseRaycastingShader.h"
#include "../OpenGLVolumeRenderingResource.h"

class RaycastingPreRenderShader;

/*!
 * @brief Raycast the volume from the position stored in the front buffer to the position stored in the back buffer. 
 * 
 * Depth map of the visible surface is generated.
 * 
 * Shaders that make up the GPU program
 * Vertex shader: raytrace.vert
 * Fragment shader: raytrace_with_planar_clipping_depth_front.frag
 */
class RaycastingShader: public BaseRaycastingShader
{
public:
    /**
     * @brief Constructor of an object of the class RaycastingShader.
     *
     */
    RaycastingShader();

    RaycastingShader(RaycastingShader *raycasting_shader);

    /*!
     * @brief Initialize the parameters required for running shader
     */
    void initialize() override;

    /**
     * @brief Set the dimensions of the scanned volume.
     *
     * @param volume_dim[] dimensionas in number of voxels.
     */
    void setVolumeDim(unsigned short volume_dim[]);
    
    /**
     * @brief Set the scaling factors.
     *
     * @param scale_factors[] scaling factors of a voxel in x-, y- and z-directions.
     */
    void setScaleFactors(float scale_factors[]);

    /**
     * @brief 
     *
     * @param openGLVolumeRenderingResource
     */
    void setOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr openGLVolumeRenderingResource);

    void copyData(RaycastingShader *raycasting_shader);

protected:

    /**
     * @brief 
     *
     * @param pre_render_shader
     */
    void updateTextureUnits(RaycastingPreRenderShader *pre_render_shader) override;

    GLshort m_scaleFactorsUniformLoc; /**< location of the scaling factors on the GPU */

    unsigned short m_volumeDimension[3]; /**< TODO: describe */

    float m_scaleFactors[4];                  /**< scale factor for compensating distortions on the texture volume */

    OpenGLVolumeRenderingResource::sPtr m_openGLVolumeRenderingResource; /**< TODO: describe */
};

#endif // RAYCASTING_SHADER_H
