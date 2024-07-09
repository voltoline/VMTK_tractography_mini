#ifndef VIS_SUB_MODULE_TRANSFORMATION_H
#define VIS_SUB_MODULE_TRANSFORMATION_H

#include "modules/visualization/VisualizationModule.h"
#include "../2d_transformation/model/Model2DTransformation.h"
#include "../2d_transformation/model/Model2DTransformationUIState.h"

#include "../3d_transformation/models/Model3DTransformation.h"

class ControllerMouseHandler2DTransformations;
class Model2DRendering;
class EventHandler2DTransformations;

class ControllerMouseHandler3DTransformations;
class Model3DRendering;
class EventHandler3DTransformations;

class ControllerToolTransformation;

class VisSubModuleTransformation: public VisualizationSubModule
{
public:
    VisSubModuleTransformation(std::string vis_sub_module_name = "VisSubModuleTransformation");
    ~VisSubModuleTransformation() { }

    Model2DTransformation *getModel2DTransformation() const;
    Model2DTransformationUIState *getModel2DTransformationUIState() const;

    Model3DTransformation *getModel3DTransformation() const;

private:
    void init(VisualizationModule *vis_module) override;

    void createModel();
    void createControllerToolTab();
    void createEventHandler();

    void createControllerMouseHandler3D();
    void createControllerMouseHandlerAxial();
    void createControllerMouseHandlerCoronal();
    void createControllerMouseHandlerSagittal();

    void configure3DView() override;
    void configureAxialView() override;
    void configureCoronalView() override;
    void configureSagittalView() override;

    void on3DRenderingSurfaceCreated() override;
    void onAxialRenderingSurfaceCreated() override;
    void onCoronalRenderingSurfaceCreated() override;
    void onSagittalRenderingSurfaceCreated() override;

    VisualizationModule *m_visModule;
    Model2DRendering *m_model2DRendering;
    Model2DTransformation *m_model2DTransformation;
    Model2DTransformationUIState *m_model2DTransformationUIState;
    EventHandler2DTransformations *m_eventHandler2DTransformations;
    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsAxial;
    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsCoronal;
    ControllerMouseHandler2DTransformations *m_controllerMouseHandler2DTransformationsSagittal;

    Model3DRendering *m_model3dRendering;
    Model3DTransformation *m_model3DTransformation;
    ControllerMouseHandler3DTransformations *m_controllerMouseHandler3DTransformations;
    EventHandler3DTransformations *m_eventHandler3DTransformations;

    ControllerToolTransformation *m_controllerToolTransformation;
};

#endif // VIS_SUB_MODULE_TRANSFORMATION_H
