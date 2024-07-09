#ifndef COMMON_OBJECTS_2D_SLICE_H
#define COMMON_OBJECTS_2D_SLICE_H

#include "model/Model2DSlice.h"
#include "model/Model2DSliceUIState.h"
#include "SliceSynchronizer.h"
#include "ui/components/RenderingSurfaceSet.h"
#include "modules/visualization/submodules/volume_renderer/model/Model2DRendering.h"
#include "modules/visualization/submodules/volume_renderer/model/Model3DRendering.h"

class CommonObjects2DSlice
{
public:
    CommonObjects2DSlice(Model3DClipping *model_3d_clipping, Model2DSlice *model_slice, Model2DSliceUIState *model_slice_ui_state,
                         SliceSynchronizer *slice_sync, RenderingSurfaceSet *rendering_surface_set,
                         Model2DRendering *model_2d_rendering, Model3DRendering *model_3d_rendering);

    Model2DSlice *getModel2DSlice() const;

    Model2DSliceUIState *getModel2DSliceUIState() const;

    SliceSynchronizer *getSliceSynchronizer() const;

    RenderingSurfaceSet *getRenderingSurfaceSet() const;

    Model2DRendering *getModel2DRendering() const;

    Model3DRendering *getModel3DRendering() const;

    Model3DClipping *getModel3DClipping() const;

private:

    Model2DSlice *m_model2DSlice;
    Model3DClipping *m_model3DClipping;
    Model2DSliceUIState *m_model2DSliceUIState;
    SliceSynchronizer *m_sliceSynchronizer;
    RenderingSurfaceSet *m_renderingSurfaceSet;
    Model2DRendering *m_model2DRendering;
    Model3DRendering *m_model3DRendering;
};

#endif // COMMON_OBJECTS_2D_SLICE_H
