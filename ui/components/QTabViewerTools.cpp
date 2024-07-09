//#include "ui/components/QTabViewerTools.h"

//#include<QButtonGroup>
//#include "../QMainWindowVMTK.h"
//#include "QTab3DTools.h"

//QTabViewerTools::QTabViewerTools(QMainWindowVMTK *parent) :
//    QWidget(parent),
//    ui(new Ui::QTabViewerTools)
//{
//    ui->setupUi(this);
//    m_mainWindow = parent;
//}

//void QTabViewerTools::createNewTabWidget()
//{
//    int countNotReg = m_mainWindow->counterNotRegister() - 1;

//    connect(ui->m_comboBoxFBOSize, SIGNAL(activated(int)), m_mainWindow->OpenGLWidget1[countNotReg], SLOT(setFBOSize(int)));
//    connect(ui->m_comboBoxSteps, SIGNAL(currentIndexChanged(int)), m_mainWindow->OpenGLWidget1[countNotReg], SLOT(setSteps(int)));
//}

//void QTabViewerTools::enableComponents(bool state)
//{
//    /** Orientation buttons (Voxel-order) **/
//    ui->m_radioButtonRadiological->setEnabled( state);
//    ui->m_radioButtonNeurological->setEnabled( state);
//}

//QTabViewerTools::~QTabViewerTools()
//{
//    delete ui;
//}
