#include "renderer/vmtkFrameBufferObject.h"
#include <math.h>
#include <assert.h>

GLuint vmtkFrameBufferObject::bound(const int &min, const int &val, const int &max)
{
    return (GLuint) fmax(min, fmin(max, val));
}

vmtkFrameBufferObject::vmtkFrameBufferObject()
{
    fbo = 0;
    texture = NULL;

    depthTexture = 0;

    m_color_buffer = 0;
    m_depth_buffer = 0;
    m_stencil_buffer = 0;
}

vmtkFrameBufferObject::vmtkFrameBufferObject(int width, int height, unsigned short num_render_buffer, Attachment attachment, GLenum target, GLenum internal_format)
{
    if(num_render_buffer > 5)
        num_render_buffer = 5;

    m_num_render_buffer_tex = num_render_buffer;
    texture = new GLuint[m_num_render_buffer_tex];

    init(width, height, attachment, internal_format, target);
}

vmtkFrameBufferObject::~vmtkFrameBufferObject()
{
    if(fbo != 0)
        glDeleteFramebuffers (1, &fbo);

    if(texture != NULL)
    {
        glDeleteTextures(m_num_render_buffer_tex, texture);
        delete[] texture;
    }

    if(depthTexture != 0)
        glDeleteTextures(1, &depthTexture);

    if(m_color_buffer > 0)
        glDeleteRenderbuffers(1, &m_color_buffer);

    if(m_depth_buffer > 0)
        glDeleteRenderbuffers(1, &m_depth_buffer);

    if(m_stencil_buffer != m_depth_buffer && m_stencil_buffer > 0)
        glDeleteRenderbuffers(1, &m_stencil_buffer);
}

void vmtkFrameBufferObject::init(int width, int height, vmtkFrameBufferObject::Attachment attachment, GLenum internal_format, GLenum target, GLint samples, bool mipmap)
{

    this->target = target;

    if(width <= 0)
        width = 1;

    if(height <= 0)
        height = 1;

    m_width = width;
    m_height = height;
    fbo = 0;
    depthTexture = 0;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for(int i = 0; i < m_num_render_buffer_tex; i++)
        texture[i] = 0;

    if (samples == 0)
    {
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

        glGenTextures(m_num_render_buffer_tex, texture);
        glGenTextures(1, &depthTexture);

        for(unsigned short i = 0; i < m_num_render_buffer_tex; i++)
        {
            glBindTexture(target, texture[i]);
            QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

                glTexImage2D(target, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            if(mipmap)
            {
                int level = 0;
                while (width > 1 || height > 1)
                {
                    width = qMax(1, width >> 1);
                    height = qMax(1, height >> 1);
                    ++level;
                    glTexImage2D(target, level, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                }
            }

            QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, target, texture[i], 0);

            QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

            if(i == 0)
            {
                glBindTexture(target, depthTexture);
                QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

                glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(target, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, depthTexture, 0);
            }
        }

        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

        GLenum buffers[m_num_render_buffer_tex];
        for(unsigned short i = 0; i < m_num_render_buffer_tex; i++)
            buffers[i] = GL_COLOR_ATTACHMENT0 + i;

        glDrawBuffers(m_num_render_buffer_tex, buffers);

        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
        glBindTexture(target, 0);
    }
    else
    {
        mipmap = false;
        GLint maxSamples;
        glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
        samples = bound(0, int(samples), int(maxSamples));
        glGenRenderbuffers(1, &m_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_color_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_color_buffer);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_SAMPLES, &samples);
    }

    if (attachment == vmtkFrameBufferObject::DepthStencil){

        glGenRenderbuffers(1, &m_depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
        assert (glIsRenderbuffer(m_depth_buffer));
        if (samples != 0)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        m_stencil_buffer = m_depth_buffer;
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil_buffer);

        bool valid = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (!valid) {
            glDeleteRenderbuffers(1, &m_depth_buffer);
            m_stencil_buffer = m_depth_buffer = 0;
        }
    }

    if (m_depth_buffer == 0 && (attachment == vmtkFrameBufferObject::DepthStencil
                              || (attachment == vmtkFrameBufferObject::Depth)))
    {
        glGenRenderbuffers(1, &m_depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
        assert(glIsRenderbuffer(m_depth_buffer));
        if (samples != 0) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, width, height);
        }
        else{
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_depth_buffer);
    }
    if (m_stencil_buffer == 0 && (attachment == vmtkFrameBufferObject::DepthStencil)) {
        glGenRenderbuffers(1, &m_stencil_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_stencil_buffer);
        assert(glIsRenderbuffer(m_stencil_buffer));

        GLenum storage =  GL_STENCIL_INDEX;
        if (samples != 0)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, storage, width, height);}
        else{
            glRenderbufferStorage(GL_RENDERBUFFER, storage, width, height);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil_buffer);
    }

    if (m_depth_buffer && m_stencil_buffer) {
        attachment = vmtkFrameBufferObject::DepthStencil;
    } else if (m_depth_buffer) {
        attachment = vmtkFrameBufferObject::Depth;
    } else {
        attachment = vmtkFrameBufferObject::NoAttachment;
    }

    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "vmtkFrameBufferObject::init --> Framebuffer is not ok!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned short vmtkFrameBufferObject::getHeight() const
{
    return m_height;
}

unsigned short vmtkFrameBufferObject::getWidth() const
{
    return m_width;
}

GLuint vmtkFrameBufferObject::getDepthTexture() const
{
  return depthTexture;
}


GLuint vmtkFrameBufferObject::getTexture(unsigned short render_buffer_tex_index) const
{
    if(render_buffer_tex_index < m_num_render_buffer_tex)
        return texture[render_buffer_tex_index];
    else
        return 0;
}

float* vmtkFrameBufferObject::getDataFromTextureAttach(unsigned short attach_index)
{
    if(attach_index < m_num_render_buffer_tex)
    {
        binding();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attach_index);

        float *tex_coord = new float[m_width * m_height * 4];
        glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_FLOAT, tex_coord);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        releasing();

        return tex_coord;
    }
    else
        return NULL;
}

GLuint vmtkFrameBufferObject::getFbo() const
{
    return fbo;
}

bool vmtkFrameBufferObject::binding()
{
    if (!fbo)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    return true;
}

bool vmtkFrameBufferObject::releasing()
{
    if (!fbo)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void vmtkFrameBufferObject::resizing(int , int )
{

}

