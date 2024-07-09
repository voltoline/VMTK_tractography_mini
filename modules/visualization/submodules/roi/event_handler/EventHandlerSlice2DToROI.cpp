#include "EventHandlerSlice2DToROI.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "../renderer/ROIRenderer2D.h"

EventHandlerSlice2DToROI::EventHandlerSlice2DToROI()
{
    m_model2DSlice = nullptr;

    m_rendererAxial = nullptr;
    m_rendererCoronal = nullptr;
    m_rendererSagittal = nullptr;
    mRenderingSurfaceSet = nullptr;
}

EventHandlerSlice2DToROI::~EventHandlerSlice2DToROI()
{
    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }
}

void EventHandlerSlice2DToROI::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    switch(event_type)
    {

    case Model2DSliceEvent::SLICING_CHANGED:
    {
        unsigned short axial = m_model2DSlice->getAxialSlice();
        unsigned short coronal = m_model2DSlice->getCoronalSlice();
        unsigned short sagittal = m_model2DSlice->getSagittalSlice();

        //Independent conditions that must be tested individually (Ting, 4/5/2022)
        if(mSliceData.mAxial != axial)
        {
            mSliceData.mAxial = axial;
            m_rendererAxial->setCurrentSlice(mSliceData.mAxial);
        }

        if(mSliceData.mCoronal != coronal) {
            mSliceData.mCoronal = coronal;
            m_rendererCoronal->setCurrentSlice(mSliceData.mCoronal);
        }

        if(mSliceData.mSagittal != sagittal) {
            mSliceData.mSagittal = sagittal;
            m_rendererSagittal->setCurrentSlice(mSliceData.mSagittal);
        }

        break;
    }

    default:
        break;
    }
}

void EventHandlerSlice2DToROI::setModel2DSlice(Model2DSlice *model2DSlice)
{
    if(m_model2DSlice == model2DSlice)
        return;

    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }

    m_model2DSlice = model2DSlice;

    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);

    mSliceData.mAxial = m_model2DSlice->getAxialSlice();
    mSliceData.mCoronal = m_model2DSlice->getCoronalSlice();
    mSliceData.mSagittal = m_model2DSlice->getSagittalSlice();
}

void EventHandlerSlice2DToROI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    mRenderingSurfaceSet = renderingSurfaceSet;
}

void EventHandlerSlice2DToROI::setRendererAxial(ROIRenderer2D *renderer)
{
    m_rendererAxial = renderer;
}

void EventHandlerSlice2DToROI::setRendererCoronal(ROIRenderer2D *renderer)
{
    m_rendererCoronal = renderer;
}

void EventHandlerSlice2DToROI::setRendererSagittal(ROIRenderer2D *renderer)
{
    m_rendererSagittal = renderer;
}
