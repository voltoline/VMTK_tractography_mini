#include "CommonObjects2DSlice.h"

CommonObjects2DSlice::CommonObjects2DSlice(Model3DClipping *model_3d_clipping, Model2DSlice *model_slice, Model2DSliceUIState *model_slice_ui_state,
                                           SliceSynchronizer *slice_sync, RenderingSurfaceSet *rendering_surface_set,
                                           Model2DRendering *model_2d_rendering, Model3DRendering *model_3d_rendering)
{
    m_model3DClipping = model_3d_clipping;
    m_model2DSlice = model_slice;
    m_model2DSliceUIState = model_slice_ui_state;
    m_sliceSynchronizer = slice_sync;
    m_renderingSurfaceSet = rendering_surface_set;
    m_model2DRendering = model_2d_rendering;
    m_model3DRendering = model_3d_rendering;
}

Model2DSlice *CommonObjects2DSlice::getModel2DSlice() const
{
    return m_model2DSlice;
}

Model2DSliceUIState *CommonObjects2DSlice::getModel2DSliceUIState() const
{
    return m_model2DSliceUIState;
}

SliceSynchronizer *CommonObjects2DSlice::getSliceSynchronizer() const
{
    return m_sliceSynchronizer;
}

RenderingSurfaceSet *CommonObjects2DSlice::getRenderingSurfaceSet() const
{
    return m_renderingSurfaceSet;
}

Model2DRendering *CommonObjects2DSlice::getModel2DRendering() const
{
    return m_model2DRendering;
}

Model3DRendering *CommonObjects2DSlice::getModel3DRendering() const
{
    return m_model3DRendering;
}

Model3DClipping *CommonObjects2DSlice::getModel3DClipping() const
{
    return m_model3DClipping;
}
