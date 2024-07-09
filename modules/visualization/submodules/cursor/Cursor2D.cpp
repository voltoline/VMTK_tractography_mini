#include "Cursor2D.h"

Cursor2D::Cursor2D()
{
    m_cursorCross = new Rectangle_Square();
    m_cursorRadius = new Circle();
    m_cursorLine = new LineP2P();
    m_enableCursor = false;
}

void Cursor2D::setShaderProgram(vmtkManagementShader *shaderProgram){
    m_cursorCross->setShaderProgram(shaderProgram);
    m_cursorRadius->setShaderProgram(shaderProgram);
    m_cursorLine->defaultResourceFile();
}

void Cursor2D::setEnableCursor(bool enable)
{
    m_enableCursor = enable;
}


glm::mat4 Cursor2D::getProjectionMatrix()
{
    return m_projectionMatrix;
}

void Cursor2D::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

glm::vec3 Cursor2D::getPos() const
{
    return m_pos;
}

void Cursor2D::setPos(const glm::vec3 &pos)
{
    m_pos = pos;
}

void Cursor2D::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    m_drawCursorType = newDrawCursorType;
}

void Cursor2D::initCrossCursor()
{
    m_cursorCross->addRectanglesDefaultForAll(4, 0.05f, 0.01f, glm::vec4(1.0f,0.0f,0.0f,1.0f));
    m_cursorCross->setInitialPositionForEach(   0, glm::vec3(-0.09,  0.0 , -0.0));
    m_cursorCross->setInitialPositionForEach(   1, glm::vec3( 0.09,  0.0 ,  0.0));
    m_cursorCross->setInitialPositionForEach(   2, glm::vec3( 0.0 ,  0.09,  0.0));
    m_cursorCross->setInitialRotateForEach(     2, glm::vec3( 0.0 ,  0.0 , 90.0) );
    m_cursorCross->setInitialPositionForEach(   3, glm::vec3( 0.0 , -0.09,  0.0));
    m_cursorCross->setInitialRotateForEach(     3, glm::vec3( 0.0 ,  0.0 , 90.0) );
}

void Cursor2D::initRadiusCursor()
{
    m_cursorRadius->initializePrimitive(Circle::TypeFilled::points);
    m_cursorRadius->addFirstCircleSettingResolution( 0.04,36,glm::vec4(1.0f,0.0f,0.0f,1.0f) );
    m_cursorRadius->setInitialPositionForAll(glm::vec3(0.0, 0.0, 0.0));
}

void Cursor2D::initLineCursor()
{
    m_cursorLine->initializePrimitive();
    m_cursorLine->addLineP2P(glm::vec3(-1.f,0.f,0.f),glm::vec3(1.f,0.f,0.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: X
    m_cursorLine->addLineP2P(glm::vec3(0.f,-1.f,0.f),glm::vec3(0.f,1.f,0.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: Y
    m_cursorLine->addLineP2P(glm::vec3(0.f,0.f,-1.f),glm::vec3(0.f,0.f,1.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: Z
}


void Cursor2D::drawCrossCursor()
{
    if(m_cursorCross->bindingRender()){
        m_cursorCross->setIDTextureActivated(0);
        m_cursorCross->setTranslateForAll( m_pos );
        m_cursorCross->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
        m_cursorCross->geometryRender();
        m_cursorCross->releasingRender();
    }
}

void Cursor2D::drawRadiusCursor()
{
    m_cursorRadius->bindingRender();
    m_cursorRadius->setIDTextureActivated(0);
    m_cursorRadius->setTranslateForAll( m_pos );
    m_cursorRadius->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
    m_cursorRadius->primitiveRender();
    m_cursorRadius->releasingRender();
}

void Cursor2D::drawLineCursor()
{
    m_cursorLine->bindingRender();
    m_cursorLine->setIDTextureActivated(0);
    m_cursorLine->setTranslateForEach(0, glm::vec3(0.f,m_pos.y,m_pos.z) ); // not move to X axis
    m_cursorLine->setTranslateForEach(1, glm::vec3(m_pos.x,0.f,m_pos.z) ); // not move to X axis
    m_cursorLine->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
    m_cursorLine->primitiveRender();
    m_cursorLine->releasingRender();
}

void Cursor2D::initCursor()
{
    initCrossCursor();
    initRadiusCursor();
    initLineCursor();
}

void Cursor2D::drawCursor()
{
    if(!m_enableCursor){return;}
    if( m_drawCursorType == DrawCursorType::invisibleCursor){ return; }

    if(glIsEnabled(GL_CULL_FACE)){ glDisable(GL_CULL_FACE); }
    if(!glIsEnabled(GL_DEPTH_TEST)){ glEnable(GL_DEPTH_TEST); }
    if(!glIsEnabled(GL_BLEND)){ glEnable(GL_BLEND); }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (m_drawCursorType) {
    case DrawCursorType::crossCursor:
        drawCrossCursor();
        break;
    case DrawCursorType::radiusCursor:
        drawRadiusCursor();
        break;
    case DrawCursorType::lineCursor:
        drawLineCursor();
        break;
    case DrawCursorType::invisibleCursor:
        break;
    default:
        drawCrossCursor();
        break;
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void Cursor2D::setModelViewMatrix(glm::mat4 modelViewMatrix) {
    m_modelViewMatrix = modelViewMatrix;
}

glm::mat4 Cursor2D::getModelViewMatrix() {
    return m_modelViewMatrix;
}
