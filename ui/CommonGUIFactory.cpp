#include "CommonGUIFactory.h"

#include "OpenGLVisualizationTab.h"

CommonGUIFactory::CommonGUIFactory(QMainWindowVMTK *main_window)
{
    m_mainWindow = main_window;
    m_parent = nullptr;
}

VisualizationTab *CommonGUIFactory::createVisualizationTab(std::string vis_tab_name)
{

    if(vis_tab_name.compare("OpenGLVisualizationTab") == 0)
    {
        OpenGLVisualizationTab *vis_tab = new OpenGLVisualizationTab(m_parent);
        return vis_tab;
    }

    return nullptr;
}

vmtk::BaseToolTab *CommonGUIFactory::createToolTab(std::string tool_tab_class_name)
{
    return nullptr;
}

void CommonGUIFactory::setParent(QWidget *parent)
{
    m_parent = parent;
}
