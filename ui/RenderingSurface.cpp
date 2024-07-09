#include "RenderingSurface.h"

#include <utilities/QCheckingOpenGL.h>

MouseEvent::MouseEvent(int pos_x, int pos_y, MouseEvent::MouseButton button, Type type)
{
    mPositionX = pos_x;
    mPositionY = pos_y;
    mMouseButton = button;
    mType = type;
}

int MouseEvent::getPosX() const
{
    return mPositionX;
}

int MouseEvent::getPosY() const
{
    return mPositionY;
}

MouseEvent::MouseButton MouseEvent::getMouseButton() const
{
    return mMouseButton;
}

MouseEvent::Type MouseEvent::getType() const
{
    return mType;
}

//==============================================================================================================================================================================

RenderingSurface::RenderingSurface(vmtkAbstractRender *renderer, std::function<void(RenderingSurface *)> on_initialization):
    mRenderer(renderer)
{
    m_width = 0;
    m_height = 0;

    m_pixelRatio = 1.0;

    m_onInitialization = on_initialization;
    mPtrController = nullptr;
}

RenderingSurface::~RenderingSurface()
{

}

void RenderingSurface::doInitializeGL()
{
    initializeGlew();
    if(mRenderer != nullptr)
    {
        mRenderer->initialize();

        m_onInitialization(this);
    }

}

void RenderingSurface::doPaintGL()
{
    if(mRenderer != nullptr)
    {
        mRenderer->render();
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

        for(auto controller: m_controllers)
            if(controller != nullptr)
                controller->afterRendering();
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    }
}

void RenderingSurface::doResizeGL(int w, int h)
{
    m_width = w;
    m_height = h;

    if(mRenderer != nullptr)
        mRenderer->resize(w, h);
}

void RenderingSurface::updatePixelRatio(float value)
{
    m_pixelRatio = value;

    if(mRenderer != nullptr)
        mRenderer->updatePixelRatio(value);
}

int RenderingSurface::getHeight() const
{
    return m_height;
}

void RenderingSurface::handleMousePressEvent(MouseEvent &mouse_event)
{
    for(ControllerRenderingSurface *controller: m_controllers)
        controller->mousePressEvent(mouse_event, this);
}

void RenderingSurface::handleMouseMoveEvent(MouseEvent &mouse_event)
{
    for(ControllerRenderingSurface *controller: m_controllers)
        controller->mouseMoveEvent(mouse_event, this);
}

void RenderingSurface::handleMouseReleaseEvent(MouseEvent &mouse_event)
{
    for(ControllerRenderingSurface *controller: m_controllers)
        controller->mouseReleaseEvent(mouse_event, this);
}

void RenderingSurface::handleMouseDoubleClickEvent(MouseEvent &mouse_event)
{
    for(ControllerRenderingSurface *controller: m_controllers)
        controller->mouseDoubleClickEvent(mouse_event, this);
}

std::vector<ControllerRenderingSurface *> RenderingSurface::getControllers() const
{
    return m_controllers;
}

int RenderingSurface::getWidth() const
{
    return m_width;
}

ControllerRenderingSurface *RenderingSurface::getController() const
{
    return mPtrController;
}

void RenderingSurface::setController(ControllerRenderingSurface *ptrController)
{
    mPtrController = ptrController;
}

void RenderingSurface::addController(ControllerRenderingSurface *ptrController)
{
    for(auto controller: m_controllers)
        if(controller == ptrController)
        {
            std::cout << "RenderingSurface::addController -> Duplicated addition of controller found!" << std::endl;
            return;
        }

    m_controllers.push_back(ptrController);
}

void RenderingSurface::removeController(ControllerRenderingSurface *ptrController)
{
    for(unsigned short i = 0; i < m_controllers.size(); i++)
        if(m_controllers[i] == ptrController)
        {
            m_controllers.erase(m_controllers.begin() + i);
            return;
        }

    std::cout << "RenderingSurface::removeController -> Controller not found!" << std::endl;
}

//==============================================================================================================================================================================

NullRenderingSurface::NullRenderingSurface():
    RenderingSurface(nullptr, [](RenderingSurface *){ })
{

}

NullRenderingSurface::~NullRenderingSurface()
{

}

void NullRenderingSurface::setRenderingContext()
{
    std::cerr << "NullRenderingSurface::setRenderingContext called!" << std::endl;
}

void NullRenderingSurface::updateRendering()
{
    std::cerr << "NullRenderingSurface::updateRendering called!" << std::endl;
}

void NullRenderingSurface::saveImage(bool)
{
    std::cerr << "NullRenderingSurface::saveImage called!" << std::endl;
}

void NullRenderingSurface::setAutoUpdateEnabled(bool)
{
    std::cerr << "NullRenderingSurface::setAutoUpdateEnabled called!" << std::endl;
}

void NullRenderingSurface::setCursorEnum(int)
{
    std::cerr << "NullRenderingSurface::setCursorEnum called!" << std::endl;
}

void NullRenderingSurface::addController(ControllerRenderingSurface *ptrController)
{
    RenderingSurface::addController(ptrController);
    std::cerr << "NullRenderingSurface::addController called!" << std::endl;
}

void NullRenderingSurface::removeController(ControllerRenderingSurface *ptrController)
{
    RenderingSurface::removeController(ptrController);
    std::cerr << "NullRenderingSurface::removeController called!" << std::endl;
}

//==============================================================================================================================================================================

ControllerRenderingSurface::ControllerRenderingSurface()
{

}

void ControllerRenderingSurface::afterResizing(int, int)
{

}

void ControllerRenderingSurface::mousePressEvent(MouseEvent &, RenderingSurface *)
{

}

void ControllerRenderingSurface::mouseMoveEvent(MouseEvent &, RenderingSurface *)
{

}

void ControllerRenderingSurface::mouseReleaseEvent(MouseEvent &, RenderingSurface *)
{

}

void ControllerRenderingSurface::mouseDoubleClickEvent(MouseEvent &, RenderingSurface *)
{

}
