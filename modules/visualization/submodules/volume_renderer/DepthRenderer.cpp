#include "DepthRenderer.h"

#include <utilities/QManagementResources.h>
#include <renderer/vmtkManagementShader.h>
#include <renderer/vmtkFrameBufferObject.h>

DepthRenderer::DepthRenderer()
{
    m_planeShader = nullptr;

    m_vaoPlane = 0;
    m_vboPlane = 0;
    m_eboPlane = 0;
}

DepthRenderer::~DepthRenderer()
{
    if(m_planeShader != nullptr)
        delete m_planeShader;

    if(m_vaoPlane != 0)
        glDeleteVertexArrays(1, &m_vaoPlane);

    if(m_vboPlane != 0)
        glDeleteBuffers(1, &m_vboPlane);

    if(m_eboPlane != 0)
        glDeleteBuffers(1, &m_eboPlane);
}

void DepthRenderer::initialize()
{
    QManagementResource *vs = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex,":/drawresult_vert");
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment,":/depth_renderer_frag");
    m_planeShader = new vmtkManagementShader;
    m_planeShader->setShader(vertex);
    m_planeShader->setShader(fragment);
    m_planeShader->linking();

    m_planeShader->binding();
    {
        glUniform1i(glGetUniformLocation(m_planeShader->getProgram(),"depth_tex"), 0);
    }
    m_planeShader->releasing();

    /*create buffer to ResultPlane*/
    glGenVertexArrays(1, &m_vaoPlane);
    glGenBuffers(1, &m_vboPlane);
    glGenBuffers(1, &m_eboPlane);

    delete vs;
    delete fs;

    initResultPlane();
}

void DepthRenderer::initResultPlane()
{
    /**
     * @struct VertexData
     */
    struct VertexData
    {
        glm::vec2 position;     /** < position coordinates */
        glm::vec2 texCoord;     /** < texture coordinates */
    };

    /**
     * @brief Position vectors of VertexData.
     */
    glm::vec2 v1, v2, v3, v4;

    /**
     * @brief Texture vectors of VertexData.
     */
    glm::vec2  vt1, vt2, vt3, vt4;

    /**
     * @brief defines the position and texture coordinates of the four vertices of a plane.
     */
    v1 = glm::vec2(-1.0f,-1.0f);
    v2 = glm::vec2(1.0f,-1.0f);
    v3 = glm::vec2(1.0f,1.0f);
    v4 = glm::vec2(-1.0f,1.0f);
    vt1 = glm::vec2(0.0f,0.0f);
    vt2 = glm::vec2(1.0f,0.0f);
    vt3 = glm::vec2(1.0f,1.0f);
    vt4 = glm::vec2(0.0f,1.0f);

    VertexData vertices[] = {
        {v1, vt1},  // v0
        {v2, vt2}, // v1
        {v3, vt3}, // v2
        {v4, vt4},  // v3
    };

    GLushort indices[] = { 0,  1,  2,  3};

    glBindVertexArray(m_vaoPlane);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPlane);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    quintptr offset = 0;
    int vertexLocation = glGetAttribLocation(m_planeShader->getProgram(), "position");
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    offset += sizeof(glm::vec2);
    int texcoordLocation = glGetAttribLocation(m_planeShader->getProgram(), "texCoord");
    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPlane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void DepthRenderer::execute(GLuint tex_to_render)
{
    /* now draw the rendered texture */
    m_planeShader->binding();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_to_render);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindVertexArray(m_vaoPlane);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);

    m_planeShader->releasing();
}
