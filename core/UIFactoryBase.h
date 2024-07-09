#ifndef UI_FACTORY_BASE_H
#define UI_FACTORY_BASE_H

#include "core/VisualizationTab.h"
#include "core/ToolBase.h"

#include <memory>

class UIFactoryBase
{
public:
    typedef std::shared_ptr<UIFactoryBase> sPtr;
    typedef std::unique_ptr<UIFactoryBase> uPtr;

    UIFactoryBase(std::string factory_class_name);
    virtual ~UIFactoryBase(){ }

    virtual VisualizationTab* createVisualizationTab(std::string vis_tab_name);
    virtual ToolBase* createTool(std::string tool_class_name);

    std::string getUIFactoryClassName() const;

private:
    std::string m_uiFactoryClassName;
};

#endif // UI_FACTORY_BASE_H
