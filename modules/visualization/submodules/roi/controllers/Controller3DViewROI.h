#ifndef CONTROLLER_3D_VIEW_ROI_H
#define CONTROLLER_3D_VIEW_ROI_H

#include "ui/RenderingSurface.h"
#include "../ModelROI.h"
#include "modules/visualization/submodules/3d_clipping/models/Model3DClipping.h"
#include <modules/visualization/submodules/3d_transformation/models/Model3DTransformation.h>

class RenderingSurfaceSet;
class Model3DTransformation;
class ModelROI;
class ROIRenderer3D;

class Controller3DViewROI:
        public ControllerRenderingSurface,
        public ModelObserver<Model3DTransformation, Model3DTransformationEvent>,
        public ModelObserver<ModelROI, ModelROISetEvent>
{
public:
    Controller3DViewROI();
    void afterRendering();

    void update(Model3DTransformation *subject, Model3DTransformationEvent event);
    void update(ModelROI * subject, ModelROISetEvent event);
    void updateMousePosition(ModelCursor * model, ModelCursorEvent event);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);

    void setRenderer(ROIRenderer3D *renderer);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setContext();

    void setModel3DTransformation(Model3DTransformation *model3DTransformation);
    void setModelROI(ModelROI *modelROI);

    void setActivationFreeDraw3D(bool state);
    void setStateRenderingFreeDraw3D(bool state);

private:

    RenderingSurfaceSet *mRenderingSurfaceSet;

    ROIRenderer3D *mRenderer;

    Model3DTransformation *m_model3DTransformation;
    ModelROI *mModelROI;

    bool mActiveFreeLine3D;
    bool mEnableRenderFreeDraw3D ;
    int m_cursorEventSlotID;
};

#endif // CONTROLLER_3D_VIEW_ROI_H
