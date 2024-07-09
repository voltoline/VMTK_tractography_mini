#include "vmtkAbstractRender.h"

#include <iostream>

#include "utilities/QCheckingOpenGL.h"

vmtkAbstractRender::vmtkAbstractRender()
{

}

void vmtkAbstractRender::initialize()
{
    for(auto renderer: m_subRenderers)
        renderer->initialize();
}

void vmtkAbstractRender::render()
{
    for(auto renderer: m_subRenderers)
    {
        renderer->render();
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    }
}

void vmtkAbstractRender::resize(int width, int height)
{
    for(auto renderer: m_subRenderers)
        renderer->resize(width, height);
}

void vmtkAbstractRender::updatePixelRatio(float value)
{
    for(auto renderer: m_subRenderers)
        renderer->pixelRatioChanged(value);
}

void vmtkAbstractRender::updateViewport(int x, int y, int width, int height)
{
    for(auto renderer: m_subRenderers)
        renderer->viewportChanged( x, y, width, height);
}

void vmtkAbstractRender::addSubRenderer(SubRendererBase *sub_renderer)
{
    if(sub_renderer)
    {
        for(auto renderer: m_subRenderers)
            if(renderer == sub_renderer)
            {
                std::cout << "vmtkAbstractRender::addSubRenderer -> Duplicated addition of subrenderer found!" << std::endl;
                return;
            }

        m_subRenderers.push_back(sub_renderer);

    } else
        std::cout << "vmtkAbstractRender::addSubRenderer -> Try to add a null pointer!" << std::endl;
}

void vmtkAbstractRender::removeSubRenderer(SubRendererBase *sub_renderer)
{
    if(sub_renderer)
    {
        for(unsigned short i = 0; i < m_subRenderers.size(); i++)
            if(m_subRenderers[i] == sub_renderer)
            {
                m_subRenderers.erase(m_subRenderers.begin() + i);
                return;
            }

        std::cout << "vmtkAbstractRender::removeSubRenderer -> Subrenderer not found!" << std::endl;

    } else
        std::cout << "vmtkAbstractRender::removeSubRenderer -> Try to remove a null pointer!" << std::endl;

}
