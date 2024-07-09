#include "UIFactoryDiffusionRendererQt.h"

#include "QToolDiffusion.h"

UIFactoryDiffusionRendererQt::UIFactoryDiffusionRendererQt():
    UIFactoryQt("UIFactoryDiffusionRendererQt")
{

}

ToolBase *UIFactoryDiffusionRendererQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("ToolDiffusion") == 0)
    {
        QToolDiffusion *tool = new QToolDiffusion(getParentWidget());
        return tool;
    }

    return nullptr;
}
