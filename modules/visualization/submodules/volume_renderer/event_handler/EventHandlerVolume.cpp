#include "EventHandlerVolume.h"

#include "../3d_renderer/BaseRaycastingVolumeRenderer3D.h"

EventHandlerVolume::EventHandlerVolume()
{
    m_openGLVolumeRenderingResource = nullptr;
    m_modelVolume = nullptr;

    m_renderingSurfaceSet = nullptr;
    m_raycastingVolumeRenderer3D = nullptr;
}

EventHandlerVolume::~EventHandlerVolume()
{
    unsubscribeEvents();
}

void EventHandlerVolume::update(ModelVolume *, ModelVolumeEvent event_type)
{
    switch(event_type)
    {

    case ModelVolumeEvent::TRANSFER_FUNCTION_CHANGED:
    {
        m_openGLVolumeRenderingResource->setTransferFunctionData(m_modelVolume->getTransferFunction());
        m_renderingSurfaceSet->update3DRendering();
        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    case ModelVolumeEvent::THRESHOLD_DATA_CHANGED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_raycastingVolumeRenderer3D->setStateThreshold(m_modelVolume->getThresholdState());
        m_raycastingVolumeRenderer3D->setNoiseThreshold(m_modelVolume->getMappedThreshold());

        m_renderingSurfaceSet->update3DRendering();
        break;
    }

    case ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED:
    {
        if(m_modelVolume->isVolumeLoadedAsTexture())
            return;

        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        /* Build the volume data in grayscale */
        m_openGLVolumeRenderingResource->setGrayscaleVolumeData(m_modelVolume->getMappedGrayscaleVolume());

        m_raycastingVolumeRenderer3D->setStateThreshold(m_modelVolume->getThresholdState());
        m_raycastingVolumeRenderer3D->setNoiseThreshold(m_modelVolume->getMappedThreshold());

        m_modelVolume->setIsVolumeLoadedAsTexture(true);

        /* Redraw the 3D view */
        m_renderingSurfaceSet->update3DRendering();

        /* Redraw the 2D views */
        m_renderingSurfaceSet->update2DRendering();

        /* Update the transfer function */

        break;
    }

    case ModelVolumeEvent::FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED:
    {
        if(m_modelVolume->isVolumeLoadedAsTexture())
            return;

        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_openGLVolumeRenderingResource->setGrayscaleVolumeData(m_modelVolume->getGrayscaleVolumeDataFloat());

        m_raycastingVolumeRenderer3D->setStateThreshold(m_modelVolume->getThresholdState());
        m_raycastingVolumeRenderer3D->setNoiseThreshold(m_modelVolume->getMappedThreshold());

        m_modelVolume->setIsVolumeLoadedAsTexture(true);

        m_renderingSurfaceSet->update3DRendering();
        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    case ModelVolumeEvent::FLOAT_RGB_VOLUME_DATA_CHANGED:
    {
        if(m_modelVolume->isVolumeLoadedAsTexture())
            return;

        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_openGLVolumeRenderingResource->setRGBVolumeData(m_modelVolume->getRGBVolumeData());

        m_raycastingVolumeRenderer3D->setStateThreshold(m_modelVolume->getThresholdState());
        m_raycastingVolumeRenderer3D->setNoiseThreshold(m_modelVolume->getMappedThreshold());

        m_modelVolume->setIsVolumeLoadedAsTexture(true);

        m_renderingSurfaceSet->update3DRendering();
        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    case ModelVolumeEvent::SAMPLE_INTERPOLATION_CHANGED:
    {
        if(m_modelVolume->getSampleInterpolation() == ModelVolume::SampleInterpolation::LINEAR)
            m_openGLVolumeRenderingResource->setSampleInterpolation(OpenGLVolumeRenderingResource::SampleInterpolation::LINEAR);
        else
            m_openGLVolumeRenderingResource->setSampleInterpolation(OpenGLVolumeRenderingResource::SampleInterpolation::NEAREST);

        m_renderingSurfaceSet->update3DRendering();
        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    default:
        break;

    }
}

RenderingSurfaceSet *EventHandlerVolume::getRenderingSurfaceSet() const
{
    return m_renderingSurfaceSet;
}

BaseRaycastingVolumeRenderer3D *EventHandlerVolume::getRaycastingVolumeRenderer3D() const
{
    return m_raycastingVolumeRenderer3D;
}

void EventHandlerVolume::unsubscribeEvents()
{
    if(m_modelVolume != nullptr)
    {
        m_modelVolume->subscribe(this, ModelVolumeEvent::TRANSFER_FUNCTION_CHANGED);
        m_modelVolume->subscribe(this, ModelVolumeEvent::THRESHOLD_DATA_CHANGED);
        m_modelVolume->subscribe(this, ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED);
        m_modelVolume->subscribe(this, ModelVolumeEvent::FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED);
        m_modelVolume->subscribe(this, ModelVolumeEvent::FLOAT_RGB_VOLUME_DATA_CHANGED);
        m_modelVolume->subscribe(this, ModelVolumeEvent::SAMPLE_INTERPOLATION_CHANGED);
    }
}

void EventHandlerVolume::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandlerVolume::setModelVolume(ModelVolume *model_volume)
{
    unsubscribeEvents();

    m_modelVolume = model_volume;

    m_modelVolume->subscribe(this, ModelVolumeEvent::TRANSFER_FUNCTION_CHANGED);
    m_modelVolume->subscribe(this, ModelVolumeEvent::THRESHOLD_DATA_CHANGED);
    m_modelVolume->subscribe(this, ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED);
    m_modelVolume->subscribe(this, ModelVolumeEvent::FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED);
    m_modelVolume->subscribe(this, ModelVolumeEvent::FLOAT_RGB_VOLUME_DATA_CHANGED);
    m_modelVolume->subscribe(this, ModelVolumeEvent::SAMPLE_INTERPOLATION_CHANGED);
}

void EventHandlerVolume::setOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr openGLVolumeRenderingResource)
{
    m_openGLVolumeRenderingResource = openGLVolumeRenderingResource;
}

void EventHandlerVolume::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D)
{
    m_raycastingVolumeRenderer3D = raycastingVolumeRenderer3D;
}
