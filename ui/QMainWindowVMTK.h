#ifndef QMAIN_WINDOW_VMTK_H
#define QMAIN_WINDOW_VMTK_H

/**
 * @file QMainWindowVMTK.h
 * @author
 * @date   2018
 *
 */

#include "utilities/QCheckingOpenGL.h"
#include <QMainWindow>
#include "ui/ui_QMainWindowVMTK.h"
#include <QList>
#include <iostream>
#include <QCloseEvent>
#include <QPointer>
#include <QActionGroup>

#include "core/BaseToolTab.h"
#include "core/ToolBase.h"
#include "core/MainWindow.h"
#include "QCustomProgressDialog.h"
#include <QFile>
#include "string.h"
#include "cstring"
#include <QAction>
#include <QDialog>
#include <QMessageBox>


class CommonGUIFactory;

namespace Ui {
    class QMainWindowVMTK;
}

/**
 * @brief Inherit MainWindow base class and implement with QT the GUI of the main window
 *
 * @class QMainWindowVMTK QMainWindowVMTK.h "QMainWindowVMTK.h"
 */
class QMainWindowVMTK : public QMainWindow, public MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct an object of the class QMainWindowVMTK
     *
     * @fn QMainWindowVMTK
     * @param parent
     */
    explicit QMainWindowVMTK(QWidget *parent = 0);

    /**
     * @brief Destruct an object of the class QMainWindowVMTK
     *
     * @fn ~QMainWindowVMTK
     */
    ~QMainWindowVMTK();

    /**
     * @brief change the current visualization tab being viewed
     *
     * @fn setCurrentTab
     * @param index         the visualization tab index
     */
    void setCurrentVisualizationTab(short index) override;

    /**
     * @brief change the current tool tab
     * @fn setCurrentToolTab
     * @param index the tool tab index
     */
    void setCurrentToolTab(short index) override;

    /**
     * @brief recovery the current index tool tab
     * @fn getCurrentVisualizationTab
     * @return return index
     */
    short getCurrentToolTab() override;

    /**
     * @brief create a progress dialog
     *
     * @fn createProgressDialog
     * @return ProgressDialog       the progress dialog created
     */
    ProgressDialog* createProgressDialog() override;

    /**
     * @brief add a new tool tab to the main window
     *
     * @fn addToolTab
     * @param tool_tab          the tool tab to be added
     * @param tool_tab_title    the title of the tool tab
     */
    vmtk::BaseToolTab* addToolTab(std::string tool_tab_class_name, std::string tool_tab_title) override;

    ToolBase *addTool(std::string tool_class_name, std::string tool_tab_title, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span) override;

    ToolBase* addToolAsDialogWindow(std::string tool_class_name, std::string tool_tab_title, std::string button_name, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span) override;

    void blockToolTabs(bool state) override;

    void blockVisualizationTabs(bool state) override;

    /**
     * @brief add a new visualization tab to the main window
     *
     * @fn addVisualizationTab
     * @param vis_tab               the visualization tab to be added
     * @param vis_tab_title         the title of the visualization tab
     * @param enable_check_box      insert or not a check box into the title of the visualization tab
     */
    VisualizationTab *addVisualizationTab(std::string vis_tab_class_name, std::string vis_tab_title, bool enable_check_box) override;

    /**
     * @brief get the amount of the visualization tabs that had their check box checked
     *
     * @fn getNumSelectedTabs
     * @return unsigned short   the amount of the checked visualization tabs
     */
    unsigned short getNumSelectedTabs() override;

    /**
     * @brief get the indices of the visualization tabs that had their check box checked
     *
     * @fn getIndexSelectedTabs
     * @return std::vector<unsigned short>  the indices of the checked visualization tabs
     */
    std::vector<unsigned short> getIndexSelectedTabs() override;

    /**
     * @brief show a message box
     *
     * @fn createMessageBox
     * @param title             the title of the message box
     * @param msg               the message to be showed
     */
    void showMessageBox(std::string title, std::string msg) override;
    bool showQuestionMessageBox(std::string title, std::string msg) override;
    void uncheckAllVisualizationTab() override;

    QMenu *addMainMenu(std::string mainMenu);

    ToolBase *addMenuToolbaseUnique(std::string mainMenuName, std::string tool_name, std::string ui_name) override;
    QMenu *addSubMenu(std::string mainMenuname, std::string subMenuName);
    ToolBase *addMenuToolBaseforTools(std::string mainMenuName, std::string groupMenuToolName, std::string tool_name, std::string ui_name) override;
    ToolBase *addMenuToolbaseUniqueAddingDialog(std::string mainMenuName, std::string subMenuName, std::string tool_name, std::string ui_name) override;
private:
    /**
     * @brief handle the event created when closing the main window
     *
     * @fn closeEvent
     * @param event     the closing event
     */
    void closeEvent(QCloseEvent *event) override;

    QGridLayout *createDefaultToolTab(std::string tool_tab_title);

    std::map<std::string, QGridLayout*> m_toolTabMap;

    Ui::QMainWindowVMTK *ui; /**< TODO: describe */

    QPointer<QAction> m_original_style = nullptr;
    QPointer<QAction> m_dark_style_style = nullptr;
    QPointer<QActionGroup> m_styles = nullptr;

    CommonGUIFactory *m_guiFactory = nullptr;

private slots:
    /**
     * @brief slot called when changing the current visualization tab
     *
     * @fn on_m_tabWidget_reg_currentChanged
     * @param index                             the index of the new current visualization tab
     */
    void on_m_tabWidget_reg_currentChanged(int index);

    void tabCheckBoxToggled(unsigned short tab_index, bool checked);

    void changeOriginalQssStyle();

    void changeLightBlueQssStyle();
    void on_m_tabWidgetTools_currentChanged(int index);

    void visualuzationTabClosed(int tab_index);

};

#endif // QMAIN_WINDOW_VMTK_H
