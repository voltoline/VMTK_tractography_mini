#include "EventHandlerDiffusionVolume.h"

template <class raycasting_renderer_type, class slice_renderer_type>
EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::EventHandlerDiffusionVolume()
{
    m_sliceRendererAxial = nullptr;
    m_sliceRendererCoronal = nullptr;
    m_sliceRendererSagittal = nullptr;
}

template<class raycasting_renderer_type, class slice_renderer_type>
void EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::update(ModelVolume *subject, ModelVolumeEvent event_type)
{
    EventHandlerVolume::update(subject, event_type);

    switch(event_type)
    {

    case ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED:
    {
        setGrayscaleRenderingActivated(true, true);
        break;
    }

    case ModelVolumeEvent::FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED:
    {
        setGrayscaleRenderingActivated(true, false);
        break;
    }

    case ModelVolumeEvent::FLOAT_RGB_VOLUME_DATA_CHANGED:
    {
        setGrayscaleRenderingActivated(false, false);
        break;
    }

    default:
        break;

    }
}

template<class raycasting_renderer_type, class slice_renderer_type>
void EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::setGrayscaleRenderingActivated(bool enabled, bool use_transfer_function)
{
    if(getRaycastingVolumeRenderer3D() != nullptr)
    {
        getRenderingSurfaceSet()->getRenderingSurface3D()->setRenderingContext();
        static_cast<raycasting_renderer_type*>(getRaycastingVolumeRenderer3D())->setEnabledGrayscaleMode(enabled, use_transfer_function);
    }

    if(m_sliceRendererAxial != nullptr)
    {
        getRenderingSurfaceSet()->getRenderingSurfaceAxial()->setRenderingContext();
        m_sliceRendererAxial->setEnabledGrayscaleMode(enabled, use_transfer_function);
    }

    if(m_sliceRendererCoronal != nullptr)
    {
        getRenderingSurfaceSet()->getRenderingSurfaceCoronal()->setRenderingContext();
        m_sliceRendererCoronal->setEnabledGrayscaleMode(enabled, use_transfer_function);
    }

    if(m_sliceRendererSagittal != nullptr)
    {
        getRenderingSurfaceSet()->getRenderingSurfaceSagittal()->setRenderingContext();
        m_sliceRendererSagittal->setEnabledGrayscaleMode(enabled, use_transfer_function);
    }
}

template<class raycasting_renderer_type, class slice_renderer_type>
void EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::setSliceRendererAxial(slice_renderer_type *sliceRendererAxial)
{
    m_sliceRendererAxial = sliceRendererAxial;
}

template<class raycasting_renderer_type, class slice_renderer_type>
void EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::setSliceRendererCoronal(slice_renderer_type *sliceRendererCoronal)
{
    m_sliceRendererCoronal = sliceRendererCoronal;
}

template<class raycasting_renderer_type, class slice_renderer_type>
void EventHandlerDiffusionVolume<raycasting_renderer_type, slice_renderer_type>::setSliceRendererSagittal(slice_renderer_type *sliceRendererSagittal)
{
    m_sliceRendererSagittal = sliceRendererSagittal;
}
