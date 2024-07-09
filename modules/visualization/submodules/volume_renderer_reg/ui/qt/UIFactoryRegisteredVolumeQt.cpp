#include "UIFactoryRegisteredVolumeQt.h"

#include "QToolVolumeBlending.h"

UIFactoryRegisteredVolumeQt::UIFactoryRegisteredVolumeQt():
    UIFactoryQt("UIFactoryRegisteredVolumeQt")
{

}

ToolBase *UIFactoryRegisteredVolumeQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("ToolVolumeBlending") == 0)
    {
        QToolVolumeBlending *tool = new QToolVolumeBlending(getParentWidget());
        return tool;
    }

    return nullptr;
}
