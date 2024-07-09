#ifndef COMMON_GUI_FACTORY_H
#define COMMON_GUI_FACTORY_H

#include "core/BaseToolTab.h"

class QMainWindowVMTK;
class QWidget;

class CommonGUIFactory //: public BaseGUIFactory
{
public:
    CommonGUIFactory(QMainWindowVMTK *main_window);

    VisualizationTab *createVisualizationTab(std::string vis_tab_name);
    vmtk::BaseToolTab *createToolTab(std::string tool_tab_class_name);

    void setParent(QWidget *parent);

private:
    QMainWindowVMTK *m_mainWindow;
    QWidget *m_parent;
};

#endif // COMMON_GUI_FACTORY_H
