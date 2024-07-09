#include "EventHandler3DTransformations.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../controllers/ControllerMouseHandler3DTransformations.h"
#include "ui/components/RenderingSurfaceSet.h"

EventHandler3DTransformations::EventHandler3DTransformations()
{
    m_model3DTransformation = nullptr;
    m_model3dRendering = nullptr;
    m_renderingSurfaceSet = nullptr;
    m_controllerMouseHandler3DTransformations = nullptr;
}

void EventHandler3DTransformations::update(Model3DTransformation *, Model3DTransformationEvent event_type)
{
    switch(event_type)
    {

    case Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED:
    {
        Model3DRenderingEvent event_2 = m_model3dRendering->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix(true));
        event_2 = m_model3dRendering->setProjectionMatrix(m_model3DTransformation->getProjectionMatrix(true));
        m_model3dRendering->notify(event_2);

        m_renderingSurfaceSet->update3DRendering();
        break;
    }

    case Model3DTransformationEvent::MOUSE_MODE_CHANGED:
    {
        if(m_controllerMouseHandler3DTransformations)
        {
            if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::MOVE)
                m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::TRANSLATION);

            if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ROTATION)
                m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::ROTATION);

            if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ZOOM)
                m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::SCALING);
        }

        break;
    }

    case Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED:
    {
        if(m_controllerMouseHandler3DTransformations)
        {
            if(m_model3DTransformation->isTransformationActivated())
                m_renderingSurfaceSet->getRenderingSurface3D()->addController(m_controllerMouseHandler3DTransformations);
            else
                m_renderingSurfaceSet->getRenderingSurface3D()->removeController(m_controllerMouseHandler3DTransformations);
        }

        break;
    }

    default:
        break;

    }
}

void EventHandler3DTransformations::setControllerMouseHandler3DTransformations(ControllerMouseHandler3DTransformations *controllerMouseHandler3DTransformations)
{
    m_controllerMouseHandler3DTransformations = controllerMouseHandler3DTransformations;
}

void EventHandler3DTransformations::setModel3dRendering(Model3DRendering *model3dRendering)
{
    m_model3dRendering = model3dRendering;
}

void EventHandler3DTransformations::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandler3DTransformations::setModel3DTransformation(Model3DTransformation *model3DTransformation)
{
    m_model3DTransformation = model3DTransformation;

    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::MOUSE_MODE_CHANGED);
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
}
