#include "EventHandlerDTI.h"

#include "../renderer/Renderer3DDTIData.h"
#include "../models/ModelUIStateDTI.h"
#include "ui/components/RenderingSurfaceSet.h"

EventHandlerDTI::EventHandlerDTI()
{
    m_rendererDTIData = nullptr;
    m_modelDTI = nullptr;
    m_modelUIStateDTI = nullptr;
    m_modelVolume = nullptr;
    m_renderingSurfaceSet = nullptr;
}


void EventHandlerDTI::update(ModelDTI *, ModelDTIEvent event_type)
{
    switch(event_type)
    {

    case ModelDTIEvent::TENSORS_COMPUTED:
    {
        m_modelUIStateDTI->setIsDTIComputed(m_modelDTI->isDTIComputed());

        if(m_modelDTI->isDTIComputed())
        {
            ModelUIStateDTIEvent event = m_modelUIStateDTI->setFiberRenderingActivated(true);
            m_modelUIStateDTI->notify(event);
        }

        if(m_rendererDTIData)
            m_rendererDTIData->setDTI(m_modelDTI->getDTI());

        break;
    }

    case ModelDTIEvent::SCALAR_INDEX_RENDERING_STATE_CHANGED:
    case ModelDTIEvent::SCALAR_INDEX_CHANGED:
    {
        if(m_modelDTI->isScalarIndexRenderingActivated() == false)
            return;

        switch(m_modelDTI->getCurrentScalarIndexMode())
        {

        case ModelDTI::ScalarIndex::FA:
        {
                ModelVolumeEvent event = m_modelVolume->setGrayscaleVolumeData(m_modelDTI->getDTI()->getFractionalAnisotropy());
                m_modelVolume->notify(event);

                event = m_modelVolume->setSampleInterpolation(ModelVolume::SampleInterpolation::NEAREST);
                m_modelVolume->notify(event);

            break;
        }

        case ModelDTI::ScalarIndex::COLOR_CODED_FA:
        {
                ModelVolumeEvent event = m_modelVolume->setRGBVolumeData(m_modelDTI->getDTI()->ColorCodedFractionalAnisotropy());
                m_modelVolume->notify(event);

                event = m_modelVolume->setSampleInterpolation(ModelVolume::SampleInterpolation::NEAREST);
                m_modelVolume->notify(event);

            break;
        }

        case ModelDTI::ScalarIndex::MD:
        {
                ModelVolumeEvent event = m_modelVolume->setGrayscaleVolumeData(m_modelDTI->getDTI()->MeanDiffusivityNormalized());
                m_modelVolume->notify(event);

                event = m_modelVolume->setSampleInterpolation(ModelVolume::SampleInterpolation::LINEAR);
                m_modelVolume->notify(event);

            break;
        }

        default:
            break;

        }

        m_renderingSurfaceSet->update3DRendering();
        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    default:
        break;
    }
}

void EventHandlerDTI::setModelVolume(ModelVolume *modelVolume)
{
    m_modelVolume = modelVolume;
}


void EventHandlerDTI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}


void EventHandlerDTI::setRendererDTIData(RendererDTIData *rendererDTIData)
{
    m_rendererDTIData = rendererDTIData;
}


void EventHandlerDTI::setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI)
{
    m_modelUIStateDTI = modelUIStateDTI;
}


void EventHandlerDTI::setModelDTI(ModelDTI *model_dti)
{
    m_modelDTI = model_dti;

    m_modelDTI->subscribe(this, ModelDTIEvent::TENSORS_COMPUTED);
    m_modelDTI->subscribe(this, ModelDTIEvent::SCALAR_INDEX_RENDERING_STATE_CHANGED);
    m_modelDTI->subscribe(this, ModelDTIEvent::SCALAR_INDEX_CHANGED);
}
