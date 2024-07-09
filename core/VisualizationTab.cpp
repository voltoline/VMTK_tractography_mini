#include "VisualizationTab.h"

VisualizationTab::VisualizationTab(std::string vis_tab_name):
    m_flagCurrentTab(false),
    m_visualizationTabName(vis_tab_name)
{

}

VisualizationTab::~VisualizationTab()
{

}

bool VisualizationTab::isCurrentTab()
{
    return m_flagCurrentTab;
}

void VisualizationTab::setStateCurrentTab(bool state)
{
    m_flagCurrentTab = state;
}

std::string VisualizationTab::getVisualizationTabName() const
{
    return m_visualizationTabName;
}
