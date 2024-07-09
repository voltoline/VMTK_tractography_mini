#include "QToolTabManager.h"

//#include "ui/components/QTabFile.h"
//#include "ui/components/QTab3DTools.h"
//#include "ui/components/QTab2DTools.h"
//#include "ui/components/QTabColorManagerTools.h"
//#include "ui/components/QTabRegister.h"
//#include "ui/components/QTabDiffusion.h"
//#include "ui/components/QTabROI.h"
//#include "ui/components/QTabDTITractography.h"
//#include "ui/QMainWindowVMTK.h"

//QToolTabManager::QToolTabManager(QMainWindowVMTK *parent):
//    ToolTabManager(parent)
//{

//}

//void QToolTabManager::setTabWidget(QTabWidget *tabWidget)
//{
//    mPtrTabWidget = tabWidget;
//}

//void QToolTabManager::setRegister(bool state)
//{
//    mPtrQToolTab3D->enableComponentsSliderSpinnerBox(state);
//}

//QTabFile *QToolTabManager::getQTabFile() const
//{
//    return mQTabFile;
//}

//void QToolTabManager::init(QMainWindowVMTK *parent = 0)
//{
//    mQTabFile = new QTabFile(parent);
//    mPtrTabWidget->addTab(mQTabFile, "File");

//    mPtrQToolTab3D = new QTab3DTools(parent);
//    mPtrTabWidget->addTab(mPtrQToolTab3D, "3D Tools");
//    mPluginToolTabRegister.push_back(PluginToolTabInfo(mPtrQToolTab3D, mPtrQToolTab3D->getPluginName()));

//    mPtrQToolTab2D = new QTab2DTools(parent);
//    mPtrTabWidget->addTab(mPtrQToolTab2D, "2D Tools");

//    mQTabColorManagerTools = new QTabColorManagerTools(parent);
//    mPtrTabWidget->addTab(mQTabColorManagerTools, "Color Manager");

//    mQtabRegister = new QTabRegister(parent);
//    mPtrTabWidget->addTab(mQtabRegister, "Register");

//    mQtabDiffusion = new QTabDiffusion(parent);
//    mPtrTabWidget->addTab(mQtabDiffusion, "Diffusion");

//    mQtabDTITractography = new QTabDTITractography(parent);
//    mPtrTabWidget->addTab(mQtabDTITractography, "DTI Tractography");

//    mQTabROI = new QTabROI(parent);
//    mPtrTabWidget->addTab(mQTabROI, "ROI");

//    m_tabMisc = new QTabMisc(parent);
//    mPtrTabWidget->addTab(m_tabMisc, "Misc.");

//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQTabFile));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mPtrQToolTab3D));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mPtrQToolTab2D));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQTabColorManagerTools));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQtabDiffusion));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQtabDTITractography));

//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQtabRegister));
//    registerToolTab(static_cast<vmtk::BaseToolTab*>(mQTabROI));
//}
