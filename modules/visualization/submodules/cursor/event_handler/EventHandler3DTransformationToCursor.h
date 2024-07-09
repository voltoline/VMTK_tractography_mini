#ifndef EVENT_HANDLER_3D_TRANSFORMATION_TO_CURSOR_H
#define EVENT_HANDLER_3D_TRANSFORMATION_TO_CURSOR_H

#include <modules/visualization/submodules/3d_transformation/models/Model3DTransformation.h>

class CursorRenderer3D;
class ModelCursor;

class EventHandler3DTransformationToCursor:
        public ModelObserver<Model3DTransformation, Model3DTransformationEvent>
{
public:
    EventHandler3DTransformationToCursor();

    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type);

    void setModel3DTransformation(Model3DTransformation *model3DTransformation);
    void setCursorRenderer3D(CursorRenderer3D *cursorRenderer3D);
    void setModelCursor(ModelCursor *modelCursor);

private:
    Model3DTransformation *m_model3DTransformation;
    CursorRenderer3D *m_cursorRenderer3D;
    ModelCursor *m_modelCursor;
};

#endif // EVENT_HANDLER_3D_TRANSFORMATION_TO_CURSOR_H
