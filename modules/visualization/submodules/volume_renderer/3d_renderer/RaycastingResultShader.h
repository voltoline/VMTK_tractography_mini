#ifndef RAYCASTING_RESULT_SHADER_H
#define RAYCASTING_RESULT_SHADER_H

#include "utilities/QCheckingOpenGL.h"
#include <glm/glm.hpp>

class vmtkManagementShader;

/*!
 * \brief Transfer the raycasted rendered image to a plane of an arbitrary size (resolution)
 */
class RaycastingResultShader
{
public:
    RaycastingResultShader();
    ~RaycastingResultShader();

    void initialize();
    void execute(GLuint tex_to_render);
    void resize(int width, int height);

private:

    /**
     * @brief initializes the geometry of a (render) plane which is texturized with the final image.
     */
    void initResultPlane();

    GLuint m_vaosResultPlane;      /**< render: vertex array object */
    GLuint m_vboResultPlane;          /**< render: data buffer */
    GLuint m_eboResultPlane;          /**< render: element array buffer */

    vmtkManagementShader *m_resultShader;       /**< displaying shader */
};

#endif // RAYCASTING_RESULT_SHADER_H
