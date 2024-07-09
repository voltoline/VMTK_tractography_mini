#include "EventHandler2DTransformations.h"

#include "ui/components/RenderingSurfaceSet.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ui/RenderingSurface.h"
#include <modules/visualization/submodules/volume_renderer/model/Model2DRendering.h>

EventHandler2DTransformations::EventHandler2DTransformations()
{
    m_isActivated = false;
    m_model2dRendering = nullptr;
    m_model2DTransformation = nullptr;
    m_model2DTransformationUIState = nullptr;
    m_renderingSurfaceSet = nullptr;

    m_controllerMouseHandler2DTransformationsAxial = nullptr;
    m_controllerMouseHandler2DTransformationsCoronal = nullptr;
    m_controllerMouseHandler2DTransformationsSagittal = nullptr;
}

void EventHandler2DTransformations::update(Model2DTransformation *, Model2DTransformationEvent event_type)
{
    switch(event_type)
    {
    case Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED:
    {
        glm::mat4 model_view_matrix = m_model2DTransformation->getAxialData().getModelViewMatrix();
        glm::mat4 projection_matrix = m_model2DTransformation->getAxialData().getProjectionMatrix();

        Model2DRenderingEvent event = m_model2dRendering->getAxialData().setModelViewMatrix(model_view_matrix);
        event = m_model2dRendering->getAxialData().setProjectionMatrix(projection_matrix);
        m_model2dRendering->notify(event);

        m_renderingSurfaceSet->updateAxialRendering();
        break;
    }

    case Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED:
    {
        glm::mat4 model_view_matrix = m_model2DTransformation->getCoronalData().getModelViewMatrix();
        glm::mat4 projection_matrix = m_model2DTransformation->getCoronalData().getProjectionMatrix();

        Model2DRenderingEvent event = m_model2dRendering->getCoronalData().setModelViewMatrix(model_view_matrix);
        event = m_model2dRendering->getCoronalData().setProjectionMatrix(projection_matrix);
        m_model2dRendering->notify(event);

        m_renderingSurfaceSet->updateSagittalRendering();
        break;
    }

    case Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED:
    {
        glm::mat4 model_view_matrix = m_model2DTransformation->getSagittalData().getModelViewMatrix();
        glm::mat4 projection_matrix = m_model2DTransformation->getSagittalData().getProjectionMatrix();

        Model2DRenderingEvent event = m_model2dRendering->getSagittalData().setModelViewMatrix(model_view_matrix);
        event = m_model2dRendering->getSagittalData().setProjectionMatrix(projection_matrix);
        m_model2dRendering->notify(event);

        m_renderingSurfaceSet->updateCoronalRendering();
        break;
    }

    case Model2DTransformationEvent::MOUSE_MODE_CHANGED:
    {
        switch(m_model2DTransformation->getMouseMode())
        {
        case Model2DTransformation::MouseMode::ROTATION:
        {
            setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::ROTATION);
            break;
        }

        case Model2DTransformation::MouseMode::ZOOM:
        {
            setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::SCALING);
            break;
        }

        case Model2DTransformation::MouseMode::MOVE:
        {
            setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::TRANSLATION);
            break;
        }

        default:
            break;
        }
    }

    default:
        break;
    }
}

void EventHandler2DTransformations::setMouseMode(ControllerMouseHandler2DTransformations::MouseMode mode)
{
    if(m_controllerMouseHandler2DTransformationsAxial)
        m_controllerMouseHandler2DTransformationsAxial->setMouseMode(mode);

    if(m_controllerMouseHandler2DTransformationsCoronal)
        m_controllerMouseHandler2DTransformationsCoronal->setMouseMode(mode);

    if(m_controllerMouseHandler2DTransformationsSagittal)
        m_controllerMouseHandler2DTransformationsSagittal->setMouseMode(mode);
}

void EventHandler2DTransformations::update(Model2DTransformationUIState *, Model2DTransformationUIStateEvent event_type)
{
    switch(event_type)
    {
    case Model2DTransformationUIStateEvent::TRANSFORMATION_2D_STATE_CHANGED:
    {
        if(m_isActivated == m_model2DTransformationUIState->isTransformationActivated())
            return;

        m_isActivated = m_model2DTransformationUIState->isTransformationActivated()
;
        updateRenderingSurfaceController();
        break;
    }

    default:
        break;
    }
}

void EventHandler2DTransformations::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;

    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::MOUSE_MODE_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED);
}

void EventHandler2DTransformations::updateRenderingSurfaceController()
{
    if(m_renderingSurfaceSet == nullptr || m_model2DTransformation == nullptr)
        return;

    if(m_model2DTransformationUIState->isTransformationActivated())
    {
        if(m_controllerMouseHandler2DTransformationsAxial)
            m_renderingSurfaceSet->getRenderingSurfaceAxial()->addController(m_controllerMouseHandler2DTransformationsAxial);

        if(m_controllerMouseHandler2DTransformationsCoronal)
            m_renderingSurfaceSet->getRenderingSurfaceCoronal()->addController(m_controllerMouseHandler2DTransformationsCoronal);

        if(m_controllerMouseHandler2DTransformationsSagittal)
            m_renderingSurfaceSet->getRenderingSurfaceSagittal()->addController(m_controllerMouseHandler2DTransformationsSagittal);

    } else {

        if(m_controllerMouseHandler2DTransformationsAxial)
            m_renderingSurfaceSet->getRenderingSurfaceAxial()->removeController(m_controllerMouseHandler2DTransformationsAxial);

        if(m_controllerMouseHandler2DTransformationsCoronal)
            m_renderingSurfaceSet->getRenderingSurfaceCoronal()->removeController(m_controllerMouseHandler2DTransformationsCoronal);

        if(m_controllerMouseHandler2DTransformationsSagittal)
            m_renderingSurfaceSet->getRenderingSurfaceSagittal()->removeController(m_controllerMouseHandler2DTransformationsSagittal);
    }
}

void EventHandler2DTransformations::setControllerMouseHandler2DTransformationsSagittal(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsSagittal)
{
    m_controllerMouseHandler2DTransformationsSagittal = controllerMouseHandler2DTransformationsSagittal;
}

void EventHandler2DTransformations::setControllerMouseHandler2DTransformationsCoronal(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsCoronal)
{
    m_controllerMouseHandler2DTransformationsCoronal = controllerMouseHandler2DTransformationsCoronal;
}

void EventHandler2DTransformations::setControllerMouseHandler2DTransformationsAxial(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsAxial)
{
    m_controllerMouseHandler2DTransformationsAxial = controllerMouseHandler2DTransformationsAxial;
}

void EventHandler2DTransformations::setModel2dRendering(Model2DRendering *model2dRendering)
{
    m_model2dRendering = model2dRendering;
}

void EventHandler2DTransformations::setModel2DTransformationUIState(Model2DTransformationUIState *model2DTransformationUIState)
{
    m_model2DTransformationUIState = model2DTransformationUIState;

    m_isActivated = m_model2DTransformationUIState->isTransformationActivated();
    m_model2DTransformationUIState->subscribe(this, Model2DTransformationUIStateEvent::TRANSFORMATION_2D_STATE_CHANGED);
}

void EventHandler2DTransformations::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}
