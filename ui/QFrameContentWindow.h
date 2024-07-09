#ifndef QFRAMECONTENTWINDOW_H
#define QFRAMECONTENTWINDOW_H

#include <QWidget>
#include <QFrame>
#include <QSplitter>
#include <QGridLayout>
#include <vector>
#include <QMainWindow>
#include <QEvent>
#include <functional>

class QDockWidget;
class vmtkAbstractRender;
class RenderingSurface;
class QGLFormat;
class QGLWidget;
class QWidgetOpenGL;

class QFrameContentWindow : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameContentWindow(QWidget *parent = 0);
    ~QFrameContentWindow();
    bool addWidgetOpenGL(QWidget *widget, std::string title = "");
    QWidgetOpenGL *addDockWidgetOpenGL(vmtkAbstractRender *render, std::function<void(RenderingSurface *)> on_initialization, const QGLFormat &format, const QGLWidget *shareWidget, bool enable_debug, std::string debug_msg, std::string title = "");

protected:
    void resizeEvent(QResizeEvent* event);

private:
    void createSplitters();

    std::vector<QDockWidget*> m_dockWidgetVector;
    std::vector<QFrame*> m_dockWidgetFrame;

    QFrame *m_frame[6] = {nullptr};
    QSplitter *m_splitter1, *m_splitter2, *m_splitter3;
    QVBoxLayout* m_layout[6] = {nullptr};
    QGridLayout *m_splitterGridLayout;

    int m_counter;
    int m_maxAddWidget;

    QMainWindow *m_subMainWindow;
    QGridLayout *m_gridLayoutSubWindow;

    bool m_minimumSizeFixed;

    QWidget *m_parent;
};

class DoubleClickEventFilter: public QObject
{
    Q_OBJECT

public:
    DoubleClickEventFilter(QObject *parent): QObject(parent){ }
    ~DoubleClickEventFilter() { }

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
            return true;
        else
            return QObject::eventFilter(obj, event);
    }
};

#endif // QFRAMECONTENTWINDOW_H
