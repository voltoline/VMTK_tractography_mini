#include "OpenGLVisualizationTab.h"

#include "QMainWindowVMTK.h"
#include "ui/QFrameContentWindow.h"
#include "ui/QWidgetOpenGL.h"
#include "renderer/vmtkAbstractRender.h"
#ifdef enableOldQGL
#include <QGLFormat>
#include <QGLWidget>
#else
#include <QSurfaceFormat>
#include <QOpenGLWidget>
#endif

#ifdef enableOldQGL
QGLWidget *OpenGLVisualizationTab::mSharedWidget = nullptr;
QGLFormat OpenGLVisualizationTab::mGLFormat;
#else
QOpenGLWidget *OpenGLVisualizationTab::mSharedWidget = nullptr;
QSurfaceFormat OpenGLVisualizationTab::mGLFormat;
#endif

bool OpenGLVisualizationTab::mFlagInitSharedWidget = false;

OpenGLVisualizationTab::OpenGLVisualizationTab(QWidget *parent):
    QWidget(parent),
    VisualizationTab("OpenGLVisualizationTab"),
    m_renderingSurfaceSet(nullptr),
    m_flagHasOpenGL(false),
    m_gridLayout(nullptr)
{
    m_parent = parent;
#ifdef enableOldQGL
    if (!QGLFormat::hasOpenGL()) /** OpenGl support or not? */
    {
        std::cout << "This system has no OpenGL support" << std::endl;
        return;
    }
#else
    QOpenGLVersionProfile vp =QOpenGLVersionProfile(mGLFormat);
    if (!vp.isValid()) /** OpenGl support or not? */
    {
        std::cout << "This system has no OpenGL support" << std::endl;
        return;
    }
#endif

    m_flagHasOpenGL = true;
    std::cout << "This system has OpenGL support" << std::endl;

    if(!mFlagInitSharedWidget)
    {
        mFlagInitSharedWidget = true;

        QCheckingOpenGL::checkingCurrentSystemProfile();
        QCheckingOpenGL::settingGLFormat(mGLFormat);
        QCheckingOpenGL::checkingProfile(mGLFormat);

#ifdef enableOldQGL
        mSharedWidget = new QGLWidget(mGLFormat);
#else
        mSharedWidget = new QOpenGLWidget();
        mSharedWidget->context()->setFormat(mGLFormat);
        mSharedWidget->context()->create();
#endif
        mSharedWidget->setDisabled(true);
    }

    m_renderingSurfaceSet = new QFrameContentWindow(this);

    m_gridLayout = new QGridLayout(this);
    m_gridLayout->addWidget(m_renderingSurfaceSet);
    m_gridLayout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(m_gridLayout);
}

OpenGLVisualizationTab::~OpenGLVisualizationTab()
{
    for(unsigned short i = 0; i < m_openGLWidgets.size(); i++)
        delete m_openGLWidgets[i];

    if(m_renderingSurfaceSet != nullptr)
        delete m_renderingSurfaceSet;

    if(m_gridLayout != nullptr)
        delete m_gridLayout;
}

RenderingSurface *OpenGLVisualizationTab::createRenderingSurface(vmtkAbstractRender *render, std::function<void(RenderingSurface*)> on_initialization, std::string title, bool enable_debug, std::string debug_msg)
{
    if(m_flagHasOpenGL)
    {
        QWidgetOpenGL *widget = m_renderingSurfaceSet->addDockWidgetOpenGL(render, on_initialization, mGLFormat, mSharedWidget, enable_debug, debug_msg, title);

        m_openGLWidgets.push_back(widget);
        return static_cast<RenderingSurface*>(widget);
    }

    return nullptr;
}
