#include "VisSubModuleCursor.h"

#include "CursorRenderer2D.h"
#include "CursorRenderer3D.h"
#include <modules/visualization/submodules/transformation/VisSubModuleTransformation.h>
#include "ui/qt/UIFactoryCursorQt.h"
#include "ui/ToolCursor.h"

#ifdef usinglib_cursor
    #include "internal_libraries/Cursor/include/EventHandler3DTransformationToCursor.h"
    #include "internal_libraries/Cursor/include/EventHandler2DTransformationToCursor.h"
#else
    #include "event_handler/EventHandler3DTransformationToCursor.h"
    #include <modules/visualization/submodules/cursor/event_handler/EventHandler2DTransformationToCursor.h>
#endif

#include <modules/visualization/submodules/2d_3d_slicing/VisSubModule2D3DSlicing.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))  /**< maximum among the input values */

template<class vis_module_type>
VisSubModuleCursor<vis_module_type>::VisSubModuleCursor(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_modelCursor = nullptr;
    m_visModule = nullptr;
    m_stateManager = nullptr;
    m_model2Dtransformation = nullptr;
    m_model2DSlice = nullptr;
    m_model3dClipping = nullptr;
    m_sliceSynchronizer = nullptr;
    m_model3Dtransformation = nullptr;

    mController2DCursorToolTab = nullptr;

    m_mouseHandlerCursor3D = nullptr;
    m_mouseHandlerCursorAxial = nullptr;
    m_mouseHandlerCursorCoronal = nullptr;
    m_mouseHandlerCursorSagittal = nullptr;
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::init(VisualizationModule *vis_module)
{
    if(vis_module->getRendererAxial() != nullptr || vis_module->getRendererCoronal() != nullptr || vis_module->getRendererSagittal() != nullptr)
    {
        m_sliceSynchronizer = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"))->getSliceSynchronizer();
        m_model2DSlice = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"))->getModel2DSlice();
        m_model3dClipping = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"))->getModel3DClipping();
        m_model2Dtransformation = static_cast<VisSubModuleTransformation*>(vis_module->getVisSubModule("VisSubModuleTransformation"))->getModel2DTransformation();
        m_model3Dtransformation = static_cast<VisSubModuleTransformation*>(vis_module->getVisSubModule("VisSubModuleTransformation"))->getModel3DTransformation();

        if(m_model2DSlice == nullptr || m_model2Dtransformation == nullptr || m_model3Dtransformation == nullptr)
            return;

        m_visModule = static_cast<vis_module_type*>(vis_module);
        m_stateManager = vis_module->getStateManager();

        m_modelCursor = new ModelCursor();
        m_modelCursor->setModelViewMatrix3D(m_model3Dtransformation->getModelViewMatrix());
        m_modelCursor->setProjectionMatrix3D(m_model3Dtransformation->getProjectionMatrix());

        initModelCursor();
        createControllers();
        createEventHandlers();
    }
}

template<class vis_module_type>
ModelCursor *VisSubModuleCursor<vis_module_type>::getModelCursor() const
{
    return m_modelCursor;
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::initModelCursor()
{
    m_modelCursor->setModel2DSlice(m_model2DSlice);

    m_modelCursor->setModelViewMatrixAxial(m_model2Dtransformation->getAxialData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixAxial(m_model2Dtransformation->getAxialData().getProjectionMatrix());

    m_modelCursor->setModelViewMatrixCoronal(m_model2Dtransformation->getCoronalData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixCoronal(m_model2Dtransformation->getCoronalData().getProjectionMatrix());

    m_modelCursor->setModelViewMatrixSagittal(m_model2Dtransformation->getSagittalData().getModelViewMatrix());
    m_modelCursor->setProjectionMatrixSagittal(m_model2Dtransformation->getSagittalData().getProjectionMatrix());

    float maxPhySlice = MAX(MAX(m_visModule->getAcquisition()->getDimensions(0) * m_visModule->getAcquisition()->getPixelSpacing(0),
                                m_visModule->getAcquisition()->getDimensions(1) * m_visModule->getAcquisition()->getPixelSpacing(1)),
                            m_visModule->getAcquisition()->getDimensions(2) * m_visModule->getAcquisition()->getPixelSpacing(2));

    float phyScale[3];
    float scale_factors[4];

    phyScale[0] = (m_visModule->getAcquisition()->getPixelSpacing(0)) / maxPhySlice;
    phyScale[1] = (m_visModule->getAcquisition()->getPixelSpacing(1)) / maxPhySlice;
    phyScale[2] = (m_visModule->getAcquisition()->getPixelSpacing(2)) / maxPhySlice;

    scale_factors[0] = (m_visModule->getAcquisition()->getDimensions(0) * m_visModule->getAcquisition()->getPixelSpacing(0)) / maxPhySlice;
    scale_factors[1] = (m_visModule->getAcquisition()->getDimensions(1) * m_visModule->getAcquisition()->getPixelSpacing(1)) / maxPhySlice;
    scale_factors[2] = (m_visModule->getAcquisition()->getDimensions(2) * m_visModule->getAcquisition()->getPixelSpacing(2)) / maxPhySlice;
    scale_factors[3] = 1.f;

    m_modelCursor->setPhyScale(phyScale);
    m_modelCursor->setScaleFactors(scale_factors);
    m_modelCursor->setStateManager(m_stateManager);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::createEventHandlers()
{
    if(m_visModule->getRenderer3D() != nullptr)
    {
        m_eventHandler3DTransformationToCursor = new EventHandler3DTransformationToCursor();
        m_eventHandler3DTransformationToCursor->setModel3DTransformation(m_model3Dtransformation);
        m_eventHandler3DTransformationToCursor->setModelCursor(m_modelCursor);
    }

    m_eventHandler2DTransformationToCursor = new EventHandler2DTransformationToCursor();
    m_eventHandler2DTransformationToCursor->setModel2DTransformation(m_model2Dtransformation);
    m_eventHandler2DTransformationToCursor->setModelCursor(m_modelCursor);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::createControllers()
{
    VisSubModuleBaseVolumeRenderer *vis_submodule_volume_rendering;
    vis_submodule_volume_rendering = static_cast<VisSubModuleBaseVolumeRenderer*>(m_visModule->getVisSubModule("VisSubModuleBaseVolumeRenderer"));
    BaseRaycastingVolumeRenderer3D *raycasting_volume_renderer_3d = static_cast<RaycastingVolumeRenderer3D*>(vis_submodule_volume_rendering->getRaycastingVolumeRenderer3D());

    m_mouseHandlerCursor3D = new ControllerMouseHandlerCursor3D();
    m_mouseHandlerCursor3D->setModelCursor(m_modelCursor);

    //Uncomment for associating a canvas to cursor (Raphael - 12/08/23)
    m_mouseHandlerCursor3D->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

    m_mouseHandlerCursor3D->setSliceSynchronizer(m_sliceSynchronizer);
    m_mouseHandlerCursor3D->setRaycastingVolumeRenderer3D(raycasting_volume_renderer_3d);

    if(m_visModule->getRendererAxial() != nullptr)
    {
        m_mouseHandlerCursorAxial = new ControllerMouseHandlerCursor2D(ControllerMouseHandlerCursor2D::View::AXIAL);
        m_mouseHandlerCursorAxial->setModelCursor(m_modelCursor);
        m_mouseHandlerCursorAxial->setSliceSynchronizer(m_sliceSynchronizer);

        //Uncomment for associating a canvas to cursor (Raphael - 12/08/23)
        m_mouseHandlerCursorAxial->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    }

    if(m_visModule->getRendererCoronal() != nullptr)
    {
        m_mouseHandlerCursorCoronal = new ControllerMouseHandlerCursor2D(ControllerMouseHandlerCursor2D::View::CORONAL);
        m_mouseHandlerCursorCoronal->setModelCursor(m_modelCursor);
        m_mouseHandlerCursorCoronal->setSliceSynchronizer(m_sliceSynchronizer);

        //Uncomment for associating a canvas to cursor (Raphael - 12/08/23)
        m_mouseHandlerCursorCoronal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    }

    if(m_visModule->getRendererSagittal() != nullptr)
    {
        m_mouseHandlerCursorSagittal = new ControllerMouseHandlerCursor2D(ControllerMouseHandlerCursor2D::View::SAGITTAL);
        m_mouseHandlerCursorSagittal->setModelCursor(m_modelCursor);
        m_mouseHandlerCursorSagittal->setSliceSynchronizer(m_sliceSynchronizer);

        //Uncomment for associating a canvas to cursor (Raphael - 12/08/23)
        m_mouseHandlerCursorSagittal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    }

    mController2DCursorToolTab = new Controller2DCursorToolTab();
    mController2DCursorToolTab->setModelCursor(m_modelCursor);

    m_visModule->getMainWindow()->addUIFactory(UIFactoryCursorQt::uPtr(new UIFactoryCursorQt()));
    m_visModule->getMainWindow()->createTool(mController2DCursorToolTab->getToolClassName(), "Exploration", 1, 0);

    ToolCursor *tool_cursor = dynamic_cast<ToolCursor*>(m_visModule->getMainWindow()->getTool(mController2DCursorToolTab->getToolClassName(), "Exploration"));
    if(tool_cursor != nullptr)
        tool_cursor->setController(m_visModule->getVisualizationTab(), mController2DCursorToolTab);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::configure3DView()
{
    VisSubModuleBaseVolumeRenderer *vis_submodule_volume_rendering;
    vis_submodule_volume_rendering = static_cast<VisSubModuleBaseVolumeRenderer*>(m_visModule->getVisSubModule("VisSubModuleBaseVolumeRenderer"));
    BaseRaycastingVolumeRenderer3D *raycasting_volume_renderer_3d = static_cast<RaycastingVolumeRenderer3D*>(vis_submodule_volume_rendering->getRaycastingVolumeRenderer3D());

    m_cursorRenderer3D = new CursorRenderer3D();
    m_cursorRenderer3D->setRaycastingVolumeRenderer3D(raycasting_volume_renderer_3d);
    m_cursorRenderer3D->setModelViewMatrix(m_model3Dtransformation->getModelViewMatrix());
    m_cursorRenderer3D->setProjectionMatrix(m_model3Dtransformation->getProjectionMatrix());
    m_cursorRenderer3D->setCursorPosition3D(m_modelCursor->getCursorPosition3D());

    m_eventHandler3DTransformationToCursor->setCursorRenderer3D(m_cursorRenderer3D);

    m_mouseHandlerCursor3D->setCursorRenderer(m_cursorRenderer3D);

    m_visModule->getRenderer3D()->addSubRenderer(m_cursorRenderer3D);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::configureAxialView()
{
    m_rendererCursorAxial = new CursorRenderer2D();
    m_rendererCursorAxial->setModelViewMatrix(m_model2Dtransformation->getAxialData().getModelViewMatrix());
    m_rendererCursorAxial->setProjectionMatrix(m_model2Dtransformation->getAxialData().getProjectionMatrix());
    m_rendererCursorAxial->setEnabled(m_modelCursor->isActivated());

    m_eventHandler2DTransformationToCursor->setRendererCursorAxial(m_rendererCursorAxial);

    m_mouseHandlerCursorAxial->setRenderer(m_rendererCursorAxial);

    m_visModule->getRendererAxial()->addSubRenderer(m_rendererCursorAxial);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::configureCoronalView()
{
    m_rendererCursorCoronal = new CursorRenderer2D();
    m_rendererCursorCoronal->setModelViewMatrix(m_model2Dtransformation->getCoronalData().getModelViewMatrix());
    m_rendererCursorCoronal->setProjectionMatrix(m_model2Dtransformation->getCoronalData().getProjectionMatrix());
    m_rendererCursorCoronal->setEnabled(m_modelCursor->isActivated());

    m_eventHandler2DTransformationToCursor->setRendererCursorCoronal(m_rendererCursorCoronal);

    m_mouseHandlerCursorCoronal->setRenderer(m_rendererCursorCoronal);

    m_visModule->getRendererCoronal()->addSubRenderer(m_rendererCursorCoronal);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::configureSagittalView()
{
    m_rendererCursorSagittal= new CursorRenderer2D();
    m_rendererCursorSagittal->setModelViewMatrix(m_model2Dtransformation->getSagittalData().getModelViewMatrix());
    m_rendererCursorSagittal->setProjectionMatrix(m_model2Dtransformation->getSagittalData().getProjectionMatrix());
    m_rendererCursorSagittal->setEnabled(m_modelCursor->isActivated());

    m_eventHandler2DTransformationToCursor->setRendererCursorSagittal(m_rendererCursorSagittal);

    m_mouseHandlerCursorSagittal->setRenderer(m_rendererCursorSagittal);

    m_visModule->getRendererSagittal()->addSubRenderer(m_rendererCursorSagittal);
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::on3DRenderingSurfaceCreated()
{
    m_cursorRenderer3D->setEnableCursor(m_modelCursor->isActivated());
    m_cursorRenderer3D->setDrawCursorType(m_modelCursor->getDrawCursorType());
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::onAxialRenderingSurfaceCreated()
{
    m_rendererCursorAxial->setEnabled(m_modelCursor->isActivated());
    m_rendererCursorAxial->setDrawCursorType(m_modelCursor->getDrawCursorType());
}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::onCoronalRenderingSurfaceCreated()
{
    m_rendererCursorCoronal->setEnabled(m_modelCursor->isActivated());
    m_rendererCursorCoronal->setDrawCursorType(m_modelCursor->getDrawCursorType());

}

template<class vis_module_type>
void VisSubModuleCursor<vis_module_type>::onSagittalRenderingSurfaceCreated()
{
    m_rendererCursorSagittal->setEnabled(m_modelCursor->isActivated());
    m_rendererCursorSagittal->setDrawCursorType(m_modelCursor->getDrawCursorType());

}
