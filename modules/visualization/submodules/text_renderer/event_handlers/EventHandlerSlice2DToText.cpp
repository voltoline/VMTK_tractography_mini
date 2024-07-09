#include "EventHandlerSlice2DToText.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "../TextRenderer2DView.h"

EventHandlerSlice2DToText::EventHandlerSlice2DToText()
{
    m_model2DSlice = nullptr;

    m_rendererAxial = nullptr;
    m_rendererCoronal = nullptr;
    m_rendererSagittal = nullptr;
    mRenderingSurfaceSet = nullptr;
}

EventHandlerSlice2DToText::~EventHandlerSlice2DToText()
{
    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }
}

void EventHandlerSlice2DToText::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    switch(event_type)
    {

    case Model2DSliceEvent::SLICING_CHANGED:
    {
        unsigned short axial = m_model2DSlice->getAxialSlice();
        unsigned short coronal = m_model2DSlice->getCoronalSlice();
        unsigned short sagittal = m_model2DSlice->getSagittalSlice();


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

    case Model2DSliceEvent::VOXEL_ORDER_CHANGED:
    {
        if(m_model2DSlice->isRadiological())
        {
            m_rendererAxial->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 0);
            m_rendererCoronal->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 0);
            m_rendererSagittal->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 0);

        } else {

            m_rendererAxial->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 1);
            m_rendererCoronal->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 1);
            m_rendererSagittal->setVoxelOrder((TextRenderer2DView::TVoxelOrder) 1);
        }

        break;
    }

    default:
        break;
    }
}

void EventHandlerSlice2DToText::setModel2DSlice(Model2DSlice *model2DSlice)
{
    if(m_model2DSlice == model2DSlice)
        return;

    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }

    m_model2DSlice = model2DSlice;

    m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);

    mSliceData.mAxial = m_model2DSlice->getAxialSlice();
    mSliceData.mCoronal = m_model2DSlice->getCoronalSlice();
    mSliceData.mSagittal = m_model2DSlice->getSagittalSlice();
}

void EventHandlerSlice2DToText::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    mRenderingSurfaceSet = renderingSurfaceSet;
}

void EventHandlerSlice2DToText::setRendererAxial(TextRenderer2DView *renderer)
{
    m_rendererAxial = renderer;
}

void EventHandlerSlice2DToText::setRendererCoronal(TextRenderer2DView *renderer)
{
    m_rendererCoronal = renderer;
}

void EventHandlerSlice2DToText::setRendererSagittal(TextRenderer2DView *renderer)
{
    m_rendererSagittal = renderer;
}
