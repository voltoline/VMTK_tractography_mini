#include "UIFactory2D3DSlicingQt.h"

#include "QTool2D3DSlicing.h"

UIFactory2D3DSlicingQt::UIFactory2D3DSlicingQt():
    UIFactoryQt("UIFactory2D3DSlicingQt")
{

}

ToolBase *UIFactory2D3DSlicingQt::createTool(std::string tool_class_name)
{
    if(tool_class_name.compare("Tool2D3DSlicing") == 0)
    {
        QTool2D3DSlicing *tool = new QTool2D3DSlicing(getParentWidget());
        return tool;
    }

    return nullptr;
}
