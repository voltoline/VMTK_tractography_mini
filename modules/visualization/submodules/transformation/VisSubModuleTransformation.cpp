#include "VisSubModuleTransformation.h"

#include <modules/visualization/submodules/volume_renderer/VisSubModuleBaseVolumeRenderer.h>
#include "../2d_transformation/controller/ControllerMouseHandler2DTransformations.h"
#include "../2d_transformation/event_handlers/EventHandler2DTransformations.h"

#include "../3d_transformation/controllers/ControllerMouseHandler3DTransformations.h"
#include "../3d_transformation/event_handlers/EventHandler3DTransformations.h"

#include "ui/qt/UIFactoryTransformationQt.h"
#include "ui/ToolTransformation.h"
#include "controllers/ControllerToolTransformation.h"

VisSubModuleTransformation::VisSubModuleTransformation(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_visModule = nullptr;
    m_model2DRendering = nullptr;
    m_model2DTransformation = nullptr;
    m_model2DTransformationUIState = nullptr;

    m_controllerMouseHandler2DTransformationsAxial = nullptr;
    m_controllerMouseHandler2DTransformationsCoronal = nullptr;
    m_controllerMouseHandler2DTransformationsSagittal = nullptr;

    m_model3dRendering = nullptr;
    m_model3DTransformation = nullptr;
    m_controllerMouseHandler3DTransformations = nullptr;
    m_eventHandler3DTransformations = nullptr;

    m_controllerToolTransformation = nullptr;
}

void VisSubModuleTransformation::init(VisualizationModule *vis_module)
{
    m_visModule = vis_module;

    VisSubModuleBaseVolumeRenderer *submodule = static_cast<VisSubModuleBaseVolumeRenderer*>(vis_module->getVisSubModule("VisSubModuleBaseVolumeRenderer"));

    if(submodule)
    {
        m_model2DRendering = submodule->getModel2DRendering();
        m_model3dRendering = submodule->getModel3DRendering();

    } else
        std::cout << "VisSubModuleTransformation::init --> VisSubModuleBaseVolumeRenderer not found!" << std::endl;

    if(m_model2DRendering && m_model3dRendering)
    {
        createModel();
        createControllerToolTab();
        createEventHandler();
    }
}

Model2DTransformation *VisSubModuleTransformation::getModel2DTransformation() const
{
    return m_model2DTransformation;
}

Model2DTransformationUIState *VisSubModuleTransformation::getModel2DTransformationUIState() const
{
    return m_model2DTransformationUIState;
}

Model3DTransformation *VisSubModuleTransformation::getModel3DTransformation() const
{
    return m_model3DTransformation;
}

void VisSubModuleTransformation::createModel()
{
    m_model2DTransformation = new Model2DTransformation();

    m_model2DTransformationUIState = new Model2DTransformationUIState();
    m_model2DTransformationUIState->setStateManager(m_visModule->getStateManager());

    m_model3DTransformation = new Model3DTransformation();
    m_model3DTransformation->setStateManager(m_visModule->getStateManager());
}

void VisSubModuleTransformation::createControllerToolTab()
{
    m_controllerToolTransformation = new ControllerToolTransformation();
    m_controllerToolTransformation->setModel2DTransformation(m_model2DTransformation);
    m_controllerToolTransformation->setModel2DTransformationUIState(m_model2DTransformationUIState);
    m_controllerToolTransformation->setModel3DTransformation(m_model3DTransformation);

    m_visModule->getMainWindow()->addUIFactory(UIFactoryTransformationQt::uPtr(new UIFactoryTransformationQt()));
    m_visModule->getMainWindow()->createTool(m_controllerToolTransformation->getToolClassName(), "Exploration", 0, 2, 2);

    ToolTransformation *tool_transformation = dynamic_cast<ToolTransformation*>(m_visModule->getMainWindow()->getTool(m_controllerToolTransformation->getToolClassName(), "Exploration"));
    if(tool_transformation != nullptr)
        tool_transformation->setController(m_visModule->getVisualizationTab(), m_controllerToolTransformation);
}

void VisSubModuleTransformation::createEventHandler()
{
    m_eventHandler2DTransformations = new EventHandler2DTransformations();
    m_eventHandler2DTransformations->setModel2DTransformation(m_model2DTransformation);
    m_eventHandler2DTransformations->setModel2DTransformationUIState(m_model2DTransformationUIState);
    m_eventHandler2DTransformations->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    m_eventHandler2DTransformations->setModel2dRendering(m_model2DRendering);

    m_eventHandler3DTransformations = new EventHandler3DTransformations();
    m_eventHandler3DTransformations->setModel3DTransformation(m_model3DTransformation);
    m_eventHandler3DTransformations->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());
    m_eventHandler3DTransformations->setModel3dRendering(m_model3dRendering);
}

void VisSubModuleTransformation::configure3DView()
{
    if(m_model3dRendering)
    {
        createControllerMouseHandler3D();

        m_eventHandler3DTransformations->setControllerMouseHandler3DTransformations(m_controllerMouseHandler3DTransformations);
    }
}

void VisSubModuleTransformation::configureAxialView()
{
    if(m_model2DRendering)
    {
        createControllerMouseHandlerAxial();

        m_eventHandler2DTransformations->setControllerMouseHandler2DTransformationsAxial(m_controllerMouseHandler2DTransformationsAxial);
    }
}

void VisSubModuleTransformation::configureCoronalView()
{
    if(m_model2DRendering)
    {
        createControllerMouseHandlerCoronal();

        m_eventHandler2DTransformations->setControllerMouseHandler2DTransformationsCoronal(m_controllerMouseHandler2DTransformationsCoronal);
    }
}

void VisSubModuleTransformation::configureSagittalView()
{
    if(m_model2DRendering)
    {        
        createControllerMouseHandlerSagittal();

        m_eventHandler2DTransformations->setControllerMouseHandler2DTransformationsSagittal(m_controllerMouseHandler2DTransformationsSagittal);
    }
}

void VisSubModuleTransformation::on3DRenderingSurfaceCreated()
{
    if(m_model3dRendering)
    {
        Model3DRenderingEvent event_2 = m_model3dRendering->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix());
        event_2 = m_model3dRendering->setProjectionMatrix(m_model3DTransformation->getProjectionMatrix());
        m_model3dRendering->notify(event_2);

        if(m_model3DTransformation->isTransformationActivated() && m_controllerMouseHandler3DTransformations != nullptr)
            m_visModule->getRenderingSurfaceSet()->getRenderingSurface3D()->addController(m_controllerMouseHandler3DTransformations);
    }
}

void VisSubModuleTransformation::onAxialRenderingSurfaceCreated()
{
    m_model2DRendering->getAxialData().setModelViewMatrix(m_model2DTransformation->getAxialData().getModelViewMatrix());
    Model2DRenderingEvent event = m_model2DRendering->getAxialData().setProjectionMatrix(m_model2DTransformation->getAxialData().getProjectionMatrix());
    m_model2DRendering->notify(event);

    if(m_model2DTransformationUIState->isTransformationActivated() && m_controllerMouseHandler2DTransformationsAxial != nullptr)
        m_visModule->getRenderingSurfaceSet()->getRenderingSurfaceAxial()->addController(m_controllerMouseHandler2DTransformationsAxial);
}

void VisSubModuleTransformation::onCoronalRenderingSurfaceCreated()
{
    m_model2DRendering->getCoronalData().setModelViewMatrix(m_model2DTransformation->getCoronalData().getModelViewMatrix());
    Model2DRenderingEvent event = m_model2DRendering->getCoronalData().setProjectionMatrix(m_model2DTransformation->getCoronalData().getProjectionMatrix());
    m_model2DRendering->notify(event);

    if(m_model2DTransformationUIState->isTransformationActivated() && m_controllerMouseHandler2DTransformationsCoronal != nullptr)
        m_visModule->getRenderingSurfaceSet()->getRenderingSurfaceCoronal()->addController(m_controllerMouseHandler2DTransformationsCoronal);
}

void VisSubModuleTransformation::onSagittalRenderingSurfaceCreated()
{
    m_model2DRendering->getSagittalData().setModelViewMatrix(m_model2DTransformation->getSagittalData().getModelViewMatrix());
    Model2DRenderingEvent event = m_model2DRendering->getSagittalData().setProjectionMatrix(m_model2DTransformation->getSagittalData().getProjectionMatrix());
    m_model2DRendering->notify(event);

    if(m_model2DTransformationUIState->isTransformationActivated() && m_controllerMouseHandler2DTransformationsSagittal != nullptr)
        m_visModule->getRenderingSurfaceSet()->getRenderingSurfaceSagittal()->addController(m_controllerMouseHandler2DTransformationsSagittal);
}

void VisSubModuleTransformation::createControllerMouseHandler3D()
{
    if(m_controllerMouseHandler3DTransformations == nullptr)
    {
        m_controllerMouseHandler3DTransformations = new ControllerMouseHandler3DTransformations();
        m_controllerMouseHandler3DTransformations->setModel3DTransformation(m_model3DTransformation);
        m_controllerMouseHandler3DTransformations->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

        if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::MOVE)
            m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::TRANSLATION);

        else if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ROTATION)
            m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::ROTATION);

        else if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ZOOM)
            m_controllerMouseHandler3DTransformations->setMouseMode(ControllerMouseHandler3DTransformations::MouseMode::SCALING);
    }
}

void VisSubModuleTransformation::createControllerMouseHandlerAxial()
{
    if(m_controllerMouseHandler2DTransformationsAxial == nullptr)
    {
        m_controllerMouseHandler2DTransformationsAxial = new ControllerMouseHandler2DTransformations(ControllerMouseHandler2DTransformations::View::AXIAL);
        m_controllerMouseHandler2DTransformationsAxial->setModel2DTransformation(m_model2DTransformation);
        m_controllerMouseHandler2DTransformationsAxial->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

        if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::MOVE)
            m_controllerMouseHandler2DTransformationsAxial->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::TRANSLATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ROTATION)
            m_controllerMouseHandler2DTransformationsAxial->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::ROTATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ZOOM)
            m_controllerMouseHandler2DTransformationsAxial->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::SCALING);
    }
}

void VisSubModuleTransformation::createControllerMouseHandlerCoronal()
{
    if(m_controllerMouseHandler2DTransformationsCoronal == nullptr)
    {
        m_controllerMouseHandler2DTransformationsCoronal = new ControllerMouseHandler2DTransformations(ControllerMouseHandler2DTransformations::View::CORONAL);
        m_controllerMouseHandler2DTransformationsCoronal->setModel2DTransformation(m_model2DTransformation);
        m_controllerMouseHandler2DTransformationsCoronal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

        if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::MOVE)
            m_controllerMouseHandler2DTransformationsCoronal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::TRANSLATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ROTATION)
            m_controllerMouseHandler2DTransformationsCoronal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::ROTATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ZOOM)
            m_controllerMouseHandler2DTransformationsCoronal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::SCALING);
    }
}

void VisSubModuleTransformation::createControllerMouseHandlerSagittal()
{
    if(m_controllerMouseHandler2DTransformationsSagittal == nullptr)
    {
        m_controllerMouseHandler2DTransformationsSagittal = new ControllerMouseHandler2DTransformations(ControllerMouseHandler2DTransformations::View::SAGITTAL);
        m_controllerMouseHandler2DTransformationsSagittal->setModel2DTransformation(m_model2DTransformation);
        m_controllerMouseHandler2DTransformationsSagittal->setRenderingSurfaceSet(m_visModule->getRenderingSurfaceSet());

        if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::MOVE)
            m_controllerMouseHandler2DTransformationsSagittal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::TRANSLATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ROTATION)
            m_controllerMouseHandler2DTransformationsSagittal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::ROTATION);

        else if(m_model2DTransformation->getMouseMode() == Model2DTransformation::MouseMode::ZOOM)
            m_controllerMouseHandler2DTransformationsSagittal->setMouseMode(ControllerMouseHandler2DTransformations::MouseMode::SCALING);
    }
}
