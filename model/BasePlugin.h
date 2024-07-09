#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <iostream>

#include "core/MainWindow.h"

namespace vmtk
{
    class BasePlugin
    {
    public:
        BasePlugin(std::string plugin_name);
        virtual ~BasePlugin() { }

        virtual void init(MainWindow *main_window) = 0;

        std::string getPluginName() const;

    private:
        std::string m_pluginName;
    };
}

#endif // BASE_PLUGIN_H
