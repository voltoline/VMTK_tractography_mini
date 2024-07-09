#ifndef VIS_SUB_MODULE_2D_3D_SLICING_H
#define VIS_SUB_MODULE_2D_3D_SLICING_H

#include "modules/visualization/VisualizationModule.h"
#include "../3d_clipping/models/Model3DClipping.h"

#include "modules/visualization/submodules/2d_slice/controller/ControllerToolSliceOrientation.h"
#include "modules/visualization/submodules/2d_slice/event_handler/EventHandler2DSlice.h"
#include "modules/visualization/submodules/2d_slice/model/Model2DSlice.h"
#include "modules/visualization/submodules/2d_slice/model/Model2DSliceUIState.h"


class Controller2D3DSlicingTool;
class EventHandler3DClipping;
class Model3DRendering;
class ControllerMouseHandler2D3DSlicing;
class SliceSynchronizer;

template <class vis_module_type>
class VisSubModule2D3DSlicing: public VisualizationSubModule
{
public:
    VisSubModule2D3DSlicing(std::string vis_sub_module_name = "VisSubModule2D3DSlicing");
    ~VisSubModule2D3DSlicing() { }

    Model3DClipping *getModel3DClipping() const;
    Model2DSlice *getModel2DSlice() const;
    SliceSynchronizer *getSliceSynchronizer() const;

private:
    void init(VisualizationModule *vis_module) override;

    void createModel();
    void createEventHandler();
    void createControllerToolTab();

    void setupSliceSynchronizer();

    void on3DRenderingSurfaceCreated() override;

    void onAxialRenderingSurfaceCreated() override;
    void onCoronalRenderingSurfaceCreated() override;
    void onSagittalRenderingSurfaceCreated() override;
    void onAllRenderingSurfacesCreated() override;

    CommonObjects2DSlice *m_commonObjects;

    vis_module_type *m_visModule;
    Model3DRendering *m_model3DRendering;
    Model3DClipping *m_model3DClipping;
    EventHandler3DClipping *m_eventHandler3DClipping;
    Controller2D3DSlicingTool *m_controllerToolTab2D3DSlicing;

    Model2DSlice *m_model2DSlice;
    Model2DSliceUIState *m_model2DSliceUIState;
    Model2DRendering *m_model2DRendering;

    SliceSynchronizer *m_sliceSynchronizer;
    ControllerToolSliceOrientation *m_controllerToolSliceOrientation;

    ControllerMouseHandler2D3DSlicing *m_mouseHandlerCursorAxial;
    ControllerMouseHandler2D3DSlicing *m_mouseHandlerCursorCoronal;
    ControllerMouseHandler2D3DSlicing *m_mouseHandlerCursorSagittal;

    EventHandler2DSlice *m_eventHandler;

    FeatureStates::sPtr m_states2DSliceTo3DClippingSync;
};

#include "VisSubModule2D3DSlicing.tpp"

#endif // VIS_SUB_MODULE_2D_3D_SLICING_H
