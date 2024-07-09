#include "QFrameContentWindow.h"

#include "ui/QWidgetOpenGL.h"
#include <QDockWidget>
#include <QLabel>

QFrameContentWindow::QFrameContentWindow(QWidget *parent):
    QFrame(parent)
{
    m_maxAddWidget = 6;
    m_counter = 0;

    m_splitter1 = nullptr;
    m_splitter2 = nullptr;
    m_splitter3 = nullptr;
    m_splitterGridLayout = nullptr;

    m_subMainWindow = nullptr;
    m_gridLayoutSubWindow = nullptr;

    m_parent = parent;
    m_minimumSizeFixed = false;

    if(m_subMainWindow == nullptr)
    {
        m_gridLayoutSubWindow = new QGridLayout(this);
        m_gridLayoutSubWindow->setSpacing(1);
        m_gridLayoutSubWindow->setContentsMargins(1, 1, 1, 1);

        m_subMainWindow = new QMainWindow(this);
        m_subMainWindow->setWindowFlags(Qt::Widget);
        m_subMainWindow->setDockNestingEnabled(true);
        m_subMainWindow->setCentralWidget(new QWidget(m_subMainWindow));
        m_subMainWindow->centralWidget()->hide();

        QString style_window("QMainWindow::separator {  height: 4px; }");
        m_subMainWindow->setStyleSheet(style_window);

        m_gridLayoutSubWindow->addWidget(m_subMainWindow);
        this->setLayout(m_gridLayoutSubWindow);
    }
}

QFrameContentWindow::~QFrameContentWindow()
{
    if(m_subMainWindow != nullptr)
        delete m_subMainWindow;

    if(m_gridLayoutSubWindow != nullptr)
        delete m_gridLayoutSubWindow;

    if(m_splitterGridLayout != nullptr)
        delete m_splitterGridLayout;
}

QWidgetOpenGL* QFrameContentWindow::addDockWidgetOpenGL(vmtkAbstractRender *render, std::function<void(RenderingSurface *)> on_initialization, const QGLFormat& format, const QGLWidget * shareWidget, bool enable_debug, std::string debug_msg, std::string title)
{
    QDockWidget *dock = new QDockWidget(title.c_str(), m_subMainWindow);
    m_dockWidgetVector.push_back(dock);

    QFrame *frame = new QFrame(dock);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    frame->setMinimumSize(75, 75);

    m_dockWidgetFrame.push_back(frame);

    QVBoxLayout *box_layout = new QVBoxLayout(frame);

    DoubleClickEventFilter *double_click_event_filter = new DoubleClickEventFilter(dock);
    dock->installEventFilter(double_click_event_filter);

    QWidgetOpenGL *widget = new QWidgetOpenGL(render, on_initialization, format, frame, shareWidget);
    widget->enableDebug(enable_debug, debug_msg);

    box_layout->addWidget(widget);
    box_layout->setContentsMargins(0, 0, 0, 0);

    frame->setLayout(box_layout);

    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(frame);

    float window_height = m_subMainWindow->size().height();

    if(m_counter == 0) {
        m_subMainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);

    } if(m_counter == 1) {
        m_subMainWindow->addDockWidget(Qt::RightDockWidgetArea, dock);

    } if(m_counter == 2) {
        int dock_height = 0.45 * window_height;

        m_dockWidgetFrame[1]->setMinimumSize(75, dock_height);
        m_dockWidgetFrame[2]->setMinimumSize(75, dock_height);

        m_subMainWindow->addDockWidget(Qt::RightDockWidgetArea, dock);

    } if(m_counter == 3) {

        int dock_height = 0.28 * window_height;

        m_dockWidgetFrame[1]->setMinimumSize(75, dock_height);
        m_dockWidgetFrame[2]->setMinimumSize(75, dock_height);
        m_dockWidgetFrame[3]->setMinimumSize(75, dock_height);

        m_subMainWindow->addDockWidget(Qt::RightDockWidgetArea, dock);
    }

    m_counter++;

    return widget;
}

void QFrameContentWindow::resizeEvent(QResizeEvent* event)
{
   QFrame::resizeEvent(event);

   if(m_minimumSizeFixed == false and m_dockWidgetFrame.size() > 0)
   {
       std::cout << "fixing dock minimum size " << std::endl;

       for(unsigned int i = 0; i < m_dockWidgetFrame.size(); i++)
           m_dockWidgetFrame[i]->setMinimumSize(75, 75);

       m_minimumSizeFixed = true;
   }
}

bool QFrameContentWindow::addWidgetOpenGL(QWidget *widget, std::string )
{
    if(m_splitter3 == nullptr)
        createSplitters();

    if(m_counter >= m_maxAddWidget)
        return false;

    m_layout[m_counter] = new QVBoxLayout;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_layout[m_counter]->setMargin(1);
#else
    m_layout[m_counter]->setContentsMargins(1, 1, 1, 1);
#endif

    m_layout[m_counter]->addWidget(widget);

    if(m_counter==0 || m_counter==1 || m_counter==4)
    {
        m_frame[m_counter] = new QFrame(m_splitter1);
        m_frame[m_counter]->setLayout(m_layout[m_counter]);
        m_splitter1->addWidget(m_frame[m_counter]);
    }

    if(m_counter==2 || m_counter==3 || m_counter==5)
    {
        m_frame[m_counter] = new QFrame(m_splitter2);
        m_frame[m_counter]->setLayout(m_layout[m_counter]);
        m_splitter2->addWidget(m_frame[m_counter]);
    }
    m_counter++;

    return true;
}

void QFrameContentWindow::createSplitters()
{
    m_splitterGridLayout = new QGridLayout(this);
    m_splitterGridLayout->setSpacing(1);
    m_splitterGridLayout->setContentsMargins(1, 1, 1, 1);

    m_splitter3 = new QSplitter(this);
    m_splitter3->setStyleSheet(QLatin1String("QSplitter::handle {\nimage:url(:/splitter_h)\n}"));
    m_splitter3->setOrientation(Qt::Vertical);

    m_splitter1 = new QSplitter(m_splitter3);
    m_splitter1->setStyleSheet(QLatin1String("QSplitter::handle {\nimage:url(:/splitter_v)\n}"));

    m_splitter3->addWidget(m_splitter1);//divisão parte de acima

    m_splitter2 = new QSplitter(m_splitter3);
    m_splitter2->setStyleSheet(QLatin1String("QSplitter::handle {\nimage:url(:/splitter_v)\n}"));

    m_splitter3->addWidget(m_splitter2);//divisão parte de abaixo

    m_splitterGridLayout->addWidget(m_splitter3);
}
