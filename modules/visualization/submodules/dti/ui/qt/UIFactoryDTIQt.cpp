#include "UIFactoryDTIQt.h"

#include "QTabDTITractography.h"
#include <core/MainWindow.h>
#include <modules/visualization/submodules/volume_renderer_diff/ui/qt/QToolDiffusion.h>

UIFactoryDTIQt::UIFactoryDTIQt(MainWindow *main_window):
    UIFactoryQt("UIFactoryDTIQt")
{
    m_mainWindow = main_window;
}

ToolBase *UIFactoryDTIQt::createTool(std::string tool_class_name)
{
        if(tool_class_name.compare("ToolTractography") == 0) {

        QTabDTITractography *tool = new QTabDTITractography(getParentWidget());
        return tool;
    }

    return nullptr;
}
