#ifndef BASE_SLICE_RENDERER_H
#define BASE_SLICE_RENDERER_H

#include <glm/glm.hpp>
#include <renderer/SubRendererBase.h>
#include "renderer/vmtkManagementShader.h"

class BaseSliceRenderer: public SubRendererBase
{
public:
    BaseSliceRenderer();
    ~BaseSliceRenderer();

    void initialize() override;
    void render() override;
    void resize(int, int) override { }

    void setVertexShaderPath(const std::string &vertexShaderPath);
    void setFragmentShaderPath(const std::string &fragmentShaderPath);

    vmtkManagementShader& getSliceShaderProgram();

    bool isInitialized() const;

    void setMVPMatrix(const glm::mat4 &mvpMatrix);

    void setPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);
    void setPlaneTex(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4);

private:
    /**
   * @brief creates vertex arrays and buffer obects
   */
    void createBuffers();

    /**
   * @brief initializes the geometry of the plane to be rendered.
   */
    void initPlane();

    void updatePlaneBuffer();

    /**
   * @brief draws the plane
   */
    void drawPlane();

    virtual void configureTextureUnits() = 0;

    vmtkManagementShader *m_spPlane;  /**< shader for slice rendering */

    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    bool m_isInitialized;

    GLuint m_vaosPlane;        /**< plane: vertex array object */
    GLuint m_vboPlane;         /**< plane: buffer data */
    GLuint m_eboPlane;         /**< plane: element array object */

    glm::mat4 m_mvpMatrix;

    glm::vec3 m_v1;           /**< position coordinates of v1 */
    glm::vec3 m_v2;           /**< position coordinates of v2 */
    glm::vec3 m_v3;           /**< position coordinates of v3 */
    glm::vec3 m_v4;           /**< position coordinates of v4 */

    glm::vec3 m_vt1;          /**< texture coordinates of v1 */
    glm::vec3 m_vt2;          /**< texture coordinates of v2 */
    glm::vec3 m_vt3;          /**< texture coordinates of v3 */
    glm::vec3 m_vt4;          /**< texture coordinates of v4 */

    /**
   * @struct VertexData
   */
    struct VertexData
    {
        glm::vec3 position; /**< position */
        glm::vec3 texCoord; /**< texCoord */
    };
};

#endif // BASE_SLICE_RENDERER_H
