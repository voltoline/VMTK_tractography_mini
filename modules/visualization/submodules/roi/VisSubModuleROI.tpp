#include "VisSubModuleROI.h"

#include <modules/visualization/submodules/transformation/VisSubModuleTransformation.h>
#include <modules/visualization/submodules/dti/VisSubModuleDTI.h>
#include <modules/visualization/submodules/roi/event_handler/EventHandlerSlice2DToROI.h>
#include "event_handler/EventHandler2DTransformationToROI.h"
#include "event_handler/EventHandlerROI.h"
#include "renderer/ROIRenderer2D.h"
#include <modules/visualization/submodules/roi/event_handler/EventHandlerDTIToROI.h>
#include "controllers/ControllerROIToolTab.h"
#include <modules/visualization/submodules/roi/controllers/Controller2DViewROI.h>
#include <modules/visualization/submodules/roi/controllers/Controller3DViewROI.h>
#include <modules/visualization/submodules/2d_3d_slicing/VisSubModule2D3DSlicing.h>

template<class vis_module_type>
VisSubModuleROI<vis_module_type>::VisSubModuleROI(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_visModule = nullptr;
    mModelROI = nullptr;
    m_modelDTI = nullptr;
    m_model2DSlice = nullptr;
    m_model2DTransformation = nullptr;
    mController3DViewROI = nullptr;
    mControllerAxialViewROI = nullptr;
    mControllerCoronalViewROI = nullptr;
    mControllerSagittalViewROI = nullptr;
    mControllerROIToolTab = nullptr;
    m_DTIEventHandlerForROI = nullptr;
    m_rendererROIAxial = nullptr;
    m_rendererROICoronal = nullptr;
    m_rendererROISagittal = nullptr;
    m_eventHandlerROI = nullptr;
    mRenderingSurfaceSet = nullptr;
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::init(VisualizationModule *vis_module)
{
    m_model2DTransformation = static_cast<VisSubModuleTransformation*>(vis_module->getVisSubModule("VisSubModuleTransformation"))->getModel2DTransformation();
    m_model2DSlice = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"))->getModel2DSlice();
    m_modelDTI = static_cast<VisSubModuleDTI<vis_module_type>*>(vis_module->getVisSubModule("VisSubModuleDTI"))->getModelDTI();

    m_visModule = static_cast<vis_module_type*>(vis_module);

    mRenderingSurfaceSet = vis_module->getRenderingSurfaceSet();

    setupModel();

    setup3DView();

    setupAxialView();

    setupCoronalView();

    setupSagittalView();

    setupTooltabController();

    setupEventHandler();
}

template<class vis_module_type>
ModelROI * VisSubModuleROI<vis_module_type>::getModelROI()
{
    return mModelROI;
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupModel()
{
    auto *acquisition = m_visModule->getAcquisition();

    mModelROI = new ModelROI();
    mModelROI->setDataAcquisition(acquisition->getDimensions(0), acquisition->getDimensions(1), acquisition->getDimensions(2),
                                  acquisition->getPixelSpacing(0), acquisition->getPixelSpacing(1), acquisition->getPixelSpacing(2), acquisition->getPatientID());
    mModelROI->setModel2DSlice(m_model2DSlice);
    mModelROI->setModel2DTransformation(m_model2DTransformation);
    mModelROI->setStateManager(m_visModule->getStateManager());
    mModelROI->setMainWindow(m_visModule->getMainWindow());
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setup3DView()
{

}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupAxialView()
{
    if(m_visModule->getRendererAxial() != nullptr)
    {
        m_rendererROIAxial = new ROIRenderer2D();

        std::cout << "VisSubModuleROI::setupAxialView " << m_model2DSlice->getAxialSlice() << "slice" << std::endl;

        m_rendererROIAxial->setCurrentSlice(m_model2DSlice->getAxialSlice());
        m_rendererROIAxial->setCurrentTractID(m_modelDTI->getCurrentFiberBundleEditionIndex());
        m_rendererROIAxial->setModelViewMatrix(m_model2DTransformation->getAxialData().getModelViewMatrix());
        m_rendererROIAxial->setProjectionMatrix(m_model2DTransformation->getAxialData().getProjectionMatrix());

        m_visModule->getRendererAxial()->addSubRenderer(m_rendererROIAxial);

        mControllerAxialViewROI = new Controller2DViewROI(Controller2DViewROI::View::AXIAL);
        mControllerAxialViewROI->setRenderingSurfaceSet(mRenderingSurfaceSet);
        mControllerAxialViewROI->setModelROI(mModelROI);
        mControllerAxialViewROI->setRoiRenderer2D(m_rendererROIAxial);
    }
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupCoronalView()
{
    if(m_visModule->getRendererCoronal() != nullptr)
    {
        m_rendererROICoronal = new ROIRenderer2D();

        std::cout << "VisSubModuleROI::setupCoronalView " << m_model2DSlice->getCoronalSlice() << "slice" << std::endl;

        m_rendererROICoronal->setCurrentSlice(m_model2DSlice->getCoronalSlice());
        m_rendererROICoronal->setCurrentTractID(m_modelDTI->getCurrentFiberBundleEditionIndex());
        m_rendererROICoronal->setModelViewMatrix(m_model2DTransformation->getCoronalData().getModelViewMatrix());
        m_rendererROICoronal->setProjectionMatrix(m_model2DTransformation->getCoronalData().getProjectionMatrix());

        m_visModule->getRendererCoronal()->addSubRenderer(m_rendererROICoronal);

        mControllerCoronalViewROI = new Controller2DViewROI(Controller2DViewROI::View::CORONAL);
        mControllerCoronalViewROI->setRenderingSurfaceSet(mRenderingSurfaceSet);
        mControllerCoronalViewROI->setModelROI(mModelROI);
        mControllerCoronalViewROI->setRoiRenderer2D(m_rendererROICoronal);
    }
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupSagittalView()
{
    if(m_visModule->getRendererSagittal() != nullptr)
    {
        m_rendererROISagittal = new ROIRenderer2D();

        std::cout << "VisSubModuleROI::setupSagittalView " << m_model2DSlice->getSagittalSlice() << "slice" << std::endl;

        m_rendererROISagittal->setCurrentSlice(m_model2DSlice->getSagittalSlice());
        m_rendererROISagittal->setCurrentTractID(m_modelDTI->getCurrentFiberBundleEditionIndex());
        m_rendererROISagittal->setModelViewMatrix(m_model2DTransformation->getSagittalData().getModelViewMatrix());
        m_rendererROISagittal->setProjectionMatrix(m_model2DTransformation->getSagittalData().getProjectionMatrix());

        m_visModule->getRendererSagittal()->addSubRenderer(m_rendererROISagittal);

        mControllerSagittalViewROI = new Controller2DViewROI(Controller2DViewROI::View::SAGITTAL);
        mControllerSagittalViewROI->setRenderingSurfaceSet(mRenderingSurfaceSet);
        mControllerSagittalViewROI->setModelROI(mModelROI);
        mControllerSagittalViewROI->setRoiRenderer2D(m_rendererROISagittal);
    }
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupEventHandler()
{
    EventHandler2DTransformationToROI *event_handler = new EventHandler2DTransformationToROI();
    event_handler->setModel2DTransformation(m_model2DTransformation);
    event_handler->setRendererROIAxial(m_rendererROIAxial);
    event_handler->setRendererROICoronal(m_rendererROICoronal);
    event_handler->setRendererROISagittal(m_rendererROISagittal);

    m_DTIEventHandlerForROI = new EventHandlerDTIToROI();
    m_DTIEventHandlerForROI->setModelDTI(m_modelDTI);
    m_DTIEventHandlerForROI->setModelROI(mModelROI);
    m_DTIEventHandlerForROI->setControllerROIToolTab(mControllerROIToolTab);
    m_DTIEventHandlerForROI->setRoiRenderer2DAxial(m_rendererROIAxial);
    m_DTIEventHandlerForROI->setRoiRenderer2DSagittal(m_rendererROISagittal);
    m_DTIEventHandlerForROI->setRoiRenderer2DCoronal(m_rendererROICoronal);
    m_DTIEventHandlerForROI->setRenderingSurfaceSet(mRenderingSurfaceSet);

    EventHandlerSlice2DToROI *event_handler_slice = new EventHandlerSlice2DToROI();
    event_handler_slice->setModel2DSlice(m_model2DSlice);
    event_handler_slice->setRendererAxial(m_rendererROIAxial);
    event_handler_slice->setRendererCoronal(m_rendererROICoronal);
    event_handler_slice->setRendererSagittal(m_rendererROISagittal);

    m_eventHandlerROI = new EventHandlerROI();
    m_eventHandlerROI->setModelROI(mModelROI);
    m_eventHandlerROI->setStateManager(m_visModule->getStateManager());
    m_eventHandlerROI->setMainWindow(m_visModule->getMainWindow());

    mControllerROIToolTab->setEventHandlerROI(m_eventHandlerROI);
}

template<class vis_module_type>
void VisSubModuleROI<vis_module_type>::setupTooltabController()
{
    mControllerROIToolTab = new ControllerROIToolTab();
    mControllerROIToolTab->set2DViewControllerROIAxial(mControllerAxialViewROI);
    mControllerROIToolTab->set2DViewControllerROICoronal(mControllerCoronalViewROI);
    mControllerROIToolTab->set2DViewControllerROISagittal(mControllerSagittalViewROI);
    mControllerROIToolTab->set3DViewControllerROI(mController3DViewROI);
    mControllerROIToolTab->setModelDTI(m_modelDTI);
    mControllerROIToolTab->setRenderingSurfaceSet(mRenderingSurfaceSet);
    mControllerROIToolTab->setModelROI(mModelROI);

    ToolTractography *tool_tract = static_cast<ToolTractography*>(m_visModule->getMainWindow()->getToolUniqueWindowFloat(mControllerROIToolTab->getToolClassName()));
    if(tool_tract != nullptr)
        tool_tract->setController(m_visModule->getVisualizationTab(), mControllerROIToolTab);
}
