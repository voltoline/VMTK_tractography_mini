#include "VisSubModuleDTI.h"

#include "event_handlers/EventHandler3DTransformationToDTI.h"
#include <modules/visualization/submodules/transformation/VisSubModuleTransformation.h>
#include "models/ModelUIStateDTI.h"
#include <modules/visualization/submodules/dti/renderer/Renderer3DDTIData.h>
#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"
#include "controllers/ControllerDTITractographyToolTab.h"
#include "event_handlers/EventHandlerDTI.h"
#include "event_handlers/EventHandlerDTIFiberTracking.h"
#include "ui/qt/UIFactoryDTIQt.h"
#include "ui/ToolTractography.h"
#include <modules/visualization/submodules/2d_3d_slicing/VisSubModule2D3DSlicing.h>

template <class vis_module_type>
VisSubModuleDTI<vis_module_type>::VisSubModuleDTI(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_visModule = nullptr;
    m_createRendererFlag = false;

    m_modelDTI = nullptr;
    m_modelUIStateDTI = nullptr;
    m_modelVolume = nullptr;
    m_model2DSlice = nullptr;
    m_model2Dtransformation = nullptr;
    m_model3DTransformation = nullptr;

    m_DTIEventHandler = nullptr;
    m_DTIFiberTrackingEventHandler = nullptr;
    m_eventHandler3DTransformationToDTI = nullptr;

    m_controllerDTITractographyToolTab = nullptr;

    m_dtiOpenGLResource = nullptr;
    m_visSubmoduleVolumeRenderer = nullptr;
}

template<class vis_module_type>
void VisSubModuleDTI<vis_module_type>::onAllRenderingSurfacesCreated()
{
//     TO COMMENT AFTER FINISH
    m_modelDTI->loadDTI("./volume/rvr_iwlls.vdti");
    m_modelDTI->notify();

    m_modelDTI->setScalarIndexRenderingActivated(true);
    m_modelDTI->notify();

    m_modelDTI->setCurrentScalarIndexMode(ModelDTI::ScalarIndex::COLOR_CODED_FA);
    m_modelDTI->notify();
}


template<class vis_module_type>
void VisSubModuleDTI<vis_module_type>::init(VisualizationModule *vis_module)
{
    m_visModule = static_cast<vis_module_type*>(vis_module);

    VisSubModule2D3DSlicing<vis_module_type> *vis_submodule_2d_slice;
    VisSubModuleTransformation *vis_submodule_transformation;

    m_visSubmoduleVolumeRenderer = static_cast<VisSubModuleBaseVolumeRenderer*>(vis_module->getVisSubModule("VisSubModuleBaseVolumeRenderer"));

    if(m_visSubmoduleVolumeRenderer != nullptr)
    {
        m_createRendererFlag = true;

        m_modelDTI = new ModelDTI();
        m_modelUIStateDTI = new ModelUIStateDTI();
        m_dtiOpenGLResource = new DTIOpenGLResource(m_modelDTI);

        auto *acq = m_visModule->getAcquisition();
        m_modelDTI->setAcquisition(acq);
        m_modelDTI->setProgressBar(vis_module->getMainWindow()->createProgressDialog());

        m_modelVolume = m_visSubmoduleVolumeRenderer->getModelVolume();

        vis_submodule_2d_slice = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"));
        vis_submodule_transformation = static_cast<VisSubModuleTransformation*>(m_visModule->getVisSubModule("VisSubModuleTransformation"));

        if(vis_submodule_2d_slice != nullptr)
            m_model2DSlice = vis_submodule_2d_slice->getModel2DSlice();

        if(vis_submodule_transformation != nullptr)
        {
            m_model2Dtransformation = vis_submodule_transformation->getModel2DTransformation();
            m_model3DTransformation = vis_submodule_transformation->getModel3DTransformation();
        }

        createEventHandlers();
        createControllers();

    } else
        std::cout << "VisSubModuleDTI::init --> VisSubModuleBaseVolumeRenderer not found!" << std::endl;
}

template<class vis_module_type>
void VisSubModuleDTI<vis_module_type>::configure3DView()
{
    if(m_createRendererFlag)
    {
        auto *acq = m_visModule->getAcquisition();

        float scale_factors[4];
        m_modelVolume->getScaleFactors(scale_factors);

        BaseRaycastingVolumeRenderer3D *raycasting_volume_renderer_3d = m_visSubmoduleVolumeRenderer->getRaycastingVolumeRenderer3D();

        RendererDTIData *renderer_dti_data = new RendererDTIData();
        renderer_dti_data->setDtiOpenGLResource(m_dtiOpenGLResource);
        renderer_dti_data->setDiff_acq(acq);
        renderer_dti_data->setRaycastingVolumeRenderer3D(raycasting_volume_renderer_3d);
        renderer_dti_data->setScaleFactors(scale_factors);

        if(m_model3DTransformation)
        {
            renderer_dti_data->setProjectionMatrix(m_model3DTransformation->getProjectionMatrix());
            renderer_dti_data->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix());
        }

        m_visModule->getRenderer3D()->addSubRenderer(renderer_dti_data);

        if(m_DTIEventHandler)
            m_DTIEventHandler->setRendererDTIData(renderer_dti_data);

        if(m_eventHandler3DTransformationToDTI)
            m_eventHandler3DTransformationToDTI->setRendererDTIData(renderer_dti_data);

        if(m_DTIFiberTrackingEventHandler)
            m_DTIFiberTrackingEventHandler->setRendererDTIData(renderer_dti_data);
    }
}

template <class vis_module_type>
ModelDTI *VisSubModuleDTI<vis_module_type>::getModelDTI() const
{
    return m_modelDTI;
}

template <class vis_module_type>
DTIOpenGLResource *VisSubModuleDTI<vis_module_type>::getDtiOpenGLResource() const
{
    return m_dtiOpenGLResource;
}

template <class vis_module_type>
ModelUIStateDTI *VisSubModuleDTI<vis_module_type>::getModelUIStateDTI() const
{
    return m_modelUIStateDTI;
}

template<class vis_module_type>
void VisSubModuleDTI<vis_module_type>::createEventHandlers()
{
    if(m_modelDTI)
    {
        m_DTIEventHandler = new EventHandlerDTI();
        m_DTIEventHandler->setModelDTI(m_modelDTI);
        m_DTIEventHandler->setModelUIStateDTI(m_modelUIStateDTI);
        m_DTIEventHandler->setModelVolume(m_modelVolume);
        m_DTIEventHandler->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    }


    if(m_modelDTI)
    {
        m_DTIFiberTrackingEventHandler = new EventHandlerDTIFiberTracking();
        m_DTIFiberTrackingEventHandler->setModelDTI(m_modelDTI);
        m_DTIFiberTrackingEventHandler->setModelUIStateDTI(m_modelUIStateDTI);
        m_DTIFiberTrackingEventHandler->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    }

    if(m_model3DTransformation)
    {
        m_eventHandler3DTransformationToDTI = new EventHandler3DTransformationToDTI();
        m_eventHandler3DTransformationToDTI->setModel3DTransformation(m_model3DTransformation);
        m_eventHandler3DTransformationToDTI->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
        m_eventHandler3DTransformationToDTI->setModelUIStateDTI(m_modelUIStateDTI);
    }
}

template<class vis_module_type>
void VisSubModuleDTI<vis_module_type>::createControllers()
{
    m_controllerDTITractographyToolTab = new ControllerDTITractographyToolTab();
    m_controllerDTITractographyToolTab->setModelDTI(m_modelDTI);
    m_controllerDTITractographyToolTab->setModelUIStateDTI(m_modelUIStateDTI);
    m_controllerDTITractographyToolTab->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

    m_visModule->getMainWindow()->addUIFactory(UIFactoryDTIQt::uPtr(new UIFactoryDTIQt(m_visModule->getMainWindow())));

    m_visModule->getMainWindow()->createMenuToolBaseUniqueAddingDialog("Diffusion", "DTI - Tractography", m_controllerDTITractographyToolTab->getToolClassName(), "UIFactoryDTIQt");

    ToolTractography *tool_tract = static_cast<ToolTractography*>(m_visModule->getMainWindow()->getToolUniqueWindowFloat(m_controllerDTITractographyToolTab->getToolClassName()));
    if(tool_tract != nullptr)
    {
        tool_tract->setController(m_visModule->getVisualizationTab(), m_controllerDTITractographyToolTab);
    }
}
