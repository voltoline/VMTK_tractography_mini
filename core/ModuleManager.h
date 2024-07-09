#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <vector>
#include <memory>

#include "core/BaseModule.h"

class MainWindow;

class PluginDiffusionAcquisitionVisualization;
class PluginAcquisitionVisualization;
class PluginFileManager;

class ModuleManager
{
public:
    ModuleManager(MainWindow *main_window);

    void init();
    void addModule(BaseModule::sPtr module);
    void removeModule(BaseModule::sPtr module);
    unsigned int getSize();
    BaseModule::sPtr getModule(unsigned short index) const;
    BaseModule::sPtr getModule(std::string module_name) const;

private:
    std::vector<BaseModule::sPtr> m_moduleVector;
    MainWindow *m_mainWindow;

    std::shared_ptr<PluginFileManager> m_pluginFileManager;
    std::shared_ptr<PluginAcquisitionVisualization> m_pluginAcquisitionVisualization;
    std::shared_ptr<PluginDiffusionAcquisitionVisualization> m_pluginDiffusionAcquisitionVisualization;
};

#endif // MODULE_MANAGER_H
