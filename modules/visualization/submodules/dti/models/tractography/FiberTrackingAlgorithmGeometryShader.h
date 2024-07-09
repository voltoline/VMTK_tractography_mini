#ifndef FIBER_TRACKING_ALGORITHM_GEOMETRY_SHADER_H
#define FIBER_TRACKING_ALGORITHM_GEOMETRY_SHADER_H

#include "FiberTrackingAlgorithm.h"

class vmtkManagementShader;
class vmtkDiffAcquisition;

class FiberTrackingAlgorithmGeometryShader: public FiberTrackingAlgorithm
{
public:
    FiberTrackingAlgorithmGeometryShader(const DTI *dti);
    ~FiberTrackingAlgorithmGeometryShader();

    void initialize();
    std::shared_ptr<FiberClusterSet> startFiberTracking();
    void setStoppingCriterionFA(float stopCriterionFA);
    void setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle);
    void setWpunct(float value);

public:
    typedef struct fiber_group_info
    {
        unsigned int seedsVBO;
        unsigned int tractsVBO;
        FiberCluster mFiberPathGroup;

        fiber_group_info()
        {
            seedsVBO = 0;
            tractsVBO = 0;
        }

    } FiberGroupInfo;

    std::vector<FiberGroupInfo> mFiberGroupsVector;

private:
    int m_max_number_components_geometry_shader;
    int m_max_number_vertices_geometry_shader;

    void createFibers();
    unsigned int trackFibers(unsigned int VBO_id_fibers, unsigned int VBO_id_seeds, unsigned int num_seeds, FiberCluster::MyVec4 *feedback, bool debug);
    void orderSeedsBySize(unsigned int array[], int low, int high, unsigned int id[], FiberCluster::MyVec4 *index);
    int partition(unsigned int array[], int low, int high, unsigned int id[], FiberCluster::MyVec4 *index);
    void swap(unsigned int * x, unsigned int * y);
    void swap(FiberCluster::MyVec4 * x, FiberCluster::MyVec4 * y);
    void orderSeedsBySizeInsertionSort(unsigned int array[], int size, unsigned int id[], FiberCluster::MyVec4 * index);

    void adjustSize(unsigned int n, unsigned int * seeds_loops, unsigned int num_seeds, unsigned int * original_size, unsigned int * iteration_size);
    unsigned int organizeRetrievedPoints(unsigned int n, unsigned int num_points, unsigned int num_valid_seeds, FiberCluster::MyVec4 * points,
                                 FiberCluster::MyVec4 * seeds, FiberCluster::MyVec4 *directions, unsigned int *iteration_size, std::vector<std::vector<FiberCluster::MyVec4> > &saved_vertices, std::vector<std::vector<FiberCluster::MyVec4> > &saved_directions, bool debug);
    unsigned int removeDuplicatePoints(std::vector<std::vector<FiberCluster::MyVec4>> &points);
    void computeLoops(unsigned int n, unsigned int * size, unsigned int * seeds_loops, bool debug);

    bool equalPoints(FiberCluster::MyVec4 a, FiberCluster::MyVec4 b);
    bool equalPoints(FiberCluster::MyVec3 a, FiberCluster::MyVec3 b);
    bool equalPoints(float ax, float ay, float az, float bx, float by, float bz);

    void setFiberStepGeometryShader();
    float m_fiber_step;

    int mAttribLocSeedPosition;
    int mAttribLocDirection;
    short mAttribLocPosition;

    vmtkManagementShader *mProgramGSFiberTracking;

    vmtkDiffAcquisition *mDiffusionAcquisition;

    float mScaleFactors[3];
    float mVoxelDim[3];
    float mVoxelCenter[3];

    unsigned int m_vao;

    MyVec4 *mFilteredSeedPositions;

    int *mNumVerticesPerFiber;
};

#endif // FIBER_TRACKING_ALGORITHM_GEOMETRY_SHADER_H
