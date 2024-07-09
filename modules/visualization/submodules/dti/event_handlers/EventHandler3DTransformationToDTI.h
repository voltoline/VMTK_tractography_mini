#ifndef EVENT_HANDLER_3D_TRANSFORMATION_TO_DTI_H
#define EVENT_HANDLER_3D_TRANSFORMATION_TO_DTI_H

#include "modules/visualization/submodules/3d_transformation/models/Model3DTransformation.h"
#include "ui/RenderingSurface.h"

class RenderingSurfaceSet;
class RendererDTIData;
class ModelUIStateDTI;

class EventHandler3DTransformationToDTI:
        public ModelObserver<Model3DTransformation, Model3DTransformationEvent>,
        public ControllerRenderingSurface
{
public:
    EventHandler3DTransformationToDTI();

    void afterRendering();

    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type);

    void setModel3DTransformation(Model3DTransformation *model3DTransformation);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setRendererDTIData(RendererDTIData *rendererDTIData);

    void setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI);

private:
    Model3DTransformation *m_model3DTransformation;
    RenderingSurfaceSet *m_renderingSurfaceSet;

    RendererDTIData *m_rendererDTIData;

    ModelUIStateDTI *m_modelUIStateDTI;
    unsigned int msg_frame_id = 0;

    bool matrix_applied;
};

#endif // EVENT_HANDLER_3D_TRANSFORMATION_TO_CR_H
