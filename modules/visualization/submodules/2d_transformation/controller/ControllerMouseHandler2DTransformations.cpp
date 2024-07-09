#include "ControllerMouseHandler2DTransformations.h"

#include "ui/components/RenderingSurfaceSet.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

ControllerMouseHandler2DTransformations::ControllerMouseHandler2DTransformations(View view)
{
    m_view = view;
    m_mouseMode = MouseMode::ROTATION;

    m_model2DTransformation = nullptr;
    m_renderingSurfaceSet = nullptr;
}

void ControllerMouseHandler2DTransformations::mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT)
    {
        m_mousePressPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

        renderingSurface->updateRendering();
    }
}

void ControllerMouseHandler2DTransformations::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT)
    {
        m_mouseCurrentPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

        handleMouseMovement();

        renderingSurface->updateRendering();

        m_setFocus();
    }
}

void ControllerMouseHandler2DTransformations::mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT)
    {
        renderingSurface->updateRendering();

        m_setFocus();
    }
}

void ControllerMouseHandler2DTransformations::handleMouseMovement()
{
    switch (m_mouseMode)
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

void ControllerMouseHandler2DTransformations::createRotationMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    glm::vec3 n = glm::vec3(0.0f, 0.0f, diff[0]);
    n = glm::normalize(n);
    glm::vec3 rotationAxis = n;
    glm::quat tempQuaternionRotation = glm::angleAxis(glm::radians(3.0f), rotationAxis);
    if(std::isnan(tempQuaternionRotation.w) || std::isnan(tempQuaternionRotation.x) || std::isnan(tempQuaternionRotation.y) || std::isnan(tempQuaternionRotation.z) )
        tempQuaternionRotation = glm::quat(1,0,0,0);

    glm::mat4 mRotationMatrix = glm::toMat4(tempQuaternionRotation) * m_getTransformationData().getRotationMatrix();
    m_mousePressPosition = m_mouseCurrentPosition;

    Model2DTransformationEvent event = m_getTransformationData().setRotationMatrix(mRotationMatrix);
    m_model2DTransformation->notify(event);
}

void ControllerMouseHandler2DTransformations::createScalingMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    float zoom_factor = 1.0f + diff.y / 500.f;
    if(zoom_factor < 0.f)
        return;

    glm::mat4 tempScalationMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoom_factor));
    glm::mat4 mScalingMatrix = tempScalationMatrix * m_getTransformationData().getScalingMatrix();

    m_mousePressPosition = m_mouseCurrentPosition; //Update old mouse pos

    Model2DTransformationEvent event = m_getTransformationData().setScalingMatrix(mScalingMatrix);
    m_model2DTransformation->notify(event);
}

void ControllerMouseHandler2DTransformations::createTranslationMatrix()
{
    glm::vec2 diff = m_mouseCurrentPosition - m_mousePressPosition;
    glm::mat4 tempTranslationMatrix = glm::translate(glm::mat4(1.0),  glm::vec3(diff.x / 100.f, -diff.y / 100.f, 0.0f));

    glm::mat4 mTranslationMatrix = tempTranslationMatrix * m_getTransformationData().getTranslationMatrix();
    m_mousePressPosition = m_mouseCurrentPosition;

    Model2DTransformationEvent event = m_getTransformationData().setTranslationMatrix(mTranslationMatrix);
    m_model2DTransformation->notify(event);
}

void ControllerMouseHandler2DTransformations::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;

    if(isAxial())
        m_getTransformationData = [this]() -> Model2DTransformation::TransformationData& { return m_model2DTransformation->getAxialData(); };

    else if(isCoronal())
        m_getTransformationData = [this]() -> Model2DTransformation::TransformationData& { return m_model2DTransformation->getCoronalData(); };

    else
        m_getTransformationData = [this]() -> Model2DTransformation::TransformationData& { return m_model2DTransformation->getSagittalData(); };
}

void ControllerMouseHandler2DTransformations::setMouseMode(const MouseMode &mouseMode)
{
    m_mouseMode = mouseMode;
}

void ControllerMouseHandler2DTransformations::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;

    if(isAxial())
    {
        m_getRenderingSurface = [this]() -> RenderingSurface* { return m_renderingSurfaceSet->getRenderingSurfaceAxial(); };
        m_setFocus = [this](){ m_renderingSurfaceSet->setFocusAxial(); };

    } else if(isCoronal()) {

        m_getRenderingSurface = [this]() -> RenderingSurface* { return m_renderingSurfaceSet->getRenderingSurfaceCoronal(); };
        m_setFocus = [this](){ m_renderingSurfaceSet->setFocusCoronal(); };

    } else {

        m_getRenderingSurface = [this]() -> RenderingSurface* { return m_renderingSurfaceSet->getRenderingSurfaceSagittal(); };
        m_setFocus = [this](){ m_renderingSurfaceSet->setFocusSagittal(); };
    }
}

bool ControllerMouseHandler2DTransformations::isAxial()
{
    return m_view == ControllerMouseHandler2DTransformations::View::AXIAL;
}

bool ControllerMouseHandler2DTransformations::isCoronal()
{
    return m_view == ControllerMouseHandler2DTransformations::View::CORONAL;
}

bool ControllerMouseHandler2DTransformations::isSagittal()
{
    return m_view == ControllerMouseHandler2DTransformations::View::SAGITTAL;
}
