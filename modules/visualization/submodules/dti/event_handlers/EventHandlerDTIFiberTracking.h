#ifndef EVENT_HANDLER_DTI_FIBER_TRACKING_H
#define EVENT_HANDLER_DTI_FIBER_TRACKING_H

#include "../models/ModelDTI.h"
#include "../models/ModelUIStateDTI.h"

class RendererDTIData;
class RenderingSurfaceSet;

class EventHandlerDTIFiberTracking:
        public ModelObserver<ModelDTI, ModelDTIEvent>,
        public ModelObserver<ModelUIStateDTI, ModelUIStateDTIEvent>
{
public:
    EventHandlerDTIFiberTracking();
    ~EventHandlerDTIFiberTracking() { }

    void update(ModelDTI *subject, ModelDTIEvent event_type);
    void update(ModelUIStateDTI *subject, ModelUIStateDTIEvent event_type);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModelDTI(ModelDTI *model_dti);
    void setRendererDTIData(RendererDTIData *rendererDTIData);
    void setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI);

private:
    RendererDTIData *m_rendererDTIData;
    ModelDTI *m_modelDTI;
    ModelUIStateDTI *m_modelUIStateDTI;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // EVENT_HANDLER_DTI_FIBER_TRACKING_H
