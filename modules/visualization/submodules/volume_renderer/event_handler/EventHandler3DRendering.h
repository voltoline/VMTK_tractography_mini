#ifndef EVENT_HANDLER_3D_RENDERING_H
#define EVENT_HANDLER_3D_RENDERING_H

#include "model/ModelObserver.h"
#include "../model/Model3DRendering.h"
#include "ui/components/RenderingSurfaceSet.h"

class BaseRaycastingVolumeRenderer3D;

class EventHandler3DRendering: public ModelObserver<Model3DRendering, Model3DRenderingEvent>
{
public:
    EventHandler3DRendering();
    ~EventHandler3DRendering();

    void update(Model3DRendering *subject, Model3DRenderingEvent event_type);

    void setModel3DRendering(Model3DRendering *model3DRendering);
    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

private:
    void unsubscribeEvents();

    Model3DRendering *m_model3DRendering;
    BaseRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // EVENT_HANDLER_3D_RENDERING_H
