#include "vmtkRender2D.h"

#include "utilities/QCheckingOpenGL.h"

vmtkRender2D::vmtkRender2D( )
{

}

void vmtkRender2D::initialize()
{
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // paint on black

    vmtkAbstractRender::initialize();
}

void vmtkRender2D::render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    computeViewport();

    glDisable(GL_CULL_FACE);

    vmtkAbstractRender::render();
}

void vmtkRender2D::resize(int w, int h)
{
    m_w = w;
    m_h = h;

    computeViewport();

    vmtkAbstractRender::resize(w, h);
    vmtkAbstractRender::updateViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void vmtkRender2D::computeViewport()
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
