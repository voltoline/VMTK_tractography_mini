#include "CursorRenderer3D.h"

#include "Cursor3D.h"

CursorRenderer3D::CursorRenderer3D()
{
    m_enableCursor = false;
    m_isInitialized = false;
    m_cursor3D = nullptr;

    m_raycastingVolumeRenderer3D = nullptr;
}

void CursorRenderer3D::initialize()
{
    if(m_enableCursor)
    {
        if(m_isInitialized)
            return;

        QManagementResource *vs = new QManagementResource;
        QManagementResource *fs = new QManagementResource;

        GLuint vertex_cursor = vs->compileResourceFileShader(QManagementResource::vertex, ":/primitive_object_vert");
        GLuint fragment_cursor = fs->compileResourceFileShader(QManagementResource::fragment, ":/primitive_object_frag");
        vmtkManagementShader *spObjects;  /**< render the cursor 3D*/
        spObjects = new vmtkManagementShader;
        spObjects->setShader(vertex_cursor);
        spObjects->setShader(fragment_cursor);
        spObjects->linking();

        m_cursor3D = new Cursor3D();
        m_cursor3D->setShaderProgram(spObjects); // initial shader program
        m_cursor3D->initCursor3D();
        m_cursor3D->setEnableCursor(m_enableCursor);
        m_cursor3D->setModelViewMatrix(m_modelViewMatrix);
        m_cursor3D->setProjectionMatrix(m_projectionMatrix);

        delete vs;
        delete fs;

        m_isInitialized = true;
    }
}

void CursorRenderer3D::render()
{
    if(m_enableCursor){
        vgf::resetViewportToSquare(m_width, m_height);

        if(m_raycastingVolumeRenderer3D)
        {
            if(m_raycastingVolumeRenderer3D->getRaycastingShader() != nullptr)
                m_cursor3D->setDepthTexture(m_raycastingVolumeRenderer3D->getRaycastingShader()->getRayTracedFBO().getDepthTexture());

            if(m_raycastingVolumeRenderer3D->getRaycastingBackShader() != nullptr)
                m_cursor3D->setDepthBackTexture(m_raycastingVolumeRenderer3D->getRaycastingBackShader()->getRayTracedFBO().getDepthTexture());

        }
        m_cursor3D->drawCursor3D(m_cursorPosition3D);
    }
}

void CursorRenderer3D::setModelViewMatrix(glm::mat4 matrix)
{
    m_modelViewMatrix = matrix;

    if(m_cursor3D)
        m_cursor3D->setModelViewMatrix(matrix);
}

void CursorRenderer3D::setProjectionMatrix(glm::mat4 matrix)
{
    m_projectionMatrix = matrix;

    if(m_cursor3D)
        m_cursor3D->setProjectionMatrix(matrix);
}

void CursorRenderer3D::resize(int w, int h)
{
    m_width = w;
    m_height = h;
}

void CursorRenderer3D::setCursorPosition3D(const glm::vec3 &cursorPosition3D)
{
    m_cursorPosition3D = cursorPosition3D;
}

void CursorRenderer3D::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D)
{
    m_raycastingVolumeRenderer3D = raycastingVolumeRenderer3D;
}

void CursorRenderer3D::setEnableCursor(bool enableCursor)
{
    m_enableCursor = enableCursor;

    if(m_enableCursor && !m_isInitialized)
        initialize();

    if(m_cursor3D)
        m_cursor3D->setEnableCursor(enableCursor);

    if(m_raycastingVolumeRenderer3D)
        m_raycastingVolumeRenderer3D->setEnableRayCastingBack(enableCursor);
}

void CursorRenderer3D::viewportChanged(int x, int y, int width, int height)
{
    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;
}

int* CursorRenderer3D::getViewport()
{
    return m_viewport;
}

void CursorRenderer3D::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    m_drawCursorType = newDrawCursorType;
    m_cursor3D->setDrawCursorType(m_drawCursorType);
}
