#include "ROIRenderer2D.h"

#include <utilities/QManagementResources.h>
#include <utilities/cursor/FreeDrawCursor.h>

ROIRenderer2D::ROIRenderer2D()
{
    m_isInitialized = false;
    m_isROIDrawingActivated = false;
    m_freeDrawCursor = nullptr;

    m_currentTractID = -1;

    m_modelViewMatrix = glm::mat4(1.0f);
    m_projectionMatrix = glm::mat4(1.0f);

    m_slice = 0;
}

void ROIRenderer2D::initialize()
{
    if(m_isInitialized)
        return;

    QManagementResource *vs_free_draw = new QManagementResource;
    QManagementResource *fs_free_draw = new QManagementResource;

    GLuint vertex_free_draw_lines = vs_free_draw->compileResourceFileShader(QManagementResource::vertex, ":/primitive_freelines_vert");
    GLuint fragment_free_draw_lines = fs_free_draw->compileResourceFileShader(QManagementResource::fragment, ":/primitive_freelines_frag");
    vmtkManagementShader *free_draw_Objects;  /**< render the cursor ROI*/
    free_draw_Objects = new vmtkManagementShader;
    free_draw_Objects->setShader(vertex_free_draw_lines);
    free_draw_Objects->setShader(fragment_free_draw_lines);
    free_draw_Objects->linking();

    m_freeDrawCursor = new FreeDrawCursor();
    m_freeDrawCursor->setShaderProgram(free_draw_Objects);
    m_freeDrawCursor->initFreeDrawCursor();

    delete vs_free_draw;
    delete fs_free_draw;

    m_isInitialized = true;
}

void ROIRenderer2D::render()
{
    if(isROIDrawingActivated())
    {
        if(m_isInitialized)
        {
            m_freeDrawCursor->setModelViewMatrix(m_modelViewMatrix);
            m_freeDrawCursor->setProjectionMatrix(m_projectionMatrix);

            m_freeDrawCursor->drawFreeDrawCursor(m_slice, m_currentTractID);
        }
    }
}

void ROIRenderer2D::resize(int /*w*/, int /*h*/)
{

}

void ROIRenderer2D::setROIDrawingActivated(bool state)
{
    m_isROIDrawingActivated = state;

    if(!m_isInitialized)
        initialize();

    m_freeDrawCursor->setEnableFreeDrawCursor(m_isROIDrawingActivated);
}

bool ROIRenderer2D::isROIDrawingActivated() const
{
    return m_isROIDrawingActivated;
}

void ROIRenderer2D::setROISlice(int id, unsigned int slice)
{
    m_freeDrawCursor->setSliceCurrentActiveFreeDrawCursor(id,slice);
}

void ROIRenderer2D::setROIColor(int id, glm::vec4 color)
{
    m_freeDrawCursor->setColorFreeDrawCursor(id, color);
}

void ROIRenderer2D::addPoint(glm::vec3 point)
{
    m_freeDrawCursor->addPoint(point);
}

void ROIRenderer2D::setCurrentActiveROI(int id)
{
    m_freeDrawCursor->setCurrentActiveFreeDrawCursor(id);
}

void ROIRenderer2D::removeROI(int id)
{
    m_freeDrawCursor->removeFreeDrawCursor(id);
}

void ROIRenderer2D::setCurrentSlice(unsigned short slice)
{
    m_slice = slice;
}

void ROIRenderer2D::addNewROI(int id, int roid_id, glm::vec4 color)
{
    m_freeDrawCursor->addFreeDrawCursor(id, roid_id, m_slice, color);
}

void ROIRenderer2D::setModelViewMatrix(const glm::mat4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}

void ROIRenderer2D::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

void ROIRenderer2D::viewportChanged(int x, int y, int width, int height)
{
    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;
}

int *ROIRenderer2D::getViewport()
{
    return m_viewport;
}

void ROIRenderer2D::setCurrentTractID(int currentTractID)
{
    m_currentTractID = currentTractID;
}
