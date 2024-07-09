#pragma once

#ifndef VIS_SUB_MODULE_ROI_H
#define VIS_SUB_MODULE_ROI_H

#include "modules/visualization/VisualizationModule.h"
#include "ModelROI.h"

class EventHandlerDTIToROI;
class ControllerROIToolTab;
class Controller2DViewROI;
class Controller3DViewROI;
class ModelDTI;
class ROIRenderer2D;
class EventHandlerROI;

template <class vis_module_type>
class VisSubModuleROI: public VisualizationSubModule
{
public:
    VisSubModuleROI(std::string vis_sub_module_name = "VisSubModuleROI");
    ~VisSubModuleROI() { }

    void init(VisualizationModule *vis_module);
    ModelROI * getModelROI();

private:
    void setupModel();
    void setup3DView();
    void setupAxialView();
    void setupCoronalView();
    void setupSagittalView();
    void setupEventHandler();
    void setupTooltabController();

    vis_module_type *m_visModule;

    ModelROI *mModelROI;
    ModelDTI *m_modelDTI;
    Model2DSlice *m_model2DSlice;
    Model2DTransformation *m_model2DTransformation;

    ROIRenderer2D *m_rendererROIAxial;
    ROIRenderer2D *m_rendererROICoronal;
    ROIRenderer2D *m_rendererROISagittal;

    ControllerROIToolTab *mControllerROIToolTab;

    Controller2DViewROI *mControllerAxialViewROI;
    Controller2DViewROI *mControllerCoronalViewROI;
    Controller2DViewROI *mControllerSagittalViewROI;
    Controller3DViewROI *mController3DViewROI;

    EventHandlerDTIToROI *m_DTIEventHandlerForROI;

    EventHandlerROI *m_eventHandlerROI;

    RenderingSurfaceSet *mRenderingSurfaceSet;
};

#include "VisSubModuleROI.tpp"

#endif // VISSUBMODULEROI_H
