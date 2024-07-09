#ifndef CURSOR_RENDERER_2D_H
#define CURSOR_RENDERER_2D_H

#include <glm/glm.hpp>

#include <renderer/SubRendererBase.h>
#include "modules/visualization/submodules/cursor/ModelCursor.h"

class Cursor2D;

class CursorRenderer2D: public SubRendererBase
{
public:
    CursorRenderer2D();

    void initialize() override;
    void render() override;
    void resize(int, int) override { }

    void setEnabled(bool enable);
    void setPosition(glm::vec3 pos);

    void setModelViewMatrix(const glm::mat4 &modelViewMatrix);
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);

    void viewportChanged(int x, int y, int width, int height) override;
    int* getViewport();

    void setDrawCursorType(DrawCursorType newDrawCursorType);

private:
    Cursor2D *m_cursor2D;

    glm::mat4 m_modelViewMatrix;
    glm::mat4 m_projectionMatrix;

    int m_viewport[4]; /**< viewport for rendered slice */

    DrawCursorType m_drawCursorType;
};

#endif // CURSOR_RENDERER_2D_H
