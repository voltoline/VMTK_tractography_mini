#ifndef VISUALIZATION_TAB_H
#define VISUALIZATION_TAB_H

#include "ui/RenderingSurface.h"
#include <iostream>
#include "renderer/vmtkAbstractRender.h"

class VisualizationTab
{
public:
    VisualizationTab(std::string vis_tab_name);
    virtual ~VisualizationTab();

    virtual void handleCloseEvent() { }

    bool isCurrentTab();
    void setStateCurrentTab(bool state);

    virtual RenderingSurface* createRenderingSurface(vmtkAbstractRender *render, std::function<void(RenderingSurface*)> on_initialization, std::string title = "", bool enable_debug = false, std::string debug_msg = "") = 0;

    std::string getVisualizationTabName() const;

private:
    bool m_flagCurrentTab;
    std::string m_visualizationTabName;
};

#endif // VISUALIZATION_TAB_H
