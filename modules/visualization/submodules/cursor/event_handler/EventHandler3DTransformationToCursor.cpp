#include "EventHandler3DTransformationToCursor.h"

#include "../CursorRenderer3D.h"
#include "../ModelCursor.h"

EventHandler3DTransformationToCursor::EventHandler3DTransformationToCursor()
{
    m_model3DTransformation = nullptr;
    m_cursorRenderer3D = nullptr;
    m_modelCursor = nullptr;
}

void EventHandler3DTransformationToCursor::update(Model3DTransformation *, Model3DTransformationEvent event_type)
{
    switch(event_type)
    {

    case Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED:
    {
        m_cursorRenderer3D->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix());
        m_cursorRenderer3D->setProjectionMatrix(m_model3DTransformation->getProjectionMatrix(true));

        m_modelCursor->setModelViewMatrix3D(m_model3DTransformation->getModelViewMatrix());
        m_modelCursor->setProjectionMatrix3D(m_model3DTransformation->getProjectionMatrix(true));

        break;
    }

    default:
        break;

    }
}

void EventHandler3DTransformationToCursor::setModel3DTransformation(Model3DTransformation *model3DTransformation)
{
    m_model3DTransformation = model3DTransformation;

    m_model3DTransformation = model3DTransformation;
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void EventHandler3DTransformationToCursor::setCursorRenderer3D(CursorRenderer3D *cursorRenderer3D)
{
    m_cursorRenderer3D = cursorRenderer3D;
}

void EventHandler3DTransformationToCursor::setModelCursor(ModelCursor *modelCursor)
{
    m_modelCursor = modelCursor;
}
