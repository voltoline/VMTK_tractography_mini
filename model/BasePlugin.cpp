#include "BasePlugin.h"

vmtk::BasePlugin::BasePlugin(std::string plugin_name):
    m_pluginName(plugin_name)
{

}

std::string vmtk::BasePlugin::getPluginName() const
{
    return m_pluginName;
}
