#ifndef VIS_SUB_MODULE_CURSOR_H
#define VIS_SUB_MODULE_CURSOR_H

#include "modules/visualization/VisualizationModule.h"
#include "ControllerMouseHandlerCursor3D.h"
#include "ControllerMouseHandlerCursor2D.h"
#include "Controller2DCursorToolTab.h"

class CursorRenderer2D;
class CursorRenderer3D;
class Model3DTransformation;
class EventHandler3DTransformationToCursor;
class EventHandler2DTransformationToCursor;
class Model3DClipping;

template <class vis_module_type>
class VisSubModuleCursor: public VisualizationSubModule
{
public:
    VisSubModuleCursor(std::string vis_sub_module_name = "VisSubModuleCursor");
    ~VisSubModuleCursor() { }

    ModelCursor *getModelCursor() const;

private:
    void init(VisualizationModule *vis_module);

    void initModelCursor();
    void createEventHandlers();
    void createControllers();

    void configure3DView() override;
    void configureAxialView() override;
    void configureCoronalView() override;
    void configureSagittalView() override;

    void on3DRenderingSurfaceCreated() override;
    void onAxialRenderingSurfaceCreated() override;
    void onCoronalRenderingSurfaceCreated() override;
    void onSagittalRenderingSurfaceCreated() override;

    ModelCursor *m_modelCursor;
    vis_module_type *m_visModule;
    StateManager *m_stateManager;
    Model2DTransformation *m_model2Dtransformation;
    Model3DTransformation *m_model3Dtransformation;
    Model2DSlice *m_model2DSlice;
    Model3DClipping *m_model3dClipping;
    SliceSynchronizer *m_sliceSynchronizer;

    CursorRenderer3D *m_cursorRenderer3D;
    CursorRenderer2D *m_rendererCursorAxial;
    CursorRenderer2D *m_rendererCursorCoronal;
    CursorRenderer2D *m_rendererCursorSagittal;

    Controller2DCursorToolTab *mController2DCursorToolTab;

    EventHandler3DTransformationToCursor *m_eventHandler3DTransformationToCursor;
    EventHandler2DTransformationToCursor *m_eventHandler2DTransformationToCursor;

    ControllerMouseHandlerCursor3D *m_mouseHandlerCursor3D;
    ControllerMouseHandlerCursor2D *m_mouseHandlerCursorAxial;
    ControllerMouseHandlerCursor2D *m_mouseHandlerCursorCoronal;
    ControllerMouseHandlerCursor2D *m_mouseHandlerCursorSagittal;
};

#include "VisSubModuleCursor.tpp"

#endif // VIS_SUB_MODULE_CURSOR_H
