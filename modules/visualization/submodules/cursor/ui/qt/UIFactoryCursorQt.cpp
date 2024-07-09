#include "UIFactoryCursorQt.h"

#include "QToolCursor.h"

UIFactoryCursorQt::UIFactoryCursorQt():
    UIFactoryQt("UIFactoryCursorQt")
{

}

ToolBase *UIFactoryCursorQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("ToolCursor") == 0)
    {
        QToolCursor *tool = new QToolCursor(getParentWidget());
        return tool;
    }

    return nullptr;
}
