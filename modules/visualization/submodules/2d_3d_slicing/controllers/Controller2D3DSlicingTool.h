#ifndef CONTROLLER_2D_3D_SLICING_TOOL_H
#define CONTROLLER_2D_3D_SLICING_TOOL_H

#include "model/ModelObserver.h"
#include "../../3d_clipping/models/Model3DClipping.h"
#include "core/ToolBase.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "modules/visualization/submodules/2d_slice/SliceSynchronizer.h"
#include "modules/visualization/submodules/2d_slice/model/Model2DSlice.h"
#include "modules/visualization/submodules/2d_slice/model/Model2DSliceUIState.h"
#include <modules/visualization/submodules/volume_renderer/model/Model3DRendering.h>

class Controller2D3DSlicingTool:
        public ModelObserver<Model3DClipping, Model3DClippingEvent>,
        public ModelObserver<Model2DSlice, Model2DSliceEvent>,
        public ModelObserver<Model2DSliceUIState, Model2DSliceUIStateEvent>,
        public ModelObserver<SliceSynchronizer, SliceSynchronizerEvent>,
        public ModelObserver<Model3DRendering, Model3DRenderingEvent>,
        public ControllerTool
{
public:
    Controller2D3DSlicingTool();
    ~Controller2D3DSlicingTool() {}

    void update(Model3DClipping *subject, Model3DClippingEvent event_type);
    void update(Model3DRendering *subject, Model3DRenderingEvent event_type);

    void setLeftClipping(int value);
    void setRightClipping(int value);
    void setBottomClipping(int value);
    void setTopClipping(int value);
    void setBackClipping(int value);
    void setFrontClipping(int value);

    void getClippingIntervals(int& left_right_min, int& left_right_max, int& bottom_top_min, int& bottom_top_max, int& back_front_min, int& back_front_max);
    void getClippingData(int& clipLeft, int& clipRight, int& clipBottom, int& clipTop, int& clipBack, int& clipFront);

    bool is3DClippingEnabled();
    bool is3DClippingActivated();
    void set3DClippingActivated(bool state);

    void resetToEdge();

    void setModel3DClipping(Model3DClipping *model3DClipping);



    void update(Model2DSlice *subject, Model2DSliceEvent event_type);
    void update(Model2DSliceUIState *subject, Model2DSliceUIStateEvent event_type);
    void update(SliceSynchronizer *subject, SliceSynchronizerEvent event_type);

    void resetToMiddle();
    void setAxialSlice(int value);
    void setCoronalSlice(int value);
    void setSagittalSlice(int value);

    bool isRadiological();
    bool isNeurological();

    void getSlicingIntervals(int& axial_min, int& axial_max, int& coronal_min, int& coronal_max, int& sagittal_min, int& sagittal_max);
    void getSlicingData(int& axial, int& coronal, int& sagittal, bool &sync_state);

    bool is2DSliceEnabled();
    bool is2DSliceActivated();
    void set2DSliceActivated(bool state);

    bool isSliceSyncronizationEnabled();

    void set3DSliceActivated(bool state);
    bool is3DSliceActivated();

    void setModel2DSlice(Model2DSlice *model);
    void setModel2DSliceUIState(Model2DSliceUIState *model2DSliceUIState);
    void setSliceSynchronizer(SliceSynchronizer *slice_synchronizer);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setModel3DRendering(Model3DRendering *model3DRendering);

private:
    Model3DClipping *m_model3DClipping;


    Model2DSlice *m_model2DSlice;
    Model2DSliceUIState *m_model2DSliceUIState;

    SliceSynchronizer *m_sliceSynchronizer;
    RenderingSurfaceSet *m_renderingSurfaceSet;

    Model3DRendering *m_model3DRendering;
};

#endif // CONTROLLER_2D_3D_SLICING_TOOL_H
