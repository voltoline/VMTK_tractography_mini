#ifndef RAYCASTING_PRE_RENDER_SHADER_H
#define RAYCASTING_PRE_RENDER_SHADER_H

#include "utilities/QCheckingOpenGL.h"
#include <glm/glm.hpp>

class vmtkManagementShader;
class vmtkFrameBufferObject;

/*!
 * \brief Render to two texture buffers, as the attachment of framebuffer objects m_FrontFBO and m_BackFBO,
 * the bounding cube of the scanned volume. The m_FrontFBO contains the samples points of the front (visible)
 * plane in texture coordinates, while the m_BackFBO the sample points of the back plane.
 */
class RaycastingPreRenderShader
{
public:
    RaycastingPreRenderShader(float scale_factors[]);

    ~RaycastingPreRenderShader();

    /*!
     * \brief Initialize the framebuffer objects with texture buffers of the default size 400x400 as attachment.
     */
    void initialize();
    /*!
     * \brief Render a cube with colors R=(1,0,0), G=(0,1,0) and B=(0,0,1) setting, respectively, at three non-ccplanar corners of the cube.
     * \param mvp model-view-projection transformation matrix.
     */
    void execute();
    /*!
     * \brief Resize the texture buffers attached to the framebuffer objects
     * \param width width in pixels
     * \param height height in pixels
     */
    void resize(int width, int height);

    /**
   * @brief sets the clipping planes.
   * @param[in] fXLeft leftmost x-clipping plane
   * @param[in] fXRight rightmost x-clipping plane
   * @param[in] fYTop topmost y-clipping plane
   * @param[in] fYBottom bottommost y-clipping plane
   * @param[in] fZBack backmost z-clipping plane
   * @param[in] fZFront frontmost z-clipping plane
   */
    void setClippingValues(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_);

    void setScaleFactors(float scale_factors[]);

    void setFBOSizeFactor(float FBOSizeFactor);

    vmtkFrameBufferObject& getBackFBO();

    vmtkFrameBufferObject& getFrontFBO();

    void setMVP(const glm::mat4 &mvp);

private:

    /**
     * @brief initializes the geometry of a cube to be rendered for getting back and front depth maps.
     */
    void initDrawCube();

    /**
     * @brief render under the viewing transformation mvp the parallel projection of a physical cube, after applying planar clipping.
     * The rendered colors correspond to the samples of the unit cube in normalized coordinates.
     * @param mvp model-view-projection transformation matrix
     */
    void itlDrawColorCube();

    /**
     * @brief render a cube.
     */
    void drawCube();

    bool m_initialized;

    GLuint vaosCube;                /**< drawCube: vertex array object */
    GLuint vboCube;                 /**< drawCube: data buffer */
    GLuint eboCube;                 /**< drawCube: element array buffer */

    float m_fClipXLeft;                       /**< x left clip plane */
    float m_fClipXRight;                      /**< x right clip plane */
    float m_fClipYTop;                        /**< y top clip plane */
    float m_fClipYBottom;                     /**< y bottom clip plane */
    float m_fClipZBack;                       /**< z back clip plane */
    float m_fClipZFront;                      /**< z front clip plane */

    GLshort m_mvpMatrixColorShaderUniformLoc;
    GLshort m_scaleFactorsColorShaderUniformLoc;

    /**
     * @struct VertexDataCube
     */
    struct VertexDataCube
    {
        glm::vec3 position; /**< position coordinates */
    };

    /**
     * @brief Position vectors of VertexDataCube.
     */
    glm::vec3 vc1, vc2, vc3, vc4, vc5, vc6, vc7, vc8, vc9, vc10, vc11, vc12,
    vc13, vc14, vc15, vc16, vc17, vc18, vc19, vc20, vc21, vc22, vc23, vc24;

    GLboolean m_flag_cube_changed;           /**< cube change status */
    GLboolean m_flag_cube_alloc;            /**< cube setup status */

    float m_scaleFactors[4];                  /**< scale factor for compensating distortions on the texture volume */

    float m_FBOSizeFactor;                   /**< scale factor for FBO */

    glm::mat4 m_mvp;

    int m_width;
    int m_height;

    vmtkManagementShader *m_ColorShader;        /**< pre-processing shader to get front and back planes */

    vmtkFrameBufferObject *m_BackFBO;       /**< framebuffer object for back side */
    vmtkFrameBufferObject *m_FrontFBO;      /**< framebuffer object for front side */
};

#endif // RAYCASTING_PRE_RENDER_SHADER_H
