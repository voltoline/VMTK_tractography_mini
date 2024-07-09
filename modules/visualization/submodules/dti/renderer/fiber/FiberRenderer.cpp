#include "FiberRenderer.h"

#include <cstdlib>
#include "modules/visualization/submodules/dti/models/tensor_fitter/DTI.h"
#include <glm/gtc/type_ptr.hpp>
#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

FiberRenderer::FiberRenderer(const DTI *dti, const float *scaleFactors):
    m_vao(0),
    m_initialized(false)
{
    m_dti = dti;

    m_scale_factors[0] = scaleFactors[0];
    m_scale_factors[1] = scaleFactors[1];
    m_scale_factors[2] = scaleFactors[2];

    m_projectionMatrix = glm::mat4(1.0f);
    m_modelViewMatrix = glm::mat4(1.0f);

    mFiberRenderingProgram = nullptr;

    seed_position_attr_loc = -1;
    fiber_point_positions_loc = -1;
    fiber_point_direction_loc = -1;
}

FiberRenderer::~FiberRenderer()
{
    if(m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);
}

void FiberRenderer::initialize(){

    if(!m_initialized)
    {
        if(m_dti == NULL)
            return;

        m_initialized = true;

        set3DViewMatrix();

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        QManagementResource *vs = new QManagementResource;
        QManagementResource *fs = new QManagementResource;

        //#######################################################################################################################################

        vs = new QManagementResource;
        fs = new QManagementResource;

        GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, ":/render_fiber_vert");
        GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment, ":/diff_particles_frag");

        mFiberRenderingProgram = new vmtkManagementShader;
        mFiberRenderingProgram->setShader(vertex);
        mFiberRenderingProgram->setShader(fragment);
        mFiberRenderingProgram->linking();
        std::cout << "mTractRenderingProgram->linking " << glGetError() << std::endl;

        delete vs;
        delete fs;

        //#######################################################################################################################################

        mFiberRenderingProgram->binding();
        {
            GLshort reorientMatrixLoc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "reorient_matrix");
            glUniformMatrix4fv(reorientMatrixLoc, 1, GL_FALSE, m_reorient_point_matrix);

            fiber_point_positions_loc = glGetAttribLocation(mFiberRenderingProgram->getProgram(), "position");
            fiber_point_direction_loc = glGetAttribLocation(mFiberRenderingProgram->getProgram(), "direction");

            GLshort projectionMatrixLoc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

            GLshort mvMatrixLoc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "modelViewMatrix");
            glUniformMatrix4fv(mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelViewMatrix));

            GLshort step_range_loc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "step_range");
            int step_range[2] = {0, 0};
            glUniform2iv(step_range_loc, 1, step_range);

            GLshort paint_activated_loc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "is_paint_activated");
            glUniform1i(paint_activated_loc, 0);
        }
        mFiberRenderingProgram->releasing();
    }

}

void FiberRenderer::render()
{
    if(!m_initialized)
    {
        std::cerr << "DiffLinesRenderTexCoordFBO::renderTexCoord ->> It was not initialized!" << std::endl;
        initialize();
        if(!m_initialized)
            return;
    }

    glBindVertexArray(m_vao);

    renderFibers();

    glBindVertexArray(0);
}

template<typename T>
void FiberRenderer::deleteBuffer(T **buffer)
{
    if(*buffer != nullptr)
    {
        delete[] *buffer;
        *buffer = nullptr;
    }
}

void FiberRenderer::reset()
{
    for(unsigned short j = 0; j < m_fiberRenderingData.size(); j++)
    {
        deleteBuffer(&m_fiberRenderingData[j].m_VBOFiberVertices);
        deleteBuffer(&m_fiberRenderingData[j].m_VBOFiberDirections);

        while(m_fiberRenderingData[j].m_vecMultiDrawArrayInfo.size() > 0)
        {
            delete[] m_fiberRenderingData[j].m_vecMultiDrawArrayInfo[0].first;
            delete[] m_fiberRenderingData[j].m_vecMultiDrawArrayInfo[0].count;
            m_fiberRenderingData[j].m_vecMultiDrawArrayInfo.erase(m_fiberRenderingData[j].m_vecMultiDrawArrayInfo.begin());
        }
    }

    m_fiberRenderingData.clear();

    setFiberStepRange(0, 0);
}

void FiberRenderer::setFiberClusters(std::vector<FiberClusterSet::sPtr> fiber_cluster_set)
{
    reset();

    if(fiber_cluster_set.size() == 0)
        return;

    for(unsigned short j = 0; j < fiber_cluster_set.size(); j++)
    {
        m_fiberRenderingData.push_back(FiberRenderingData());

        if(fiber_cluster_set[j] == nullptr)
            continue;

        m_fiberRenderingData[j].m_numFiberGroups = fiber_cluster_set[j]->size();
        m_fiberRenderingData[j].m_VBOFiberVertices = new GLuint[m_fiberRenderingData[j].m_numFiberGroups];
        m_fiberRenderingData[j].m_VBOFiberDirections = new GLuint[m_fiberRenderingData[j].m_numFiberGroups];

        for(unsigned short i = 0; i < m_fiberRenderingData[j].m_numFiberGroups; i++)
        {
            m_fiberRenderingData[j].m_VBOFiberVertices[i] = (*fiber_cluster_set[j])[i].getVBOFiberVertices();
            m_fiberRenderingData[j].m_VBOFiberDirections[i] = (*fiber_cluster_set[j])[i].getVBOFiberDirections();

            if(m_fiberRenderingData[j].m_VBOFiberVertices[i] == 0)
            {
                glGenBuffers(1, &m_fiberRenderingData[j].m_VBOFiberVertices[i]);
                glBindBuffer(GL_ARRAY_BUFFER, m_fiberRenderingData[j].m_VBOFiberVertices[i]);
                glBufferData(GL_ARRAY_BUFFER,
                             (*fiber_cluster_set[j])[i].getNumFiberSegmentPoints() * sizeof(GLfloat) * 4,
                             (*fiber_cluster_set[j])[i].getFiberVertices(), GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            unsigned int num_fibers = (*fiber_cluster_set[j])[i].getNumFibers();
            unsigned int count_first = 0;
            MultiDrawArrayInfo draw_info;
            draw_info.first = new GLint[num_fibers];
            draw_info.count = new GLint[num_fibers];
            draw_info.primcount = num_fibers;

            for(unsigned int k = 0; k < num_fibers; k++)
            {
                draw_info.first[k] = count_first;
                draw_info.count[k] = (*fiber_cluster_set[j])[i].getNumVerticesPerFiber()[k];
                count_first += draw_info.count[k];
            }

            m_fiberRenderingData[j].m_vecMultiDrawArrayInfo.push_back(draw_info);
        }
    }

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
    acc_time = 0;
    count_acc = 0;
    acc_query = 0;
#endif

}

void FiberRenderer::setFiberStepRange(int min, int max)
{
    if(mFiberRenderingProgram)
    {
        mFiberRenderingProgram->binding();

        GLshort step_range_loc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "step_range");
        int step_range[2] = {min, max};
        glUniform2iv(step_range_loc, 1, step_range);

        mFiberRenderingProgram->releasing();
    }

}

void FiberRenderer::setFiberPaintingActivated(bool state)
{
    if(mFiberRenderingProgram)
    {
        mFiberRenderingProgram->binding();

        GLshort paint_activated_loc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "is_paint_activated");
        glUniform1i(paint_activated_loc, state);

        mFiberRenderingProgram->releasing();
    }
}

void FiberRenderer::renderFibers()
{
    mFiberRenderingProgram->binding();

    glEnable(GL_DEPTH_TEST);

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
    GLuint query;
    GLuint64 elapsed_time;
    int done = 0;

    glGenQueries(1, &query);
    glBeginQuery(GL_TIME_ELAPSED,query);

    auto start1 = std::chrono::steady_clock::now();
#endif

    for(unsigned short j = 0; j < m_fiberRenderingData.size(); j++)
    {
        if(m_fiberRenderingData[j].m_numFiberGroups != 0)
        {
            for(unsigned short i = 0; i < m_fiberRenderingData[j].m_numFiberGroups; i++)
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_fiberRenderingData[j].m_VBOFiberVertices[i]);
                glVertexAttribPointer(fiber_point_positions_loc, 4, GL_FLOAT, false, 4 * sizeof(float), BUFFER_OFFSET(0));
                glEnableVertexAttribArray(fiber_point_positions_loc);


                glBindBuffer(GL_ARRAY_BUFFER, m_fiberRenderingData[j].m_VBOFiberDirections[i]);
                glVertexAttribPointer(fiber_point_direction_loc, 4, GL_FLOAT, false, 4 * sizeof(float), BUFFER_OFFSET(0));
                glEnableVertexAttribArray(fiber_point_direction_loc);

                glMultiDrawArrays(GL_LINE_STRIP, m_fiberRenderingData[j].m_vecMultiDrawArrayInfo[i].first, m_fiberRenderingData[j].m_vecMultiDrawArrayInfo[i].count, m_fiberRenderingData[j].m_vecMultiDrawArrayInfo[i].primcount);

                glDisableVertexAttribArray(fiber_point_positions_loc);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }
    }
#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
    auto end1 = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
    acc_time += elapsed;
    count_acc++;

    glEndQuery(GL_TIME_ELAPSED);

    // retrieving the recorded elapsed time
    // wait until the query result is available
    while (!done) {
        glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &done);
    }

    // get the query result
    glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
    acc_query += (elapsed_time / 1000000.0);
    std::cout << "Time Elapsed query: " << (elapsed_time / 1000000.0) << " ms - mean " << (acc_query / count_acc) << " ms " << std::endl;
    std::cout << "Time Elapsed chrono: " << elapsed << " ms - mean " << (acc_time / count_acc) << " ms - count " << count_acc << " - acc" << acc_time << std::endl;
#endif

    glDisable(GL_DEPTH_TEST);

    mFiberRenderingProgram->releasing();
}

void FiberRenderer::resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void FiberRenderer::setModelViewMatrix(glm::mat4 modelView)
{
    m_modelViewMatrix = modelView;

    if(m_initialized)
    {
        mFiberRenderingProgram->binding();
        {
            GLshort mvMatrixLoc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "modelViewMatrix");
            glUniformMatrix4fv(mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelViewMatrix));
        }
        mFiberRenderingProgram->releasing();
    }
}

void FiberRenderer::setProjectionMatrix(glm::mat4 projection)
{
    m_projectionMatrix = projection;

    if(m_initialized)
    {
        mFiberRenderingProgram->binding();
        {
            GLshort projectionMatrixLoc = glGetUniformLocation(mFiberRenderingProgram->getProgram(), "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
        }
        mFiberRenderingProgram->releasing();
    }
}

void FiberRenderer::set3DViewMatrix()
{
    m_reorient_point_matrix_translation.setTranslation(-0.5f, -0.5f, -0.5f);
    m_reorient_point_matrix_scale.setScale(2.f * m_scale_factors[0] , 2.f * m_scale_factors[1], 2.f * m_scale_factors[2]);
    m_reorient_point_matrix_rotation.identity();

    m_reorient_point_matrix = m_reorient_point_matrix_rotation * m_reorient_point_matrix_scale * m_reorient_point_matrix_translation;
}
