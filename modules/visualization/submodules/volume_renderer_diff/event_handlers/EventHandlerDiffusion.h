#ifndef EVENT_HANDLER_DIFFUSION_H
#define EVENT_HANDLER_DIFFUSION_H

#include "../models/ModelDiffusionUI.h"
#include "modules/visualization/submodules/volume_renderer/model/ModelVolume.h"

class RenderingSurfaceSet;

class EventHandlerDiffusion:
        public ModelObserver<ModelDiffusionUI, ModelDiffusionUIEvent>
{
public:
    EventHandlerDiffusion();
    ~EventHandlerDiffusion() { }

    void update(ModelDiffusionUI *subject, ModelDiffusionUIEvent event_type);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModelDiffusion(ModelDiffusionUI *model_diffusion);
    void setModelVolume(ModelVolume *modelVolume);

private:

    ModelDiffusionUI *m_modelDiffusion;
    ModelVolume *m_modelVolume;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // EVENT_HANDLER_DIFFUSION_H
