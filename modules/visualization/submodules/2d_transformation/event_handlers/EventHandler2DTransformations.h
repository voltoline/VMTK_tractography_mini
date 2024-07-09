#ifndef EVENT_HANDLER_2D_TRANSFORMATIONS_H
#define EVENT_HANDLER_2D_TRANSFORMATIONS_H

#include "ui/RenderingSurface.h"
#include "../model/Model2DTransformationUIState.h"
#include "../model/Model2DTransformation.h"
#include "../controller/ControllerMouseHandler2DTransformations.h"

class RenderingSurfaceSet;
class Model2DRendering;

class EventHandler2DTransformations:
        public ModelObserver<Model2DTransformation, Model2DTransformationEvent>,
        public ModelObserver<Model2DTransformationUIState, Model2DTransformationUIStateEvent>
{
public:
    EventHandler2DTransformations();

    void update(Model2DTransformation *subject, Model2DTransformationEvent event_type);
    void update(Model2DTransformationUIState *subject, Model2DTransformationUIStateEvent event_type);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
    void setModel2DTransformationUIState(Model2DTransformationUIState *model2DTransformationUIState);
    void setModel2dRendering(Model2DRendering *model2dRendering);

    void setControllerMouseHandler2DTransformationsAxial(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsAxial);
    void setControllerMouseHandler2DTransformationsCoronal(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsCoronal);
    void setControllerMouseHandler2DTransformationsSagittal(ControllerMouseHandler2DTransformations *controllerMouseHandler2DTransformationsSagittal);

private:
    void updateRenderingSurfaceController();
    void setMouseMode(ControllerMouseHandler2DTransformations::MouseMode mode);

    bool m_isActivated;

    Model2DTransformation *m_model2DTransformation;
    Model2DTransformationUIState *m_model2DTransformationUIState;
    Model2DRendering *m_model2dRendering;

    RenderingSurfaceSet *m_renderingSurfaceSet;

    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsAxial;
    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsCoronal;
    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsSagittal;
};

#endif // CONTROLLER_MOUSE_HANDLER_2D_TRANSFORMATIONS_H
