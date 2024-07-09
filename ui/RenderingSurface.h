#ifndef RENDERING_SURFACE_H
#define RENDERING_SURFACE_H

#include <vector>
#include <iostream>
#include <memory>
#include <functional>

#include "renderer/vmtkAbstractRender.h"

class ControllerRenderingSurface;

class MouseEvent
{
public:

    enum class MouseButton
    {
        NONE,
        LEFT,
        RIGHT,
        SCROLL
    };

    enum class Type
    {
        MOUSE_SINGLE_CLICK,
        MOUSE_DOUBLE_CLICK
    };

    MouseEvent(int pos_x, int pos_y, MouseButton button = MouseButton::NONE, Type type = Type::MOUSE_SINGLE_CLICK);

    int getPosX() const;
    int getPosY() const;
    MouseButton getMouseButton() const;
    Type getType() const;

private:
    int mPositionX;
    int mPositionY;
    MouseButton mMouseButton;
    Type mType;
};

//==============================================================================================================================================================================

class RenderingSurface
{
public:
    typedef std::shared_ptr<RenderingSurface> sPtr;
    typedef std::weak_ptr<RenderingSurface> wPtr;

    RenderingSurface(vmtkAbstractRender *renderer, std::function<void(RenderingSurface *)> on_initialization);
    virtual ~RenderingSurface();

    virtual void enableDebug(bool enable, std::string debug_msg) = 0;
    virtual void setRenderingContext() = 0;
    virtual void updateRendering() = 0;
    virtual void createFocusBorder() = 0;
    virtual void removeFocusBorder() = 0;
    virtual void saveImage(bool enable_streaming = false) = 0;
    virtual void setAutoUpdateEnabled(bool enabled) = 0;
    virtual void setCursorEnum(int idCursorEnum) = 0;

    ControllerRenderingSurface *getController() const;
    std::vector<ControllerRenderingSurface *> getControllers() const;
    void setController(ControllerRenderingSurface *ptrController);

    virtual void addController(ControllerRenderingSurface *ptrController);
    virtual void removeController(ControllerRenderingSurface *ptrController);

//    vmtkAbstractRender *getRenderer() const;

    int getWidth() const;
    int getHeight() const;

protected:
    virtual void initializeGlew() = 0;
    virtual void doInitializeGL();
    virtual void doPaintGL();
    virtual void doResizeGL(int w, int h);
    virtual void updatePixelRatio(float value);

    void handleMousePressEvent(MouseEvent &mouse_event);
    void handleMouseMoveEvent(MouseEvent &mouse_event);
    void handleMouseReleaseEvent(MouseEvent &mouse_event);
    void handleMouseDoubleClickEvent(MouseEvent &mouse_event);

private:
    std::vector<ControllerRenderingSurface*> m_controllers;
    ControllerRenderingSurface *mPtrController;
    vmtkAbstractRender *mRenderer;
    std::function<void(RenderingSurface*)> m_onInitialization;

    int m_width;
    int m_height;
    float m_pixelRatio;
};

//==============================================================================================================================================================================

class NullRenderingSurface: public RenderingSurface
{
public:
    typedef std::shared_ptr<NullRenderingSurface> sPtr;
    typedef std::weak_ptr<NullRenderingSurface> wPtr;

    NullRenderingSurface();
    ~NullRenderingSurface();

    void enableDebug(bool ,std::string) {}
    void setRenderingContext();
    void updateRendering();
    void createFocusBorder() { }
    void removeFocusBorder() { }
    void saveImage(bool enable_streaming = false);
    void setAutoUpdateEnabled(bool );
    void setCursorEnum(int);
    void addController(ControllerRenderingSurface *ptrController) override;
    void removeController(ControllerRenderingSurface *ptrController) override;

protected:
    void initializeGlew() {}
};

//==============================================================================================================================================================================

class ControllerRenderingSurface
{
public:
    ControllerRenderingSurface();
    virtual ~ControllerRenderingSurface() {}

    virtual void afterRendering() {}
    virtual void afterResizing(int w, int h);
    virtual void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    virtual void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    virtual void mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    virtual void mouseDoubleClickEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
};

#endif // RENDERING_SURFACE_H
