#ifndef EVENT_HANDLER_SLICE_2D_TO_ROI_H
#define EVENT_HANDLER_SLICE_2D_TO_ROI_H

#include "ui/RenderingSurface.h"
#include "model/ModelObserver.h"
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/Model2DSlice.h"
#else
#include <modules/visualization/submodules/2d_slice/model/Model2DSlice.h>
#endif
class RenderingSurfaceSet;
class ROIRenderer2D;

class EventHandlerSlice2DToROI: public ModelObserver<Model2DSlice, Model2DSliceEvent>
{
public:
    EventHandlerSlice2DToROI();
    ~EventHandlerSlice2DToROI();

    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

    void setRendererAxial(ROIRenderer2D *renderer);
    void setRendererCoronal(ROIRenderer2D *renderer);
    void setRendererSagittal(ROIRenderer2D *renderer);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModel2DSlice(Model2DSlice *model2DSlice);

private:

    typedef struct
    {
        int mAxial;
        int mCoronal;
        int mSagittal;

    } SliceData;

    SliceData mSliceData;

    ROIRenderer2D *m_rendererAxial;
    ROIRenderer2D *m_rendererCoronal;
    ROIRenderer2D *m_rendererSagittal;

    Model2DSlice *m_model2DSlice;

    RenderingSurfaceSet *mRenderingSurfaceSet;
};

#endif // EVENT_HANDLER_SLICE_2D_TO_ROI_H
