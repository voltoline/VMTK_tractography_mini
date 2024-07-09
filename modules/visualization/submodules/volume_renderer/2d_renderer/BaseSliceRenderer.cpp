#include "BaseSliceRenderer.h"

#include <utilities/QManagementResources.h>
#include <glm/gtc/type_ptr.hpp>

BaseSliceRenderer::BaseSliceRenderer()
{
    m_spPlane = nullptr;
    m_isInitialized = false;

    m_vaosPlane = 0;
    m_vboPlane = 0;
    m_eboPlane = 0;

    m_mvpMatrix = glm::mat4(1.0);

    m_v1 = glm::vec3( 1.0f,  1.0f, 0.0f);
    m_v2 = glm::vec3(-1.0f,  1.0f, 0.0f);
    m_v3 = glm::vec3(-1.0f, -1.0f, 0.0f);
    m_v4 = glm::vec3( 1.0f, -1.0f, 0.0f);

    m_vt1 = glm::vec3(1.0f, 0.0f, 0.5f);
    m_vt2 = glm::vec3(0.0f, 0.0f, 0.5f);
    m_vt3 = glm::vec3(0.0f, 1.0f, 0.5f);
    m_vt4 = glm::vec3(1.0f, 1.0f, 0.5f);
}

BaseSliceRenderer::~BaseSliceRenderer()
{
    if(m_spPlane != nullptr)
        delete m_spPlane;

    if(m_vaosPlane > 0)
        glDeleteVertexArrays(1, &m_vaosPlane);

    if(m_vboPlane > 0)
        glDeleteBuffers(1, &m_vboPlane);

    if(m_eboPlane > 0)
        glDeleteBuffers(1, &m_eboPlane);
}

void BaseSliceRenderer::initialize()
{
    if(isInitialized())
        return;

    QManagementResource *vs = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, m_vertexShaderPath);
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment, m_fragmentShaderPath);
    m_spPlane = new vmtkManagementShader;
    m_spPlane->setShader(vertex);
    m_spPlane->setShader(fragment);
    m_spPlane->linking();

    delete vs;
    delete fs;

    m_spPlane->binding();
    {
        glUniformMatrix4fv(glGetUniformLocation(m_spPlane->getProgram(), "mvp_matrix"), 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));
    }
    m_spPlane->releasing();

    createBuffers();

    initPlane();

    m_isInitialized = true;
}

void BaseSliceRenderer::createBuffers()
{
    /*Create buffers to plane*/
    glGenVertexArrays(1,&m_vaosPlane);
    glGenBuffers(1, &m_vboPlane);
    glGenBuffers(1, &m_eboPlane);
}

void BaseSliceRenderer::initPlane()
{
    VertexData vertices[] = {
        {m_v1, m_vt1}, // v0
        {m_v2, m_vt2}, // v1
        {m_v3, m_vt3}, // v2
        {m_v4, m_vt4}  // v3
    };

    GLushort indices[] = { 0,  1,  2,  3};

    glBindVertexArray(m_vaosPlane);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboPlane);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_DYNAMIC_DRAW);

        int offset = 0;
        int vertexLocation = glGetAttribLocation(m_spPlane->getProgram(), "position");
        glEnableVertexAttribArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(size_t)offset);

        offset += sizeof(glm::vec3);
        int texcoordLocation = glGetAttribLocation(m_spPlane->getProgram(), "texCoord");
        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(size_t)offset);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPlane);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void BaseSliceRenderer::updatePlaneBuffer()
{
    VertexData vertices[] = {
        {m_v1, m_vt1}, // v0
        {m_v2, m_vt2}, // v1
        {m_v3, m_vt3}, // v2
        {m_v4, m_vt4}  // v3
    };

    glBindVertexArray(m_vaosPlane);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboPlane);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData), vertices);
    }
    glBindVertexArray(0);
}

void BaseSliceRenderer::render()
{
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    m_spPlane->binding();
    {
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
        configureTextureUnits();
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

        drawPlane();
        QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    }
    m_spPlane->releasing();
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void BaseSliceRenderer::drawPlane()
{
    glBindVertexArray(m_vaosPlane);
    {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
    }
    glBindVertexArray(0);
}

vmtkManagementShader& BaseSliceRenderer::getSliceShaderProgram()
{
    if(!isInitialized())
        throw std::runtime_error("BaseSliceRenderer::getSliceShader -->> Object not initialized!");

    return *m_spPlane;
}

bool BaseSliceRenderer::isInitialized() const
{
    return m_isInitialized;
}

void BaseSliceRenderer::setVertexShaderPath(const std::string &vertexShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
}

void BaseSliceRenderer::setFragmentShaderPath(const std::string &fragmentShaderPath)
{
    m_fragmentShaderPath = fragmentShaderPath;
}

void BaseSliceRenderer::setMVPMatrix(const glm::mat4 &mvpMatrix)
{
    m_mvpMatrix = mvpMatrix;

    if(isInitialized())
    {
        m_spPlane->binding();
        {
            glUniformMatrix4fv(glGetUniformLocation(m_spPlane->getProgram(), "mvp_matrix"), 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));
        }
        m_spPlane->releasing();

    } else
        std::cerr << "BaseSliceRenderer::setMVPMatrix -->> Object not initialized!" << std::endl;
}

void BaseSliceRenderer::setPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    m_v1 = v1;
    m_v2 = v2;
    m_v3 = v3;
    m_v4 = v4;

    if(isInitialized())
        updatePlaneBuffer();
    else
        std::cerr << "BaseSliceRenderer::setPlanePos -->> Object not initialized!" << std::endl;
}

void BaseSliceRenderer::setPlaneTex(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    m_vt1 = v1;          /**< texture coordinates of v1 */
    m_vt2 = v2;          /**< texture coordinates of v2 */
    m_vt3 = v3;          /**< texture coordinates of v3 */
    m_vt4 = v4;          /**< texture coordinates of v4 */

    if(isInitialized())
        updatePlaneBuffer();
    else
        std::cerr << "BaseSliceRenderer::setPlaneTex -->> Object not initialized!" << std::endl;
}
