#ifndef EVENT_HANDLER_2D_TRANSFORMATION_TO_CURSOR_H
#define EVENT_HANDLER_2D_TRANSFORMATION_TO_CURSOR_H

#include <modules/visualization/submodules/2d_transformation/model/Model2DTransformation.h>

class CursorRenderer2D;
class ModelCursor;

class EventHandler2DTransformationToCursor: public ModelObserver<Model2DTransformation, Model2DTransformationEvent>
{
public:
    EventHandler2DTransformationToCursor();

    void update(Model2DTransformation *subject, Model2DTransformationEvent event_type);

    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
    void setRendererCursorAxial(CursorRenderer2D *rendererCursorAxial);
    void setRendererCursorCoronal(CursorRenderer2D *rendererCursorCoronal);
    void setRendererCursorSagittal(CursorRenderer2D *rendererCursorSagittal);
    void setModelCursor(ModelCursor *modelCursor);

private:
    Model2DTransformation *m_model2DTransformation;
    ModelCursor *m_modelCursor;

    CursorRenderer2D *m_rendererCursorAxial;
    CursorRenderer2D *m_rendererCursorCoronal;
    CursorRenderer2D *m_rendererCursorSagittal;
};

#endif // EVENT_HANDLER_2D_TRANSFORMATION_TO_CURSOR_H
