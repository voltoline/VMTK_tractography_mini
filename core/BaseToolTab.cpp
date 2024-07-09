#include "BaseToolTab.h"

vmtk::BaseToolTab::BaseToolTab(std::string tool_tab_class_name, std::string tool_tab_title):
    m_toolTabClassName(tool_tab_class_name),
    m_toolTabTitle(tool_tab_title)
{

}

std::string vmtk::BaseToolTab::getToolTabClassName() const
{
    return m_toolTabClassName;
}

std::string vmtk::BaseToolTab::getToolTabTitle() const
{
    return m_toolTabTitle;
}

//=======================================================================================================

ControllerToolTab::ControllerToolTab(std::string tool_tab_class_name):
    m_toolTabClassName(tool_tab_class_name)
{
    m_isFixed = false;
    m_isUIEnabled = true;
    m_toolTab = nullptr;
}

void ControllerToolTab::updateUI()
{
    if(m_toolTab != nullptr)
        m_toolTab->updateComponents();
}

void ControllerToolTab::setToolTab(vmtk::BaseToolTab *tool_tab)
{
    m_toolTab = tool_tab;
}

std::string ControllerToolTab::getToolTabClassName() const
{
    return m_toolTabClassName;
}

bool ControllerToolTab::isFixed() const
{
    return m_isFixed;
}

void ControllerToolTab::setIsFixed(bool is_fixed)
{
    m_isFixed = is_fixed;
}

bool ControllerToolTab::isUIEnabled() const
{
    return m_isUIEnabled;
}

void ControllerToolTab::setUIEnabled(bool is_ui_enabled)
{
    m_isUIEnabled = is_ui_enabled;
    updateUI();
}

vmtk::BaseToolTab *ControllerToolTab::getToolTab() const
{
    return m_toolTab;
}
