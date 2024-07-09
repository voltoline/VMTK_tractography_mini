#include "EventHandlerDTIFiberTracking.h"

#include "../renderer/Renderer3DDTIData.h"
#include "ui/components/RenderingSurfaceSet.h"

EventHandlerDTIFiberTracking::EventHandlerDTIFiberTracking()
{
    m_renderingSurfaceSet = nullptr;
    m_rendererDTIData = nullptr;
    m_modelDTI = nullptr;
    m_modelUIStateDTI = nullptr;
}


void EventHandlerDTIFiberTracking::update(ModelDTI *, ModelDTIEvent event_type)
{
    switch(event_type)
    {

    case ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED:
    case ModelDTIEvent::FIBER_BUNDLE_ADDED:
    case ModelDTIEvent::FIBER_BUNDLE_DELETED:
    case ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();
        m_rendererDTIData->setFiberClusters(m_modelDTI->getVisibleFiberBundles());

        m_renderingSurfaceSet->update3DRendering();

        break;
    }

    default:
        break;
    }
}


void EventHandlerDTIFiberTracking::update(ModelUIStateDTI *, ModelUIStateDTIEvent event_type)
{
    switch(event_type)
    {
    case ModelUIStateDTIEvent::FIBER_RENDERING_STATE_CHANGED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_rendererDTIData->enableTractographyRendering(m_modelUIStateDTI->isFiberRenderingActivated());

        if(m_modelUIStateDTI->isFiberRenderingActivated())
            m_rendererDTIData->setFiberClusters(m_modelDTI->getVisibleFiberBundles());

        m_renderingSurfaceSet->update3DRendering();

        break;
    }

    default:
        break;
    }
}

void EventHandlerDTIFiberTracking::setRendererDTIData(RendererDTIData *rendererDTIData)
{
    m_rendererDTIData = rendererDTIData;

    if(m_rendererDTIData != nullptr && m_renderingSurfaceSet != nullptr && m_modelUIStateDTI != nullptr)
    {
        m_rendererDTIData->enableTractographyRendering(m_modelUIStateDTI->isFiberRenderingActivated());
        if(m_modelUIStateDTI->isFiberRenderingActivated())
            m_rendererDTIData->setFiberClusters(m_modelDTI->getVisibleFiberBundles());
    }
}


void EventHandlerDTIFiberTracking::setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI)
{
    m_modelUIStateDTI = modelUIStateDTI;

    m_modelUIStateDTI->subscribe(this, ModelUIStateDTIEvent::FIBER_RENDERING_STATE_CHANGED);

    if(m_rendererDTIData != nullptr && m_renderingSurfaceSet != nullptr && m_modelUIStateDTI != nullptr)
    {
        m_rendererDTIData->enableTractographyRendering(m_modelUIStateDTI->isFiberRenderingActivated());
        if(m_modelUIStateDTI->isFiberRenderingActivated())
            m_rendererDTIData->setFiberClusters(m_modelDTI->getVisibleFiberBundles());
    }
}


void EventHandlerDTIFiberTracking::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;

    if(m_rendererDTIData != nullptr && m_renderingSurfaceSet != nullptr && m_modelUIStateDTI != nullptr)
    {
        m_rendererDTIData->enableTractographyRendering(m_modelUIStateDTI->isFiberRenderingActivated());
        if(m_modelUIStateDTI->isFiberRenderingActivated())
            m_rendererDTIData->setFiberClusters(m_modelDTI->getVisibleFiberBundles());
    }
}


void EventHandlerDTIFiberTracking::setModelDTI(ModelDTI *model_dti)
{
    m_modelDTI = model_dti;

    m_modelDTI->subscribe(this, ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED);
    m_modelDTI->subscribe(this, ModelDTIEvent::FIBER_BUNDLE_ADDED);
    m_modelDTI->subscribe(this, ModelDTIEvent::FIBER_BUNDLE_DELETED);
    m_modelDTI->subscribe(this, ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED);
}
