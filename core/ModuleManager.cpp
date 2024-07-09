#include "ModuleManager.h"

#include "modules/PluginFileManager.h"

ModuleManager::ModuleManager(MainWindow *main_window):
    m_mainWindow(main_window)
{

}

void ModuleManager::init()
{
    m_pluginFileManager = std::make_shared<PluginFileManager>();
    m_pluginFileManager->init(m_mainWindow);
}

void ModuleManager::addModule(BaseModule::sPtr module)
{
    m_moduleVector.push_back(module);
    module->init(m_mainWindow);
}

void ModuleManager::removeModule(BaseModule::sPtr module)
{
    for(unsigned short i = 0; i < m_moduleVector.size(); i++)
        if(m_moduleVector[i] == module)
            m_moduleVector.erase(m_moduleVector.begin() + i);
}

unsigned int ModuleManager::getSize()
{
    return m_moduleVector.size();
}

BaseModule::sPtr ModuleManager::getModule(unsigned short index) const
{
    return m_moduleVector[index];
}

BaseModule::sPtr ModuleManager::getModule(std::string module_name) const
{
    for(auto module: m_moduleVector)
        if(module->getModuleClassName().compare(module_name) == 0)
            return module;

    return nullptr;
}
