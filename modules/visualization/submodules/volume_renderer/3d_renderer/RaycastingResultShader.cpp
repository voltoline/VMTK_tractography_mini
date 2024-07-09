#include "RaycastingResultShader.h"

#include <utilities/QManagementResources.h>
#include <renderer/vmtkManagementShader.h>
#include <renderer/vmtkFrameBufferObject.h>

RaycastingResultShader::RaycastingResultShader()
{
    m_resultShader = nullptr;

    m_vaosResultPlane = 0;
    m_vboResultPlane = 0;
    m_eboResultPlane = 0;
}

RaycastingResultShader::~RaycastingResultShader()
{
    if(m_resultShader != nullptr)
        delete m_resultShader;

    if(m_vaosResultPlane != 0)
        glDeleteVertexArrays(1, &m_vaosResultPlane);

    if(m_vboResultPlane != 0)
        glDeleteBuffers(1, &m_vboResultPlane);

    if(m_eboResultPlane != 0)
        glDeleteBuffers(1, &m_eboResultPlane);
}

void RaycastingResultShader::initialize()
{
    QManagementResource *vs = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex,":/drawresult_vert");
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment,":/drawresult_frag");
    m_resultShader= new vmtkManagementShader;
    m_resultShader->setShader(vertex);
    m_resultShader->setShader(fragment);
    m_resultShader->linking();

    m_resultShader->binding();
    {
        glUniform1i(glGetUniformLocation(m_resultShader->getProgram(),"raycast_fbo"), 0);
    }
    m_resultShader->releasing();

    /*create buffer to ResultPlane*/
    glGenVertexArrays(1, &m_vaosResultPlane);
    glGenBuffers(1, &m_vboResultPlane);
    glGenBuffers(1, &m_eboResultPlane);

    delete vs;
    delete fs;

    initResultPlane();
}

void RaycastingResultShader::initResultPlane()
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

    glBindVertexArray(m_vaosResultPlane);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboResultPlane);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    quintptr offset = 0;
    int vertexLocation = glGetAttribLocation(m_resultShader->getProgram(), "position");
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    offset += sizeof(glm::vec2);
    int texcoordLocation = glGetAttribLocation(m_resultShader->getProgram(), "texCoord");
    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboResultPlane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void RaycastingResultShader::execute(GLuint tex_to_render)
{
    /* now draw the rendered texture */
    m_resultShader->binding();

    glClearColor(0.5,0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_to_render);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(m_vaosResultPlane);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);

    m_resultShader->releasing();
}
