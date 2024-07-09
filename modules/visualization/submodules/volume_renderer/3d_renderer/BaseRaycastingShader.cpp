#include "BaseRaycastingShader.h"

#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"
#include "utilities/functions/vmtkGeneralFunctions.h"

BaseRaycastingShader::BaseRaycastingShader()
{
    m_rayTracedFBO = nullptr;

    m_FBOSizeFactor = 1.0f;

    m_isInitialized = false;

    m_enableMPR = false;

    m_shader = nullptr;
    m_bFixedLowSteps = false;
    m_bEnableFixedLowSteps = false;
    m_iSteps = 0;

    m_noiseThreshold = 0.0f;              /**< default noise threshold with resperct to the raw volume data */
    m_stateThreshold = false;

    m_vaosPlaneRayTraced = 0;
    m_vboPlaneRayTraced = 0;
    m_eboPlaneRayTraced = 0;

    m_MVP = glm::mat4(1.0);

    m_highlightedPlanesUniformLoc = -1;
    m_highlightedPlanesActivatedUniformLoc = -1;
}

BaseRaycastingShader::BaseRaycastingShader(BaseRaycastingShader *raycasting_shader)
{
    m_vertexShaderPath = raycasting_shader->m_vertexShaderPath;
    m_fragmentShaderPath = raycasting_shader->m_fragmentShaderPath;
    m_FBOSizeFactor = raycasting_shader->m_FBOSizeFactor;
    m_bFixedLowSteps = raycasting_shader->m_bFixedLowSteps;
    m_bEnableFixedLowSteps = raycasting_shader->m_bEnableFixedLowSteps;
    m_iSteps = raycasting_shader->m_iSteps;
    m_enableMPR = raycasting_shader->m_enableMPR;
    m_stateThreshold = raycasting_shader->m_stateThreshold;
    m_noiseThreshold = raycasting_shader->m_noiseThreshold;
}

BaseRaycastingShader::~BaseRaycastingShader()
{
    if(m_rayTracedFBO != nullptr)
        delete m_rayTracedFBO;

    if(m_shader != nullptr)
        delete m_shader;

    if(m_vaosPlaneRayTraced > 0)
        glDeleteVertexArrays(1, &m_vaosPlaneRayTraced);

    if(m_vboPlaneRayTraced > 0)
        glDeleteBuffers(1, &m_vboPlaneRayTraced);

    if(m_eboPlaneRayTraced > 0)
        glDeleteBuffers(1, &m_eboPlaneRayTraced);
}

void BaseRaycastingShader::initialize()
{
    QManagementResource *vs = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, m_vertexShaderPath);
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment, m_fragmentShaderPath);
    m_shader = new vmtkManagementShader;
    m_shader->setShader(vertex);
    m_shader->setShader(fragment);
    m_shader->linking();

    delete vs;
    delete fs;

    m_shader->binding();
    {
        m_steps_mode_loc = glGetUniformLocation(m_shader->getProgram(),"steps_mode");
        m_noiseThresholdUniformLoc = glGetUniformLocation(m_shader->getProgram(), "noise_threshold");
        m_stateNoiseThresholdUniformLoc = glGetUniformLocation(m_shader->getProgram(), "state_noise_threshold");
        m_clippingPlaneUniformLoc = glGetUniformLocation(m_shader->getProgram(), "clipping_plane");
        m_MVPUniformLoc = glGetUniformLocation(m_shader->getProgram(), "mvp_matrix");

        glUniform1f(m_noiseThresholdUniformLoc, m_noiseThreshold);
        glUniform1i(m_stateNoiseThresholdUniformLoc, m_stateThreshold);

        glUniformMatrix4fv(m_MVPUniformLoc, 1, GL_FALSE,  glm::value_ptr(m_MVP));

        if(!m_enableMPR)
            m_clippingPlane = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        glUniform4fv(m_clippingPlaneUniformLoc, 1, glm::value_ptr(m_clippingPlane));

        if (m_bFixedLowSteps && m_bEnableFixedLowSteps)
            glUniform1i(m_steps_mode_loc, 50);
        else
            glUniform1i(m_steps_mode_loc, m_iSteps * 100);

        float eq_plane[3] = {0.5, 0.5, 0.5};

        m_highlightedPlanesUniformLoc = glGetUniformLocation(m_shader->getProgram(), "highlighted_planes");
        m_highlightedPlanesActivatedUniformLoc = glGetUniformLocation(m_shader->getProgram(), "highlight_plane_activated");

        if(m_highlightedPlanesUniformLoc >= 0)
            glUniform3fv(m_highlightedPlanesUniformLoc, 1, eq_plane);
        else
            std::cerr << "BaseRaycastingShader::initialize -> Uniform highlighted_planes not found!" << std::endl;

        if(m_highlightedPlanesActivatedUniformLoc >= 0)
            glUniform1i(m_highlightedPlanesActivatedUniformLoc, 0);
        else
            std::cerr << "BaseRaycastingShader::initialize -> Uniform highlight_plane_activated not found!" << std::endl;
    }
    m_shader->releasing();

    /*create buffer to PlaneRayTraced*/
    glGenVertexArrays(1, &m_vaosPlaneRayTraced);
    glGenBuffers(1, &m_vboPlaneRayTraced);
    glGenBuffers(1, &m_eboPlaneRayTraced);

    m_rayTracedFBO = new vmtkFrameBufferObject(400,400);

    createVectorPlanes();
    initPlaneRayTraced();

    m_isInitialized = true;
}

void BaseRaycastingShader::execute(RaycastingPreRenderShader *pre_render_shader)
{
    glEnable(GL_DEPTH_TEST);

    getRayTracedFBO().binding();

    glClearColor(0.5, 0.5, 0.5, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      //clear buffers
    glViewport(0, 0, m_width * getFBOSizeFactor(), m_height * getFBOSizeFactor());

    m_shader->binding();

    updateTextureUnits(pre_render_shader);

    drawPlaneRayTraced();
    m_shader->releasing();
    getRayTracedFBO().releasing();

    glDisable(GL_DEPTH_TEST);
}

void BaseRaycastingShader::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    if(m_rayTracedFBO != nullptr)
    {
        delete m_rayTracedFBO;
        m_rayTracedFBO = nullptr;
    }

    m_rayTracedFBO = new vmtkFrameBufferObject(width * m_FBOSizeFactor, height * m_FBOSizeFactor);
}

void BaseRaycastingShader::createVectorPlanes()
{
    v1 = glm::vec2(-1.0f,-1.0f);
    v2 = glm::vec2(1.0f,-1.0f);
    v3 = glm::vec2(1.0f,1.0f);
    v4 = glm::vec2(-1.0f,1.0f);
    vt1 = glm::vec2(0.0f,0.0f);
    vt2 = glm::vec2(1.0f,0.0f);
    vt3 = glm::vec2(1.0f,1.0f);
    vt4 = glm::vec2(0.0f,1.0f);
}

void BaseRaycastingShader::initPlaneRayTraced()
{
    VertexData vertices[] = {
        {v1, vt1},  // v0
        {v2, vt2}, // v1
        {v3, vt3}, // v2
        {v4, vt4},  // v3
    };

    GLushort indices[] = { 0,  1,  2,  3};
    glBindVertexArray(m_vaosPlaneRayTraced);
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPlaneRayTraced);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    quintptr offset = 0;
    int vertexLocation = glGetAttribLocation(m_shader->getProgram(), "position");
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    offset += sizeof(glm::vec2);
    int texcoordLocation = glGetAttribLocation(m_shader->getProgram(), "textCoord");
    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPlaneRayTraced);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void BaseRaycastingShader::drawPlaneRayTraced()
{
    glBindVertexArray(m_vaosPlaneRayTraced);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
}

void BaseRaycastingShader::setNoiseThreshold(float noiseThreshold)
{
    m_noiseThreshold = noiseThreshold;

    if(m_shader == nullptr)
        return;

    m_shader->binding();
    {
        glUniform1f(m_noiseThresholdUniformLoc, m_noiseThreshold);
    }
    m_shader->releasing();
}

void BaseRaycastingShader::setStateThreshold(bool stateThreshold)
{
    m_stateThreshold = stateThreshold;

    if(m_shader == nullptr)
        return;

    m_shader->binding();
    {
        glUniform1i(m_stateNoiseThresholdUniformLoc, m_stateThreshold);
    }
    m_shader->releasing();
}

void BaseRaycastingShader::setMVP(const glm::mat4 &MVP)
{
    m_MVP = MVP;
    if(m_shader == nullptr)
        return;

    m_shader->binding();
    {
        glUniformMatrix4fv(m_MVPUniformLoc, 1, GL_FALSE,  glm::value_ptr(MVP));
    }
    m_shader->releasing();
}

void BaseRaycastingShader::setBFixedLowSteps(bool bFixedLowSteps)
{
    m_bFixedLowSteps = bFixedLowSteps;
}

void BaseRaycastingShader::setBEnableFixedLowSteps(bool bEnableFixedLowSteps)
{
    m_bEnableFixedLowSteps = bEnableFixedLowSteps;
}

void BaseRaycastingShader::setISteps(int iSteps)
{
    m_iSteps = iSteps;

    if(m_shader == nullptr)
         return;

     m_shader->binding();
     {
         if (m_bFixedLowSteps && m_bEnableFixedLowSteps)
             glUniform1i(m_steps_mode_loc, 50);
         else
             glUniform1i(m_steps_mode_loc, m_iSteps * 100);
     }
     m_shader->releasing();
}

void BaseRaycastingShader::setVertexShaderPath(const std::string &vertexShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
}

void BaseRaycastingShader::setFragmentShaderPath(const std::string &framentShaderPath)
{
    m_fragmentShaderPath = framentShaderPath;
}

vmtkManagementShader& BaseRaycastingShader::getShader()
{
    if(m_shader == nullptr)
        assert(!"RaycastingShader::getShader -->> Object not initialized!");

    return *m_shader;
}

vmtkFrameBufferObject& BaseRaycastingShader::getRayTracedFBO()
{
    if(m_rayTracedFBO == nullptr)
        assert(!"BaseRaycastingShader::getRayTracedFBO -->> Object not initialized!");

    return *m_rayTracedFBO;
}

float BaseRaycastingShader::getFBOSizeFactor() const
{
    return m_FBOSizeFactor;
}

void BaseRaycastingShader::setFBOSizeFactor(float FBOSizeFactor)
{
    m_FBOSizeFactor = FBOSizeFactor;
}

void BaseRaycastingShader::setRayTracedFBO(vmtkFrameBufferObject *rayTracedFBO)
{
    if(m_rayTracedFBO != nullptr)
        delete m_rayTracedFBO;

    m_rayTracedFBO = rayTracedFBO;
}

bool BaseRaycastingShader::isInitialized() const
{
    return m_isInitialized;
}

void BaseRaycastingShader::setEnableMPR(bool enableMPR)
{
    m_enableMPR = enableMPR;

    if(!m_enableMPR)
        setClippingPlane(glm::vec4(0.0f,0.0f,0.0f,0.0f));
}

void BaseRaycastingShader::setClippingPlane(const glm::vec4 &clipping_plane)
{
    if(m_enableMPR)
        m_clippingPlane = clipping_plane;
    else
        m_clippingPlane = glm::vec4(0.0f,0.0f,0.0f,0.0f);

    if(m_shader == nullptr)
        return;

    m_shader->binding();
    {
        glUniform4fv(m_clippingPlaneUniformLoc, 1, glm::value_ptr(m_clippingPlane));
    }
    m_shader->releasing();
}

void BaseRaycastingShader::getZDepthFrontFromRayCasting(int wX, int wY, glm::vec4 viewport, float &depthZFront)
{
    vgf::setViewport(viewport);

    getRayTracedFBO().binding();

    glReadPixels(wX, wY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZFront);

    getRayTracedFBO().releasing();
}

void BaseRaycastingShader::copyData(BaseRaycastingShader *raycasting_shader)
{
    setFBOSizeFactor(raycasting_shader->m_FBOSizeFactor);
    setBFixedLowSteps(raycasting_shader->m_bFixedLowSteps);
    setBEnableFixedLowSteps(raycasting_shader->m_bEnableFixedLowSteps);
    setISteps(raycasting_shader->m_iSteps);
    setEnableMPR(raycasting_shader->m_enableMPR);
    setClippingPlane(raycasting_shader->m_clippingPlane);

    setStateThreshold(raycasting_shader->m_stateThreshold);
    setNoiseThreshold(raycasting_shader->m_noiseThreshold);

    setMVP(raycasting_shader->m_MVP);

    resize(raycasting_shader->m_width, raycasting_shader->m_height);
}

void BaseRaycastingShader::highlightPlanes(float slice_coord_tex[3])
{
    if(m_highlightedPlanesUniformLoc < 0)
        return;

    m_shader->binding();
    {
        glUniform3fv(m_highlightedPlanesUniformLoc, 1, slice_coord_tex);
    }
    m_shader->releasing();
}

void BaseRaycastingShader::highlightPlanesActivated(bool state)
{
    if(m_highlightedPlanesActivatedUniformLoc < 0)
        return;

    m_shader->binding();
    {
        if(state)
            glUniform1i(m_highlightedPlanesActivatedUniformLoc, 1);
        else
            glUniform1i(m_highlightedPlanesActivatedUniformLoc, 0);
    }
    m_shader->releasing();
}
