#ifndef EVENT_HANDLER_2D_RENDERING_H
#define EVENT_HANDLER_2D_RENDERING_H

#include "model/ModelObserver.h"
#include "../model/Model2DRendering.h"
#include "ui/components/RenderingSurfaceSet.h"

class BaseSliceRenderer;

class EventHandler2DRendering: public ModelObserver<Model2DRendering, Model2DRenderingEvent>
{
public:
    EventHandler2DRendering();
    ~EventHandler2DRendering();

    void update(Model2DRendering *subject, Model2DRenderingEvent event_type);

    void setModel2DRendering(Model2DRendering *model2DRendering);
    void setSagittalSliceRenderer(BaseSliceRenderer *slice_renderer);
    void setCoronalSliceRenderer(BaseSliceRenderer *coronalSliceRenderer);
    void setAxialSliceRenderer(BaseSliceRenderer *axialSliceRenderer);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

private:
    void unsubscribeEvents();

    Model2DRendering *m_model2DRendering;

    RenderingSurfaceSet *m_renderingSurfaceSet;

    BaseSliceRenderer *m_sagittalSliceRenderer;
    BaseSliceRenderer *m_coronalSliceRenderer;
    BaseSliceRenderer *m_axialSliceRenderer;
};

#endif // EVENT_HANDLER_2D_RENDERING_H
