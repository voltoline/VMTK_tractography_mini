#ifndef VIS_SUB_MODULE_DTI_H
#define VIS_SUB_MODULE_DTI_H

#include "modules/visualization/VisualizationModule.h"
#include "models/ModelDTI.h"
#include "models/ModelUIStateDTI.h"
#include "DTIOpenGLResource.h"

class ControllerDTITractographyToolTab;
class EventHandlerDTI;
class EventHandlerDTIFiberTracking;
class ModelVolume;
class EventHandler3DTransformationToDTI;
class Model2DSlice;
class Model2DTransformation;
class Model3DTransformation;
class VisSubModuleBaseVolumeRenderer;

template <class vis_module_type>
class VisSubModuleDTI: public VisualizationSubModule
{
public:
    VisSubModuleDTI(std::string vis_sub_module_name = "VisSubModuleDTI");
    ~VisSubModuleDTI() { }

    void init(VisualizationModule *vis_module);

    void configure3DView();
    void onAllRenderingSurfacesCreated();

    ModelDTI *getModelDTI() const;
    ModelUIStateDTI *getModelUIStateDTI() const;
    DTIOpenGLResource *getDtiOpenGLResource() const;

private:
    void createEventHandlers();
    void createControllers();

    vis_module_type *m_visModule;

    bool m_createRendererFlag;
    ModelDTI *m_modelDTI;
    ModelUIStateDTI *m_modelUIStateDTI;

    ModelVolume *m_modelVolume;
    Model2DSlice *m_model2DSlice;
    Model2DTransformation *m_model2Dtransformation;
    Model3DTransformation *m_model3DTransformation;

    DTIOpenGLResource *m_dtiOpenGLResource;

    VisSubModuleBaseVolumeRenderer *m_visSubmoduleVolumeRenderer;

    EventHandlerDTI *m_DTIEventHandler;
    EventHandlerDTIFiberTracking *m_DTIFiberTrackingEventHandler;
    EventHandler3DTransformationToDTI *m_eventHandler3DTransformationToDTI;

    ControllerDTITractographyToolTab *m_controllerDTITractographyToolTab;
};

#include "VisSubModuleDTI.tpp"

#endif // VIS_SUB_MODULE_DTI_H
