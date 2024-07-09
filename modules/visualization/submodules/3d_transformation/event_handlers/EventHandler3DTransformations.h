#ifndef EVENT_HANDLER_3D_TRANSFORMATIONS_H
#define EVENT_HANDLER_3D_TRANSFORMATIONS_H

#include <glm/glm.hpp>
#include <modules/visualization/submodules/volume_renderer/model/Model3DRendering.h>
#include "../models/Model3DTransformation.h"

class RenderingSurfaceSet;
class ControllerMouseHandler3DTransformations;

class EventHandler3DTransformations: public ModelObserver<Model3DTransformation, Model3DTransformationEvent>
{
public:
    EventHandler3DTransformations();

    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type);

    void setModel3DTransformation(Model3DTransformation *model3DTransformation);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModel3dRendering(Model3DRendering *model3dRendering);
    void setControllerMouseHandler3DTransformations(ControllerMouseHandler3DTransformations *controllerMouseHandler3DTransformations);

private:

    Model3DTransformation *m_model3DTransformation;
    Model3DRendering *m_model3dRendering;
    RenderingSurfaceSet *m_renderingSurfaceSet;
    ControllerMouseHandler3DTransformations *m_controllerMouseHandler3DTransformations;
};

#endif // CONTROLLER_MOUSE_HANDLER_3D_TRANSFORMATIONS_H
