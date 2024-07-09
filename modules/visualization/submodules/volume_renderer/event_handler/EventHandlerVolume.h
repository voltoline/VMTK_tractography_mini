#ifndef EVENT_HANDLER_VOLUME_H
#define EVENT_HANDLER_VOLUME_H

#include "model/ModelObserver.h"
#include "../model/ModelVolume.h"
#include "ui/components/RenderingSurfaceSet.h"
#include "../OpenGLVolumeRenderingResource.h"

class BaseRaycastingVolumeRenderer3D;

class EventHandlerVolume: public ModelObserver<ModelVolume, ModelVolumeEvent>
{
public:
    EventHandlerVolume();
    ~EventHandlerVolume();

    void update(ModelVolume *subject, ModelVolumeEvent event_type);

    void setRenderingSurfaceSet(RenderingSurfaceSet *getRenderingSurfaceSet);
    void setModelVolume(ModelVolume *model_volume);
    void setOpenGLVolumeRenderingResource(OpenGLVolumeRenderingResource::sPtr openGLVolumeRenderingResource);
    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *getRaycastingVolumeRenderer3D);

protected:
    RenderingSurfaceSet *getRenderingSurfaceSet() const;
    BaseRaycastingVolumeRenderer3D *getRaycastingVolumeRenderer3D() const;

private:
    void unsubscribeEvents();

    RenderingSurfaceSet *m_renderingSurfaceSet;
    ModelVolume *m_modelVolume;
    OpenGLVolumeRenderingResource::sPtr m_openGLVolumeRenderingResource;
    BaseRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;
};

#endif // RENDERING_EVENT_HANDLER_H
