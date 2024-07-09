#include "ToolBase.h"

ToolBase::ToolBase(std::string tool_class_name):
    m_toolClassName(tool_class_name)
{

}

std::string ToolBase::getToolClassName() const
{
    return m_toolClassName;
}

//=======================================================================================================

ControllerTool::ControllerTool(std::string tool_tab_class_name):
    m_toolClassName(tool_tab_class_name)
{
    m_isFixed = false;
    m_tool = nullptr;
}

void ControllerTool::setTool(ToolBase *tool)
{
    m_tool = tool;
}

std::string ControllerTool::getToolClassName() const
{
    return m_toolClassName;
}

bool ControllerTool::isFixed() const
{
    return m_isFixed;
}

void ControllerTool::setIsFixed(bool is_fixed)
{
    m_isFixed = is_fixed;
}

ToolBase *ControllerTool::getTool() const
{
    return m_tool;
}
