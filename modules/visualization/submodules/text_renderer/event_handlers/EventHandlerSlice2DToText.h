#ifndef EVENT_HANDLER_SLICE_2D_TO_TEXT_H
#define EVENT_HANDLER_SLICE_2D_TO_TEXT_H

#include "ui/RenderingSurface.h"
#include "model/ModelObserver.h"
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/Model2DSlice.h"
#else
#include <modules/visualization/submodules/2d_slice/model/Model2DSlice.h>
#endif
class RenderingSurfaceSet;
class TextRenderer2DView;

class EventHandlerSlice2DToText: public ModelObserver<Model2DSlice, Model2DSliceEvent>
{
public:
    EventHandlerSlice2DToText();
    ~EventHandlerSlice2DToText();

    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

    void setRendererAxial(TextRenderer2DView *renderer);
    void setRendererCoronal(TextRenderer2DView *renderer);
    void setRendererSagittal(TextRenderer2DView *renderer);
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

    TextRenderer2DView *m_rendererAxial;
    TextRenderer2DView *m_rendererCoronal;
    TextRenderer2DView *m_rendererSagittal;

    Model2DSlice *m_model2DSlice;

    RenderingSurfaceSet *mRenderingSurfaceSet;
};

#endif // EVENT_HANDLER_SLICE_2D_TO_TEXT_H
