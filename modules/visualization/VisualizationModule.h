#ifndef VISUALIZATION_MODULE_H
#define VISUALIZATION_MODULE_H

#include "core/BaseModule.h"
#include "ui/components/RenderingSurfaceSet.h"
#include "StateManager.h"
#include <core/MainWindow.h>

class VisualizationModule;
class RenderingSurface;

class VisualizationSubModule
{
public:
    VisualizationSubModule(std::string vis_sub_module_name);
    virtual ~VisualizationSubModule() { }

    virtual void init(VisualizationModule *vis_module) = 0;

    virtual void configure3DView() { }
    virtual void configureAxialView() { }
    virtual void configureCoronalView() { }
    virtual void configureSagittalView() { }

    virtual void on3DRenderingSurfaceCreated() { }
    virtual void onAxialRenderingSurfaceCreated() { }
    virtual void onCoronalRenderingSurfaceCreated() { }
    virtual void onSagittalRenderingSurfaceCreated() { }

    virtual void onAllRenderingSurfacesCreated() { }

    virtual void saveWorkSesseion(std::string /*folder_path*/){ }
    virtual void loadWorkSesseion(std::string /*folder_path*/){ }

    std::string getVisSubModuleName() const;

private:
    std::string m_visSubModuleName;
};

//============================================================================================================================================================================

class VisualizationModule: public BaseModule
{
public:
    VisualizationModule(std::string module_name);
    ~VisualizationModule();

    void init(MainWindow *mainWindow);
    virtual void loadSubModules() = 0;

    StateManager *getStateManager();

    void createVisualizationTab(std::string vis_tab_title, bool enable_checkbox = true);

    vmtkAbstractRender *getRenderer3D();
    vmtkAbstractRender *getRendererAxial();
    vmtkAbstractRender *getRendererCoronal();
    vmtkAbstractRender *getRendererSagittal();

    RenderingSurfaceSet *getRenderingSurfaceSet();

    VisualizationSubModule* getVisSubModule(std::string module_name);

    void addVisSubModule(VisualizationSubModule* module);

    MainWindow *getMainWindow() const;

    VisualizationTab *getVisualizationTab() const;

    void loadSession(std::string folder_path);
    void saveSession(std::string folder_path);

private:
    virtual vmtkAbstractRender* createRenderer3D() = 0;
    virtual vmtkAbstractRender* createRenderer2D() = 0;

    void createStateManager();
    void createRenderingSurfaceSet();

    void addNumInitializedRenderingSurface();

    MainWindow *m_mainWindow;

    bool m_createRenderer3D;
    bool m_createRendererAxial;
    bool m_createRendererCoronal;
    bool m_createRendererSagittal;

    vmtkAbstractRender *m_renderer3D;
    vmtkAbstractRender *m_rendererAxial;
    vmtkAbstractRender *m_rendererCoronal;
    vmtkAbstractRender *m_rendererSagittal;

    RenderingSurface *m_renderingSurface3D;
    RenderingSurface *m_renderingSurfaceAxial;
    RenderingSurface *m_renderingSurfaceCoronal;
    RenderingSurface *m_renderingSurfaceSagittal;

    unsigned short m_numInitializedRenderingSurface;

protected:
    VisualizationTab *m_visualizationTab;
    RenderingSurfaceSet *m_renderingSurfaceSet;

    StateManager *m_stateManager;

    std::vector<VisualizationSubModule *> m_subModuleVector;
};

#endif // VISUALIZATION_MODULE_H
