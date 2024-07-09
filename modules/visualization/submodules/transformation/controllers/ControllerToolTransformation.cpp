#include "ControllerToolTransformation.h"

#include "../ui/ToolTransformation.h"
#include "../../2d_transformation/model/Model2DTransformation.h"

ControllerToolTransformation::ControllerToolTransformation():
    ControllerTool("ToolTransformation"),
    m_model3DTransformation(nullptr),
    m_model2DTransformation(nullptr),
    m_model2DTransformationUIState(nullptr)
{

}

void ControllerToolTransformation::update(Model3DTransformation *, Model3DTransformationEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {

    case Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED:
    {
        static_cast<ToolTransformation*>(getTool())->updateTransformationEnabled();
        break;
    }

    case Model3DTransformationEvent::MOUSE_MODE_CHANGED:
    {
        static_cast<ToolTransformation*>(getTool())->updateMouseMode();
        break;
    }

    default:
        break;

    }
}

void ControllerToolTransformation::update(Model2DTransformationUIState *, Model2DTransformationUIStateEvent event)
{
    if(event == Model2DTransformationUIStateEvent::TRANSFORMATION_2D_STATE_CHANGED)
        if(getTool())
            static_cast<ToolTransformation*>(getTool())->updateTransformationEnabled();
}

void ControllerToolTransformation::setModel3DTransformation(Model3DTransformation *model3D)
{
    m_model3DTransformation = model3D;
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::MOUSE_MODE_CHANGED);
}

void ControllerToolTransformation::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;
}

void ControllerToolTransformation::setModel2DTransformationUIState(Model2DTransformationUIState *model2DTransformationState)
{
    m_model2DTransformationUIState = model2DTransformationState;

    m_model2DTransformationUIState->subscribe(this, Model2DTransformationUIStateEvent::TRANSFORMATION_2D_STATE_CHANGED);
}

void ControllerToolTransformation::setMouseMode(short mode)
{
    if(m_model3DTransformation->isTransformationActivated())
    {
        switch(mode)
        {
        case -2:
            m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::ROTATION);
            break;

        case -3:
            m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::ZOOM);
            break;

        case -4:
            m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::MOVE);
            break;
        }

        m_model3DTransformation->notify();
    }

    if(m_model2DTransformationUIState->isTransformationActivated())
    {
        Model2DTransformationEvent event = Model2DTransformationEvent::NONE;
        switch(mode)
        {
        case -2:
            event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::ROTATION);
            break;

        case -3:
            event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::ZOOM);
            break;

        case -4:
            event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::MOVE);
            break;
        }

        m_model2DTransformation->notify(event);
    }
}

short ControllerToolTransformation::getMouseMode()
{
    switch(m_model3DTransformation->getMouseMode())
    {
    case Model3DTransformation::MouseMode::ROTATION:
        return -2;

    case Model3DTransformation::MouseMode::ZOOM:
        return -3;

    case Model3DTransformation::MouseMode::MOVE:
        return -4;

    default:
        return 0;

    }
}

bool ControllerToolTransformation::is3DTransformationActivated()
{
    return m_model3DTransformation->isTransformationActivated();
}

bool ControllerToolTransformation::is2DTransformationActivated()
{
    return m_model2DTransformationUIState->isTransformationActivated();
}

bool ControllerToolTransformation::is3DTransformationEnabled()
{
    return m_model3DTransformation->isEnabled();
}

bool ControllerToolTransformation::is2DTransformationEnabled()
{
    return m_model2DTransformationUIState->isEnabled();
}

void ControllerToolTransformation::set3DTransformationActivated(bool state)
{
    m_model3DTransformation->setTransformationActivated(state);
    m_model3DTransformation->notify(this);
}

void ControllerToolTransformation::set2DTransformationActivated(bool state)
{
    m_model2DTransformationUIState->setTransformationActivated(state);
}

void ControllerToolTransformation::resetTransformations()
{
    if(m_model3DTransformation->isTransformationActivated())
    {
        m_model3DTransformation->resetTransformations();
        m_model3DTransformation->notify();
    }

    if(m_model2DTransformationUIState->isTransformationActivated())
    {
        Model2DTransformationEvent event = m_model2DTransformation->getAxialData().resetTransformations();
        m_model2DTransformation->notify(event);

        event = m_model2DTransformation->getCoronalData().resetTransformations();
        m_model2DTransformation->notify(event);

        event = m_model2DTransformation->getSagittalData().resetTransformations();
        m_model2DTransformation->notify(event);
    }
}
