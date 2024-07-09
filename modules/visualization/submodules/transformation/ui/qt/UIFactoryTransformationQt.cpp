#include "UIFactoryTransformationQt.h"

#include "QToolTransformation.h"

UIFactoryTransformationQt::UIFactoryTransformationQt():
    UIFactoryQt("UIFactoryTransformationQt")
{

}

ToolBase *UIFactoryTransformationQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("ToolTransformation") == 0)
    {
        QToolTransformation *tool = new QToolTransformation(getParentWidget());
        return tool;
    }

    return nullptr;
}
