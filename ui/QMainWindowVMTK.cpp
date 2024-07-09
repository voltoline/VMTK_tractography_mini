#include "ui/QMainWindowVMTK.h"

#include "CommonGUIFactory.h"

#include "core/BaseModule.h"
#include "ui/qt/UIFactoryQt.h"
#include "ui/components/QDialogButton.h"

QMainWindowVMTK::QMainWindowVMTK(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QMainWindowVMTK)
{
    ui->setupUi(this);

    ui->m_tabWidgetTools->removeTab(0); //Remove the tab tool layouts (temporarily).
    ui->m_pushButtonSavePreferences->setVisible(false);
    ui->m_pushButtonPrint->setVisible(false);

    connect(ui->m_tabWidget_reg, SIGNAL(tabCheckBoxToggled(unsigned short, bool)), this, SLOT(tabCheckBoxToggled(unsigned short, bool)));

    m_original_style = new QAction("Original", this);
    m_original_style->setCheckable(true);
    m_original_style->setChecked(true);
    connect(m_original_style, SIGNAL(triggered()), this, SLOT(changeOriginalQssStyle()));

    m_dark_style_style = new QAction("Light Blue", this);
    m_dark_style_style->setCheckable(true);
    m_dark_style_style->setChecked(false);
    connect(m_dark_style_style, SIGNAL(triggered()), this, SLOT(changeLightBlueQssStyle()));

    m_styles = new QActionGroup(this);
    m_styles->setExclusive(true);
    m_styles->addAction(m_original_style);
    m_styles->addAction(m_dark_style_style);

    QMenu *styleMenu = new QMenu("Style");
    ui->m_menuBar->addMenu(styleMenu);
    styleMenu->addAction(m_original_style);
    styleMenu->addAction(m_dark_style_style);

    m_guiFactory = new CommonGUIFactory(this);

    connect(ui->m_tabWidget_reg, SIGNAL(tabCloseRequested(int)), this, SLOT(visualuzationTabClosed(int)));

    MainWindow::init();
}

QMainWindowVMTK::~QMainWindowVMTK()
{
    delete ui;
}

void QMainWindowVMTK::setCurrentVisualizationTab(short index)
{
    ui->m_tabWidget_reg->setCurrentIndex(index);
}

void QMainWindowVMTK::setCurrentToolTab(short index)
{
    ui->m_tabWidgetTools->setCurrentIndex(index);
}

short QMainWindowVMTK::getCurrentToolTab()
{
    return static_cast<short>( ui->m_tabWidgetTools->currentIndex() );
}

ProgressDialog *QMainWindowVMTK::createProgressDialog()
{
    return new QCustomProgressDialog(this);
}

vmtk::BaseToolTab* QMainWindowVMTK::addToolTab(std::string tool_tab_class_name, std::string tool_tab_title)
{
    m_guiFactory->setParent(ui->m_tabWidgetTools);
    vmtk::BaseToolTab *tool_tab = m_guiFactory->createToolTab(tool_tab_class_name);
    QWidget *qtool_tab = dynamic_cast<QWidget*>(tool_tab);
    if(qtool_tab)
    {
        if(tool_tab_title.empty())
            tool_tab_title = tool_tab->getToolTabTitle();

        ui->m_tabWidgetTools->addTab(qtool_tab, tool_tab_title.c_str());

    } else
        std::cerr << "QMainWindowVMTK::addToolTab -->> cant dynamic cast tool tab! " << std::endl;

    return tool_tab;
}

ToolBase *QMainWindowVMTK::addTool(std::string tool_class_name, std::string tool_tab_title, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span)
{
    ToolBase *tool_tab = nullptr;

    for(auto &f: getUIFactories())
    {
        static_cast<UIFactoryQt*>(f.get())->setParentWidget(this);
        tool_tab = f->createTool(tool_class_name);
        if(tool_tab != nullptr)
            break;
    }

    if(tool_tab == nullptr)
    {
        std::cerr << "QMainWindowVMTK::addTool -->> " << tool_class_name << " not found!" << std::endl;
        return nullptr;
    }

    QWidget *qtool_tab = dynamic_cast<QWidget*>(tool_tab);

    if(tool_tab)
    {
        qtool_tab->setParent(this->centralWidget());

        auto it = m_toolTabMap.find(tool_tab_title);
        if (it != m_toolTabMap.end())
        {
            QGridLayout* tooltab_layout = m_toolTabMap[tool_tab_title];
            tooltab_layout->addWidget(qtool_tab, row, col, row_span, col_span, Qt::AlignLeft);

        } else {

            QGridLayout *tooltab_layout = createDefaultToolTab(tool_tab_title);
            tooltab_layout->addWidget(qtool_tab, row, col, row_span, col_span, Qt::AlignLeft);

            m_toolTabMap[tool_tab_title] = tooltab_layout;
        }

    } else
        std::cerr << "QMainWindowVMTK::addTool -->> cant dynamic cast tool tab! " << std::endl;


    return tool_tab;
}

ToolBase *QMainWindowVMTK::addToolAsDialogWindow(std::string tool_class_name, std::string tool_tab_title, std::string button_name, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span)
{
    ToolBase *tool_tab = nullptr;

    for(auto &f: getUIFactories())
    {
        static_cast<UIFactoryQt*>(f.get())->setParentWidget(this);
        tool_tab = f->createTool(tool_class_name);
        if(tool_tab != nullptr)
            break;
    }

    if(tool_tab == nullptr)
    {
        std::cerr << "QMainWindowVMTK::addToolAsDialogWindow -->> Tool " << tool_class_name << " not found!" << std::endl;
        return nullptr;
    }

    QWidget *qtool_tab = dynamic_cast<QWidget*>(tool_tab);

    if(tool_tab)
    {
        QDialogButton *dialog_button = new QDialogButton();
        dialog_button->setButtonText(button_name);
        dialog_button->setCustomWidget(qtool_tab);

        auto it = m_toolTabMap.find(tool_tab_title);
        if (it != m_toolTabMap.end())
        {
            QGridLayout* tooltab_layout = m_toolTabMap[tool_tab_title];
            tooltab_layout->addWidget(dialog_button, row, col, row_span, col_span);

        } else {

            QGridLayout *tooltab_layout = createDefaultToolTab(tool_tab_title);
            tooltab_layout->addWidget(dialog_button, row, col, row_span, col_span);

            m_toolTabMap[tool_tab_title] = tooltab_layout;
        }

    } else
        std::cerr << "QMainWindowVMTK::addTool -->> cant dynamic cast tool tab! " << std::endl;


    return tool_tab;
}


QMenu *QMainWindowVMTK::addMainMenu(std::string mainMenu)
{
    QString qstrMenuName = QString::fromStdString(mainMenu);
    QMenu *menu = ui->m_menuBar->findChild<QMenu *>(qstrMenuName);

    if(menu == nullptr){
        menu = ui->m_menuBar->addMenu(qstrMenuName);
        menu->setObjectName(qstrMenuName);
        menu->setParent(ui->m_menuBar);
    }
    return menu;
}


QMenu *QMainWindowVMTK::addSubMenu(std::string mainMenuname,std::string subMenuName ){
    QString qstrMenuName = QString::fromStdString(mainMenuname);
    QMenu *menuMain = ui->m_menuBar->findChild<QMenu *>(qstrMenuName);
    if(menuMain == nullptr){
        menuMain = new QMenu(qstrMenuName);
        menuMain->setObjectName(qstrMenuName);
        ui->m_menuBar->addMenu(menuMain);
    }
    if( !menuMain ){ return nullptr; }
    QString qstrSubMenuName = QString::fromStdString(subMenuName);
    QMenu *subMenu = menuMain->findChild<QMenu *>(qstrSubMenuName);
    if( !subMenu ){
        subMenu = menuMain->addMenu(qstrSubMenuName);
        subMenu->setObjectName(qstrSubMenuName);
    }
    return subMenu;
}


ToolBase * QMainWindowVMTK::addMenuToolbaseUniqueAddingDialog(std::string mainMenuName, std::string subMenuName, std::string tool_name, std::string ui_name)
{
    ToolBase *tool = nullptr;
    QDialog *m_dialog = new QDialog(ui->m_menuBar);

    for(auto &f: getUIFactories())
    {
        if( f.get()->getUIFactoryClassName().compare(ui_name) == 0 ){
            static_cast<UIFactoryQt*>(f.get())->setParentWidget(m_dialog);
            tool = f->createTool(tool_name);
            if(tool != nullptr)
                break;
        }
    }

    if(tool == nullptr)
    {
        std::cerr << Q_FUNC_INFO << " -->> Tool " << tool_name << " not found!" << std::endl;
        delete m_dialog;
        return nullptr;
    }

    QWidget *qtool = dynamic_cast<QWidget*>(tool);

    if(qtool)
    {
        QMenu *qmainMenu = addMainMenu(mainMenuName);
        if(qmainMenu == nullptr )
        {
            delete m_dialog;
            return nullptr;
        }

        Qt::WindowFlags flags = m_dialog->windowFlags();
        m_dialog->setWindowFlags(flags | Qt::Tool);
        QHBoxLayout *m_dialogLayout = new QHBoxLayout(m_dialog);
        m_dialogLayout->setContentsMargins(0, 0, 0, 0);
        m_dialog->setLayout(m_dialogLayout);
        m_dialogLayout->addWidget(qtool);

        QString qstrTool_name = QString::fromStdString(subMenuName);
        if(qmainMenu->actions().size() == 0 ){
            qmainMenu->addAction(qstrTool_name, m_dialog,  SLOT(show()) );
        }
        else{
            int count = 0;
            foreach (QAction* a, qmainMenu->actions())
            {
                if( a->text() == qstrTool_name){  count++; break; }
            }
            if(count == 0){
                qmainMenu->addAction(QString::fromStdString(subMenuName), m_dialog,  SLOT(show()) );
            }
        }
    } else {
        std::cerr << Q_FUNC_INFO << " -->> cant dynamic cast tool tab! " << std::endl;
        delete m_dialog;
    }
    return tool;
}

ToolBase * QMainWindowVMTK::addMenuToolbaseUnique(std::string mainMenuName, std::string tool_name, std::string ui_name)
{
    ToolBase *tool = nullptr;

    for(auto &f: getUIFactories())
    {
        if( f.get()->getUIFactoryClassName().compare(ui_name) == 0 ){
            static_cast<UIFactoryQt*>(f.get())->setParentWidget(this);
            tool = f->createTool(tool_name);
            if(tool != nullptr)
                break;
        }
    }

    if(tool == nullptr)
    {
        std::cerr << Q_FUNC_INFO << " -->> Tool " << tool_name << " not found!" << std::endl;
        return nullptr;
    }

    QWidget *qtool = dynamic_cast<QWidget*>(tool);

    if(qtool)
    {
       qtool->setParent(this);
       Qt::WindowFlags flags = qtool->windowFlags();
       qtool->setWindowFlags(flags | Qt::Dialog);
       QMenu *qmainMenu = addMainMenu(mainMenuName);
       if(qmainMenu == nullptr ){ return nullptr; }
       QString qstrTool_name = QString::fromStdString(tool_name);
       if(qmainMenu->actions().size() == 0 ){
           qmainMenu->addAction(qstrTool_name, qtool,  SLOT(show()) );
       }
       else{
           int count = 0;
           foreach (QAction* a, qmainMenu->actions())
           {
               if( a->text() == qstrTool_name){  count++; break; }
           }
           if(count == 0){
               qmainMenu->addAction(QString::fromStdString(tool_name), qtool,  SLOT(show()) );
           }
       }
    } else {
        std::cerr << Q_FUNC_INFO << " -->> cant dynamic cast tool tab! " << std::endl;
    }
    return tool;
}

ToolBase * QMainWindowVMTK::addMenuToolBaseforTools(std::string mainMenuName, std::string groupMenuToolName, std::string tool_name, std::string ui_name)
{
    ToolBase *tool = nullptr;
    for(auto &f: getUIFactories())
    {
        if( f.get()->getUIFactoryClassName().compare(ui_name) == 0 ){
            static_cast<UIFactoryQt*>(f.get())->setParentWidget(this);
            tool = f->createTool(tool_name);
            if(tool != nullptr)
                break;
        }
    }
    if(tool == nullptr)
    {
        std::cerr << Q_FUNC_INFO << "-->> Tool " << tool_name << " not found!" << std::endl;
        return nullptr;
    }

    QWidget *qtool = dynamic_cast<QWidget*>(tool);

    if(qtool)
    {
        QMenu *qSubMenu = addSubMenu(mainMenuName, groupMenuToolName);
        if(qSubMenu == nullptr ){ return nullptr; }

        QString qstrTool_name = QString::fromStdString(tool_name);
        if(qSubMenu->actions().size() == 0 ){
            qSubMenu->addAction(qstrTool_name, qtool,  SLOT(show()) );
        }
        else{
            int count = 0;
            foreach (QAction* a, qSubMenu->actions())
            {
                if( a->text() == qstrTool_name){  count++; break; }
            }
            if(count == 0){
                qSubMenu->addAction(QString::fromStdString(tool_name), qtool,  SLOT(show()) );
            }
        }
    } else {
        std::cerr << Q_FUNC_INFO << " -->> cant dynamic cast tool tab! " << std::endl;
    }
    return tool;
}

void QMainWindowVMTK::blockToolTabs(bool state)
{
    if(state)
        ui->m_tabWidgetTools->setEnabled(false);

    else
        ui->m_tabWidgetTools->setEnabled(true);
}

void QMainWindowVMTK::blockVisualizationTabs(bool state)
{
    if(state)
        ui->m_tabWidget_reg->tabBar()->setEnabled(false);

    else
        ui->m_tabWidget_reg->tabBar()->setEnabled(true);
}


QGridLayout* QMainWindowVMTK::createDefaultToolTab(std::string tool_tab_title)
{
    QWidget *widget = new QWidget(this->centralWidget());

    QHBoxLayout *widget_layout = new QHBoxLayout(widget);
    widget_layout->setContentsMargins(0, 0, 0, 0);

    QFrame *frame = new QFrame(widget);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QGridLayout *frame_layout = new QGridLayout(frame);
    frame_layout->setContentsMargins(0, 0, 0, 0);
    frame_layout->setVerticalSpacing(2);
    frame_layout->setHorizontalSpacing(2);

    widget_layout->addWidget(frame, 0, Qt::AlignLeft);

    ui->m_tabWidgetTools->addTab(widget, tool_tab_title.c_str());

    return frame_layout;
}

VisualizationTab* QMainWindowVMTK::addVisualizationTab(std::string vis_tab_class_name, std::string vis_tab_title, bool enable_check_box)
{
    m_guiFactory->setParent(ui->m_tabWidget_reg);
    VisualizationTab *vis_tab = m_guiFactory->createVisualizationTab(vis_tab_class_name);
    QWidget *qvis_tab = dynamic_cast<QWidget*>(vis_tab);
    if(qvis_tab)
    {
        qvis_tab->setParent(this);
        ui->m_tabWidget_reg->addTab(qvis_tab, vis_tab_title.c_str(), enable_check_box);

    } else
        std::cerr << "QMainWindowVMTK::addVisualizationTab -->> cant dynamic cast visualization tab!" << std::endl;

    return vis_tab;
}

unsigned short QMainWindowVMTK::getNumSelectedTabs()
{
    return ui->m_tabWidget_reg->countTabsWithCheck();
}

std::vector<unsigned short> QMainWindowVMTK::getIndexSelectedTabs()
{
    return ui->m_tabWidget_reg->listIDTabsWithCheck();
}

void QMainWindowVMTK::showMessageBox(std::string title, std::string msg)
{
    QMessageBox::information(this, QString::fromStdString(title), QString::fromStdString(msg));
}

bool QMainWindowVMTK::showQuestionMessageBox(std::string title, std::string msg)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, title.c_str(), msg.c_str(), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        return true;
    else
        return false; //qDebug() << "Yes was *not* clicked";
}

void QMainWindowVMTK::uncheckAllVisualizationTab()
{
    ui->m_tabWidget_reg->uncheckAllTab();
}

void QMainWindowVMTK::on_m_tabWidget_reg_currentChanged(int index)
{
    MainWindow::currentVisualizationTabChanged(index);
}

void QMainWindowVMTK::tabCheckBoxToggled(unsigned short tab_index, bool checked)
{
    notifyTabCheckBoxTooggled(tab_index, checked);
}

void QMainWindowVMTK::changeOriginalQssStyle()
{
      QFile File(":/original_2");
      File.open(QFile::ReadOnly);
      QString styleSheet = QLatin1String(File.readAll());
      this->setStyleSheet(styleSheet);
      QFile File_tab(":/original_tab_widget");
      File_tab.open(QFile::ReadOnly);
      styleSheet = QLatin1String(File_tab.readAll());
      ui->m_tabWidgetTools->setStyleSheet(styleSheet);
      ui->m_tabWidget_reg->setStyleSheet("background-color: rgb(76, 76, 76);");
      ui->m_frame9_2->setStyleSheet("background-color: QLinearGradient(x1:0, y1:0, x2:0, y2:1, stop:0 #616161, stop: 0.5 #505050, stop: 0.6 #434343, stop:1 #656565);");
}

void QMainWindowVMTK::changeLightBlueQssStyle()
{
      QFile File(":/light_blue");
      File.open(QFile::ReadOnly);
      QString styleSheet = QLatin1String(File.readAll());
      this->setStyleSheet(styleSheet);
      QFile File_tab(":/light_blue_tab_widget");
      File_tab.open(QFile::ReadOnly);
      styleSheet = QLatin1String(File_tab.readAll());
      ui->m_tabWidgetTools->setStyleSheet(styleSheet);
      ui->m_tabWidget_reg->setStyleSheet("background-color: rgb(229, 229, 229);");
      ui->m_frame9_2->setStyleSheet("QLinearGradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ccebff, stop: 0.5 #ffffff, stop: 0.6 #ffffff, stop:1 #ccebff);");
}

void QMainWindowVMTK::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    MainWindow::handleCloseEvent();
    event->accept();
}

void QMainWindowVMTK::on_m_tabWidgetTools_currentChanged(int index)
{
    if(index==-1){return;}
    std::string class_name = "";
    class_name = ui->m_tabWidgetTools->widget(index)->objectName().toStdString();
    MainWindow::currentToolTabChanged(index, class_name);
}

void QMainWindowVMTK::visualuzationTabClosed(int tab_index)
{
    MainWindow::visualizationTabClosed(tab_index);
    delete ui->m_tabWidget_reg->widget(tab_index);
}
