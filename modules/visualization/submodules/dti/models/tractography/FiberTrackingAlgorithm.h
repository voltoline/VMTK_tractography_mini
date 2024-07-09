#ifndef FIBER_TRACKIN_GALGORITHM_H
#define FIBER_TRACKIN_GALGORITHM_H

#include <iostream>
#include "FiberClusterSet.h"

class vmtkManagementShader;
class vmtkDiffAcquisition;
class DTI;

class FiberTrackingAlgorithm
{
public:
    FiberTrackingAlgorithm(const DTI *dti);
    virtual ~FiberTrackingAlgorithm();

    virtual void initialize() = 0;
    virtual FiberClusterSet::sPtr startFiberTracking() = 0;
    virtual void setSeeds(float *seed_buffer, unsigned int num_seeds);

    virtual void setMaxSteps(unsigned int fiberTrackingMaxSteps);
    virtual void setMinSteps(unsigned int fiberTrackingMinSteps);
    virtual void setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA);
    virtual void setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle);
    virtual void setWpunct(float value) { }

    virtual void setROIPlaneAngle(float angle);
    virtual void setActivatedROIPlaneFilter(bool value);

    void initialize_seed();

    unsigned int getFiberTrackingMaxSteps() const;

    float getFiberTrackingStoppingCriterionFA() const;

    float getFiberTrackingStoppingCriterionAngleCos() const;
    void setFiberTrackingStoppingCriterionAngleCos(float fiberTrackingStoppingCriterionAngleCos);

    unsigned int getFiberTrackingMinSteps() const;

    float getFiberTrackingStoppingCriterionAngle() const;

    void resetRestrictionSize();
    void setFiberStep(float step);

    float* getVoxelDim();
    void setDimensions();
    void setFiberStep();

    void setFilterOnTexture(int * filter_not_indexes, int size_not, int * filter_and_indexes, int size_and);

    /**
     * @brief FiberTrackingAlgorithm::setFilterOnTexture
     * @param filter_not_indexes array coordinates of voxels labeled as NOT
     * @param size_not number of voxels labeled as NOT
     * @param number_filter_not first element of each NOT ROI in the not_index_array
     * @param size_number_filter_not number of NOT ROIs
     * @param filter_and_indexes vector of the AND labeled voxels (grid) coordinates
     * @param size_and number of AND labeled voxels
     * @param number_filter_and vector of (size-1) voxels of
     * @param size_number_filter_and
     */
    void setFilterOnTexture(int * filter_not_indexes, int size_not, int * number_filter_not, int size_number_filter_not,
                            int * filter_and_indexes, int size_and, int * number_filter_and, int size_number_filter_and);

    float getFiberTrackingWpunct() const;
    void setFiberTrackingWpunct(float fiberTrackingWpunct);

#ifdef TRACTOGRAPHY_TIME_MEASURE
    double preprocessing_time;
    double track_time;
    double query_time;
#endif

protected:

    template<typename T> void deleteGLBuffer(T *id_buffer);
    template<typename T> void deleteGLTexture(T *id_texture);
    template<typename T> void deleteBuffer(T **buffer);

    void createTensorImages();
    void filterSeeds();
    void countNumVerticesPerFiber();
    void filterSeedsAndSave();
    void createFiberGroupsVector();

    void reset();

    unsigned int mTexIdTensorCoefficientPart01;
    unsigned int mTexIdTensorCoefficientPart02;

    unsigned int mTransformFeedbackVBO;

    unsigned int mVBOCreatedSeeds;

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

    unsigned int mNumCreatedSeeds;
    FiberClusterSet::sPtr mFiberClusterVector;

    float m_phyDim[3];

private:
    unsigned int mTransformFeedbackID;

    vmtkManagementShader *mProgramSeedFiltering;

    MyVec4 *mSeedPositions;
    SeedInfo *mSeedInfo;
    unsigned int mNumFilteredSeed;
    unsigned int mNumFiberSegmentPoints;
    unsigned int mMaxNumFiberSegmentPointsPerBuffer;

    const DTI *m_dti;

    float mVoxelDim[3];

    unsigned int m_vao;

    short mAttribLocPosition;

    unsigned int mFiberTrackingMaxSteps;
    unsigned int mFiberTrackingMinSteps;

    float mFiberTrackingWpunct;
    float mFiberTrackingStoppingCriterionFA;
    float mFiberTrackingStoppingCriterionAngle;
    float mFiberTrackingStoppingCriterionAngleCos;

    float m_ROIPlaneAngle;
    bool m_isROIPlaneFilterActivated;
};

#endif // FIBER_TRACKIN_GALGORITHM_H
