#include "PluginFileManager.h"

PluginFileManager::PluginFileManager():
    BasePlugin("PluginFileManager")
{

}

void PluginFileManager::init(MainWindow *main_window)
{
    m_fileManagerModule = std::make_shared<FileManagerModule>();
    main_window->getModuleManager().addModule(std::static_pointer_cast<BaseModule>(m_fileManagerModule));
}
