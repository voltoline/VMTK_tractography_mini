#ifndef UI_FACTORY_DTI_QT_H
#define UI_FACTORY_DTI_QT_H

#include "ui/qt/UIFactoryQt.h"

class MainWindow;

class UIFactoryDTIQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactoryDTIQt> sPtr;
    typedef std::unique_ptr<UIFactoryDTIQt> uPtr;

    UIFactoryDTIQt(MainWindow *main_window);
    ~UIFactoryDTIQt() { }

    ToolBase *createTool(std::string tool_class_name) override;

private:
    MainWindow *m_mainWindow;
};

#endif // UI_FACTORY_DTI_QT_H
