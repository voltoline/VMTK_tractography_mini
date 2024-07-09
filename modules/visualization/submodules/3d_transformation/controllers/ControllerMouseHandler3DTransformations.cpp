#include "ControllerMouseHandler3DTransformations.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ui/components/RenderingSurfaceSet.h"

ControllerMouseHandler3DTransformations::ControllerMouseHandler3DTransformations()
{
    m_mouseMode = MouseMode::ROTATION;
    m_model3DTransformation = nullptr;
    m_renderingSurfaceSet = nullptr;
}

void ControllerMouseHandler3DTransformations::mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT)
        return;

    m_mousePressPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

    renderingSurface->updateRendering();
}

void ControllerMouseHandler3DTransformations::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT)
        return;

    m_mouseCurrentPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

    handleMouseMovement();
    m_mousePressPosition = m_mouseCurrentPosition;

    renderingSurface->updateRendering();

    m_renderingSurfaceSet->setFocus3D();
}

void ControllerMouseHandler3DTransformations::mouseReleaseEvent(MouseEvent &/*mouse_event*/, RenderingSurface */*renderingSurface*/)
{
    m_renderingSurfaceSet->setFocus3D();
}

void ControllerMouseHandler3DTransformations::handleMouseMovement()
{
    switch(m_mouseMode)
    {
    case MouseMode::ROTATION:
        createRotationMatrix();
        break;

    case MouseMode::SCALING:
        createScalingMatrix();
        break;

    case MouseMode::TRANSLATION:
        createTranslationMatrix();
        break;

    default:
        break;
    }
}

void ControllerMouseHandler3DTransformations::createRotationMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    glm::vec3 n = glm::vec3(diff[1], diff[0], 0.0f);
    n = glm::normalize(n);
    glm::vec3 rotationAxis = n;
    glm::quat mQuaternionRotation = glm::angleAxis(glm::radians(3.0f), rotationAxis);
    if(std::isnan(mQuaternionRotation.w) || std::isnan(mQuaternionRotation.x) || std::isnan(mQuaternionRotation.y) || std::isnan(mQuaternionRotation.z) )
        mQuaternionRotation = glm::quat(1,0,0,0);

    glm::mat4 mRotationMatrix = glm::toMat4(mQuaternionRotation) * m_model3DTransformation->getRotationMatrix();

    m_model3DTransformation->setRotationMatrix(mRotationMatrix);
    m_model3DTransformation->notify();
}

void ControllerMouseHandler3DTransformations::createScalingMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    float zoom_factor = (float) (1.0f + diff.y / 500.f);
    if(zoom_factor < 0.f)
        return;

    glm::mat4 temp_scalationMatrix = glm::scale(glm::mat4(1.0), glm::vec3(float(zoom_factor)));
    glm::mat4 mScalingMatrix = temp_scalationMatrix * m_model3DTransformation->getScalingMatrix();

    m_model3DTransformation->setScalingMatrix(mScalingMatrix);
    m_model3DTransformation->notify();
}

void ControllerMouseHandler3DTransformations::createTranslationMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    glm::mat4 temp_translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(diff.x / 100, -diff.y / 100, 0.0f));
    glm::mat4 mTranslationMatrix = temp_translationMatrix * m_model3DTransformation->getTranslationMatrix();

    m_model3DTransformation->setTranslationMatrix(mTranslationMatrix);
    m_model3DTransformation->notify();
}

void ControllerMouseHandler3DTransformations::setMouseMode(const MouseMode &mouseMode)
{
    m_mouseMode = mouseMode;
}

void ControllerMouseHandler3DTransformations::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void ControllerMouseHandler3DTransformations::setModel3DTransformation(Model3DTransformation *model3DTransformation)
{
    m_model3DTransformation = model3DTransformation;
}
