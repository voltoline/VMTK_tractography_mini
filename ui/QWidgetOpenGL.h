/**
  * @file QWidgetOpenGL.h
  * @brief manages the QGLWindget widget and its rendering modes.
  * @author José Angel Ivan Rubianes Silva, Raphael Voltoline and Wallace Souze Loos
  * @date March 2015
  * @note This code is adapted from the code available at https://www.cg.tuwien.ac.at/courses/Visualisierung/2009-2010/Beispiel1/Thaler_Windisch/.
  */
#pragma once

#ifndef QWIDGET_OPENGL_H
#define QWIDGET_OPENGL_H

#include "utilities/QCheckingOpenGL.h"
#include "renderer/vmtkAbstractRender.h"
#include "RenderingSurface.h"
#ifdef enableOldQGL
#include <QGLFormat>
#else
#include <QSurfaceFormat>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#endif
#include <QMouseEvent>

//#define OPENGL_DEBUG
//#define ENABLE_DOUBLE_CLICK

#ifdef OPENGL_DEBUG
class QOpenGLDebugMessage;
class QOpenGLDebugLogger;
#endif



/**
 * @brief The QWidgetOpenGL class
 * @details this class represents a QGLWidget that is used for displaying results using OpenGL.
 */
#ifdef enableOldQGL
class QWidgetOpenGL : public QGLWidget, public RenderingSurface
#else
class  QWidgetOpenGL : public QOpenGLWidget, public RenderingSurface
#endif
{
    Q_OBJECT         // declares that the class should use signals and slots
public:
    typedef std::shared_ptr<QWidgetOpenGL> sPtr;

    /**
     * @brief QWidgetOpenGL constructor.
     * @param[in] format format of the contained GL rendering context
     * @param[in] parent parent window
     * @return pointer to the created vmtkOpenGL
     */
#ifdef enableOldQGL
    QWidgetOpenGL(vmtkAbstractRender *render, std::function<void(RenderingSurface *)> on_initialization, const QGLFormat& format, QWidget *parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
#else
    QWidgetOpenGL( vmtkAbstractRender *render, ControllerRenderingSurface *controller, const QSurfaceFormat& format, QWidget *parent = 0, const QOpenGLWidget * shareWidget = nullptr, Qt::WindowFlags f = Qt::Widget);
#endif
    /**
     * @brief QWidgetOpenGL destructor
     */
    virtual ~QWidgetOpenGL();

    void enableDebug(bool enable, std::string debug_msg = "");
    void setRenderingContext();
    void createFocusBorder();
    void removeFocusBorder();
    void saveImage(bool enable_streaming = false);
    void updateRendering();
    void setAutoUpdateEnabled(bool enabled);
    void setCursorEnum(int idCursorEnum);

protected:

    void initializeGlew();

    /**
     * @brief initializes OpenGL context. Both SliceRenderer:: and RayTracedRenderer:: renders are instantiated.
     */
    void initializeGL();

    /**
    * @brief This methods gets called on every resize and the first time the OpenGL widget is created.
    * The method takes care of recalculating aspect ration and perspective or orthographic projection.
    * @param[in] w the width of the new QGLWidget
    * @param[in] h the height of the new QGLWidget
    */
    void resizeGL(int w, int h);

    /**
    * @brief This methods gets called on every resize and the first time the OpenGL widget is created.
    * The method takes care of recalculating aspect ration and perspective or orthographic projection.
    */
    void paintGL();

    void mousePressEvent(QMouseEvent *ev_mouse);
    void mouseMoveEvent(QMouseEvent *ev_mouse);
    void mouseReleaseEvent(QMouseEvent *ev_mouse);

    bool init_web_channel;

private:

    void initDebug();

#ifdef ENABLE_DOUBLE_CLICK
    struct mouse_click_state
    {
        bool m_firstPress;
        bool m_firstMove;
        bool m_firstRelease;

        mouse_click_state()
        {
            reset();
        }

        void reset()
        {
            m_firstPress = false;
            m_firstMove = false;
            m_firstRelease = false;
        }

    } m_mouseClickStates;

    bool m_singleClicked;
    bool m_timerCallExecuted;
    bool m_doubleClickState;

    MouseEvent * firstClickEvent;
    MouseEvent * secondClickEvent;
#endif

    float m_pixelRatio;

    static bool m_Glew_Was_Init;

    QString m_debugMsg;
    bool m_enableDebug;
    bool m_autoUpdateEnabled;


private slots:
#ifdef ENABLE_DOUBLE_CLICK
    void mousePressTimer();
#endif
    void screenChanged(QScreen*);

signals:
    void updateRenderingImage(QString rendering_image);

#ifdef OPENGL_DEBUG
    QOpenGLDebugLogger *m_debugLogger;

private slots:
    void messageLogged(const QOpenGLDebugMessage &msg);
#endif
};

#endif // QWIDGET_OPENGL_H
