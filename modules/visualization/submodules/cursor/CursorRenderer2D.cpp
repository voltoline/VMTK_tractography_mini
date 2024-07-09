#include "CursorRenderer2D.h"

#include "Cursor2D.h"
#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"

CursorRenderer2D::CursorRenderer2D()
{
    m_cursor2D = nullptr;
    m_cursor2D = new Cursor2D();
}


void CursorRenderer2D::initialize()
{
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    m_cursor2D->initCursor();
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void CursorRenderer2D::render()
{
        m_cursor2D->setModelViewMatrix(m_modelViewMatrix);
        m_cursor2D->setProjectionMatrix(m_projectionMatrix);
        m_cursor2D->drawCursor();
}

void CursorRenderer2D::setEnabled(bool enable)
{
    m_cursor2D->setEnableCursor(enable);
}

void CursorRenderer2D::setPosition(glm::vec3 pos)
{
    m_cursor2D->setPos(pos);
}

void CursorRenderer2D::setModelViewMatrix(const glm::mat4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}

void CursorRenderer2D::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

void CursorRenderer2D::viewportChanged(int x, int y, int width, int height)
{
    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;
}

int* CursorRenderer2D::getViewport()
{
    return m_viewport;
}

void CursorRenderer2D::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    m_drawCursorType = newDrawCursorType;
    m_cursor2D->setDrawCursorType(m_drawCursorType);
}
