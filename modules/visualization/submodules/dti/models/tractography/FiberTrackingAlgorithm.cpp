#include "FiberTrackingAlgorithm.h"

#include "utilities/QCheckingOpenGL.h"
#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"
#include "modules/visualization/submodules/dti/models/tensor_fitter/DTI.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

FiberTrackingAlgorithm::FiberTrackingAlgorithm(const DTI *dti):
    mSeedPositions(nullptr),
    mSeedInfo(nullptr),
    m_vao(0)
{
    m_dti =dti;

   mVoxelDim[0] = 1.f / m_dti->getDimensions(0);
   mVoxelDim[1] = 1.f / m_dti->getDimensions(1);
   mVoxelDim[2] = 1.f / m_dti->getDimensions(2);

   m_phyDim[0] = m_dti->getDimensions(0) * m_dti->getPixelSpacing(0);
   m_phyDim[1] = m_dti->getDimensions(1) * m_dti->getPixelSpacing(1);
   m_phyDim[2] = m_dti->getDimensions(2) * m_dti->getPixelSpacing(2);

   mProgramSeedFiltering = nullptr;

   mVBOCreatedSeeds = 0;
   mTransformFeedbackVBO = 0;

   mFiberTrackingWpunct = 0.5;
   mFiberTrackingMaxSteps = 1000;
   mFiberTrackingStoppingCriterionFA = 0.15f;
   mFiberTrackingStoppingCriterionAngle = 11.f;
   mFiberTrackingStoppingCriterionAngleCos =  std::cos((mFiberTrackingStoppingCriterionAngle * 3.14159265) / 180.0);
   mFiberTrackingMinSteps = 100;

   mTexIdTensorCoefficientPart01 = 0;
   mTexIdTensorCoefficientPart02 = 0;

   mMaxNumFiberSegmentPointsPerBuffer = 2000000;

   reset();
}

template<typename T>
void FiberTrackingAlgorithm::deleteGLBuffer(T *id_buffer)
{
    if(*id_buffer != 0)
    {
        glDeleteBuffers(1, id_buffer);
        *id_buffer = 0;
    }
}

template<typename T>
void FiberTrackingAlgorithm::deleteGLTexture(T *id_texture)
{
    if(*id_texture != 0)
    {
        glDeleteTextures(1, id_texture);
        *id_texture = 0;
    }
}

FiberTrackingAlgorithm::~FiberTrackingAlgorithm()
{
    if(mProgramSeedFiltering != nullptr)
        delete mProgramSeedFiltering;

    deleteGLBuffer(&mTransformFeedbackVBO);
    deleteGLBuffer(&mVBOCreatedSeeds);
    deleteGLTexture(&mTexIdTensorCoefficientPart01);
    deleteGLTexture(&mTexIdTensorCoefficientPart02);
}

void FiberTrackingAlgorithm::setSeeds(float *seed_buffer, unsigned int num_seeds)
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

void FiberTrackingAlgorithm::setMaxSteps(unsigned int fiberTrackingMaxSteps)
{
    mFiberTrackingMaxSteps = fiberTrackingMaxSteps;

    mProgramSeedFiltering->binding();
    {
        glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::setMinSteps(unsigned int fiberTrackingMinSteps)
{
    mFiberTrackingMinSteps = fiberTrackingMinSteps;
}

void FiberTrackingAlgorithm::setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA)
{
    mFiberTrackingStoppingCriterionFA = fiberTrackingStoppingCriterionFA;

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle)
{
    mFiberTrackingStoppingCriterionAngle = fiberTrackingStoppingCriterionAngle;
    setFiberTrackingStoppingCriterionAngleCos(std::cos((mFiberTrackingStoppingCriterionAngle * 3.14159265) / 180.0));

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionAngleCos"), getFiberTrackingStoppingCriterionAngleCos());
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::setROIPlaneAngle(float angle)
{
    m_ROIPlaneAngle = std::cos((angle * 3.14159265) / 180.0);

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "roiPlaneAngle"), m_ROIPlaneAngle);
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::setActivatedROIPlaneFilter(bool value)
{
    m_isROIPlaneFilterActivated = value;

    mProgramSeedFiltering->binding();
    {
        std::cout << "setActivatedROIPlaneFilter " << glGetUniformLocation(mProgramSeedFiltering->getProgram(), "isROIPlaneFilterActivated") << std::endl;

        if(m_isROIPlaneFilterActivated)
            glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "isROIPlaneFilterActivated"), 1);
        else
            glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "isROIPlaneFilterActivated"), 0);
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::initialize_seed()
{
    QManagementResource *vs = new QManagementResource;

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

    mProgramSeedFiltering->binding();
    {
        mAttribLocPosition = glGetAttribLocation(mProgramSeedFiltering->getProgram(), "seed_position");

        glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "maxSteps"), mFiberTrackingMaxSteps);
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionFA"), mFiberTrackingStoppingCriterionFA);
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "stoppingCriterionAngleCos"), mFiberTrackingStoppingCriterionAngleCos);
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "w_punct"), mFiberTrackingWpunct);

        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "roiPlaneAngle"), std::cos((m_ROIPlaneAngle * 3.14159265) / 180.0));

        if(m_isROIPlaneFilterActivated)
            glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "isROIPlaneFilterActivated"), 1);
        else
            glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "isROIPlaneFilterActivated"), 0);

        glGenBuffers(1, &mVBOCreatedSeeds);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOCreatedSeeds);
        {
            glVertexAttribPointer(mAttribLocPosition, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(mAttribLocPosition);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &mTransformFeedbackVBO);
        glGenTransformFeedbacks(1, &mTransformFeedbackID);

        std::cout << "glUniformMatrix4fv " << glGetError() << std::endl;

        setDimensions();

        glUniform3fv(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "phy_dimension"), 1, m_phyDim);

        createTensorImages();
        resetRestrictionSize();
    }
    mProgramSeedFiltering->releasing();

    //##########################################
}

void FiberTrackingAlgorithm::createTensorImages()
{
    unsigned int num_voxels = m_dti->getDimensions(0) * m_dti->getDimensions(1) * m_dti->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = m_dti->getTensorCoefficients();

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
                 m_dti->getDimensions(0),
                 m_dti->getDimensions(1),
                 m_dti->getDimensions(2), 0,
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
                 m_dti->getDimensions(0),
                 m_dti->getDimensions(1),
                 m_dti->getDimensions(2), 0,
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

unsigned int FiberTrackingAlgorithm::getFiberTrackingMaxSteps() const
{
    return mFiberTrackingMaxSteps;
}

float FiberTrackingAlgorithm::getFiberTrackingStoppingCriterionFA() const
{
    return mFiberTrackingStoppingCriterionFA;
}

float FiberTrackingAlgorithm::getFiberTrackingStoppingCriterionAngleCos() const
{
    return mFiberTrackingStoppingCriterionAngleCos;
}

void FiberTrackingAlgorithm::setFiberTrackingStoppingCriterionAngleCos(float fiberTrackingStoppingCriterionAngleCos)
{
    mFiberTrackingStoppingCriterionAngleCos = fiberTrackingStoppingCriterionAngleCos;
}

unsigned int FiberTrackingAlgorithm::getFiberTrackingMinSteps() const
{
    return mFiberTrackingMinSteps;
}

float FiberTrackingAlgorithm::getFiberTrackingStoppingCriterionAngle() const
{
    return mFiberTrackingStoppingCriterionAngle;
}

void FiberTrackingAlgorithm::resetRestrictionSize()
{
    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), 0);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), 0);
}


void FiberTrackingAlgorithm::setFilterOnTexture(int * filter_not_indexes, int size_not, int * filter_and_indexes, int size_and)
{
    resetRestrictionSize();

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), size_not);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), size_and);


    unsigned int num_voxels = m_dti->getDimensions(0) * m_dti->getDimensions(1) * m_dti->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = m_dti->getTensorCoefficients();

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[0];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[1];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[2];
        tensor_coeff_buffer[i].w = 0.0;
    }
    int dim_x = m_dti->getDimensions(0);
    int dim_y = m_dti->getDimensions(1);

    //Assign Not Indexes
    for(int i = 0; i < size_not; i++)
    {
        unsigned int index = filter_not_indexes[i*3] + filter_not_indexes[i*3 + 1]*dim_x + filter_not_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(1.0);
    }

    //Assign And Indexes
    for(int i = 0; i < size_and; i++)
    {
        int index = filter_and_indexes[i*3] + filter_and_indexes[(i*3) + 1]*dim_x + filter_and_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(2.0);
    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 m_dti->getDimensions(0),
                 m_dti->getDimensions(1),
                 m_dti->getDimensions(2), 0,
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

void FiberTrackingAlgorithm::setFilterOnTexture(int * filter_not_indexes, int size_not, int * number_filter_not, int size_number_filter_not,
                                                             int * filter_and_indexes, int size_and, int * number_filter_and, int size_number_filter_and)
{
    resetRestrictionSize();

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_not"), size_number_filter_not);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "number_filter_and"), size_number_filter_and);


    unsigned int num_voxels = m_dti->getDimensions(0) * m_dti->getDimensions(1) * m_dti->getDimensions(2);
    MyVec4 *tensor_coeff_buffer = new MyVec4[num_voxels];

    DTI::TensorCoefficient *tensor_coefficients = m_dti->getTensorCoefficients();

    for(unsigned int i = 0; i < num_voxels; i++)
    {
        tensor_coeff_buffer[i].x = tensor_coefficients[i].m_coeff[0];
        tensor_coeff_buffer[i].y = tensor_coefficients[i].m_coeff[1];
        tensor_coeff_buffer[i].z = tensor_coefficients[i].m_coeff[2];
        tensor_coeff_buffer[i].w = 0.0;
    }
    int dim_x = m_dti->getDimensions(0);
    int dim_y = m_dti->getDimensions(1);

    //Assign Not Indexes
    for(int i = 0; i < size_not; i++)
    {
        unsigned int index = filter_not_indexes[i*3] + filter_not_indexes[i*3 + 1]*dim_x + filter_not_indexes[(i*3) + 2]*dim_x*dim_y;
        tensor_coeff_buffer[index].w = static_cast<float>(1.0);
    }

    //Assign And Indexes
    int and_value = 2;

    int current_index_and = 0;

    if(number_filter_and != nullptr)   //Ting: vector of the number of voxels in AND ROIs
    {
        int current_limit = number_filter_and[current_index_and]+1;

        for(int i = 0; i < size_and; i++)
        {
            unsigned int index = filter_and_indexes[i*3] + filter_and_indexes[(i*3) + 1]*dim_x + filter_and_indexes[(i*3) + 2]*dim_x*dim_y;
            if(i > current_limit)
            {
                and_value = and_value << 1;
                current_index_and++;
                if (current_index_and < size_number_filter_and) {    //Ting: avoid memory leakage (28/05/2022)
                    current_limit = current_limit + number_filter_and[current_index_and] + 1;
                }
             }

            if ((unsigned int)(tensor_coeff_buffer[index].w) > 1) {//Ting: NOT labeled voxels are kept
                //It should be invalid state ... ROI of and_value must be removed (Ting, 31/05/2022).
                std::cout << "AND restrictions must be mutually exclusive." << std::endl;
            } else if ((unsigned int)(tensor_coeff_buffer[index].w) == 0) {
                //Only unlabeled voxel can have its label changed (Ting, 31/05/2022)
                tensor_coeff_buffer[index].w = static_cast<float>(and_value);
            }
        }
    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, mTexIdTensorCoefficientPart01);

    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA32F,
                 m_dti->getDimensions(0),
                 m_dti->getDimensions(1),
                 m_dti->getDimensions(2), 0,
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

float * FiberTrackingAlgorithm::getVoxelDim()
{
    return mVoxelDim;
}

void FiberTrackingAlgorithm::setDimensions()
{
    unsigned short dim_x = m_dti->getDimensions(0);
    unsigned short dim_y = m_dti->getDimensions(1);
    unsigned short dim_z = m_dti->getDimensions(2);

    mProgramSeedFiltering->binding();
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_x"), dim_x);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_y"), dim_y);
    glUniform1i(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "dim_z"), dim_z);
}

void FiberTrackingAlgorithm::setFiberStep(float step)
{
    mProgramSeedFiltering->binding();
    GLshort direction_factor_loc = glGetUniformLocation(mProgramSeedFiltering->getProgram(), "fiberStep");
    glUniform1f(direction_factor_loc, step);
}

void FiberTrackingAlgorithm::filterSeeds()
{
    mProgramSeedFiltering->binding();
    {
        countNumVerticesPerFiber();
        filterSeedsAndSave();
        createFiberGroupsVector();
    }
    mProgramSeedFiltering->releasing();
}

void FiberTrackingAlgorithm::countNumVerticesPerFiber()
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

void FiberTrackingAlgorithm::filterSeedsAndSave()
{
    mNumFilteredSeed = 0;

    glBindBuffer(GL_ARRAY_BUFFER, mTransformFeedbackVBO);
    SeedInfo *tex_coord = static_cast<SeedInfo *>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
    {
        int num_vertice = 0;
        unsigned int max_fiber_length = 0;
        unsigned int ref = 0xffffffff;   //1's mask
         std::cout << "DEBUG CS - Num Created Seeds Filter Seeds and Save: " << mNumCreatedSeeds << std::endl;
         std::cout << "DEBUG CS - Min Steps: " << getFiberTrackingMinSteps() << std::endl;
         std::cout << "DEBUG CS - Max Steps: " << getFiberTrackingMaxSteps() << std::endl;
        for(unsigned int i = 0; i < mNumCreatedSeeds; i+=2)
        {
            unsigned int count = tex_coord[i].num_vertices + tex_coord[i + 1].num_vertices;

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

            if(count >= getFiberTrackingMinSteps() && count <= getFiberTrackingMaxSteps() && ((union_tract & ref) == ref))
            {
                    num_vertice += tex_coord[i].num_vertices;

                    mSeedInfo[mNumFilteredSeed] = tex_coord[i];

                    mNumFilteredSeed++;

                    num_vertice += tex_coord[i + 1].num_vertices;

                    mSeedInfo[mNumFilteredSeed] = tex_coord[i + 1];

                    mNumFilteredSeed++;

                if(count > max_fiber_length)
                    max_fiber_length = count;

            }
        }
        mNumFiberSegmentPoints = static_cast <unsigned int>(num_vertice);

        std::cout << "num_vertice " << num_vertice << " num_valid_seeds " << mNumFilteredSeed << " max_fiber_length " << max_fiber_length << std::endl;
        std::cout << "mNumFiberSegmentPoints " << mNumFiberSegmentPoints << std::endl;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

}

void FiberTrackingAlgorithm::createFiberGroupsVector()
{
    unsigned short num_buffer = 1;
    unsigned int subset_num_fiber_segments = 0;
    unsigned int subset_num_seed_point = 0;
    unsigned int subset_num_vertices = 0;

    mFiberClusterVector = std::make_shared<FiberClusterSet>();
    float PhysDim[3];
    PhysDim[0] = static_cast<float>(m_dti->getDimensions(0) * m_dti->getPixelSpacing(0));
    PhysDim[1] = static_cast<float>(m_dti->getDimensions(1) * m_dti->getPixelSpacing(1));
    PhysDim[2] = static_cast<float>(m_dti->getDimensions(2) * m_dti->getPixelSpacing(2));
    mFiberClusterVector->setPhysicalDimension(PhysDim);

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

void FiberTrackingAlgorithm::reset()
{
    mNumCreatedSeeds = 0;
    mNumFilteredSeed = 0;
    mNumFiberSegmentPoints = 0;

    deleteBuffer(&mSeedPositions);
    deleteBuffer(&mSeedInfo);

    mFiberClusterVector = std::make_shared<FiberClusterSet>();
    float PhysDim[3];
    PhysDim[0] = static_cast<float>(m_dti->getDimensions(0) * m_dti->getPixelSpacing(0));
    PhysDim[1] = static_cast<float>(m_dti->getDimensions(1) * m_dti->getPixelSpacing(1));
    PhysDim[2] = static_cast<float>(m_dti->getDimensions(2) * m_dti->getPixelSpacing(2));
    mFiberClusterVector->setPhysicalDimension(PhysDim);
}

float FiberTrackingAlgorithm::getFiberTrackingWpunct() const
{
    return mFiberTrackingWpunct;
}

void FiberTrackingAlgorithm::setFiberTrackingWpunct(float fiberTrackingWpunct)
{
    mFiberTrackingWpunct = fiberTrackingWpunct;

    mProgramSeedFiltering->binding();
    {
        glUniform1f(glGetUniformLocation(mProgramSeedFiltering->getProgram(), "w_punct"), mFiberTrackingWpunct);
    }
    mProgramSeedFiltering->releasing();
}

template<typename T>
void FiberTrackingAlgorithm::deleteBuffer(T **buffer)
{
    if(*buffer != nullptr)
    {
        delete[] *buffer;
        *buffer = nullptr;
    }
}
