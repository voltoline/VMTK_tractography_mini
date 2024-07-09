#include "renderer/vmtkRender3D.h"

#include "utilities/QCheckingOpenGL.h"
#include <glm/gtc/type_ptr.hpp>

vmtkRender3D::vmtkRender3D ( )
{


}

void vmtkRender3D::initialize()
{
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.5,0.5,0.5,1.0);

    vmtkAbstractRender::initialize();

    executeInitializes();

}

void vmtkRender3D::resize(int width, int height)
{
    m_w = width;
    m_h = height;

    computeViewport();

    vmtkAbstractRender::resize(width, height);
    vmtkAbstractRender::updateViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

    executeResizes(width, height);
}

void vmtkRender3D::render()
{    
    glClearColor(0.5,0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vmtkAbstractRender::render();

    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    executeRenders();

    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void vmtkRender3D::computeViewport()
{
    if (m_w < m_h)
    {
        glViewport (0, (m_h-m_w)/2, m_w, m_w);
        m_viewport[0] = 0;
        m_viewport[1] = (m_h-m_w)/2;
        m_viewport[2] = m_w;
        m_viewport[3] = m_w;

    } else {

        glViewport ((m_w-m_h)/2, 0, m_h, m_h);
        m_viewport[0] = (m_w-m_h)/2;
        m_viewport[1] = 0;
        m_viewport[2] = m_h;
        m_viewport[3] = m_h;
    }
}

void vmtkRender3D::registerInitializes(std::function<void ()> initializes, size_t &id)
{
    m_initializes.push_back(initializes);
    id = m_initializes.size()-1;
}

void vmtkRender3D::registerRenders(std::function<void ()> renders, size_t &id)
{
    m_renders.push_back(renders);
    id = m_renders.size()-1;
}

void vmtkRender3D::registerResizes(std::function<void (int, int)> resizes, size_t &id)
{
    m_resizes.push_back(resizes);
    id = m_resizes.size()-1;
}

void vmtkRender3D::updateInitializes(std::function<void ()> initializes, size_t id)
{
    m_initializes[id] = initializes;
}

void vmtkRender3D::updateRenders(std::function<void ()> renders, size_t id)
{
    m_renders[id] = renders;
}

void vmtkRender3D::updateResizes(std::function<void (int, int)> resizes, size_t id)
{
    m_resizes[id] = resizes;
}

void vmtkRender3D::unregisterInitializes(size_t id)
{
    m_initializes[id] = [=]()->void { };
}

void vmtkRender3D::unregisterRenders(size_t id)
{
    m_renders[id] = [=]()->void { };
}

void vmtkRender3D::unregisterResizes(size_t id)
{
    m_resizes[id] = [=](int,int)->void { };
}

void vmtkRender3D::executeInitializes()
{
    for (auto f : m_initializes){ f(); }
}

void vmtkRender3D::executeRenders()
{
    for (auto f : m_renders){ f(); }
}

void vmtkRender3D::executeResizes(int w, int h)
{
    for (auto f : m_resizes){ f( w, h ); }
}
