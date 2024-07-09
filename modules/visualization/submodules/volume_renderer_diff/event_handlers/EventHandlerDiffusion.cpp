#include "EventHandlerDiffusion.h"

#include "ui/components/RenderingSurfaceSet.h"

EventHandlerDiffusion::EventHandlerDiffusion()
{
    m_modelDiffusion = nullptr;
    m_modelVolume = nullptr;
    m_renderingSurfaceSet = nullptr;
}

void EventHandlerDiffusion::update(ModelDiffusionUI *, ModelDiffusionUIEvent event_type)
{
    switch(event_type)
    {

    case ModelDiffusionUIEvent::DWI_RENDERING_STATE_CHANGED:
    case ModelDiffusionUIEvent::DWI_INDEX_CHANGED:
    {       
        if(m_modelDiffusion->isDWIRenderingActivated())
        {
            vmtkDiffAcquisition *acq = m_modelDiffusion->getAcquisition();
            unsigned short *volume_data = reinterpret_cast<unsigned short*>(acq->getImageBuffer(m_modelDiffusion->getCurrentDWIIndex()));

            ModelVolumeEvent event = m_modelVolume->setGrayscaleVolumeData(volume_data, acq->getBitsAllocated(), acq->getUmax());
            m_modelVolume->notify(event);

            event = m_modelVolume->setSampleInterpolation(ModelVolume::SampleInterpolation::LINEAR);
            m_modelVolume->notify(event);

            m_renderingSurfaceSet->update3DRendering();
            m_renderingSurfaceSet->update2DRendering();
        }

        break;
    }

    default:
        break;
    }
}


void EventHandlerDiffusion::setModelVolume(ModelVolume *modelVolume)
{
    m_modelVolume = modelVolume;
}


void EventHandlerDiffusion::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandlerDiffusion::setModelDiffusion(ModelDiffusionUI *model_diffusion)
{
    m_modelDiffusion = model_diffusion;

    m_modelDiffusion->subscribe(this, ModelDiffusionUIEvent::DWI_RENDERING_STATE_CHANGED);
    m_modelDiffusion->subscribe(this, ModelDiffusionUIEvent::DWI_INDEX_CHANGED);
}
