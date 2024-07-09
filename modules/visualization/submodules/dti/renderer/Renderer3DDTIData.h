/**
  * @file Renderer3DDTIData.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef RENDERER_DTI_DATA_H
#define RENDERER_DTI_DATA_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderer/SubRendererBase.h>
#include <vector>
#include "../models/tractography/FiberClusterSet.h"

class vmtkDiffAcquisition;
class BaseRaycastingVolumeRenderer3D;
class DTIOpenGLResource;
class DepthRenderer;
class DTI;
class FiberRenderer;

/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Render DTI data as grayscale, colormaps, superquadrics glyphs, or fibers.
 *
 * Three forms of visualization of white matter fibers are considered: maps of indices of diffusion anisotropy
 * ("Toward a quantitative assessment of diffusion anisotropy" by C. Pierpaoli e PJ Basser, Magn Reson Med, 1996),
 * superquadric glyphs (based on "Superquadric tensor glyphs" by G. Kindlmann, VISSYM'04),
 * and tractography (based on "Tensorlines: advection-diffusion based propagation through diffusion tensor fields"
 * by D. Weinstein et al, Visualization'99).
 *
 * There are 5 different implementations of the rendering of superquadrics tensor glyphs: DiffSuperquadricRaycasting3DRender,
 * DiffSuperquadricRender, SuperquadricRaycast3DRendererGeometryShader, DiffSuperquadric3DRenderParallel,
 * Superquadric3DRendererGeometryShader.
 *
 */
class RendererDTIData: public SubRendererBase
{
public:
    /**
     * @brief Constructor of an object of the class RenderDTIData
     *
     */
    RendererDTIData();

    /**
     * @brief Setup common rendering parameters
     *
     */
    void initialize();
    /**
     * @brief Render the data according to the modes enabled via the variable m_renderGlyphs, m_renderTracts and m_fiberRender.
     *
     */
    void render();
    /**
     * @brief Set the viewport rectangle.
     *
     * @param[in] width width of the rectangle
     * @param[in] height height of the rectangle
     */
    void resize(int width, int height);

    /**
     * @brief Enable the tract rendering.
     *
     * @param[in] render_tracts Enabled (1) or Disabled (0).
     */
    void enableTractographyRendering(bool render_tracts);

    /**
     * @brief Set the threshold for fractional anisotropy (FA)
     *
     * @param[in] fa_threshold threshold in the range of 0 and 1.0.
     */
    void setFaThreshold(float fa_threshold);
    /**
     * @brief Enable the blending effect of each tensor glyph with respect to the indices of diffusion anisotropy.
     *
     * @param[in] enabled Enabled (1) or Disabled (0).
     */
    void setBlendingEnabled(bool enabled);

    /**
     * @brief Set the DWI volume to be rendered.
     *
     * @param[in] diff_acq Identifier of DWI volume
     */
    void setDiff_acq(vmtkDiffAcquisition *diff_acq);
    /**
     * @brief Set the scale factors of the scanned voxels.
     *
     * @param[in] scale_factors[] scale factore in x-(0), y-(1) and z-(2) directions.
     */
    void setScaleFactors(float scale_factors[4]);

    /**
     * @brief Set OpenGL rendering parameters.
     *
     * @param[in] dtiOpenGLResource parameter values.
     */
    void setDtiOpenGLResource(DTIOpenGLResource *dtiOpenGLResource);

    /**
     * @brief Set projection matrix.
     *
     * @param[in] projectionMatrix 4x4 transformation matrix.
     */
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);

    /**
     * @brief Set modelview transformation matrix.
     *
     * @param modelViewMatrix 4x4 transformation matrix.
     */
    void setModelViewMatrix(const glm::mat4 &modelViewMatrix);

    /**
     * @brief Set the 3D rendering procedure.
     *
     * @param[in] raycastingVolumeRenderer3D rendering procedure.
     */
    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D);

    void setDTI(const DTI *dti);

    void setFiberStepRange(int min, int max);
    void setFiberPaintingActivated(bool state);

    void setFiberClusters(std::vector<FiberClusterSet::sPtr> fiber_cluster_set);

    void setZoomFactor(float factor);

private:

    DepthRenderer *m_depthRenderer; /**< an instance of the class DepthRenderer */
    FiberRenderer *m_fiberRender; /**< an instance of the class fiberRenderer */

    int m_height; /**< height of the viewport rectangle */
    int m_width; /**< width of the viewport rectangle */

    bool m_isBlendingActivated; /**< 1 (enabled)/0 (disabled) blending glyphs with overlapped maps */
    float m_faThreshold; /**< threshold of fractional anisotropy, below which  voxel is not rendered */

    bool m_renderLines; /**< 1 (enabled)/0 (disabled) line rendering */
    bool m_renderSuperquadric; /**< 1 (enabled)/0 (disabled) superquadric rendering */
    bool m_renderTracts; /**< 1 (enabled)/0 (disabled) tract rendering */
    vmtkDiffAcquisition *m_diff_acq; /**< diffusion weighted (DWI) volume */

    float m_fScaleFactors[4]; /**< scale factors of each voxel along x-, y- and z-directions */

    glm::mat4 m_projectionMatrix;  /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;  /**< modelview transformation matrix */

    BaseRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;

    DTIOpenGLResource *m_dtiOpenGLResource; /**< OpenGL rendering resources */

    const DTI *m_dti;

    std::vector<FiberClusterSet::sPtr> m_fiberClusterSet;
};

#endif // RENDERER_DTI_DATA_H
