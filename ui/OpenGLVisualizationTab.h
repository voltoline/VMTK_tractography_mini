#ifndef OPENGL_VISUALIZATION_TAB_H
#define OPENGL_VISUALIZATION_TAB_H

#include "utilities/QCheckingOpenGL.h"
#include "core/VisualizationTab.h"
#include "RenderingSurface.h"
#include <QWidget>
#include <memory>
#ifndef enableOldQGL
#include <QOpenGLVersionProfile>
#endif

class QFrameContentWindow;
class QWidgetOpenGL;
class vmtkAbstractRender;



#ifdef enableOldQGL
class QGLWidget;
class QGLFormat;
#else
class QOpenGLWidget;
class QSurfaceFormat;
#endif

class QMainWindowVMTK;
class QGridLayout;

class OpenGLVisualizationTab: public QWidget, public VisualizationTab
{
public:
    OpenGLVisualizationTab(QWidget *parent);
    ~OpenGLVisualizationTab();

    RenderingSurface* createRenderingSurface(vmtkAbstractRender *render, std::function<void(RenderingSurface*)> on_shader_initialization, std::string title = "", bool enable_debug = false, std::string debug_msg = "");

private:
    QWidget *m_parent;
    QFrameContentWindow *m_renderingSurfaceSet;
    QList<QWidgetOpenGL *> m_openGLWidgets;
    bool m_flagHasOpenGL;
    QGridLayout *m_gridLayout;

    static bool mFlagInitSharedWidget;


#ifdef enableOldQGL
static QGLWidget *mSharedWidget;
static QGLFormat mGLFormat;
#else
static QOpenGLWidget *mSharedWidget;
static QSurfaceFormat mGLFormat;
#endif

};

#endif // OPENGL_VISUALIZATION_TAB_H
