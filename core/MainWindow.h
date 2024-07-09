#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ModuleManager.h"
#include "VisualizationTab.h"
#include "model/Signal.hpp"
#include "BaseToolTab.h"
#include "BaseModule.h"
#include "UIFactoryBase.h"
#include <vector>

class ProgressDialog;

enum class MainWindowEvent
{
    TAB_CHECKBOX_TOGGLED
};

/**
 * @brief
 *
 * @class MainWindow MainWindow.h "MainWindow.h"
 */
class MainWindow
{
public:
    /**
     * @brief
     *
     * @fn MainWindow
     */
    MainWindow();

    /**
     * @brief
     *
     * @fn ~MainWindow
     */
    virtual ~MainWindow();

    /**
     * @brief
     *
     * @fn setCurrentVisualizationTab
     * @param index
     */
    virtual void setCurrentVisualizationTab(short index) = 0;

    /**
     * @brief
     *
     * @fn createProgressDialog
     * @return ProgressDialog
     */
    virtual ProgressDialog* createProgressDialog() = 0;

    /**
     * @brief
     *
     * @fn addVisualizationTab
     * @param vis_tab
     * @param vis_tab_title
     * @param enable_check_box
     */
    virtual VisualizationTab* addVisualizationTab(std::string vis_tab_class_name, std::string vis_tab_title, bool enable_check_box) = 0;

    /**
     * @brief
     *
     * @fn getNumSelectedTabs
     * @return unsigned short
     */
    virtual unsigned short getNumSelectedTabs() = 0;

    /**
     * @brief
     *
     * @fn getIndexSelectedTabs
     * @return std::vector<unsigned short>
     */
    virtual std::vector<unsigned short> getIndexSelectedTabs() = 0;

    /**
     * @brief
     * @fn setCurrentToolTab
     * @param index
     */
    virtual void setCurrentToolTab(short index) = 0;

    /**
     * @brief
     * @fn getCurrentToolTab
     * @return index
     */
    virtual short getCurrentToolTab() = 0;

    /**
     * @brief
     *
     * @fn showMessageBox
     * @param title
     * @param msg
     */
    virtual void showMessageBox(std::string title, std::string msg) = 0;

    virtual bool showQuestionMessageBox(std::string title, std::string msg) = 0;

    virtual void uncheckAllVisualizationTab() = 0;

    virtual void blockToolTabs(bool state) = 0;

    virtual void blockVisualizationTabs(bool state) = 0;

    /**
     * @brief
     *
     * @fn init
     */
    void init();

    VisualizationTab* createVisualizationTab(std::string vis_tab_title, BaseModule *module, std::string vis_tab_class_name = "", bool enable_check_box = true);

    /**
     * @brief
     *
     * @fn getModuleManager
     * @return ModuleManager
     */
    ModuleManager& getModuleManager();

    /**
     * @brief
     *
     * @fn getModuleWithVisualizationTab
     * @param index
     * @return std::shared_ptr<BaseModule>
     */
    std::shared_ptr<BaseModule> getModuleWithVisualizationTab(unsigned short vis_index);

    /**
     * @brief
     *
     * @fn getNumVisualizationTab
     * @return unsigned short
     */
    unsigned short getNumVisualizationTab();


    int addTabCheckBoxToggledHandler(std::function<void(unsigned short, bool)> slot);

    int addCurrentVisualizationTabChangedHandler(std::function<void(unsigned short)> slot);

    int addCurrentToolTabChangedHandler(std::function<void (short, std::string) > slot);

    short getVisualizationTabIndexOf(VisualizationTab *vis_tab);

    vmtk::BaseToolTab *getToolTab(std::string tool_tab_class_name);

    bool createToolAsDialogWindow(std::string tool_name, std::string tool_tab_title, std::string button_name);

    bool createToolAsDialogWindow(std::string tool_name, std::string tool_tab_title, std::string button_name, unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1);

    bool createTool(std::string tool_name, std::string tool_tab_title);

    bool createTool(std::string tool_name, std::string tool_tab_title, unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1);

    ToolBase *getTool(std::string tool_name, std::string tool_tab_title);

    void addUIFactory(UIFactoryBase::uPtr ui_factory);

    bool createMenuToolBaseUnique(std::string menuParent, std::string tool_name, std::string ui_name);

    bool createMenuToolBaseUniqueAddingDialog(std::string mainMenu, std::string subMenuName, std::string tool_name, std::string ui_name);

    ToolBase *getToolUniqueWindowFloat(std::string tool_name);

    bool createMenuToolBaseforTools(std::string mainMenuName, std::string groupMenuToolName, std::string tool_name, std::string ui_name);

protected:
    /**
     * @brief
     *
     * @fn currentTabChanged
     * @param index
     */
    void currentVisualizationTabChanged(short index);

    void visualizationTabClosed(int index);

    /**
     * @brief
     * @fn currentToolTabChanged
     * @param index
     * @param class_name
     */
    void currentToolTabChanged(short index, std::string class_name);

    /**
     * @brief
     *
     * @fn handleCloseEvent
     */
    void handleCloseEvent();

    void notifyTabCheckBoxTooggled(unsigned short tab_index, bool checked);

    void notifyCurrentVisualizationTabChanged(unsigned short tab_index);

    void notifyCurrentToolTabChanged(short tab_index, std::string tab_class_name);

    std::vector<UIFactoryBase::uPtr> &getUIFactories();

    /**
     * @brief
     *
     * @fn addToolTab
     * @param tool_tab
     * @param tool_tab_title
     */
    virtual vmtk::BaseToolTab* addToolTab(std::string tool_tab_class_name, std::string tool_tab_title) = 0;

    virtual ToolBase* addTool(std::string tool_class_name, std::string tool_tab_title, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span) = 0;

    virtual ToolBase* addToolAsDialogWindow(std::string tool_class_name, std::string tool_tab_title, std::string button_name, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span) = 0;

    virtual ToolBase* addMenuToolbaseUnique(std::string mainMenu, std::string tool_name, std::string ui_name) = 0;

    virtual ToolBase* addMenuToolbaseUniqueAddingDialog(std::string mainMenu, std::string subMenuName, std::string tool_name, std::string ui_name) = 0;

    virtual ToolBase *addMenuToolBaseforTools(std::string mainMenuName, std::string groupMenuToolName, std::string tool_name, std::string ui_name) = 0;


private:
    typedef struct _ui_free_space
    {
        unsigned short NUM_ROW = 3;
        unsigned short NUM_COL = 10;
        bool m_freeSpace[3][10];

        _ui_free_space()
        {
            for(int i = 0; i < NUM_ROW; ++i)
                for(int j = 0; j < NUM_COL; ++j)
                    m_freeSpace[i][j] = true;
        }

        bool isOutOfRange(unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1)
        {
            if(row >= NUM_ROW || col >= NUM_COL || (row + row_span - 1) >= NUM_ROW || (col + col_span - 1) >= NUM_COL)
            {
                std::cerr << "UIFreeSpace --> Location (" << row << "," << col << ","  << row_span << "," << col_span << ") is out of range!" << std::endl;
                return true;
            }

            return false;
        }

        bool findNextFreeSpace(unsigned short &row, unsigned short &col)
        {
            for(int i = 0; i < NUM_ROW; ++i)
                for(int j = 0; j < NUM_COL; ++j)
                    if(m_freeSpace[i][j] == true)
                    {
                        row = i;
                        col = j;
                        return true;
                    }

            return false;
        }

        bool isAvalible(unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1)
        {
            if(isOutOfRange(row, col, row_span, col_span))
                return false;

            for(int i = row; i < (row + row_span); ++i)
                for(int j = col; j < (col + col_span); ++j)
                    if(m_freeSpace[i][j] == false)
                        return false;

            return true;
        }

        void setAllocated(unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1)
        {
            if(isOutOfRange(row, col, row_span, col_span))
                return;

            for(int i = row; i < (row + row_span); ++i)
                for(int j = col; j < (col + col_span); ++j)
                    m_freeSpace[i][j] = false;
        }

        void setFree(unsigned short row, unsigned short col, unsigned short row_span=1, unsigned short col_span=1)
        {
            if(isOutOfRange(row, col, row_span, col_span))
                return;

            for(int i = row; i < (row + row_span); ++i)
                for(int j = col; j < (col + col_span); ++j)
                    m_freeSpace[i][j] = true;
        }

    } UIFreeSpace;

    std::vector<VisualizationTab*> m_visualizationTabs;                  /**< TODO: describe */
    VisualizationTab *m_currentVisualizationTab;

    std::vector<vmtk::BaseToolTab*> m_toolTabs;
    ModuleManager *m_moduleManager;                                      /**< TODO: describe */
    std::vector<BaseModule::sPtr> *m_moduleWithTabVector;    /**< TODO: describe */

    Signal<unsigned short, bool> m_signalTabCheckBoxToggled;
    Signal<unsigned short> m_signalCurrentVisualizationTabChanged;
    Signal<unsigned short, std::string> m_signalCurrentToolTabChanged;

    std::vector<UIFactoryBase::uPtr> m_uiFactories;
    std::map<std::string, std::vector<ToolBase*> > *m_toolsMap;
    std::map<std::string, UIFreeSpace> *m_toolTabFreeSpace;
};

#endif // MAIN_WINDOW_H
