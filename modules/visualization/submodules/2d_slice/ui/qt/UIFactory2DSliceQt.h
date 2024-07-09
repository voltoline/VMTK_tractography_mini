#ifndef UI_FACTORY_2D_SLICE_QT_H
#define UI_FACTORY_2D_SLICE_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactory2DSliceQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactory2DSliceQt> sPtr;
    typedef std::unique_ptr<UIFactory2DSliceQt> uPtr;

    UIFactory2DSliceQt();
    ~UIFactory2DSliceQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_2D_SLICE_QT_H
