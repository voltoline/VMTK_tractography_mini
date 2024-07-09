#include "VisualizationModule.h"

VisualizationSubModule::VisualizationSubModule(std::string vis_sub_module_name):
    m_visSubModuleName(vis_sub_module_name)
{

}

std::string VisualizationSubModule::getVisSubModuleName() const
{
    return m_visSubModuleName;
}

//============================================================================================================================================================================

VisualizationModule::VisualizationModule(std::string module_name):
    BaseModule(module_name)
{
    m_mainWindow = nullptr;

    m_createRenderer3D = true;
    m_createRendererAxial = true;
    m_createRendererCoronal = true;
    m_createRendererSagittal = true;

    m_renderer3D = nullptr;
    m_rendererAxial = nullptr;
    m_rendererCoronal = nullptr;
    m_rendererSagittal = nullptr;

    m_renderingSurface3D = nullptr;
    m_renderingSurfaceAxial = nullptr;
    m_renderingSurfaceCoronal = nullptr;
    m_renderingSurfaceSagittal = nullptr;

    m_stateManager = nullptr;

    m_visualizationTab = nullptr;
    m_renderingSurfaceSet = nullptr;

    m_numInitializedRenderingSurface = 0;

    createRenderingSurfaceSet();
}

VisualizationModule::~VisualizationModule()
{
    std::cout << "VisualizationModule delete" << std::endl;

    for(unsigned short i = 0; i < m_subModuleVector.size(); i++)
        delete m_subModuleVector[i];

    if(m_renderingSurfaceSet != nullptr)
        delete m_renderingSurfaceSet;

    if(m_stateManager != nullptr)
        delete m_stateManager;
}

void VisualizationModule::init(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;

    if(m_createRenderer3D && m_renderer3D == nullptr)
        m_renderer3D = createRenderer3D();

    if(m_createRendererAxial && m_rendererAxial == nullptr)
        m_rendererAxial = createRenderer2D();

    if(m_createRendererCoronal && m_rendererCoronal == nullptr)
        m_rendererCoronal = createRenderer2D();

    if(m_createRendererSagittal && m_rendererSagittal == nullptr)
        m_rendererSagittal = createRenderer2D();

    loadSubModules();

    if(m_createRenderer3D && m_renderingSurface3D == nullptr)
    {
        std::function<void(RenderingSurface*)> on_initialization = [this](RenderingSurface* surface)
        {
            m_renderingSurfaceSet->setRenderingSurface3D(surface);
            for(auto vis_sub_module: m_subModuleVector)
            {
                surface->setRenderingContext();
                vis_sub_module->on3DRenderingSurfaceCreated();
            }
            this->addNumInitializedRenderingSurface();
        };
        m_renderingSurface3D = m_visualizationTab->createRenderingSurface(m_renderer3D, on_initialization,"3D view", false, "");
    }

    if(m_createRendererAxial && m_renderingSurfaceAxial == nullptr)
    {
        std::function<void(RenderingSurface*)> on_initialization = [this](RenderingSurface* surface)
        {
            m_renderingSurfaceSet->setRenderingSurfaceAxial(surface);
            for(auto vis_sub_module: m_subModuleVector)
            {
                surface->setRenderingContext();
                vis_sub_module->onAxialRenderingSurfaceCreated();
            }
            this->addNumInitializedRenderingSurface();
        };
        m_renderingSurfaceAxial = m_visualizationTab->createRenderingSurface(m_rendererAxial, on_initialization, "Axial view", false, "");
    }

    if(m_createRendererCoronal && m_renderingSurfaceCoronal == nullptr)
    {
        std::function<void(RenderingSurface*)> on_initialization = [this](RenderingSurface* surface)
        {
            m_renderingSurfaceSet->setRenderingSurfaceCoronal(surface);
            for(auto vis_sub_module: m_subModuleVector)
            {
                surface->setRenderingContext();
                vis_sub_module->onCoronalRenderingSurfaceCreated();
            }
            this->addNumInitializedRenderingSurface();
        };
        m_renderingSurfaceCoronal = m_visualizationTab->createRenderingSurface(m_rendererCoronal, on_initialization, "Coronal view", false, "");
    }

    if(m_createRendererSagittal && m_renderingSurfaceSagittal == nullptr)
    {
        std::function<void(RenderingSurface*)> on_initialization = [this](RenderingSurface* surface)
        {
            m_renderingSurfaceSet->setRenderingSurfaceSagittal(surface);
            for(auto vis_sub_module: m_subModuleVector)
            {
                surface->setRenderingContext();
                vis_sub_module->onSagittalRenderingSurfaceCreated();
            }
            this->addNumInitializedRenderingSurface();
        };
        m_renderingSurfaceSagittal = m_visualizationTab->createRenderingSurface(m_rendererSagittal, on_initialization, "Sagittal view", false, "");
    }
}

void VisualizationModule::addNumInitializedRenderingSurface()
{
    unsigned short num_init = 0;

    if(m_createRenderer3D)
        num_init++;

    if(m_createRendererAxial)
        num_init++;

    if(m_createRendererCoronal)
        num_init++;

    if(m_createRendererSagittal)
        num_init++;

    m_numInitializedRenderingSurface++;

    if(m_numInitializedRenderingSurface == num_init)
    {
        std::cout << "call onAllRenderingSurfacesCreated " << m_numInitializedRenderingSurface << " " << num_init << std::endl;
        for(auto vis_sub_module: m_subModuleVector)
            vis_sub_module->onAllRenderingSurfacesCreated();
    }
}

StateManager *VisualizationModule::getStateManager()
{
    createStateManager();
    return m_stateManager;
}

void VisualizationModule::createStateManager()
{
    if(m_stateManager == nullptr)
        m_stateManager = new StateManager();
}

void VisualizationModule::createVisualizationTab(std::string vis_tab_title, bool enable_checkbox)
{
    if(m_visualizationTab == nullptr)
        m_visualizationTab = m_mainWindow->createVisualizationTab(vis_tab_title, this, "OpenGLVisualizationTab", enable_checkbox);
}

void VisualizationModule::createRenderingSurfaceSet()
{
    if(m_renderingSurfaceSet == nullptr)
        m_renderingSurfaceSet = new RenderingSurfaceSet();
}

VisualizationTab *VisualizationModule::getVisualizationTab() const
{
    return m_visualizationTab;
}

void VisualizationModule::loadSession(std::string folder_path)
{
    for(auto vis_sub_module: m_subModuleVector)
        vis_sub_module->loadWorkSesseion(folder_path);
}

void VisualizationModule::saveSession(std::string folder_path)
{
    for(auto vis_sub_module: m_subModuleVector)
        vis_sub_module->saveWorkSesseion(folder_path);
}

VisualizationSubModule *VisualizationModule::getVisSubModule(std::string vis_sub_module_name)
{
    for(auto vis_sub_module: m_subModuleVector)
        if(vis_sub_module->getVisSubModuleName().compare(vis_sub_module_name) == 0)
            return vis_sub_module;

    return nullptr;
}

void VisualizationModule::addVisSubModule(VisualizationSubModule *vis_sub_module)
{
    m_subModuleVector.push_back(vis_sub_module);

        vis_sub_module->init(this);

        if(m_createRenderer3D && m_renderer3D)
            vis_sub_module->configure3DView();
        
        if(m_createRendererAxial && m_rendererAxial)
            vis_sub_module->configureAxialView();
        
        if(m_createRendererCoronal && m_rendererCoronal)
            vis_sub_module->configureCoronalView();
        
        if(m_createRendererSagittal && m_rendererSagittal)
            vis_sub_module->configureSagittalView();
}

MainWindow *VisualizationModule::getMainWindow() const
{
    return m_mainWindow;
}

vmtkAbstractRender *VisualizationModule::getRendererSagittal()
{
    return m_rendererSagittal;
}

vmtkAbstractRender *VisualizationModule::getRendererCoronal()
{
    return m_rendererCoronal;
}

vmtkAbstractRender *VisualizationModule::getRendererAxial()
{
    return m_rendererAxial;
}

vmtkAbstractRender *VisualizationModule::getRenderer3D()
{
    return m_renderer3D;
}

RenderingSurfaceSet *VisualizationModule::getRenderingSurfaceSet()
{
    if(m_renderingSurfaceSet == nullptr)
        createRenderingSurfaceSet();

    return m_renderingSurfaceSet;
}
