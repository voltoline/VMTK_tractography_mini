#include "QWidgetOpenGL.h"

#include <QBuffer>
#include <QFileDialog>
#include <QTimer>
#include <QApplication>

#ifdef enableOldQGL
#include <QDesktopWidget>
#endif

#include <QWindow>

#ifdef OPENGL_DEBUG
#include <QOpenGLDebugLogger>
#endif

#ifdef _CREATE_WEB_SERVER
    #include "web/QWebSocketServerThread.h"
#endif


bool QWidgetOpenGL::m_Glew_Was_Init = false;
#ifdef enableOldQGL
QWidgetOpenGL::QWidgetOpenGL(vmtkAbstractRender *render, std::function<void(RenderingSurface*)> on_initialization, const QGLFormat& format, QWidget *parent, const QGLWidget * shareWidget, Qt::WindowFlags f):
    QGLWidget(format, parent, shareWidget, f),
    RenderingSurface(render, on_initialization)
{
#else
QWidgetOpenGL::QWidgetOpenGL( vmtkAbstractRender *render, ControllerRenderingSurface *controller, const QSurfaceFormat &format, QWidget *parent, const QOpenGLWidget *shareWidget, Qt::WindowFlags f):
    QOpenGLWidget(parent, f),
    RenderingSurface(render, controller)
{
#endif
    m_autoUpdateEnabled = false;
    m_enableDebug = false;

    init_web_channel = false;

    QString borderNotFocus ="QWidget { border: 2px solid #555555;}";
    parentWidget()->parentWidget()->setStyleSheet(borderNotFocus);

#ifndef enableOldQGL
//    QOpenGLContext* c = context();
    context()->setFormat(format);
    context()->setShareContext(shareWidget->context());
    context()->create();
#endif

#ifdef ENABLE_DOUBLE_CLICK
    m_singleClicked = true;

    m_timerCallExecuted = true;
    m_doubleClickState = false;
    firstClickEvent = new MouseEvent(0, 0, MouseEvent::MouseButton::LEFT);
    secondClickEvent = new MouseEvent(0, 0, MouseEvent::MouseButton::LEFT, MouseEvent::Type::MOUSE_DOUBLE_CLICK);
#endif

    m_pixelRatio = static_cast<float>(devicePixelRatioF());
    updatePixelRatio(m_pixelRatio);

    connect(windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(screenChanged(QScreen*)));

#ifdef OPENGL_DEBUG
    m_debugLogger = nullptr;
#endif
}

// DESTRUCTOR
QWidgetOpenGL::~QWidgetOpenGL()
{
#ifdef OPENGL_DEBUG
    if(m_debugLogger != nullptr)
    {
        makeCurrent();
        delete m_debugLogger;
    }
#endif
}

void QWidgetOpenGL::enableDebug(bool enable,  std::string debug_msg)
{
    m_enableDebug = enable;
    m_debugMsg = QString(debug_msg.c_str());

    if(!debug_msg.empty())
        m_debugMsg += QString("\n");
}
void QWidgetOpenGL::initDebug()
{
#ifdef OPENGL_DEBUG
    m_debugLogger = Q_NULLPTR;
    m_debugLogger = new QOpenGLDebugLogger(this);
    if (m_debugLogger->initialize())
    {
        //    qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger << "\n";
        std::cout << "GL_DEBUG Debug Logger " << m_debugLogger << std::endl;
        connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
        m_debugLogger->startLogging();
    }
#endif
}

void QWidgetOpenGL::setRenderingContext()
{
    makeCurrent();
}

#ifdef OPENGL_DEBUG
void QWidgetOpenGL::messageLogged(const QOpenGLDebugMessage &msg)
{
    QString error;
    error += m_debugMsg;

    // Format based on severity
    switch (msg.severity())
    {
    case QOpenGLDebugMessage::NotificationSeverity:
//        return;
        error += "--";
        break;

    case QOpenGLDebugMessage::HighSeverity:
        error += "!!";
        break;

    case QOpenGLDebugMessage::MediumSeverity:
        error += "!~";
        break;

    case QOpenGLDebugMessage::LowSeverity:
        error += "~~";
        break;
    }

    error += " (";

    // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break

    switch (msg.source())
    {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
    }

//#undef CASE

    error += " : ";

    // Format based on type
//#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break

    switch (msg.type())
    {
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
    }

#undef CASE

    error += ")";
    //  qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
    std::cout << qPrintable(error) << "\n" << qPrintable(msg.message()) << std::endl;
}
#endif

void QWidgetOpenGL::createFocusBorder()
{
    QString borderFocus ="QWidget { border: 2px solid QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);}";
    parentWidget()->parentWidget()->setStyleSheet(borderFocus);
}

void QWidgetOpenGL::removeFocusBorder()
{
    QString borderNotFocus ="QWidget { border: 2px solid #555555;}";
    parentWidget()->parentWidget()->setStyleSheet(borderNotFocus);
}

void QWidgetOpenGL::saveImage(bool enable_streaming)
{
    if(enable_streaming)
    {
#ifdef _CREATE_WEB_SERVER
        if(!init_web_channel)
        {
            init_web_channel = true;
            QWebSocketServerThread::getWebChannel()->registerObject(QStringLiteral("rendering_surface_3d"), this);
        }

        QImage image = grabFrameBuffer();

        QByteArray ba;
        QBuffer bu(&ba);
        image.save(&bu, "PNG");
        QString imgBase64 = ba.toBase64(QByteArray::Base64Encoding);
        emit updateRenderingImage(imgBase64);
#endif

    } else {

        QFileDialog *saveImageDialog = new QFileDialog(this);
        saveImageDialog->setFileMode(QFileDialog::AnyFile);
        saveImageDialog->setAcceptMode(QFileDialog::AcceptSave);
#if __APPLE__
        saveImageDialog->setWindowModality( Qt::WindowModal ); // precisa colocar pelo que não é compativel com as janelas nativas do MAC OS
#endif
        QString filename = saveImageDialog->getSaveFileName(this, "Save image", "", "Image Files (*.png *.jpg *.bmp)");
#ifdef enableOldQGL
        QImage image = grabFrameBuffer();
#else
        QImage image = grabFramebuffer();
#endif
        image.save(filename);

        delete saveImageDialog;
    }

}

// gets called only one time
void QWidgetOpenGL::initializeGL()
{
    if(m_enableDebug)
        initDebug();

    makeCurrent();
    doInitializeGL();
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

// resizes the viewport
void QWidgetOpenGL::resizeGL( int w, int h )
{
    makeCurrent();
    doResizeGL(w, h);
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

// repaints the content of QGLWidget
void QWidgetOpenGL::paintGL()
{
    makeCurrent();
    doPaintGL();

    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    if(m_autoUpdateEnabled)
        update();

    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void QWidgetOpenGL::updateRendering()
{
    makeCurrent();
    update();
}

void QWidgetOpenGL::setAutoUpdateEnabled(bool enabled)
{
    m_autoUpdateEnabled = enabled;
}

void QWidgetOpenGL::setCursorEnum(int idCursorEnum)
{
    setCursor(static_cast<Qt::CursorShape>(idCursorEnum));
}

void QWidgetOpenGL::mousePressEvent(QMouseEvent *ev_mouse)
{
    int pos_x = ev_mouse->pos().x();
    int pos_y = ev_mouse->pos().y();

    pos_x = pos_x * m_pixelRatio;
    pos_y = pos_y * m_pixelRatio;

#ifdef ENABLE_DOUBLE_CLICK
    if (ev_mouse->type() == QEvent::MouseButtonPress)
    {
//        std::cout << "before Clicked" << std::endl;

        if(m_timerCallExecuted)
            m_timerCallExecuted = false;
        else
            return;

        m_singleClicked = true;

//        std::cout << "after Clicked" << std::endl;

        m_mouseClickStates.reset();

        if(ev_mouse->button() == Qt::LeftButton)
        {
            MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::LEFT);
            *firstClickEvent = mouse_event;

        } else if(ev_mouse->button() == Qt::RightButton) {

            MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::RIGHT);
            *firstClickEvent = mouse_event;
        }

        QTimer::singleShot(500, this, SLOT(mousePressTimer()));

    } else if (ev_mouse->type() == QEvent::MouseButtonDblClick) {

        m_singleClicked = false;

        if(m_mouseClickStates.m_firstPress)
            return;

        if(ev_mouse->button() == Qt::LeftButton)
        {
            MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::LEFT, MouseEvent::Type::MOUSE_DOUBLE_CLICK);
            *secondClickEvent = mouse_event;

        } else if(ev_mouse->button() == Qt::RightButton) {

            MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::RIGHT, MouseEvent::Type::MOUSE_DOUBLE_CLICK);
            *secondClickEvent = mouse_event;
        }

        handleMouseDoubleClickEvent(*secondClickEvent);

        m_doubleClickState = true;
    }
#else
    makeCurrent();

    if(ev_mouse->button() == Qt::LeftButton)
    {
        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::LEFT);
        handleMousePressEvent(mouse_event);

    } else if(ev_mouse->button() == Qt::RightButton) {

        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::RIGHT);
        handleMousePressEvent(mouse_event);
    }

#endif
}

#ifdef ENABLE_DOUBLE_CLICK
void QWidgetOpenGL::mousePressTimer()
{
    makeCurrent();

    if(m_timerCallExecuted)
        return;

    m_timerCallExecuted = true;

    if(m_singleClicked)
    {
        m_mouseClickStates.m_firstPress = true;

//        std::cout << "Single Clicked" << std::endl;

        handleMousePressEvent(*firstClickEvent);

        if(m_mouseClickStates.m_firstRelease == true)
        {
//            std::cout << "mouse release" << std::endl;

            handleMouseReleaseEvent(*firstClickEvent);

            m_mouseClickStates.reset();
        }
    }
}
#endif

void QWidgetOpenGL::screenChanged(QScreen *)
{
    m_pixelRatio = static_cast<float>(devicePixelRatioF());
    updatePixelRatio(m_pixelRatio);
}

void QWidgetOpenGL::mouseMoveEvent(QMouseEvent *ev_mouse)
{
#ifdef ENABLE_DOUBLE_CLICK
    if(m_doubleClickState)
        return;

    if(m_singleClicked && !m_timerCallExecuted)
    {
        m_mouseClickStates.m_firstRelease = false;
        mousePressTimer();
    }

    m_mouseClickStates.m_firstMove = true;
#endif

    makeCurrent();

//    std::cout << "mouse move" << std::endl;

    int pos_x = ev_mouse->pos().x();
    int pos_y = ev_mouse->pos().y();

    pos_x = pos_x * m_pixelRatio;
    pos_y = pos_y * m_pixelRatio;

    if(ev_mouse->buttons() == Qt::LeftButton)
    {
        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::LEFT);
        handleMouseMoveEvent(mouse_event);

    } else if(ev_mouse->buttons() == Qt::RightButton) {

        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::RIGHT);
        handleMouseMoveEvent(mouse_event);
    }
}

void QWidgetOpenGL::mouseReleaseEvent(QMouseEvent *ev_mouse)
{
#ifdef ENABLE_DOUBLE_CLICK
    if(m_doubleClickState)
    {
        m_doubleClickState = false;
        return;
    }

    if(m_mouseClickStates.m_firstPress == false)
    {
        m_mouseClickStates.m_firstRelease = true;
        return;
    }
#endif

    makeCurrent();

    int pos_x = ev_mouse->pos().x();
    int pos_y = ev_mouse->pos().y();

    pos_x = pos_x * m_pixelRatio;
    pos_y = pos_y * m_pixelRatio;

    if(ev_mouse->button() == Qt::LeftButton)
    {
        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::LEFT);
        handleMouseReleaseEvent(mouse_event);

    } else if(ev_mouse->button() == Qt::RightButton) {

        MouseEvent mouse_event(pos_x, pos_y, MouseEvent::MouseButton::RIGHT);
        handleMouseReleaseEvent(mouse_event);

    } else {

        MouseEvent mouse_event(pos_x, pos_y);
        handleMouseReleaseEvent(mouse_event);
    }
}

void QWidgetOpenGL::initializeGlew()
{
    if(!m_Glew_Was_Init)
    {
        m_Glew_Was_Init = true;
        QCheckingOpenGL::initializeGlew(this->format());
    }
}
