#ifndef UI_FACTORY_DIFFUSION_RENDERER_QT_H
#define UI_FACTORY_DIFFUSION_RENDERER_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactoryDiffusionRendererQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactoryDiffusionRendererQt> sPtr;
    typedef std::unique_ptr<UIFactoryDiffusionRendererQt> uPtr;

    UIFactoryDiffusionRendererQt();
    ~UIFactoryDiffusionRendererQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_DIFFUSION_RENDERER_QT_H
