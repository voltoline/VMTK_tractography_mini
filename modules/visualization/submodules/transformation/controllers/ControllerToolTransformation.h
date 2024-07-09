#ifndef CONTROLLER_TOOL_TRANSFORMATION_H
#define CONTROLLER_TOOL_TRANSFORMATION_H

#include "model/ModelObserver.h"
#include "../../3d_transformation/models/Model3DTransformation.h"
#include "../../2d_transformation/model/Model2DTransformationUIState.h"
#include "core/ToolBase.h"

class Model2DTransformation;

class ControllerToolTransformation:
        public ModelObserver<Model3DTransformation, Model3DTransformationEvent>,
        public ModelObserver<Model2DTransformationUIState, Model2DTransformationUIStateEvent>,
        public ControllerTool
{
public:
    ControllerToolTransformation();
    ~ControllerToolTransformation() {}

    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type) override;
    void update(Model2DTransformationUIState*, Model2DTransformationUIStateEvent event) override;

    void setMouseMode(short mode);
    short int getMouseMode();

    void resetTransformations();

    bool is3DTransformationActivated();
    bool is2DTransformationActivated();

    bool is3DTransformationEnabled();
    bool is2DTransformationEnabled();

    void set3DTransformationActivated(bool state);
    void set2DTransformationActivated(bool state);

    void setModel3DTransformation(Model3DTransformation *model3D);
    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
    void setModel2DTransformationUIState(Model2DTransformationUIState *model2DTransformationState);

private:
    Model3DTransformation *m_model3DTransformation;
    Model2DTransformation *m_model2DTransformation;
    Model2DTransformationUIState *m_model2DTransformationUIState;
};

#endif // CONTROLLER_TOOL_TRANSFORMATION_H
