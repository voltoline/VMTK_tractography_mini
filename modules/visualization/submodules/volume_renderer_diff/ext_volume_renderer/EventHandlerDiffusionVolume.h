#ifndef EVENT_HANDLER_DIFFUSION_VOLUME_H
#define EVENT_HANDLER_DIFFUSION_VOLUME_H

#include <modules/visualization/submodules/volume_renderer/event_handler/EventHandlerVolume.h>

template <class raycasting_renderer_type, class slice_renderer_type>
class EventHandlerDiffusionVolume: public EventHandlerVolume
{
public:
    EventHandlerDiffusionVolume();

    void update(ModelVolume *subject, ModelVolumeEvent event_type);

    void setSliceRendererAxial(slice_renderer_type *sliceRendererAxial);
    void setSliceRendererCoronal(slice_renderer_type *sliceRendererCoronal);
    void setSliceRendererSagittal(slice_renderer_type *sliceRendererSagittal);

private:
    void setGrayscaleRenderingActivated(bool enabled, bool use_transfer_function);

    slice_renderer_type *m_sliceRendererAxial;
    slice_renderer_type *m_sliceRendererCoronal;
    slice_renderer_type *m_sliceRendererSagittal;
};

#include "EventHandlerDiffusionVolume.tpp"

#endif // EVENT_HANDLER_DIFFUSION_VOLUME_H
