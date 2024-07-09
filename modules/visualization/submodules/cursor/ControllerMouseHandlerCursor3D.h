#ifndef CONTROLLER_MOUSE_HANDLER_CURSOR_3D_H
#define CONTROLLER_MOUSE_HANDLER_CURSOR_3D_H

#include "ui/RenderingSurface.h"
#include "ModelCursor.h"

class RenderingSurfaceSet;
class SliceSynchronizer;
class CursorRenderer3D;
class BaseRaycastingVolumeRenderer3D;

class ControllerMouseHandlerCursor3D:
        public ControllerRenderingSurface
{
public:
    ControllerMouseHandlerCursor3D();

    void handleCursorStateChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);
    void handleCursorPositionChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);
    void handleCursorLockStateChangeEvent(ModelCursor *, ModelCursorEvent event_type);
    void handleCursorDrawTypeChangeEvent(ModelCursor *subject, ModelCursorEvent event_type);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface) override;
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface) override;

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModelCursor(ModelCursor *modelCursor);
    void setSliceSynchronizer(SliceSynchronizer *sliceSynchronizer);
    void setCursorRenderer(CursorRenderer3D *cursorRenderer);
    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D);

    void handleCursorOffScreenChangeEvent(ModelCursor *modelCursor, ModelCursorEvent event_type);
private:
    void updateRenderingSurfaceController(ControllerRenderingSurface *controller);

    bool m_mouseCursorEnable;
    bool m_mouseEventLock;

    CursorRenderer3D *m_renderer;
    RenderingSurfaceSet *m_renderingSurfaceSet;
    ModelCursor *m_modelCursor;
    SliceSynchronizer *m_sliceSynchronizer;

    BaseRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;

    int m_cursorEventSlotID;
    int m_cursorStateChangedHandlerSlotID;
    int m_cursorLockStateChangedHandlerSlotId;
    int m_cursorDrawTypeChangedHandlerSlotID;

    int m_cursorOffScreenSlotID;
};

#endif // CONTROLLER_MOUSE_HANDLER_CURSOR_3D_H
