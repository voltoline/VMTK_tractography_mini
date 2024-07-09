#ifndef PLUGIN_FILE_MANAGER_H
#define PLUGIN_FILE_MANAGER_H

#include <memory>

#include "model/BasePlugin.h"
#include "file_reader/FileManagerModule.h"

class PluginFileManager: public vmtk::BasePlugin
{
public:
    PluginFileManager();

    void init(MainWindow *main_window);

private:
    std::shared_ptr<FileManagerModule> m_fileManagerModule;
};

#endif // PLUGIN_FILE_MANAGER_H
