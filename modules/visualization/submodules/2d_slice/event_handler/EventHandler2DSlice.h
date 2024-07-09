#ifndef SLICE_2D_EVENT_HANDLER_H
#define SLICE_2D_EVENT_HANDLER_H

#include "ui/RenderingSurface.h"
#include "model/ModelObserver.h"

#include "modules/visualization/submodules/2d_slice/model/Model2DSlice.h"

class RenderingSurfaceSet;
class ModelVolume;
class Model2DRendering;
class Model3DClipping;
class Model3DRendering;
class CommonObjects2DSlice;

class EventHandler2DSlice: public ModelObserver<Model2DSlice, Model2DSliceEvent>
{
public:

    EventHandler2DSlice(CommonObjects2DSlice *common_objects);
    ~EventHandler2DSlice();

    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

private:

    typedef struct
    {
        int m_axial;
        int m_coronal;
        int m_sagittal;

    } SliceData;

    SliceData m_sliceData;
    Model3DClipping *m_model3DClipping;
    Model2DSlice *m_model2DSlice;
    Model2DRendering *m_model2DRendering;
    Model3DRendering *m_model3DRendering;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // SLICE_2D_EVENT_HANDLER_H
