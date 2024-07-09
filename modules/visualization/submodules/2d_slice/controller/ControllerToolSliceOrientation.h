#ifndef CONTROLLER_SLICE_ORIENTATION_H
#define CONTROLLER_SLICE_ORIENTATION_H

#include "model/ModelObserver.h"
#include "core/ToolBase.h"
#include "ui/components/RenderingSurfaceSet.h"


#include "modules/visualization/submodules/2d_slice/model/Model2DSlice.h"
#include "modules/visualization/submodules/2d_slice/model/Model2DSliceUIState.h"

class CommonObjects2DSlice;
class Model2DSliceUIState;

class ControllerToolSliceOrientation:
        public ModelObserver<Model2DSlice, Model2DSliceEvent>,
        public ControllerTool
{
public:
    ControllerToolSliceOrientation(CommonObjects2DSlice *common_objects);
    ~ControllerToolSliceOrientation();

    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

    void setRadiologicalMode();
    void setNeurologicalMode();

    bool is2DSliceActivated();

    bool isRadiological();
    bool isNeurological();

    bool isOrientationEnabled();

private:
    Model2DSlice *m_model2DSlice;
    Model2DSliceUIState *m_model2DSliceUIState;
};

#endif // CONTROLLER_SLICE_ORIENTATION_H
