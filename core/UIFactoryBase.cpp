#include "UIFactoryBase.h"

UIFactoryBase::UIFactoryBase(std::string factory_class_name)
{
    m_uiFactoryClassName = factory_class_name;
}

VisualizationTab *UIFactoryBase::createVisualizationTab(std::string /*vis_tab_name*/)
{
    return nullptr;
}

ToolBase *UIFactoryBase::createTool(std::string /*tool_class_name*/)
{
    return nullptr;
}

std::string UIFactoryBase::getUIFactoryClassName() const
{
    return m_uiFactoryClassName;
}

