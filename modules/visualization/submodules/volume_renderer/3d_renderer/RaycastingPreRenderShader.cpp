#include "RaycastingPreRenderShader.h"

#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"
#include <renderer/vmtkFrameBufferObject.h>
#include <glm/gtc/type_ptr.hpp>

RaycastingPreRenderShader::RaycastingPreRenderShader(float scale_factors[])
{
    m_flag_cube_changed = true;           /**< cube change status */
    m_flag_cube_alloc = false;            /**< cube setup status */

    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    m_fClipXLeft = -1.0f;
    m_fClipXRight = 1.0f;
    m_fClipYTop = 1.0f;
    m_fClipYBottom = -1.0f;
    m_fClipZBack = 1.0f;
    m_fClipZFront = -1.0f;

    m_FBOSizeFactor = 1.0f;

    m_mvp = glm::mat4(1.0);

    m_BackFBO = nullptr;
    m_FrontFBO = nullptr;

    m_initialized = false;

    m_ColorShader = nullptr;

    vaosCube = 0;
    vboCube = 0;
    eboCube = 0;
}

RaycastingPreRenderShader::~RaycastingPreRenderShader()
{
    if(m_BackFBO != nullptr)
        delete m_BackFBO;

    if(m_FrontFBO != nullptr)
        delete m_FrontFBO;

    if(m_ColorShader != nullptr)
        delete m_ColorShader;

    if(vaosCube > 0)
        glDeleteVertexArrays(1, &vaosCube);

    if(vboCube > 0)
        glDeleteBuffers(1, &vboCube);

    if(eboCube > 0)
        glDeleteBuffers(1, &eboCube);
}

void RaycastingPreRenderShader::initialize()
{
    if(m_initialized)
        return;

    m_BackFBO = new vmtkFrameBufferObject(400, 400);
    m_FrontFBO = new vmtkFrameBufferObject(400, 400, 1, vmtkFrameBufferObject::Attachment::Depth);

    QManagementResource *vs = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex,":/position_is_color_vert");
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment,":/position_is_color_frag");

    m_ColorShader = new vmtkManagementShader;
    m_ColorShader->setShader(vertex);
    m_ColorShader->setShader(fragment);
    m_ColorShader->linking();

    m_ColorShader->binding();
    {
        m_mvpMatrixColorShaderUniformLoc = glGetUniformLocation(m_ColorShader->getProgram(), "mvp_matrix");
        m_scaleFactorsColorShaderUniformLoc = glGetUniformLocation(m_ColorShader->getProgram(), "scaleFactors");

        glUniformMatrix4fv(m_mvpMatrixColorShaderUniformLoc, 1, GL_FALSE, glm::value_ptr(m_mvp));
        glUniform4fv(m_scaleFactorsColorShaderUniformLoc, 1, m_scaleFactors);
    }
    m_ColorShader->releasing();

    delete vs;
    delete fs;

    /*create buffers to Cube*/
    glGenVertexArrays(1, &vaosCube);
    glGenBuffers(1, &vboCube);
    glGenBuffers(1, &eboCube);

    initDrawCube();

    m_initialized = true;
}

void RaycastingPreRenderShader::setScaleFactors(float scale_factors[])
{
    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    if(m_ColorShader == nullptr)
        return;

    m_ColorShader->binding();
    {
        glUniform4fv(m_scaleFactorsColorShaderUniformLoc, 1, m_scaleFactors);
    }
    m_ColorShader->releasing();
}

void RaycastingPreRenderShader::initDrawCube(){

    const float clip_x_left = m_fClipXLeft;     // initialized with -1
    const float clip_x_right = m_fClipXRight;   // initialized with +1
    const float clip_y_top = m_fClipYTop;       // initialized with +1
    const float clip_y_bottom = m_fClipYBottom; // initialized with -1
    const float clip_z_front = m_fClipZBack;    // initialized with +1
    const float clip_z_back = m_fClipZFront;    // initialized with -1

    //left side
    vc1 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_back);
    vc2 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_front);
    vc3 = glm::vec3(clip_x_left, clip_y_top, clip_z_front);
    vc4 = glm::vec3(clip_x_left, clip_y_top, clip_z_back);

    //front side
    vc5 = glm::vec3(clip_x_left, clip_y_top, clip_z_front);
    vc6 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_front);
    vc7 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_front);
    vc8 = glm::vec3(clip_x_right, clip_y_top, clip_z_front);

    //right side
    vc9 = glm::vec3(clip_x_right, clip_y_top, clip_z_back);
    vc10 = glm::vec3(clip_x_right, clip_y_top, clip_z_front);
    vc11 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_front);
    vc12 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_back);

    //back side
    vc13 = glm::vec3(clip_x_right, clip_y_top, clip_z_back);
    vc14 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_back);
    vc15 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_back);
    vc16 = glm::vec3(clip_x_left, clip_y_top, clip_z_back);

    //bottom
    vc17 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_back);
    vc18 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_back);
    vc19 = glm::vec3(clip_x_right, clip_y_bottom, clip_z_front);
    vc20 = glm::vec3(clip_x_left, clip_y_bottom, clip_z_front);

    //top
    vc21 = glm::vec3(clip_x_left, clip_y_top, clip_z_back);
    vc22 = glm::vec3(clip_x_left, clip_y_top, clip_z_front);
    vc23 = glm::vec3(clip_x_right, clip_y_top, clip_z_front);
    vc24 = glm::vec3(clip_x_right, clip_y_top, clip_z_back);


    if(m_flag_cube_changed)
    {
        m_flag_cube_changed = false;

        VertexDataCube vertices[] = {
            {vc1},  // 0
            {vc2},  // 1
            {vc3},  // 2
            {vc4},  // 3
            {vc5},  // 4
            {vc6},  // 5
            {vc7},  // 6
            {vc8},  // 7
            {vc9},  // 8
            {vc10}, // 9
            {vc11}, // 10
            {vc12}, // 11
            {vc13}, // 12
            {vc14}, // 13
            {vc15}, // 14
            {vc16}, // 15
            {vc17}, // 16
            {vc18}, // 17
            {vc19}, // 18
            {vc20}, // 19
            {vc21}, // 20
            {vc22}, // 21
            {vc23}, // 22
            {vc24} // 23
        };

        if(!m_flag_cube_alloc)
        {
            m_flag_cube_alloc = true;

            GLushort indices[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

            glBindVertexArray(vaosCube);
            glBindBuffer(GL_ARRAY_BUFFER, vboCube);
            glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(VertexDataCube), vertices, GL_DYNAMIC_DRAW);

            int vertexLocation = glGetAttribLocation(m_ColorShader->getProgram(), "position");
            glEnableVertexAttribArray(vertexLocation);
            glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (const void *) 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboCube);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLushort), indices, GL_DYNAMIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vboCube);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(VertexDataCube), vertices);
        }
    }
}

void RaycastingPreRenderShader::setFBOSizeFactor(float FBOSizeFactor)
{
    m_FBOSizeFactor = FBOSizeFactor;
}

vmtkFrameBufferObject& RaycastingPreRenderShader::getBackFBO()
{
    if(m_BackFBO == nullptr)
        throw std::runtime_error("RaycastingPreRenderShader::getBackFBO -->> Object not initialized!");

    return *m_BackFBO;
}

vmtkFrameBufferObject& RaycastingPreRenderShader::getFrontFBO()
{
    if(m_FrontFBO == nullptr)
        throw std::runtime_error("RaycastingPreRenderShader::getFrontFBO -->> Object not initialized!");

    return *m_FrontFBO;
}

void RaycastingPreRenderShader::setClippingValues(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_)
{
    m_fClipXLeft = fXLeft_;
    m_fClipXRight = fXRight_;
    m_fClipYTop = fYTop_;
    m_fClipYBottom = fYBottom_;
    m_fClipZBack = fZBack_;
    m_fClipZFront = fZFront_;

    m_flag_cube_changed = true;

    if(m_initialized)
        initDrawCube();
    else
        std::cerr << "RaycastingPreRenderShader::setClippingValues -->> Object not initialized!" << std::endl;
}

void RaycastingPreRenderShader::itlDrawColorCube()
{
    m_ColorShader->binding();
    {
        drawCube();
    }
    m_ColorShader->releasing();
}

void RaycastingPreRenderShader::drawCube()
{
    glBindVertexArray(vaosCube);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*4));
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*8));
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*12));
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*16));
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*20));
}

void RaycastingPreRenderShader::setMVP(const glm::mat4 &mvp)
{
    m_mvp = mvp;

    if(m_ColorShader == nullptr)
        return;

    m_ColorShader->binding();
    {
        glUniformMatrix4fv(m_mvpMatrixColorShaderUniformLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    }
    m_ColorShader->releasing();
}

void RaycastingPreRenderShader::execute()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    m_BackFBO->binding();

    glClearColor(0.0,0.0,0.0,1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //clear buffers

    if (m_width < m_height) {
        glViewport (0, (m_height-m_width)/2, m_width, m_width);
    } else {
        glViewport ((m_width-m_height)/2, 0, m_height, m_height);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    itlDrawColorCube();
    m_BackFBO->releasing();

    glCullFace(GL_BACK);
    m_FrontFBO->binding();

    glClearColor(0.0,0.0,0.0,1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //clear buffers

    if (m_width < m_height) {
        glViewport (0, (m_height-m_width)/2, m_width, m_width);
    } else {
        glViewport ((m_width-m_height)/2, 0, m_height, m_height);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    itlDrawColorCube();
    m_FrontFBO->releasing();

    glDisable(GL_DEPTH_TEST);
}

void RaycastingPreRenderShader::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    if(m_BackFBO != nullptr)
    {
        delete m_BackFBO;
        m_BackFBO = nullptr;
    }

    if(m_FrontFBO != nullptr)
    {
        delete m_FrontFBO;
        m_FrontFBO = nullptr;
    }

    m_BackFBO = new vmtkFrameBufferObject(width * m_FBOSizeFactor, height * m_FBOSizeFactor);
    m_FrontFBO = new vmtkFrameBufferObject(width * m_FBOSizeFactor, height * m_FBOSizeFactor, 1, vmtkFrameBufferObject::Attachment::Depth);
}
