#ifndef BASE_RAYCASTING_SHADER_H
#define BASE_RAYCASTING_SHADER_H

#include <renderer/vmtkFrameBufferObject.h>
#include <glm/glm.hpp>

class RaycastingPreRenderShader;
class vmtkManagementShader;

class BaseRaycastingShader
{
public:
    BaseRaycastingShader();

    BaseRaycastingShader(BaseRaycastingShader *raycasting_shader);

    virtual ~BaseRaycastingShader();

    virtual void initialize();

    /*!
     * @brief Generate an image of the medical volume by shooting rays from the points stored in the front plane and
     * tracing this ray along the scanned volume (loaded as a texture) until the corresponding point in the back plane.
     *
     * @param pre_render_shader object containing the raycasting intervals, delimited by the
     * front and the back planes.
     */
    virtual void execute(RaycastingPreRenderShader *pre_render_shader);

    /*!
     * @brief Resize the resolution of output image.
     *
     * @param width width in pixels.
     * @param height height in pixels.
     */
    virtual void resize(int width, int height);

    vmtkFrameBufferObject& getRayTracedFBO();

    float getFBOSizeFactor() const;

    void setFBOSizeFactor(float FBOSizeFactor);

    bool isInitialized() const;

    /**
     * @brief Enable or disable multiplanar reformatting.
     *
     * @param enableMPR enabled (1) or disabled (0).
     */
    void setEnableMPR(bool enableMPR);

    /**
     * @brief Set the clipping plane.
     *
     * @param getClipping_plane Coefficients of the plane.
     */
    void setClippingPlane(const glm::vec4 &getClipping_plane);

    /**
     * @brief Set the background noise threshold.
     *
     * @param noiseThreshold threshold below which the voxel is discarded.
     */
    virtual void setNoiseThreshold(float noiseThreshold);

    /**
     * @brief Enable or disable the removal of the background noise.
     *
     * @param stateThreshold enabled (1) or disabled (0).
     */
    void setStateThreshold(bool stateThreshold);

    /**
     * @brief Set modelview-projection transformation matrix.
     *
     * @param MVP matrix.
     */
    void setMVP(const glm::mat4 &MVP);

    /**
     * @brief Enable default number of steps along a ray.
     *
     * @param bFixedLowSteps enabled (1) or disabled (0).
     */
    void setBFixedLowSteps(bool bFixedLowSteps);

    /**
     * @brief
     *
     * @param bEnableFixedLowSteps
     */
    void setBEnableFixedLowSteps(bool bEnableFixedLowSteps); ///Ting: is it necessary??? 28/04/2020

    /**
     * @brief Set the number of steps.
     *
     * @param iSteps
     */
    void setISteps(int iSteps);  ///Ting: in the current implementation they are fixed in 50/100! 28/04/2020

    /**
     * @brief Set the vertex shader.
     *
     * @param vertexShaderPath file.
     */
    void setVertexShaderPath(const std::string &vertexShaderPath);

    /**
     * @brief Set the fragment shader.
     *
     * @param framentShaderPath file.
     */
    void setFragmentShaderPath(const std::string &framentShaderPath);

    vmtkManagementShader& getShader();

    /**
     * @brief Get the depth map.
     *
     * @param wX width of the map.
     * @param wY height of the map.
     * @param viewport viewport rectangle.
     * @param depthZFront depth map.
     */
    void getZDepthFrontFromRayCasting(int wX, int wY, glm::vec4 viewport, float &depthZFront);

    void copyData(BaseRaycastingShader *raycasting_shader);

    void highlightPlanes(float slice_coord_tex[3]);

    void highlightPlanesActivated(bool state);

protected:
    void setRayTracedFBO(vmtkFrameBufferObject *rayTracedFBO);

private:
    /**
     * @brief Define the position and texture coordinates of the four vertices of a plane
     * on which the raycasted volume is mapped.
     */
    void createVectorPlanes();

    /**
     * @brief Initialize the geometry of a (pre-render) plane on which the ray
     * casted volume is projected.
     */
    void initPlaneRayTraced();

    /**
     * @brief Draw the plane with the image of raycasted volume as a texture.
     */
    void drawPlaneRayTraced();

    virtual void updateTextureUnits(RaycastingPreRenderShader *pre_render_shader) = 0;

    float m_FBOSizeFactor;                   /**< scale factor for FBO */
    vmtkFrameBufferObject *m_rayTracedFBO;  /**< framebuffer object for raycasted image */
    bool m_isInitialized;

    bool m_enableMPR; /**< TODO: describe */
    glm::vec4 m_clippingPlane;  /**< coefficients of the clipping plane */

    /**
     * @brief Data of textured vertices.
     */
    struct VertexData
    {
        glm::vec2 position;     /** < position coordinates */
        glm::vec2 texCoord;     /** < texture coordinates */
    };

    /**
     * @brief Position coordinates of the vertices of the final image plane.
     */
    glm::vec2 v1, v2, v3, v4;

    /**
     * @brief Texture coordinates of the vertices of the final image plane.
     */
    glm::vec2  vt1, vt2, vt3, vt4;

    GLuint m_vaosPlaneRayTraced;      /**< vertex array object to render the image on a plane */
    GLuint m_vboPlaneRayTraced;       /**< data buffer to render the image on a plane */
    GLuint m_eboPlaneRayTraced;       /**< element array buffer to render the image on a plane */

    int m_width;                    /**< width of the viewport rectangle */
    int m_height;                   /**< height of the viewport rectangle */

    std::string m_vertexShaderPath; /**< file of vertex shader */
    std::string m_fragmentShaderPath; /**< file of fragment shader */

    float m_noiseThreshold;                   /**< threshold for removing the noise out of the scalp */
    bool m_stateThreshold;
    glm::mat4 m_MVP;                          /**< modelview-projection transformation matrix */

    bool m_bFixedLowSteps;                    /**< whether low resolution step is fixed? */
    bool m_bEnableFixedLowSteps;               /**< whether low resolution steps are allowed?  */
    int m_iSteps;                             /**< number of sampling steps along the casted ray */

    vmtkManagementShader *m_shader;     /**< raycast program */

    GLshort m_steps_mode_loc; /**<  location of sampling mode variable (high or low) on the GPU  */
    GLshort m_noiseThresholdUniformLoc; /**< location of threshold variable on the GPU */
    GLshort m_stateNoiseThresholdUniformLoc; /**< location of noise removal state (enabled - 1 or disabled - 0) on the GPU */
    GLshort m_clippingPlaneUniformLoc; /**< location of clipping plane variable on the GPU */
    GLshort m_MVPUniformLoc; /**< location of the modelview-projection variable on the GPU */

    GLshort m_highlightedPlanesUniformLoc;
    GLshort m_highlightedPlanesActivatedUniformLoc;
};

#endif // BASE_RAYCASTING_SHADER_H
