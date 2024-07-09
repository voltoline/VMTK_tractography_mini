#include "FiberTrackingAlgorithmComputeShader.h"

#include <limits>
#include <algorithm>
#include <bitset>
#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"
//#include "model/dicom/vmtkDiffAcquisition.h"
#include <modules/file_reader/dicom/vmtkDiffAcquisition.h>
//#include "model/NiftiFileLoader3DAcquisition.h"

FiberTrackingAlgorithmComputeShader::FiberTrackingAlgorithmComputeShader(vmtkDiffAcquisition *diff_acq, const float *scaleFactors):
    mSeedPositions(nullptr),
    mSeedInfo(nullptr),
    mFilterNOTPositions(nullptr),
    mFilterANDPositions(nullptr),
    m_vao(0)
{
    mDiffusionAcquisition = diff_acq;

    mProgramSeedFiltering = nullptr;
    mProgramFiberTracking = nullptr;

    mVoxelDim[0] = 1.f / mDiffusionAcquisition->getDimensions(0);
    mVoxelDim[1] = 1.f / mDiffusionAcquisition->getDimensions(1);
    mVoxelDim[2] = 1.f / mDiffusionAcquisition->getDimensions(2);

    mFiberTrackingMaxSteps = 1000;
    mFiberTrackingStoppingCriterionFA = 0.15f;
    mFiberTrackingStoppingCriterionAngle = 11.f;
    mFiberTrackingStoppingCriterionAngleCos =  std::cos((mFiberTrackingStoppingCriterionAngle * 3.14159265) / 180.0);
    mFiberTrackingMinSteps = 100;

    mMaxNumFiberSegmentPointsPerBuffer = 2000000;

    Attributes& sharedAttr = GlobalDataBase::getInstance()->getDataBase()[static_cast<unsigned short>(mDiffusionAcquisition->getId())];
    mTexIdFractionalAnisotropy = sharedAttr.get<GLuint>("tex_id_fa");
    mTexIdMainDiffusionDirection = sharedAttr.get<GLuint>("tex_id_main_diff_vector");
    *mTexIdFractionalAnisotropy = 0;
    *mTexIdMainDiffusionDirection = 0;

    mVBOCreatedSeeds = 0;
    mTransformFeedbackVBO = 0;
    mAttribLocPosition = 0;

    //mAttribFilterNOTLocPosition = 0;
    //mAttribFilterANDLocPosition = 0;
    //mVBOFilterAND = 0;
    //mVBOFilterAND = 0;
    //mFilterNotTex = 0;
    //mFilterAndTex = 0;


    mTexIdTensorCoefficientPart01 = 0;
    mTexIdTensorCoefficientPart02 = 0;
    mTexBufferIdSeedPosition = 0;
    mTexBufferIdFiberInitIndex = 0;
    mTexBufferIdFiberVertices = 0;
    mTexBufferIdFiberDirections = 0;

    reset();

}

template<typename T>
void FiberTrackingAlgorithmComputeShader::deleteBuffer(T **buffer)
{
    if(*buffer != nullptr)
    {
        delete[] *buffer;
        *buffer = nullptr;
    }
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

void FiberTrackingAlgorithmComputeShader::reset()
{
    mNumCreatedSeeds = 0;
    mNumFilteredSeed = 0;
    mNumFiberSegmentPoints = 0;

    deleteBuffer(&mSeedPositions);
    deleteBuffer(&mSeedInfo);

    if(mFiberClusterVector != nullptr)
        mFiberClusterVector->eraseAll();

    mFiberClusterVector = std::make_shared<FiberClusterSet>();
    float PhysDim[3];
    PhysDim[0] = static_cast<float>(mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getPixelSpacing(0));
    PhysDim[1] = static_cast<float>(mDiffusionAcquisition->getDimensions(1) * mDiffusionAcquisition->getPixelSpacing(1));
    PhysDim[2] = static_cast<float>(mDiffusionAcquisition->getDimensions(2) * mDiffusionAcquisition->getPixelSpacing(2));
    mFiberClusterVector->setPhysicalDimension(PhysDim);

//    while(mFiberClusterVector.size() > 0)
//    {
//        mFiberClusterVector[0].deleteBuffers();
//        mFiberClusterVector.erase(mFiberClusterVector.begin());
//    }
}

//void FiberTrackingAlgorithmComputeShader::resetFilterNOT()
//{
//    mSizeFilterNOT = 0;
//    mNumFilterNOT = 0;
//    deleteBuffer(&mFilterNOTPositions);
//}

//void FiberTrackingAlgorithmComputeShader::resetFilterAND()
//{
//    mSizeFilterAND = 0;
//    mNumFilterAND = 0;
//    deleteBuffer(&mFilterANDPositions);
//}

void FiberTrackingAlgorithmComputeShader::resetFilterSize()
{
    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), 0);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), 0);
}

FiberTrackingAlgorithmComputeShader::~FiberTrackingAlgorithmComputeShader()
{
    reset();

    if(mProgramFiberTracking != nullptr)
        delete mProgramFiberTracking;

    if(mProgramSeedFiltering != nullptr)
        delete mProgramSeedFiltering;

    deleteGLBuffer(&mVBOCreatedSeeds);
    //deleteGLBuffer(&mVBOFilterAND);
    //deleteGLBuffer(&mVBOFilterNOT);
    //deleteGLBuffer(&mFilterNotTex);
    //deleteGLBuffer(&mFilterAndTex);
    deleteGLBuffer(&mTransformFeedbackVBO);
    deleteGLTexture(&mTexBufferIdSeedPosition);
    deleteGLTexture(&mTexBufferIdFiberInitIndex);
    deleteGLTexture(&mTexBufferIdFiberVertices);
    deleteGLTexture(&mTexBufferIdFiberDirections);
    deleteGLTexture(&mTexIdTensorCoefficientPart01);
    deleteGLTexture(&mTexIdTensorCoefficientPart02);

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

    QManagementResource *vs = new QManagementResource;

    //#######################################################################################################################################

    vs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, ":/seed_filtering_vert");

    std::cout << "mSeedFilteringProgram->linking BEFORE " << glGetError() << std::endl;
    mProgramSeedFiltering = new vmtkManagementShader;
    mProgramSeedFiltering->setShader(vertex);
    mProgramSeedFiltering->linking();
    std::cout << "mSeedFilteringProgram->linking " << glGetError() << std::endl;

    const char *varying_names_filter[] = {"fiberSeedPos", "fiberFinalLength", "fiberEnable"};
    glTransformFeedbackVaryings(mProgramSeedFiltering->getProgram(), 3, varying_names_filter, GL_INTERLEAVED_ATTRIBS);
    std::cout << "glTransformFeedbackVaryings " << glGetError() << std::endl;

    glLinkProgram(mProgramSeedFiltering->getProgram());
    std::cout << "mSeedFilteringProgram->linking() " << glGetError() << std::endl;

    delete vs;

    //---------------------------------------------------------------------------------------------------------------------------------------

    mProgramSeedFiltering->binding();
    {
        mAttribLocPosition = glGetAttribLocation(mProgramSeedFiltering->getProgram(), "seed_position");
//        mAttribFilterNOTLocPosition = glGetAttribLocation(mProgramSeedFiltering->getProgram(), "filter_not_position");
//        mAttribFilterANDLocPosition = glGetAttribLocation(mProgramSeedFiltering->getProgram(), "filter_and_position");

        glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionAngleCos"), mFiberTrackingStoppingCriterionAngleCos);

        glGenBuffers(1, &mVBOCreatedSeeds);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOCreatedSeeds);
        {
            glVertexAttribPointer(mAttribLocPosition, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(mAttribLocPosition);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

//        glGenTextures(1, &mFilterNotTex);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_BUFFER, mFilterNotTex);

//        glGenTextures(1, &mFilterAndTex);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_BUFFER, mFilterAndTex);

//        glGenBuffers(1, &mVBOFilterAND);


        glGenBuffers(1, &mTransformFeedbackVBO);
        std::cout << "glUniformMatrix4fv " << glGetError() << std::endl;

        createTensorImages();
        resetFilterSize();
    }
    mProgramSeedFiltering->releasing();

    //#######################################################################################################################################

    QManagementResource *comp = new QManagementResource;

    GLuint compute = comp->compileResourceFileShader(QManagementResource::compute, ":/fiber_tracking_comp");

    mProgramFiberTracking = new vmtkManagementShader;
    mProgramFiberTracking->setShader(compute);
    mProgramFiberTracking->linking();

    delete comp;

    std::cout << "mFiberTrackingProgram " << glGetError() << std::endl;

    //---------------------------------------------------------------------------------------------------------------------------------------

    mProgramFiberTracking->binding();
    {
        std::cout << "glBindVertexArray" << glGetError() << std::endl;

        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionAngleCos"), mFiberTrackingStoppingCriterionAngleCos);

        setFiberStep();
        setDimensions();

        std::cout << "createMainDirectionTex " << glGetError() << std::endl;

        glGenTextures(1, &mTexBufferIdSeedPosition);
        glGenTextures(1, &mTexBufferIdFiberInitIndex);
        glGenTextures(1, &mTexBufferIdFiberVertices);
        glGenTextures(1, &mTexBufferIdFiberDirections);
    }
    mProgramFiberTracking->releasing();
}

std::shared_ptr<FiberClusterSet> FiberTrackingAlgorithmComputeShader::startFiberTracking()
{
//    std::string text = "fiber_tracking";
//    unsigned int msg_mark_id = 3;
//    //        std::cout << text << " " << "msg_mark_id " << msg_mark_id << std::endl;
////    std::cout << "=========================" << text <<  "========================="  << std::endl;

//    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, msg_mark_id, text.length(), text.c_str());

    if(mNumCreatedSeeds != 0)
    {
        glBindVertexArray(m_vao);

        filterSeeds();
        createFibers();

        glBindVertexArray(0);

//        glPopDebugGroup();
        return mFiberClusterVector;

    } else {

//        glPopDebugGroup();
        return std::make_shared<FiberClusterSet>();

    }
}

void FiberTrackingAlgorithmComputeShader::setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle)
{
    mFiberTrackingStoppingCriterionAngle = fiberTrackingStoppingCriterionAngle;
    mFiberTrackingStoppingCriterionAngleCos =  std::cos((mFiberTrackingStoppingCriterionAngle * 3.14159265) / 180.0);

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionAngleCos"), mFiberTrackingStoppingCriterionAngleCos);
    }
    mProgramSeedFiltering->releasing();

    mProgramFiberTracking->binding();
    {
        std::cout << "setStoppingCriterionAngle " << mFiberTrackingStoppingCriterionAngleCos << " err " << glGetError() << std::endl;
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionAngleCos"), mFiberTrackingStoppingCriterionAngleCos);
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA)
{
    mFiberTrackingStoppingCriterionFA = fiberTrackingStoppingCriterionFA;

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
    }
    mProgramSeedFiltering->releasing();

    mProgramFiberTracking->binding();
    {
        std::cout << "setStoppingCriterionFA err " << glGetError() << std::endl;
        glUniform1f(glGetUniformLocation(mProgramFiberTracking->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setMinSteps(unsigned int fiberTrackingMinSteps)
{
    mFiberTrackingMinSteps = fiberTrackingMinSteps;
}

void FiberTrackingAlgorithmComputeShader::setMaxSteps(unsigned int fiberTrackingMaxSteps)
{
    mFiberTrackingMaxSteps = fiberTrackingMaxSteps;

    mProgramSeedFiltering->binding();
    {
        glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
    }
    mProgramSeedFiltering->releasing();

    mProgramFiberTracking->binding();
    {
        std::cout << "setMaxSteps err" << glGetError() << std::endl;
        glUniform1i(glGetUniformLocation(mProgramFiberTracking->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::createTensorImages()
{
    unsigned int num_voxels = mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getDimensions(1) * mDiffusionAcquisition->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = mDiffusionAcquisition->getDTI()->getTensorCoefficients();

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[0];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[1];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[2];
        tensor_coeff_buffer[i].w = 0.0;
    }

    glGenTextures(1, &mTexIdTensorCoefficientPart01);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 mDiffusionAcquisition->getDimensions(0),
                 mDiffusionAcquisition->getDimensions(1),
                 mDiffusionAcquisition->getDimensions(2), 0,
                 GL_RGBA,
                 GL_FLOAT,
                 tensor_coeff_buffer);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_3D, 0);

    std::cout << "mTextureIDTensorCoefficientPart01 " << glGetError() << std::endl;

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[3];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[4];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[5];
        tensor_coeff_buffer[i].w = 0.0;
    }

    glGenTextures(1, &mTexIdTensorCoefficientPart02);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart02);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 mDiffusionAcquisition->getDimensions(0),
                 mDiffusionAcquisition->getDimensions(1),
                 mDiffusionAcquisition->getDimensions(2), 0,
                 GL_RGBA,
                 GL_FLOAT,
                 tensor_coeff_buffer);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] tensor_coeff_buffer;

    std::cout << "mTextureIDTensorCoefficientPart02 " << glGetError() << std::endl;
}

void FiberTrackingAlgorithmComputeShader::setFilterOnTexture(int * filter_not_indexes, int size_not, int * filter_and_indexes, int size_and)
{
    resetFilterSize();

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), size_not);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), size_and);


    unsigned int num_voxels = mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getDimensions(1) * mDiffusionAcquisition->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = mDiffusionAcquisition->getDTI()->getTensorCoefficients();

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[0];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[1];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[2];
        tensor_coeff_buffer[i].w = 0.0;
    }
    int dim_x = mDiffusionAcquisition->getDimensions(0);
    int dim_y = mDiffusionAcquisition->getDimensions(1);
    //int dim_z = mDiffusionAcquisition->getDimensions(2);
    //Assign Not Indexes
    for(unsigned int i = 0; i < size_not; i++)
    {
        unsigned int index = filter_not_indexes[i*3] + filter_not_indexes[i*3 + 1]*dim_x + filter_not_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(1.0);
    }

    //Assign And Indexes
    for(unsigned int i = 0; i < size_and; i++)
    {
        int index = filter_and_indexes[i*3] + filter_and_indexes[(i*3) + 1]*dim_x + filter_and_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(2.0);
    }

//    glGenTextures(1, &mTexIdTensorCoefficientPart01);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 mDiffusionAcquisition->getDimensions(0),
                 mDiffusionAcquisition->getDimensions(1),
                 mDiffusionAcquisition->getDimensions(2), 0,
                 GL_RGBA,
                 GL_FLOAT,
                 tensor_coeff_buffer);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] tensor_coeff_buffer;

    std::cout << "mTextureIDTensorCoefficientPart01 " << glGetError() << std::endl;
}

void FiberTrackingAlgorithmComputeShader::setFilterOnTexture(int * filter_not_indexes, int size_not, int * number_filter_not, int size_number_filter_not,
                                                             int * filter_and_indexes, int size_and, int * number_filter_and, int size_number_filter_and)
{
    resetFilterSize();

    std::cout << "HERE - Num filter AND: " << size_number_filter_and << std::endl;
    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), size_number_filter_not);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), size_number_filter_and);


    unsigned int num_voxels = mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getDimensions(1) * mDiffusionAcquisition->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = mDiffusionAcquisition->getDTI()->getTensorCoefficients();

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[0];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[1];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[2];
        tensor_coeff_buffer[i].w = 0.0;
    }
    int dim_x = mDiffusionAcquisition->getDimensions(0);
    int dim_y = mDiffusionAcquisition->getDimensions(1);
    //int dim_z = mDiffusionAcquisition->getDimensions(2);
    //Assign Not Indexes
    for(unsigned int i = 0; i < size_not; i++)
    {
        unsigned int index = filter_not_indexes[i*3] + filter_not_indexes[i*3 + 1]*dim_x + filter_not_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(1.0);
    }

    //Assign And Indexes
    int and_value = 2;
    int current_index_and = 0;
    int current_limit = number_filter_and[current_index_and];
    for(unsigned int i = 0; i < size_and; i++)
    {
        int index = filter_and_indexes[i*3] + filter_and_indexes[(i*3) + 1]*dim_x + filter_and_indexes[(i*3) + 2]*dim_x*dim_y;
        if(i > current_limit)
        {
            and_value = and_value << 1;
//            std::cout << "Change AND Value to: " << and_value << std::endl;
            current_index_and++;
            current_limit = current_limit + number_filter_and[current_index_and];
        }
        tensor_coeff_buffer[index].w = static_cast<float>(and_value);
    }

//    glGenTextures(1, &mTexIdTensorCoefficientPart01);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 mDiffusionAcquisition->getDimensions(0),
                 mDiffusionAcquisition->getDimensions(1),
                 mDiffusionAcquisition->getDimensions(2), 0,
                 GL_RGBA,
                 GL_FLOAT,
                 tensor_coeff_buffer);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] tensor_coeff_buffer;

    std::cout << "mTextureIDTensorCoefficientPart01 " << glGetError() << std::endl;
}

void FiberTrackingAlgorithmComputeShader::setSeeds(float *seed_buffer, unsigned int num_seeds)
{
    reset();

    mNumCreatedSeeds = num_seeds;
    mSeedPositions = new MyVec4[mNumCreatedSeeds];
    mSeedInfo = new SeedInfo[mNumCreatedSeeds];

    for(unsigned int i = 0; i < mNumCreatedSeeds; i++)
    {
        mSeedPositions[i].x = seed_buffer[i * 4];
        mSeedPositions[i].y = seed_buffer[i * 4 + 1];
        mSeedPositions[i].z = seed_buffer[i * 4 + 2];
        mSeedPositions[i].w = seed_buffer[i * 4 + 3];
    }


    glBindBuffer(GL_ARRAY_BUFFER, mVBOCreatedSeeds);
    {
        glBufferData(GL_ARRAY_BUFFER, mNumCreatedSeeds * sizeof(MyVec4), mSeedPositions, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mTransformFeedbackVBO);
    {
        glBufferData(GL_ARRAY_BUFFER, mNumCreatedSeeds * (sizeof(SeedInfo)), NULL, GL_STREAM_READ);
    }
}

//void FiberTrackingAlgorithmComputeShader::setFilterNOT(int *filter_buffer, int size)
//{
//    resetFilterNOT();

//    //mNumFilterNOT = num_filter;

//    mSizeFilterNOT = size;

//    std::cout << "Setting Filter NOT with size: " << mSizeFilterNOT << std::endl;

//    mProgramSeedFiltering->binding();
//    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "size_filter_not"), mSizeFilterNOT * 3);

//    mFilterNOTPositions = new int[3*mSizeFilterNOT];

//    for(int i = 0; i < mSizeFilterNOT; i++)
//    {
//        mFilterNOTPositions[i * 3] = filter_buffer[i * 3];
//        mFilterNOTPositions[i * 3 + 1] = filter_buffer[i * 3 + 1];
//        mFilterNOTPositions[i * 3 +2] = filter_buffer[i * 3 + 2];
//        //mFilterNOTPositions[i].w = filter_buffer[i * 4 + 3];
//    }

//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterNOT);
//    {
//        glBufferData(GL_TEXTURE_BUFFER, mSizeFilterNOT * 3 * sizeof(int), mFilterNOTPositions, GL_STATIC_DRAW);
//    }

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_BUFFER, mFilterNotTex);
//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterNOT);
//    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32I, mVBOFilterNOT);


//}

//void FiberTrackingAlgorithmComputeShader::setFilterAND(int *filter_buffer, int size)
//{
//    resetFilterAND();

//    //mNumFilterAND = num_filter;

//    mSizeFilterAND = size;

//    std::cout << "Setting Filter AND with size: " << mSizeFilterAND << std::endl;

//    mProgramSeedFiltering->binding();
//    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "size_filter_and"), mSizeFilterAND * 3);

//    //mFilterANDPositions = new MyINTVec3[mSizeFilterAND];
//    mFilterANDPositions = new int[mSizeFilterAND*3];

//    for(int i = 0; i < mSizeFilterAND; i++)
//    {
//        mFilterANDPositions[i * 3] = filter_buffer[i * 3];
//        mFilterANDPositions[i * 3 + 1] = filter_buffer[i * 3 + 1];
//        mFilterANDPositions[i * 3 + 2] = filter_buffer[i * 3 + 2];
//        //mFilterANDPositions[i].w = filter_buffer[i * 4 + 3];
//    }

//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterAND);
//    {
//        glBufferData(GL_TEXTURE_BUFFER, mSizeFilterAND * 3 * sizeof(int), mFilterANDPositions, GL_STATIC_DRAW);
//    }

//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_BUFFER, mFilterAndTex);
//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterAND);
//    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32I, mVBOFilterAND);
//}

//std::shared_ptr<FiberClusterSet> FiberTrackingAlgorithmComputeShader::getFiberClusterVector()
//{
//    return mFiberClusterVector;
//}

unsigned int FiberTrackingAlgorithmComputeShader::getNumCreatedSeeds() const
{
    return mNumCreatedSeeds;
}

void FiberTrackingAlgorithmComputeShader::filterSeeds()
{
    mProgramSeedFiltering->binding();
    {
        countNumVerticesPerFiber();
        filterSeedsAndSave();
        createFiberGroupsVector();
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithmComputeShader::countNumVerticesPerFiber()
{
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "tensorCoeffPart01"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart02);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "tensorCoeffPart02"), 3);

    glBindBuffer(GL_ARRAY_BUFFER, mVBOCreatedSeeds);
    {
        glVertexAttribPointer(mAttribLocPosition, 4, GL_FLOAT, false, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(mAttribLocPosition);
        glBufferData(GL_ARRAY_BUFFER, mNumCreatedSeeds * sizeof(MyVec4), mSeedPositions, GL_STATIC_DRAW);
    }

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_BUFFER, mFilterNotTex);
//    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(),  "filter_not_indexes"), 0);

//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_BUFFER, mFilterAndTex);
//    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(),  "filter_and_indexes"), 1);

//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterAND);
//    {
//        glBufferData(GL_TEXTURE_BUFFER, mSizeFilterAND * sizeof(MyINTVec3), mFilterANDPositions, GL_STATIC_DRAW);
//    }

//    glBindBuffer(GL_TEXTURE_BUFFER, mVBOFilterNOT);
//    {
//        glBufferData(GL_TEXTURE_BUFFER, mSizeFilterNOT * sizeof(MyINTVec3), mFilterNOTPositions, GL_STATIC_DRAW);
//    }


    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mTransformFeedbackVBO);
    {
        glEnable(GL_RASTERIZER_DISCARD);
        {
            glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, mNumCreatedSeeds);
            glEndTransformFeedback();
        }
        glDisable(GL_RASTERIZER_DISCARD);
    }
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void FiberTrackingAlgorithmComputeShader::filterSeedsAndSave()
{
    mNumFilteredSeed = 0;

    glBindBuffer(GL_ARRAY_BUFFER, mTransformFeedbackVBO);
    SeedInfo *tex_coord = (SeedInfo *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    {
        int num_vertice = 0;
        int max_fiber_length = 0;
        int ref = 0xffffffff;   //1's mask
        for(unsigned int i = 0; i < mNumCreatedSeeds; i+=2)
        {
            int count = tex_coord[i].num_vertices + tex_coord[i + 1].num_vertices;
            //Get number of and filter and Create mask accordingly
            int union_tract;
            if(tex_coord[i].flag_enable == 0 || tex_coord[i+1].flag_enable == 0)
            {
                union_tract = 0;
            }
            else
            {
                union_tract = tex_coord[i].flag_enable | tex_coord[i+1].flag_enable;
            }

//            std::cout << "Mask Tract value:    " << std::bitset<32>(tex_coord[i].flag_enable) << std::endl;
//            std::cout << "Mask Tract +1 value: " << std::bitset<32>(tex_coord[i+1].flag_enable) << std::endl;

            if(count >= mFiberTrackingMinSteps && count <= mFiberTrackingMaxSteps && ((union_tract & ref) == ref)) // ((tex_coord[i].flag_enable == 1 && tex_coord[i+1].flag_enable == 1)))
//            if(count <= 10)
//            if(tex_coord[i].num_vertices >= 80 || tex_coord[i + 1].num_vertices >= 80)
            {
//                if(tex_coord[i].num_vertices > 1)
//                {
                    num_vertice += tex_coord[i].num_vertices;

                    mSeedInfo[mNumFilteredSeed] = tex_coord[i];

//                unsigned int voxel_index = (int(mSeedVolumeCoord[i].x) / 2) +
//                                           (int(mSeedVolumeCoord[i].y) / 2) * mDiffusionAcquisition->getDimensions(0) +
//                                           (int(mSeedVolumeCoord[i].z) / 2) * mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getDimensions(1);

//                std::cout << "fa (work with loadseed) " << mDiffusionAcquisition->getDTI()->FractionalAnisotropy(voxel_index) << std::endl;
//                std::cout << "seed 0 " << mNumVerticesPerFiber[mNumFilteredSeed] << std::endl;

                    mNumFilteredSeed++;
//                }


//                if(tex_coord[i + 1].num_vertices > 1)
//                {
                    num_vertice += tex_coord[i + 1].num_vertices;

                    mSeedInfo[mNumFilteredSeed] = tex_coord[i + 1];

//                std::cout << "seed 1 " << mNumVerticesPerFiber[mNumFilteredSeed] << std::endl << std::endl;



//                if(mNumFilteredSeed < 10)
//                {
//                   std::cout << "gpu seed 0 " << i << " x " << tex_coord[i].x <<
//                                                                 " y " << tex_coord[i].y <<
//                                                                 " z " << tex_coord[i].z <<
//                                                                 " w " << tex_coord[i].w <<std::endl;
//                   std::cout << "gpu seed 1 " << i << " x " << tex_coord[i + 1].x <<
//                                                                 " y " << tex_coord[i + 1].y <<
//                                                                 " z " << tex_coord[i + 1].z <<
//                                                                 " w " << tex_coord[i + 1].w <<std::endl;
//                   float x, y, z;
//                   unsigned int voxel_index = mSeedVolumeCoord[i].x + mSeedVolumeCoord[i].y * mDiffusionAcquisition->getDimensions(0) + mSeedVolumeCoord[i].z * mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getDimensions(1);
//                   mDiffusionAcquisition->getDTI()->getEigenVector01(voxel_index, x, y, z);
//                   DTI::TensorCoefficient *tensor_coefficients = mDiffusionAcquisition->getDTI()->getTensorCoefficients();
//                   std::cout << "tensor coeff " << " x " << tensor_coefficients[voxel_index].m_coeff[0] <<
//                                " y " << tensor_coefficients[voxel_index].m_coeff[1] <<
//                                " z " << tensor_coefficients[voxel_index].m_coeff[2] << std::endl;
//                   std::cout << "tensor " << " x " << x <<
//                                                                   " y " << y <<
//                                                                   " z " << z << std::endl;

//                }

                    mNumFilteredSeed++;
//                }

                if(count > max_fiber_length)
                    max_fiber_length = count;

//                lenght_count[count]++;
            }
        }
        mNumFiberSegmentPoints = num_vertice;

        std::cout << "num_vertice " << num_vertice << " num_valid_seeds " << mNumFilteredSeed << " max_fiber_length " << max_fiber_length << std::endl;
        std::cout << "mNumFiberSegmentPoints " << mNumFiberSegmentPoints << std::endl;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

//    std::sort(mSeedInfo, mSeedInfo + mNumFilteredSeed);
}

void FiberTrackingAlgorithmComputeShader::createFiberGroupsVector()
{
    unsigned short num_buffer = 1;
    unsigned int subset_num_fiber_segments = 0;
    unsigned int subset_num_seed_point = 0;
    unsigned int subset_num_vertices = 0;

    if(mFiberClusterVector != nullptr)
        mFiberClusterVector->eraseAll();

    mFiberClusterVector = std::make_shared<FiberClusterSet>();
    float PhysDim[3];
    PhysDim[0] = static_cast<float>(mDiffusionAcquisition->getDimensions(0) * mDiffusionAcquisition->getPixelSpacing(0));
    PhysDim[1] = static_cast<float>(mDiffusionAcquisition->getDimensions(1) * mDiffusionAcquisition->getPixelSpacing(1));
    PhysDim[2] = static_cast<float>(mDiffusionAcquisition->getDimensions(2) * mDiffusionAcquisition->getPixelSpacing(2));
    mFiberClusterVector->setPhysicalDimension(PhysDim);


//    while(mFiberClusterVector.size() > 0)
//    {
//        mFiberClusterVector[0].deleteBuffers();
//        mFiberClusterVector.erase(mFiberClusterVector.begin());
//    }

//    m_fiberClusterSet = std::make_shared<FiberClusterSet>();

    for(unsigned int i = 0; i < mNumFilteredSeed; i++)
    {
        int fiber_lenght = mSeedInfo[i].num_vertices;
        unsigned int fiber_segment_count = subset_num_vertices + fiber_lenght;
        bool flag_create_fiber_group = false;

        if((i % 2) == 0)
        {
            if((fiber_segment_count + mSeedInfo[i + 1].num_vertices) > mMaxNumFiberSegmentPointsPerBuffer)
                flag_create_fiber_group = true;
        }

        if((fiber_segment_count > mMaxNumFiberSegmentPointsPerBuffer) || flag_create_fiber_group)
        {
            flag_create_fiber_group = false;

            FiberCluster::MyVec4 *seed_positions = new FiberCluster::MyVec4[subset_num_seed_point];
            unsigned int *subset_num_vertices_per_fiber = new unsigned int[subset_num_seed_point];
            for(unsigned int j = 0; j < subset_num_seed_point; j++)
            {
                unsigned int filtered_seed_position_index = i - subset_num_seed_point + j;
                seed_positions[j] = mSeedInfo[filtered_seed_position_index].pos;
                subset_num_vertices_per_fiber[j] = mSeedInfo[filtered_seed_position_index].num_vertices;
            }

            std::shared_ptr<FiberCluster> fiber_cluster = std::make_shared<FiberCluster>();
            fiber_cluster->setNumFibers(subset_num_seed_point);
            fiber_cluster->setNumVerticesPerFiber(subset_num_vertices_per_fiber);
            fiber_cluster->setNumFiberSegmentPoints(subset_num_fiber_segments);
            fiber_cluster->setSeedPositions(seed_positions, subset_num_seed_point);
            mFiberClusterVector->add(fiber_cluster);

            std::cout << "num_buffer " << num_buffer <<
                         " subset_num_seed_point " << (*mFiberClusterVector)[num_buffer - 1].getNumSeeds() <<
                         " subset_num_fiber_segments " << (*mFiberClusterVector)[num_buffer - 1].getNumFiberSegmentPoints() << std::endl;

            num_buffer++;
            subset_num_vertices = 0;
            subset_num_seed_point = 0;
            subset_num_fiber_segments = 0;
        }

        subset_num_vertices += fiber_lenght;
        subset_num_seed_point++;
        subset_num_fiber_segments = subset_num_vertices;
    }

    FiberCluster::MyVec4 *seed_positions = new FiberCluster::MyVec4[subset_num_seed_point];
    unsigned int *subset_num_vertices_per_fiber = new unsigned int[subset_num_seed_point];
    for(unsigned int j = 0; j < subset_num_seed_point; j++)
    {
        unsigned int filtered_seed_position_index = mNumFilteredSeed - subset_num_seed_point + j;
        seed_positions[j] = mSeedInfo[filtered_seed_position_index].pos;
        subset_num_vertices_per_fiber[j] = mSeedInfo[filtered_seed_position_index].num_vertices;
    }

    std::shared_ptr<FiberCluster> fiber_cluster = std::make_shared<FiberCluster>();
    fiber_cluster->setNumFibers(subset_num_seed_point);
    fiber_cluster->setNumVerticesPerFiber(subset_num_vertices_per_fiber);
    fiber_cluster->setNumFiberSegmentPoints(subset_num_fiber_segments);
    fiber_cluster->setSeedPositions(seed_positions, subset_num_seed_point);
    mFiberClusterVector->add(fiber_cluster);

    std::cout << "num_buffer " << num_buffer <<
                 " subset_num_seed_point " << (*mFiberClusterVector)[num_buffer - 1].getNumSeeds() <<
                 " subset_num_fiber_segments " << (*mFiberClusterVector)[num_buffer - 1].getNumFiberSegmentPoints() << std::endl;
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
            std::cout << "###################################################################################################################" << std::endl;

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

//            glGenBuffers(1, &mFiberClusterVector[i].m_VBOSeeds);
//            glGenBuffers(1, &mFiberClusterVector[i].m_VBOFiberVertices);
//            glGenBuffers(1, &mFiberClusterVector[i].m_VBOFiberDirections);
            glGenBuffers(1, &fiber_buffer_init_index_vbo);

            std::cout << "===================================================================================================================" << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, VBOSeeds);
            {
                glBufferData(GL_ARRAY_BUFFER, num_seed_positions * sizeof(GLfloat) * 4, seed_positions, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdSeedPosition);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBOSeeds);
                glBindImageTexture(1, mTexBufferIdSeedPosition, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            std::cout << "i " << i << " mFilteredSeedsVBO " << glGetError() << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, fiber_buffer_init_index_vbo);
            {
                glBufferData(GL_ARRAY_BUFFER, num_seed_positions * sizeof(GLint), fiber_buffer_init_index, GL_STATIC_DRAW);
                glBindTexture(GL_TEXTURE_BUFFER, mTexBufferIdFiberInitIndex);
                glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, fiber_buffer_init_index_vbo);
                glBindImageTexture(2, mTexBufferIdFiberInitIndex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32I);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            std::cout << "i " << i << " mFilteredSeedsVBO " << glGetError() << std::endl;

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

            //std::cout << "i " << i << " mTractsVBO " << glGetError() << std::endl;

//            unsigned short num_work_groups = num_seed_positions / (32 * 32);
//            if((num_seed_positions % (32 * 32)) != 0)
//                num_work_groups++;
//            glDispatchCompute(num_work_groups, 1, 1);


            unsigned short num_total_work_groups = num_seed_positions / (8 * 8);
            if((num_seed_positions % (8 * 8)) != 0)
                num_total_work_groups++;

            unsigned short num_work_groups_x = std::floor(std::sqrt(num_total_work_groups));
            unsigned short num_work_groups_y = num_work_groups_x;
            unsigned short num_remaining_work_groups = num_total_work_groups - (num_work_groups_x * num_work_groups_y);

            std::cout << "num_total_work_groups " << num_total_work_groups << std::endl;
            std::cout << "num_work_groups_x " << num_work_groups_x << std::endl;
            std::cout << "num_work_groups_y " << num_work_groups_y << std::endl;
            std::cout << "num_remaining_work_groups " << num_remaining_work_groups << std::endl;

            if(num_remaining_work_groups > 0)
            {
                num_work_groups_x += (num_remaining_work_groups / num_work_groups_y);

                if((num_remaining_work_groups % num_work_groups_y) != 0)
                    num_work_groups_x++;
            }

            std::cout << "num_work_groups_x " << num_work_groups_x << std::endl;
            std::cout << "num_work_groups_y " << num_work_groups_y << std::endl;

            glDispatchCompute(num_work_groups_x, num_work_groups_y, 1);

//            glDeleteBuffers(1, &mFiberGroupsVector[i].tractsVBO);
            glDeleteBuffers(1, &VBOSeeds);
            glDeleteBuffers(1, &fiber_buffer_init_index_vbo);

            (*mFiberClusterVector)[i].setVBOFiberDirections(VBOFiberDirections);
            (*mFiberClusterVector)[i].setVBOFiberVertices(VBOFiberVertices);
        }
    }
    mProgramFiberTracking->releasing();
}

void FiberTrackingAlgorithmComputeShader::setFiberStep()
{
    float fiber_step;

    if(mVoxelDim[0] < mVoxelDim[1])
    {
        if(mVoxelDim[0] < mVoxelDim[2])
            fiber_step = mVoxelDim[0];
        else
            fiber_step = mVoxelDim[2];

    } else if(mVoxelDim[1] < mVoxelDim[2])
        fiber_step = mVoxelDim[1];
    else
        fiber_step = mVoxelDim[2];

    fiber_step = fiber_step / 4.f;

    mProgramSeedFiltering->binding();
    GLshort direction_factor_loc = glGetUniformLocation(mProgramSeedFiltering->getProgram(), "fiberStep");
    glUniform1f(direction_factor_loc, fiber_step);

    mProgramFiberTracking->binding();
    direction_factor_loc = glGetUniformLocation(mProgramFiberTracking->getProgram(), "fiberStep");
    glUniform1f(direction_factor_loc, fiber_step);
}

void FiberTrackingAlgorithmComputeShader::setDimensions()
{
    unsigned short dim_x = mDiffusionAcquisition->getDimensions(0);
    unsigned short dim_y = mDiffusionAcquisition->getDimensions(1);
    unsigned short dim_z = mDiffusionAcquisition->getDimensions(2);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_x"), dim_x);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_y"), dim_y);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_z"), dim_z);

}
