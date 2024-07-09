#include "UIFactory2DSliceQt.h"

#include "QToolSliceOrientation.h"

UIFactory2DSliceQt::UIFactory2DSliceQt():
    UIFactoryQt("UIFactory2DSliceQt")
{

}

ToolBase *UIFactory2DSliceQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("ToolSliceOrientation") == 0)
    {
        QToolSliceOrientation *tool = new QToolSliceOrientation(getParentWidget());
        return tool;
    }

    return nullptr;
}
