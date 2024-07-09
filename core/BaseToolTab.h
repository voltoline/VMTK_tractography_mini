#ifndef BASE_TOOL_TAB_H
#define BASE_TOOL_TAB_H

#include <vector>
#include <iostream>
#include "VisualizationTab.h"

class ControllerToolTab;

namespace vmtk
{
    /**
     * @brief
     *
     * @class BaseToolTab BaseToolTab.h "BaseToolTab.h"
     */
    class BaseToolTab
    {
    public:
        /**
         * @brief
         *
         * @fn BaseToolTab
         * @param tool_tab_class_name
         * @param tool_tab_title
         */
        BaseToolTab(std::string tool_tab_class_name, std::string tool_tab_title);

        /**
         * @brief
         *
         * @fn ~BaseToolTab
         */
        virtual ~BaseToolTab() { }

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
         * @fn getToolTabClassName
         * @return std::string
         */
        std::string getToolTabClassName() const;

        /**
         * @brief
         *
         * @fn getToolTabTitle
         * @return std::string
         */
        std::string getToolTabTitle() const;

    private:
        std::string m_toolTabClassName; /**< TODO: describe */
        std::string m_toolTabTitle; /**< TODO: describe */
    };
}

/**
 * @brief
 *
 * @class ControllerToolTab BaseToolTab.h "BaseToolTab.h"
 */
class ControllerToolTab
{
public:
    /**
     * @brief
     *
     * @fn ControllerToolTab
     * @param tool_tab_class_name
     */
    ControllerToolTab(std::string tool_tab_class_name);

    /**
     * @brief
     *
     * @fn ~ControllerToolTab
     */
    virtual ~ControllerToolTab() { }

    /**
     * @brief
     *
     * @fn updateUI
     */
    void updateUI();

    /**
     * @brief
     *
     * @fn setToolTab
     * @param tool_tab
     */
    void setToolTab(vmtk::BaseToolTab *tool_tab);

    /**
     * @brief
     *
     * @fn getToolTabClassName
     * @return std::string
     */
    std::string getToolTabClassName() const;

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

    /**
     * @brief
     *
     * @fn isUIEnabled
     * @return bool
     */
    bool isUIEnabled() const;

    /**
     * @brief
     *
     * @fn setUIEnabled
     * @param is_ui_enabled
     */
    void setUIEnabled(bool is_ui_enabled);

    vmtk::BaseToolTab *getToolTab() const;

private:
    vmtk::BaseToolTab *m_toolTab; /**< TODO: describe */
    std::string m_toolTabClassName; /**< TODO: describe */
    bool m_isFixed; /**< TODO: describe */
    bool m_isUIEnabled; /**< TODO: describe */
};

#endif // BASE_TOOL_TAB_H
