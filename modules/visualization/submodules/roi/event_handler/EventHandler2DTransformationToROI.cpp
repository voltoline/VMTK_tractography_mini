#include "EventHandler2DTransformationToROI.h"

#include "../renderer/ROIRenderer2D.h"

EventHandler2DTransformationToROI::EventHandler2DTransformationToROI()
{
    m_rendererROIAxial = nullptr;
    m_rendererROICoronal = nullptr;
    m_rendererROISagittal = nullptr;

    m_model2DTransformation = nullptr;
}

void EventHandler2DTransformationToROI::update(Model2DTransformation */*subject*/, Model2DTransformationEvent event_type)
{
    switch(event_type)
    {

    case Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererROIAxial)
        {
            m_rendererROIAxial->setModelViewMatrix(m_model2DTransformation->getAxialData().getModelViewMatrix());
            m_rendererROIAxial->setProjectionMatrix(m_model2DTransformation->getAxialData().getProjectionMatrix());
        }

        break;
    }

    case Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererROICoronal)
        {
            m_rendererROICoronal->setModelViewMatrix(m_model2DTransformation->getCoronalData().getModelViewMatrix());
            m_rendererROICoronal->setProjectionMatrix(m_model2DTransformation->getCoronalData().getProjectionMatrix());
        }

        break;
    }

    case Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED:
    {
        if(m_rendererROISagittal)
        {
            m_rendererROISagittal->setModelViewMatrix(m_model2DTransformation->getSagittalData().getModelViewMatrix());
            m_rendererROISagittal->setProjectionMatrix(m_model2DTransformation->getSagittalData().getProjectionMatrix());
        }

        break;
    }

    default:
        break;

    }
}

void EventHandler2DTransformationToROI::setModel2DTransformation(Model2DTransformation *model2DTransformation)
{
    m_model2DTransformation = model2DTransformation;

    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED);
    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED);
}

void EventHandler2DTransformationToROI::setRendererROIAxial(ROIRenderer2D *rendererROIAxial)
{
    m_rendererROIAxial = rendererROIAxial;
}

void EventHandler2DTransformationToROI::setRendererROICoronal(ROIRenderer2D *rendererROICoronal)
{
    m_rendererROICoronal = rendererROICoronal;
}

void EventHandler2DTransformationToROI::setRendererROISagittal(ROIRenderer2D *rendererROISagittal)
{
    m_rendererROISagittal = rendererROISagittal;
}
