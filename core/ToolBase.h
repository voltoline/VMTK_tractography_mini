#ifndef TOOL_BASE_H
#define TOOL_BASE_H

#include <vector>
#include <iostream>
#include "VisualizationTab.h"

class ControllerTool;

class ToolBase
{
public:
    /**
     * @brief
     *
     * @fn ToolBase
     * @param tool_class_name
     * @param tool_tab_title
     */
    ToolBase(std::string tool_class_name);

    /**
     * @brief
     *
     * @fn ~ToolBase
     */
    virtual ~ToolBase() { }

    /**
     * @brief
     *
     * @fn updateComponents
     */
    virtual void updateComponents() = 0;

    /**
     * @brief
     *
     * @fn currentVisualizationTabChanged
     * @param vis_tab
     */
    virtual void currentVisualizationTabChanged(VisualizationTab *vis_tab) = 0;

    /**
     * @brief
     *
     * @fn getToolClassName
     * @return std::string
     */
    std::string getToolClassName() const;

private:
    std::string m_toolClassName; /**< TODO: describe */
};

/**
 * @brief
 *
 * @class ControllerTool ToolBase.h "ToolBase.h"
 */
class ControllerTool
{
public:
    /**
     * @brief
     *
     * @fn ControllerTool
     * @param tool_class_name
     */
    ControllerTool(std::string tool_class_name);

    /**
     * @brief
     *
     * @fn ~ControllerTool
     */
    virtual ~ControllerTool() { }

    /**
     * @brief
     *
     * @fn setTool
     * @param tool_tab
     */
    void setTool(ToolBase *tool);

    /**
     * @brief
     *
     * @fn getToolClassName
     * @return std::string
     */
    std::string getToolClassName() const;

    /**
     * @brief
     *
     * @fn isFixed
     * @return bool
     */
    bool isFixed() const;

    /**
     * @brief
     *
     * @fn setIsFixed
     * @param is_fixed
     */
    void setIsFixed(bool is_fixed);

    ToolBase *getTool() const;

private:
    ToolBase *m_tool; /**< TODO: describe */
    std::string m_toolClassName; /**< TODO: describe */
    bool m_isFixed; /**< TODO: describe */
};

#endif // TOOL_BASE_H
