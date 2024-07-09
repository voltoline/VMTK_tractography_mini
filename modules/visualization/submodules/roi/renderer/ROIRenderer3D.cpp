#include "ROIRenderer3D.h"

#include <utilities/QManagementResources.h>
#include "utilities/cursor/FreeDrawCursor.h"

ROIRenderer3D::ROIRenderer3D()
{
    m_enabeFreeDraw = false;
    m_freeLineDraw = nullptr;

    m_currentTractID = -1;

    m_w = -1;                     /**< viewport width */
    m_h = -1;
}

void ROIRenderer3D::initialize()
{
    QManagementResource *vs_free_line = new QManagementResource;
    QManagementResource *fs_free_line = new QManagementResource;

    //Free Draw Line ROI
    GLuint vertex_free_draw = vs_free_line->compileResourceFileShader(QManagementResource::vertex, ":/primitive_freelines_vert");
    GLuint fragment_free_draw = fs_free_line->compileResourceFileShader(QManagementResource::fragment, ":/primitive_freelines_frag");
    vmtkManagementShader *freedrawObjects;  /**< render the Free Lines cursor 3D*/
    freedrawObjects = new vmtkManagementShader;
    freedrawObjects->setShader(vertex_free_draw);
    freedrawObjects->setShader(fragment_free_draw);
    freedrawObjects->linking();
    m_freeLineDraw = new FreeDrawCursor();
    m_freeLineDraw->setShaderProgram(freedrawObjects); // initial shader program
    m_freeLineDraw->initFreeDrawCursor();

    delete vs_free_line;
    delete fs_free_line;
}

void ROIRenderer3D::render()
{
    drawFreeDraw3Dfrom3D();
}

void ROIRenderer3D::resize(int width, int height)
{
    m_w = width;
    m_h = height;
}

void ROIRenderer3D::setEnableFreeDraw(bool enable)
{
    m_enabeFreeDraw = enable;
    m_freeLineDraw->setEnableFreeDrawCursor(enable);
}

void ROIRenderer3D::makeFreeDraw3D()
{
    float m_projLeft = -1.3f, m_projBottom = -1.3f;
    float m_projRight = 1.3f, m_projTop = 1.3f;

    m_freeLineDraw->setModelViewMatrix(m_modelViewMatrix);
    vgf::resetViewportToSquare(m_w,m_h);
    glm::mat4 pm = vgf::makeNearProjectionMatrix(m_modelViewMatrix, m_projLeft, m_projRight, m_projBottom, m_projTop);
    m_freeLineDraw->setProjectionMatrix(pm);
    m_freeLineDraw->drawFreeDrawCursor(0, m_currentTractID);
}

void ROIRenderer3D::drawFreeDraw3Dfrom3D()
{
    if(m_enabeFreeDraw){
        makeFreeDraw3D();
    }
}

void ROIRenderer3D::setCurrentTractID(int currentTractID)
{
    m_currentTractID = currentTractID;
}

void ROIRenderer3D::addNewPointFreeDraw3D(glm::vec3 point)
{
    m_freeLineDraw->addPoint(point);
}

void ROIRenderer3D::addNewFreeDraw3D(int id, int tract_id)
{
    m_freeLineDraw->addFreeDrawCursor(id, tract_id, 0, glm::vec4(1.0, 0.0, 0.0, 1.0));
}

void ROIRenderer3D::removeFreeDraw3D(int index)
{
    m_freeLineDraw->removeFreeDrawCursor(index);
}

void ROIRenderer3D::setCurrentActiveFreeDraw3D(int index)
{
    m_freeLineDraw->setCurrentActiveFreeDrawCursor(index);
}

void ROIRenderer3D::setModelViewMatrix(glm::mat4 modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}
