#include "FiberTrackingAlgorithmComputeShader.h"

#include <limits>
#include <algorithm>
#include <bitset>
#include <cmath>
#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"
#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"
#include "utilities/QCheckingOpenGL.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

FiberTrackingAlgorithmComputeShader::FiberTrackingAlgorithmComputeShader(const DTI *dti):
    FiberTrackingAlgorithm(dti),
    m_vao(0)
{
    mProgramFiberTracking = nullptr;

    mAttribLocPosition = 0;
    mTexBufferIdSeedPosition = 0;
    mTexBufferIdFiberInitIndex = 0;
    mTexBufferIdFiberVertices = 0;
    mTexBufferIdFiberDirections = 0;

}

template<typename T>
void FiberTrackingAlgorithmComputeShader::deleteGLBuffer(T *id_buffer)
{
    if(*id_buffer != 0)
    {
        glDeleteBuffers(1, id_buffer);
        *id_buffer = 0;
    }
}

template<typename T>
void FiberTrackingAlgorithmComputeShader::deleteGLTexture(T *id_texture)
{
    if(*id_texture != 0)
    {
        glDeleteTextures(1, id_texture);
        *id_texture = 0;
    }
}

FiberTrackingAlgorithmComputeShader::~FiberTrackingAlgorithmComputeShader()
{
    reset();

    if(mProgramFiberTracking != nullptr)
        delete mProgramFiberTracking;

    deleteGLBuffer(&mVBOCreatedSeeds);
    deleteGLBuffer(&mTransformFeedbackVBO);
    deleteGLTexture(&mTexBufferIdSeedPosition);
    deleteGLTexture(&mTexBufferIdFiberInitIndex);
    deleteGLTexture(&mTexBufferIdFiberVertices);
    deleteGLTexture(&mTexBufferIdFiberDirections);

    if(m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);
}

void FiberTrackingAlgorithmComputeShader::initialize()
{
    GLint value;
    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &value);
    std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES " << value << std::endl;

    glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &value);
    std::cout << "GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS " << value << std::endl;

    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &value);
    std::cout << "GL_MAX_GEOMETRY_OUTPUT_COMPONENTS " << value << std::endl;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &value);
    std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE 0 " << value << std::endl;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &value);
    std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE 1 " << value << std::endl;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &value);
    std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE 2 " << value << std::endl;

    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
    std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS " << value << std::endl;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    initialize_seed();

    //#######################################################################################################################################

    QManagementResource *comp = new QManagementResource;

    GLuint compute = comp->compileResourceFileShader(QManagementResource::compute, ":/fiber_tracking_comp");

    mProgramFiberTracking = new vmtkManagementShader;
    mProgramFiberTracking->setShader(compute);
    mProgramFiberTracking->linking();

    delete comp;

    //---------------------------------------------------------------------------------------------------------------------------------------

    mProgramFiberTracking->binding();
    {
        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "maxSteps"), getFiberTrackingMaxSteps());
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionFA"), getFiberTrackingStoppingCriterionFA());
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionAngleCos"), getFiberTrackingStoppingCriterionAngleCos());
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "w_punct"), getFiberTrackingWpunct());

        glUniform3fv(glGetUniformLocation(mProgramFiberTracking->getProgram(), "phy_dimension"), 1, m_phyDim);

        setFiberStepFiberTracking();

        glGenTextures(1, &mTexBufferIdSeedPosition);
        glGenTextures(1, &mTexBufferIdFiberInitIndex);
        glGenTextures(1, &mTexBufferIdFiberVertices);
        glGenTextures(1, &mTexBufferIdFiberDirections);
    }
    mProgramFiberTracking->releasing();
}

FiberClusterSet::sPtr FiberTrackingAlgorithmComputeShader::startFiberTracking()
{
    if(mNumCreatedSeeds != 0)
    {
        glBindVertexArray(m_vao);

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
        auto start1 = std::chrono::steady_clock::now();
#endif

        filterSeeds();

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
        auto end1 = std::chrono::steady_clock::now();

        GLuint query;
        GLuint64 elapsed_time;
        int done = 0;

        glGenQueries(1, &query);
        glBeginQuery(GL_TIME_ELAPSED,query);

        auto start2 = std::chrono::steady_clock::now();
#endif

        createFibers();

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
        auto end2 = std::chrono::steady_clock::now();

        glEndQuery(GL_TIME_ELAPSED);

        // retrieving the recorded elapsed time
        // wait until the query result is available
        while (!done) {
            glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &done);
        }

        // get the query result
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
        std::cout << "Time Elapsed query: " << (elapsed_time / 1000000.0) << " ms - mean " << std::endl;

        query_time = (elapsed_time / 1000000.0);

        preprocessing_time = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
        track_time = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
#endif

        glBindVertexArray(0);

        return mFiberClusterVector;

    } else {

        return std::make_shared<FiberClusterSet>();

    }
}

void FiberTrackingAlgorithmComputeShader::setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle)
{
    FiberTrackingAlgorithm::setStoppingCriterionAngle(fiberTrackingStoppingCriterionAngle);

    mProgramFiberTracking->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionAngleCos"), getFiberTrackingStoppingCriterionAngleCos());
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setWpunct(float value)
{
    setFiberTrackingWpunct(value);

    mProgramFiberTracking->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "w_punct"), getFiberTrackingWpunct());
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA)
{
    FiberTrackingAlgorithm::setStoppingCriterionFA(fiberTrackingStoppingCriterionFA);

    mProgramFiberTracking->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionFA"), getFiberTrackingStoppingCriterionFA());
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setMaxSteps(unsigned int fiberTrackingMaxSteps)
{
    FiberTrackingAlgorithm::setMaxSteps(fiberTrackingMaxSteps);

    mProgramFiberTracking->binding();
    {
        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "maxSteps"), getFiberTrackingMaxSteps());
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::createFibers()
{
    mProgramFiberTracking->binding();
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);
        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "tensorCoeffPart01"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart02);
        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "tensorCoeffPart02"), 3);

        for(unsigned short i = 0; i < (*mFiberClusterVector).size(); i++)
        {
//            std::cout << "###################################################################################################################" << std::endl;

            unsigned int num_seed_positions = (*mFiberClusterVector)[i].getNumSeeds();
            unsigned int num_remaining_fiber_segments_points = (*mFiberClusterVector)[i].getNumFiberSegmentPoints();
            FiberCluster::MyVec4 *seed_positions = (*mFiberClusterVector)[i].getSeedPositions();
            unsigned int fiber_buffer_init_index[num_seed_positions];
            unsigned int acc_index = 0;

            for(unsigned int j = 0; j < num_seed_positions; j++)
            {
                fiber_buffer_init_index[j] = acc_index;
                acc_index += (*mFiberClusterVector)[i].getNumVerticesPerFiber()[j];
            }

            glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "numSeed"), num_seed_positions);

            GLuint fiber_buffer_init_index_vbo;

            GLuint VBOSeeds;
            GLuint VBOFiberVertices;
            GLuint VBOFiberDirections;

            glGenBuffers(1, &VBOSeeds);
            glGenBuffers(1, &VBOFiberVertices);
            glGenBuffers(1, &VBOFiberDirections);

            glGenBuffers(1, &fiber_buffer_init_index_vbo);

//            std::cout << "===================================================================================================================" << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, VBOSeeds);
            {
                glBufferData(GL_ARRAY_BUFFER, num_seed_positions * sizeof(GLfloat) * 4, seed_positions, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdSeedPosition);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBOSeeds);
                glBindImageTexture(1, mTexBufferIdSeedPosition, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, fiber_buffer_init_index_vbo);
            {
                glBufferData(GL_ARRAY_BUFFER, num_seed_positions * sizeof(GLint), fiber_buffer_init_index, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdFiberInitIndex);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, fiber_buffer_init_index_vbo);
                glBindImageTexture(2, mTexBufferIdFiberInitIndex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32I);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBOFiberVertices);
            {
                glBufferData(GL_ARRAY_BUFFER, num_remaining_fiber_segments_points * sizeof(GLfloat) * 4, NULL, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdFiberVertices);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBOFiberVertices);
                glBindImageTexture(3, mTexBufferIdFiberVertices, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBOFiberDirections);
            {
                glBufferData(GL_ARRAY_BUFFER, num_remaining_fiber_segments_points * sizeof(GLfloat) * 4, NULL, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdFiberDirections);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBOFiberDirections);
                glBindImageTexture(4, mTexBufferIdFiberDirections, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            unsigned short num_total_work_groups = num_seed_positions / (8 * 8);
            if((num_seed_positions % (8 * 8)) != 0)
                num_total_work_groups++;

            unsigned short num_work_groups_x = std::floor(std::sqrt(num_total_work_groups));
            unsigned short num_work_groups_y = num_work_groups_x;
            unsigned short num_remaining_work_groups = num_total_work_groups - (num_work_groups_x * num_work_groups_y);

            if(num_remaining_work_groups > 0)
            {
                num_work_groups_x += (num_remaining_work_groups / num_work_groups_y);

                if((num_remaining_work_groups % num_work_groups_y) != 0)
                    num_work_groups_x++;
            }

            glDispatchCompute(num_work_groups_x, num_work_groups_y, 1);

            glDeleteBuffers(1, &VBOSeeds);
            glDeleteBuffers(1, &fiber_buffer_init_index_vbo);

            (*mFiberClusterVector)[i].setVBOFiberDirections(VBOFiberDirections);
            (*mFiberClusterVector)[i].setVBOFiberVertices(VBOFiberVertices);
        }
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setFiberStepFiberTracking()
{
    float fiber_step;
    float * voxelDim = getVoxelDim();

    if(voxelDim[0] < voxelDim[1])
    {
        if(voxelDim[0] < voxelDim[2])
            fiber_step = voxelDim[0];
        else
            fiber_step = voxelDim[2];

    } else if(voxelDim[1] < voxelDim[2])
        fiber_step = voxelDim[1];
    else
        fiber_step = voxelDim[2];

    fiber_step = fiber_step / 4.f;

    setFiberStep(fiber_step);

    mProgramFiberTracking->binding();
    GLshort direction_factor_loc = glGetUniformLocation(mProgramFiberTracking->getProgram(), "fiberStep");
    glUniform1f(direction_factor_loc, fiber_step);
}


