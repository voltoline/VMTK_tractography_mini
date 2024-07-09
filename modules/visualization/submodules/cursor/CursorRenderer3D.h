#ifndef CURSOR_RENDERER_3D_H
#define CURSOR_RENDERER_3D_H

#include <glm/glm.hpp>

#include <modules/visualization/submodules/volume_renderer/3d_renderer/RaycastingVolumeRenderer3D.h>
#include <modules/visualization/submodules/volume_renderer/3d_renderer/RaycastingShader.h>
#include <modules/visualization/submodules/volume_renderer_reg/ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.h>
#include "modules/visualization/submodules/cursor/ModelCursor.h"

class Cursor3D;

class CursorRenderer3D: public SubRendererBase
{
public:
    CursorRenderer3D();

    void initialize() override;
    void render() override;
    void resize(int w, int h) override;

    Cursor3D *m_cursor3D;

    void setEnableCursor(bool enableCursor);
    void setCursorPosition3D(const glm::vec3 &cursorPosition3D);
    void setProjectionMatrix(glm::mat4 matrix);
    void setModelViewMatrix(glm::mat4 matrix);

    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D);

    void viewportChanged(int x, int y, int width, int height) override;
    int* getViewport();

    void setDrawCursorType(DrawCursorType newDrawCursorType);

private:
    bool m_enableCursor;
    bool m_isInitialized;
    DrawCursorType m_drawCursorType;

    int m_width;
    int m_height;

    glm::vec3 m_cursorPosition3D;

    glm::mat4 m_projectionMatrix;   /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;    /**< modelview transformation matrix */

    int m_viewport[4]; /**< viewport for rendered slice */

    BaseRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;
};

#endif // CURSORRENDERER3D_H
