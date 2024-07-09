#include "EventHandler2DTransformationToCursor.h"

#include "../CursorRenderer2D.h"
#include "../ModelCursor.h"

EventHandler2DTransformationToCursor::EventHandler2DTransformationToCursor()
{
    m_model2DTransformation = nullptr;
    m_modelCursor = nullptr;

    m_rendererCursorAxial = nullptr;
    m_rendererCursorCoronal = nullptr;
    m_rendererCursorSagittal = nullptr;
}

void EventHandler2DTransformationToCursor::update(Model2DTransformation */*subject*/, Model2DTransformationEvent event_type)
{
    switch(event_type)
    {

    case Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererCursorAxial)
        {
            m_rendererCursorAxial->setModelViewMatrix(m_model2DTransformation->getAxialData().getModelViewMatrix());
            m_rendererCursorAxial->setProjectionMatrix(m_model2DTransformation->getAxialData().getProjectionMatrix());
        }

        if(m_modelCursor)
        {
            m_modelCursor->setModelViewMatrixAxial(m_model2DTransformation->getAxialData().getModelViewMatrix());
            m_modelCursor->setProjectionMatrixAxial(m_model2DTransformation->getAxialData().getProjectionMatrix());
        }

        break;
    }

    case Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererCursorCoronal)
        {
            m_rendererCursorCoronal->setModelViewMatrix(m_model2DTransformation->getCoronalData().getModelViewMatrix());
            m_rendererCursorCoronal->setProjectionMatrix(m_model2DTransformation->getCoronalData().getProjectionMatrix());
        }

        if(m_modelCursor)
        {
            m_modelCursor->setModelViewMatrixCoronal(m_model2DTransformation->getCoronalData().getModelViewMatrix());
            m_modelCursor->setProjectionMatrixCoronal(m_model2DTransformation->getCoronalData().getProjectionMatrix());
        }

        break;
    }

    case Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererCursorSagittal)
        {
            m_rendererCursorSagittal->setModelViewMatrix(m_model2DTransformation->getSagittalData().getModelViewMatrix());
            m_rendererCursorSagittal->setProjectionMatrix(m_model2DTransformation->getSagittalData().getProjectionMatrix());
        }

        if(m_modelCursor)
        {
            m_modelCursor->setModelViewMatrixSagittal(m_model2DTransformation->getSagittalData().getModelViewMatrix());
            m_modelCursor->setProjectionMatrixSagittal(m_model2DTransformation->getSagittalData().getProjectionMatrix());
        }

        break;
    }

    default:
        break;

    }
}

void EventHandler2DTransformationToCursor::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;

    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED);
}

void EventHandler2DTransformationToCursor::setRendererCursorAxial(CursorRenderer2D *rendererCursorAxial)
{
    m_rendererCursorAxial = rendererCursorAxial;
}

void EventHandler2DTransformationToCursor::setRendererCursorCoronal(CursorRenderer2D *rendererCursorCoronal)
{
    m_rendererCursorCoronal = rendererCursorCoronal;
}

void EventHandler2DTransformationToCursor::setRendererCursorSagittal(CursorRenderer2D *rendererCursorSagittal)
{
    m_rendererCursorSagittal = rendererCursorSagittal;
}

void EventHandler2DTransformationToCursor::setModelCursor(ModelCursor *modelCursor)
{
    m_modelCursor = modelCursor;
}
