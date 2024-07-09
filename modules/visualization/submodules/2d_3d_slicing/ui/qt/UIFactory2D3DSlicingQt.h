#ifndef UI_FACTORY_2D_3D_SLICING_QT_H
#define UI_FACTORY_2D_3D_SLICING_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactory2D3DSlicingQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactory2D3DSlicingQt> sPtr;
    typedef std::unique_ptr<UIFactory2D3DSlicingQt> uPtr;

    UIFactory2D3DSlicingQt();
    ~UIFactory2D3DSlicingQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_3D_CLIPPING_QT_H
