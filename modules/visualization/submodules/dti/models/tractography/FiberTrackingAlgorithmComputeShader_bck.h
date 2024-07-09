#ifndef FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H
#define FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H

#include "utilities/QCheckingOpenGL.h"
#include "model/SharedAttributes.h"
#include "FiberCluster.h"
#include "FiberTrackingAlgorithm.h"


class vmtkManagementShader;
class vmtkDiffAcquisition;

#define BUFFER_OFFSET(x)  ((const void*) (x))

class FiberTrackingAlgorithmComputeShader: public FiberTrackingAlgorithm
{
public:

    FiberTrackingAlgorithmComputeShader(vmtkDiffAcquisition *diff_acq, const float *scaleFactors);
    ~FiberTrackingAlgorithmComputeShader();

    void initialize();
    std::shared_ptr<FiberClusterSet> startFiberTracking();
    void setSeeds(float *seed_buffer, unsigned int num_seeds);
//    void setFilterNOT(int *filter_buffer, int size);
//    void setFilterAND(int *filter_buffer, int size);
    void setFilterOnTexture(int *filter_not_indexes, int size_not, int *filter_and_indexes, int size_and);
    void setFilterOnTexture(int * filter_not_indexes, int size_not, int * number_filter_not, int size_number_filter_not,
                            int * filter_and_indexes, int size_and, int * number_filter_and, int size_number_filter_and);

    void setMaxSteps(unsigned int fiberTrackingMaxSteps);
    void setMinSteps(unsigned int fiberTrackingMinSteps);
    void setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA);
    void setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle);

    unsigned int getNumCreatedSeeds() const;
//    std::shared_ptr<FiberClusterSet> getFiberClusterVector();

private:
    void reset();
//    void resetFilterNOT();
//    void resetFilterAND();
    void resetFilterSize();
    void checkEndianess();
    void filterSeeds();
    void countNumVerticesPerFiber();
    void filterSeedsAndSave();
    void createFiberGroupsVector();
    void createFibers();
    void setFiberStep();
    void setDimensions();
    void createTensorImages();

    template<typename T> void deleteBuffer(T **buffer);
    template<typename T> void deleteGLBuffer(T *id_buffer);
    template<typename T> void deleteGLTexture(T *id_texture);

    unsigned int mFiberTrackingMaxSteps;
    unsigned int mFiberTrackingMinSteps;

    float mFiberTrackingStoppingCriterionFA;
    float mFiberTrackingStoppingCriterionAngle;
    float mFiberTrackingStoppingCriterionAngleCos;
    unsigned int mNumCreatedSeeds;
    unsigned int mNumFilterNOT;
    unsigned int mNumFilterAND;
    int mSizeFilterNOT;
    int mSizeFilterAND;

    GLuint mVBOCreatedSeeds;
    GLuint mTransformFeedbackVBO;

//    GLuint mVBOFilterAND;
//    GLuint mVBOFilterNOT;
//    GLuint mFilterNotTex;
//    GLuint mFilterAndTex;


    GLshort mAttribLocPosition;
    GLshort mAttribFilterNOTLocPosition;
    GLshort mAttribFilterANDLocPosition;

    AttributePtr<GLuint> mTexIdMainDiffusionDirection;
    AttributePtr<GLuint> mTexIdFractionalAnisotropy;

    GLuint mTexIdTensorCoefficientPart01;
    GLuint mTexIdTensorCoefficientPart02;

    vmtkManagementShader *mProgramSeedFiltering;
    vmtkManagementShader *mProgramFiberTracking;

    vmtkDiffAcquisition *mDiffusionAcquisition;

    float mVoxelDim[3];

    unsigned int mNumFilteredSeed;
    unsigned int mNumFiberSegmentPoints;
    unsigned int mMaxNumFiberSegmentPointsPerBuffer;


    typedef struct my_vec4
    {
        float x;
        float y;
        float z;
        float w;

        template<class T>
        void operator=(const T &point)
        {
            x = point.x;
            y = point.y;
            z = point.z;
            w = point.w;
        }

    } MyVec4;

    typedef struct my_vec3
    {
        float x;
        float y;
        float z;

        template<class T>
        void operator=(const T &point)
        {
            x = point.x;
            y = point.y;
            z = point.z;
        }

    } MyVec3;

    typedef struct my_int_vec3
    {
        int x;
        int y;
        int z;

        template<class T>
        void operator=(const T &point)
        {
            x = point.x;
            y = point.y;
            z = point.z;
        }

    } MyINTVec3;

    typedef struct seed_info_
    {
        MyVec4 pos;
        int num_vertices;
        int flag_enable;

        void operator=(const seed_info_ &seed_info)
        {
            pos = seed_info.pos;
            num_vertices = seed_info.num_vertices;
            flag_enable = seed_info.flag_enable;
        }

        bool operator<(const seed_info_ &seed_info)
        {
            return num_vertices < seed_info.num_vertices;
        }

    } SeedInfo;

    MyVec4 *mSeedPositions;
    SeedInfo *mSeedInfo;

    int *mFilterANDPositions;
    int *mFilterNOTPositions;

    GLuint m_vao;

    GLuint mTexBufferIdSeedPosition;
    GLuint mTexBufferIdFiberInitIndex;
    GLuint mTexBufferIdFiberVertices;
    GLuint mTexBufferIdFiberDirections;

    MyVec3 *mSeedVolumeCoord;

//    std::vector<FiberCluster> mFiberClusterVector;
    std::shared_ptr<FiberClusterSet> mFiberClusterVector;
};

#endif // FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H
