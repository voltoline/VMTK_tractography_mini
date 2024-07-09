#ifndef CONTROLLER2DVIEWROI_H
#define CONTROLLER2DVIEWROI_H

#include "ui/RenderingSurface.h"
#include "model/ModelObserver.h"
#include "../ModelROI.h"

class RenderingSurfaceSet;
class ROIRenderer2D;

class Controller2DViewROI: public ControllerRenderingSurface, public ModelObserver<ModelROI, ModelROISetEvent>
{
public:
    enum class View
    {
        AXIAL,
        CORONAL,
        SAGITTAL
    };

    Controller2DViewROI(View view);
    ~Controller2DViewROI();

    void update(ModelROI *subject, ModelROISetEvent event);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseDoubleClickEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setContext();

    void setActivationFreeDrawROI(bool activation);
    void setStateRenderingFreeDrawCursorROI(bool state);

    void setModelROI(ModelROI *modelROI);
    void setRoiRenderer2D(ROIRenderer2D *roiRenderer2D);

private:

    bool isAxial();
    bool isCoronal();
    bool isSagittal();

    void update2DRendering();

    View m_view;
    bool m_enableRenderFreeDrawROI;
    bool m_activeFreeDrawROI;

    bool m_mouseCaptureEnable;

    RenderingSurfaceSet *m_renderingSurfaceSet;
    ModelROI *m_modelROI;
    ModelCursor *m_modelCursor;
    ROIRenderer2D *m_roiRenderer2D;
};

#endif // CONTROLLER2DVIEWROI_H
