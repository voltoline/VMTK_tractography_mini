#ifndef EVENT_HANDLER_DTI_H
#define EVENT_HANDLER_DTI_H

#include "../models/ModelDTI.h"
#include "../models/ModelUIStateDTI.h"
#include "modules/visualization/submodules/volume_renderer/model/ModelVolume.h"

class RendererDTIData;
class RenderingSurfaceSet;

class EventHandlerDTI:
        public ModelObserver<ModelDTI, ModelDTIEvent>
{
public:
    EventHandlerDTI();
    ~EventHandlerDTI() { }

    void update(ModelDTI *subject, ModelDTIEvent event_type);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModelDTI(ModelDTI *model_dti);
    void setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI);
    void setModelVolume(ModelVolume *modelVolume);
    void setRendererDTIData(RendererDTIData *rendererDTIData);

    void updateGlyphRendering();

private:

    RendererDTIData *m_rendererDTIData;

    ModelDTI *m_modelDTI;
    ModelUIStateDTI *m_modelUIStateDTI;
    ModelVolume *m_modelVolume;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // EVENT_HANDLER_DTI_H
