#ifndef CONTROLLER_MOUSE_HANDLER_CURSOR_2D_H
#define CONTROLLER_MOUSE_HANDLER_CURSOR_2D_H

#include "ui/RenderingSurface.h"
#include "ModelCursor.h"

class RenderingSurfaceSet;
class SliceSynchronizer;
class CursorRenderer2D;
class Model3DClipping;

class ControllerMouseHandlerCursor2D: public ControllerRenderingSurface
{
public:

    enum class View
    {
        AXIAL,
        CORONAL,
        SAGITTAL
    };

    ControllerMouseHandlerCursor2D(View view);

    void handleCursorStateChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);
    void handleCursorPositionChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);
    void handleCursorLockStateChangeEvent(ModelCursor *, ModelCursorEvent event_type);
    void handleCursorDrawTypeChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface) override;
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface) override;

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setRenderer(CursorRenderer2D *renderer);
    void setSliceSynchronizer(SliceSynchronizer *sliceSynchronizer);
    void setModelCursor(ModelCursor *modelCursor);

    bool isAxial();
    bool isCoronal();
    bool isSagittal();

private:

    void updateRenderingSurfaceController(ControllerRenderingSurface *controller);

    void updateRenders();

    bool m_mouseCursorEnable;
    bool m_mouseEventLock;
    CursorRenderer2D *m_renderer;
    RenderingSurfaceSet *m_renderingSurfaceSet;
    SliceSynchronizer *m_sliceSynchronizer;
    ModelCursor *m_modelCursor;

    int m_cursorPositionChangedHandlerSlotID;
    int m_cursorStateChangedHandlerSlotID;
    int m_cursorLockStateChangedHandlerSlotId;
    int m_cursorDrawTypeChangedHandlerSlotID;

    View m_view;
};

#endif // CONTROLLER_MOUSE_HANDLER_CURSOR_2D_H
