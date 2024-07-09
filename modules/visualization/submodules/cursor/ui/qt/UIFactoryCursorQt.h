#ifndef UI_FACTORY_CURSOR_QT_H
#define UI_FACTORY_CURSOR_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactoryCursorQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactoryCursorQt> sPtr;
    typedef std::unique_ptr<UIFactoryCursorQt> uPtr;

    UIFactoryCursorQt();
    ~UIFactoryCursorQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_CURSOR_QT_H
