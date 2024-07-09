#ifndef DEPTH_RENDERER_H
#define DEPTH_RENDERER_H

#include "utilities/QCheckingOpenGL.h"
#include <glm/glm.hpp>

class vmtkManagementShader;

class DepthRenderer
{
public:
    DepthRenderer();
    ~DepthRenderer();

    void initialize();
    void execute(GLuint tex_to_render);
    void resize(int width, int height);

private:

    /**
     * @brief initializes the geometry of a (render) plane which is texturized with the depth texture.
     */
    void initResultPlane();

    GLuint m_vaoPlane;      /**< render: vertex array object */
    GLuint m_vboPlane;          /**< render: data buffer */
    GLuint m_eboPlane;          /**< render: element array buffer */

    vmtkManagementShader *m_planeShader;       /**< displaying shader */
};

#endif // DEPTH_RENDERER_H
