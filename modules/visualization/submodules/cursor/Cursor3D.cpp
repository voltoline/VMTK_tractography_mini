#include "Cursor3D.h"

Cursor3D::Cursor3D()
{
    m_cursorCylinder = new Cylinder();
    m_cursorRadius = new Sphere();
    m_cursorLine = new LineP2P();
    m_enableCursor = false;
    m_enableFixedAlpha = false;
    m_idDepthTexture = 0;
    m_idDepthBackTexture = 0;
}

void Cursor3D::setShaderProgram(vmtkManagementShader *shaderProgram){
    m_cursorCylinder->setShaderProgram(shaderProgram);
    m_cursorRadius->setShaderProgram(shaderProgram);
    m_cursorLine->defaultResourceFile();
}

void Cursor3D::setEnableCursor(bool enable)
{
    m_enableCursor = enable;
}

glm::mat4 Cursor3D::getProjectionMatrix()
{
    return m_projectionMatrix;
}


void Cursor3D::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

void Cursor3D::initCursor3D()
{
    initCrossCursor();
    initRadiusCursor();
    initLineCursor();
}

void Cursor3D::initCrossCursor(){
    m_cursorCylinder->addCylindersDefaultForAll(6, 0.01f, 0.06f, 24 ,glm::vec4(1.0f,0.0f,0.0f,1.0f));
    m_cursorCylinder->setInitialPositionForEach(   0, glm::vec3(-0.05, 0.0, -0.0));
    m_cursorCylinder->setInitialRotateForEach(     0, glm::vec3(0.0f, 0.0f, 90.0f) );
    m_cursorCylinder->setInitialPositionForEach(   1, glm::vec3(0.05, 0.0, 0.0));
    m_cursorCylinder->setInitialRotateForEach(     1, glm::vec3(0.0f, 0.0f, 90.0f) );
    m_cursorCylinder->setInitialPositionForEach(   2, glm::vec3(0.0, 0.05, 0.0) );
    m_cursorCylinder->setInitialPositionForEach(   3, glm::vec3(0.0, -0.05, 0.0) );
    m_cursorCylinder->setInitialPositionForEach(   4, glm::vec3(0.00, 0.0, -0.05) );
    m_cursorCylinder->setInitialRotateForEach(     4, glm::vec3(90.0f, 0.0f, 0.0f) );
    m_cursorCylinder->setInitialPositionForEach(   5, glm::vec3(0.00, 0.0, 0.05) );
    m_cursorCylinder->setInitialRotateForEach(     5, glm::vec3(90.0f, 0.0f, 0.0f) );
}

void Cursor3D::initRadiusCursor()
{
    m_cursorRadius->initializePrimitive();
    m_cursorRadius->addFirstSphereCustomized(0.01,24,24,glm::vec4(1.0f,0.0f,0.0f,1.0f));
    m_cursorRadius->setInitialPositionForAll(glm::vec3(0.0, 0.0, 0.0));
}

void Cursor3D::initLineCursor()
{
    m_cursorLine->initializePrimitive();
    m_cursorLine->addLineP2P(glm::vec3(-1.f,0.f,0.f),glm::vec3(1.f,0.f,0.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: X
    m_cursorLine->addLineP2P(glm::vec3(0.f,-1.f,0.f),glm::vec3(0.f,1.f,0.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: Y
    m_cursorLine->addLineP2P(glm::vec3(0.f,0.f,-1.f),glm::vec3(0.f,0.f,1.f), glm::vec4(1.0f,0.0f,0.0f,1.0f)); // Axis: Z
}

void Cursor3D::setDepthTexture(GLuint idDepthTexture)
{
     m_idDepthTexture = idDepthTexture;
}

void Cursor3D::setDepthBackTexture(GLuint idDepthBackTexture)
{
    m_idDepthBackTexture = idDepthBackTexture;
}

void Cursor3D::setEnableFixedAlpha(bool enableFixedAlpha)
{
    m_enableFixedAlpha = enableFixedAlpha;
}

void Cursor3D::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    m_drawCursorType = newDrawCursorType;
}

void Cursor3D::drawCrossCursor(glm::vec3 pos){
    m_cursorCylinder->bindingRender();
    m_cursorCylinder->setIDTextureActivated(0);
    m_cursorCylinder->setTranslateForAll( pos );
    m_cursorCylinder->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
    m_cursorCylinder->setDepthTexture(m_idDepthTexture);
    m_cursorCylinder->setDepthTextureBack(m_idDepthBackTexture);
    m_cursorCylinder->geometryRender( ModeTypeFilled::solid );
    m_cursorCylinder->releasingRender();

}

void Cursor3D::drawRadiusCursor(glm::vec3 pos)
{
    m_cursorRadius->bindingRender();
    m_cursorRadius->setIDTextureActivated(0);
    m_cursorRadius->setTranslateForAll( pos );
    m_cursorRadius->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
    m_cursorRadius->setDepthTexture(m_idDepthTexture);
    m_cursorRadius->setDepthTextureBack(m_idDepthBackTexture);
    m_cursorRadius->primitiveRender( );
    m_cursorRadius->releasingRender();
}

void Cursor3D::drawLineCursor(glm::vec3 pos)
{
    m_cursorLine->bindingRender();
    m_cursorLine->setIDTextureActivated(0);
    m_cursorLine->setTranslateForEach(0, glm::vec3(0.f,pos.y,pos.z) ); // not move to X axis
    m_cursorLine->setTranslateForEach(1, glm::vec3(pos.x,0.f,pos.z) ); // not move to X axis
    m_cursorLine->setTranslateForEach(2, glm::vec3(pos.x,pos.y,0.f) ); // not move to X axis
    m_cursorLine->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix );
    m_cursorLine->setDepthTexture(m_idDepthTexture);
    m_cursorLine->setDepthBackTexture(m_idDepthBackTexture);
    m_cursorLine->primitiveRender();
    m_cursorLine->releasingRender();
}


void Cursor3D::drawCursor3D(glm::vec3 pos)
{
    if(!m_enableCursor){return;}
    if( m_drawCursorType == DrawCursorType::invisibleCursor){ return; }

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (m_drawCursorType) {
    case DrawCursorType::crossCursor:
        drawCrossCursor(pos);
        break;
    case DrawCursorType::radiusCursor:
        drawRadiusCursor(pos);
        break;
    case DrawCursorType::lineCursor:
        drawLineCursor(pos);
        break;
    case DrawCursorType::invisibleCursor:
        break;
    default:
        drawCrossCursor(pos);
        break;
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

}

void Cursor3D::setModelViewMatrix(glm::mat4 modelViewMatrix) {
    m_modelViewMatrix = modelViewMatrix;
}

glm::mat4 Cursor3D::getModelViewMatrix() {
    return m_modelViewMatrix;
}
