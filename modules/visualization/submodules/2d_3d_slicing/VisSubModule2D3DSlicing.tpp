#include "VisSubModule2D3DSlicing.h"

#include <modules/visualization/submodules/volume_renderer/VisSubModuleBaseVolumeRenderer.h>
#include "controllers/Controller2D3DSlicingTool.h"
#include "controllers/ControllerMouseHandler2D3DSlicing.h"
#include "../3d_clipping/event_handlers/EventHandler3DClipping.h"
#include "ui/qt/UIFactory2D3DSlicingQt.h"
#include "ui/Tool2D3DSlicing.h"

#include "../2d_slice/ui/qt/UIFactory2DSliceQt.h"
#include "../2d_slice/ui/ToolSliceOrientation.h"
#include "../2d_slice/CommonObjects2DSlice.h"

template<class vis_module_type>
VisSubModule2D3DSlicing<vis_module_type>::VisSubModule2D3DSlicing(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_visModule = nullptr;
    m_model3DRendering = nullptr;
    m_model3DClipping = nullptr;
    m_eventHandler3DClipping = nullptr;
    m_controllerToolTab2D3DSlicing = nullptr;

    m_model2DSlice = nullptr;
    m_model2DSliceUIState = nullptr;
    m_sliceSynchronizer = nullptr;
    m_eventHandler = nullptr;

    m_mouseHandlerCursorAxial = nullptr;
    m_mouseHandlerCursorCoronal = nullptr;
    m_mouseHandlerCursorSagittal = nullptr;

    m_states2DSliceTo3DClippingSync = nullptr;
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::init(VisualizationModule *vis_module)
{
    if(vis_module->getRenderer3D() != nullptr)
    {
        m_visModule = static_cast<vis_module_type*>(vis_module);
        VisSubModuleBaseVolumeRenderer *submodule = static_cast<VisSubModuleBaseVolumeRenderer*>(vis_module->getVisSubModule("VisSubModuleBaseVolumeRenderer"));

        if(submodule)
        {
            m_model3DRendering = submodule->getModel3DRendering();
            m_model2DRendering = static_cast<VisSubModuleBaseVolumeRenderer*>(m_visModule->getVisSubModule("VisSubModuleBaseVolumeRenderer"))->getModel2DRendering();

        } else
            std::cout << "VisSubModule2D3DSlicing::init --> VisSubModuleBaseVolumeRenderer not found!" << std::endl;

        {
            createModel();
            setupSliceSynchronizer();

            m_commonObjects = new CommonObjects2DSlice(m_model3DClipping, m_model2DSlice, m_model2DSliceUIState, m_sliceSynchronizer,
                                                       m_visModule->getRenderingSurfaceSet(), m_model2DRendering, m_model3DRendering);

            createControllerToolTab();
            createEventHandler();
        }
    }
}

template<class vis_module_type>
Model2DSlice *VisSubModule2D3DSlicing<vis_module_type>::getModel2DSlice() const
{
    return m_model2DSlice;
}

template<class vis_module_type>
SliceSynchronizer *VisSubModule2D3DSlicing<vis_module_type>::getSliceSynchronizer() const
{
    return m_sliceSynchronizer;
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::createModel()
{
    m_model3DClipping = new Model3DClipping();
    m_model3DClipping->setStateManager(m_visModule->getStateManager());
    m_model3DClipping->setActivated(false);
    m_model3DClipping->setLeftRightInterval(1, m_visModule->getAcquisition()->getDimensions(0));
    m_model3DClipping->setBottomTopInterval(1, m_visModule->getAcquisition()->getDimensions(1));
    m_model3DClipping->setBackFrontInterval(1, m_visModule->getAcquisition()->getDimensions(2));
    m_model3DClipping->resetClipping();
    m_model3DClipping->notify(); // clean events

    m_model2DSlice = new Model2DSlice();
    m_model2DSlice->setAxialInterval(1, m_visModule->getAcquisition()->getDimensions(2));
    m_model2DSlice->setCoronalInterval(1, m_visModule->getAcquisition()->getDimensions(1));
    m_model2DSlice->setSagittalInterval(1, m_visModule->getAcquisition()->getDimensions(0));
    m_model2DSlice->resetSlices();

    m_model2DSliceUIState = new Model2DSliceUIState();
    m_model2DSliceUIState->setStateManager(m_visModule->getStateManager());
    m_model2DSliceUIState->setActivated(true);
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::createEventHandler()
{
    m_eventHandler3DClipping = new EventHandler3DClipping();
    m_eventHandler3DClipping->setModel3DClipping(m_model3DClipping);
    m_eventHandler3DClipping->setModel3DRendering(m_model3DRendering);


    m_eventHandler = new EventHandler2DSlice(m_commonObjects);
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::setupSliceSynchronizer()
{
    {
        m_sliceSynchronizer = new SliceSynchronizer();
        m_sliceSynchronizer->setModel2DSlice(m_model2DSlice);
        m_sliceSynchronizer->setModel3DClipping(m_model3DClipping);
        m_sliceSynchronizer->setStateManager(m_visModule->getStateManager());
    }
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::createControllerToolTab()
{
    m_controllerToolTab2D3DSlicing = new Controller2D3DSlicingTool();
    m_controllerToolTab2D3DSlicing->setModel3DClipping(m_model3DClipping);
    m_controllerToolTab2D3DSlicing->setModel2DSlice(m_model2DSlice);
    m_controllerToolTab2D3DSlicing->setModel2DSliceUIState(m_model2DSliceUIState);
    m_controllerToolTab2D3DSlicing->setSliceSynchronizer(m_sliceSynchronizer);
    m_controllerToolTab2D3DSlicing->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    m_controllerToolTab2D3DSlicing->setModel3DRendering(m_model3DRendering);

    m_controllerToolSliceOrientation = new ControllerToolSliceOrientation(m_commonObjects);

    m_visModule->getMainWindow()->addUIFactory(UIFactory2D3DSlicingQt::uPtr(new UIFactory2D3DSlicingQt()));
    m_visModule->getMainWindow()->createTool(m_controllerToolTab2D3DSlicing->getToolClassName(), "Exploration", 0, 3, 2);

    Tool2D3DSlicing *tool_3d = dynamic_cast<Tool2D3DSlicing*>(m_visModule->getMainWindow()->getTool(m_controllerToolTab2D3DSlicing->getToolClassName(), "Exploration"));
    if(tool_3d != nullptr)
        tool_3d->setController(m_visModule->getVisualizationTab(), m_controllerToolTab2D3DSlicing);

    m_visModule->getMainWindow()->addUIFactory(UIFactory2DSliceQt::uPtr(new UIFactory2DSliceQt()));
    m_visModule->getMainWindow()->createTool(m_controllerToolSliceOrientation->getToolClassName(), "Exploration", 0, 0);

    ToolSliceOrientation *tool_orientation = dynamic_cast<ToolSliceOrientation*>(m_visModule->getMainWindow()->getTool(m_controllerToolSliceOrientation->getToolClassName(), "Exploration"));
    if(tool_orientation != nullptr)
        tool_orientation->setController(m_visModule->getVisualizationTab(), m_controllerToolSliceOrientation);

    {
        if(m_visModule->getRendererAxial() != nullptr)
        {
            m_mouseHandlerCursorAxial = new ControllerMouseHandler2D3DSlicing(ControllerMouseHandler2D3DSlicing::View::AXIAL);
            m_mouseHandlerCursorAxial->setModel2DSlice(m_model2DSlice);
            m_mouseHandlerCursorAxial->setModel3dClipping(m_model3DClipping);
            m_mouseHandlerCursorAxial->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
        }

        if(m_visModule->getRendererCoronal() != nullptr)
        {
            m_mouseHandlerCursorCoronal = new ControllerMouseHandler2D3DSlicing(ControllerMouseHandler2D3DSlicing::View::CORONAL);
            m_mouseHandlerCursorCoronal->setModel2DSlice(m_model2DSlice);
            m_mouseHandlerCursorCoronal->setModel3dClipping(m_model3DClipping);
            m_mouseHandlerCursorCoronal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
        }

        if(m_visModule->getRendererSagittal() != nullptr)
        {
            m_mouseHandlerCursorSagittal = new ControllerMouseHandler2D3DSlicing(ControllerMouseHandler2D3DSlicing::View::SAGITTAL);
            m_mouseHandlerCursorSagittal->setModel2DSlice(m_model2DSlice);
            m_mouseHandlerCursorSagittal->setModel3dClipping(m_model3DClipping);
            m_mouseHandlerCursorSagittal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
        }
    }
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::on3DRenderingSurfaceCreated()
{
    if(m_model3DClipping)
    {
        m_model3DRendering->setHighlightedPlaneAxial(m_model2DSlice->getAxialSlice());
        m_model3DRendering->setHighlightedPlaneSagittal(m_model2DSlice->getSagittalSlice());
        Model3DRenderingEvent event = m_model3DRendering->setHighlightedPlaneCoronal(m_model2DSlice->getCoronalSlice());
        m_model3DRendering->notify(event);

        unsigned short left = m_model3DClipping->getLeftClipping(Model3DClipping::Orientation::LPS);
        unsigned short right = m_model3DClipping->getRightClipping(Model3DClipping::Orientation::LPS);
        unsigned short bottom = m_model3DClipping->getBottomClipping(Model3DClipping::Orientation::LPS);
        unsigned short top = m_model3DClipping->getTopClipping(Model3DClipping::Orientation::LPS);
        unsigned short back = m_model3DClipping->getBackClipping();
        unsigned short front = m_model3DClipping->getFrontClipping();

        event = m_model3DRendering->setProxyGeometryDimFromClippingValue(left, right, bottom, top, back, front);
        m_model3DRendering->notify(event);
    }
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::onAxialRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    m_model2DSlice->getAxialTexCoord(v1, v2, v3, v4);

    Model2DRenderingEvent event = m_model2DRendering->getAxialData().setPlaneTexCoord(v1, v2, v3, v4);
    m_model2DRendering->notify(event);
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::onCoronalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    m_model2DSlice->getCoronalTexCoord(v1, v2, v3, v4);

    Model2DRenderingEvent event = m_model2DRendering->getCoronalData().setPlaneTexCoord(v1, v2, v3, v4);
    m_model2DRendering->notify(event);
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::onSagittalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    m_model2DSlice->getSagittalTexCoord(v1, v2, v3, v4);

    Model2DRenderingEvent event = m_model2DRendering->getSagittalData().setPlaneTexCoord(v1, v2, v3, v4);
    m_model2DRendering->notify(event);
}

template<class vis_module_type>
void VisSubModule2D3DSlicing<vis_module_type>::onAllRenderingSurfacesCreated()
{
    {
        if(m_visModule->getStateManager()->addDefaultFeature("2DSLICE_TO_3DCLIPPING_SYNC"))
        {
            std::function<void(bool, bool)> callback = [this](bool , bool is_activated)
            {
                if(m_mouseHandlerCursorAxial)
                    m_mouseHandlerCursorAxial->setSyncActivated(is_activated);

                if(m_mouseHandlerCursorCoronal)
                    m_mouseHandlerCursorCoronal->setSyncActivated(is_activated);

                if(m_mouseHandlerCursorSagittal)
                    m_mouseHandlerCursorSagittal->setSyncActivated(is_activated);
            };

            m_states2DSliceTo3DClippingSync = m_visModule->getStateManager()->getStatesOf("2DSLICE_TO_3DCLIPPING_SYNC");
            m_states2DSliceTo3DClippingSync->setStateChangedCallback(callback);

            m_visModule->getStateManager()->activateFeature("2DSLICE_TO_3DCLIPPING_SYNC");
            callback(m_states2DSliceTo3DClippingSync->isEnabled(), m_states2DSliceTo3DClippingSync->isActivated());
        }
    }
}

template<class vis_module_type>
Model3DClipping *VisSubModule2D3DSlicing<vis_module_type>::getModel3DClipping() const
{
    return m_model3DClipping;
}
