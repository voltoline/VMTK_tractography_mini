#ifndef FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H
#define FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H

#include "FiberTrackingAlgorithm.h"
#include "FiberClusterSet.h"

class vmtkManagementShader;
class vmtkDiffAcquisition;

class FiberTrackingAlgorithmComputeShader: public FiberTrackingAlgorithm
{
public:

    FiberTrackingAlgorithmComputeShader(const DTI *dti);
    ~FiberTrackingAlgorithmComputeShader();

    void initialize();
    FiberClusterSet::sPtr startFiberTracking();

    void setMaxSteps(unsigned int fiberTrackingMaxSteps);
    void setStoppingCriterionFA(float fiberTrackingStoppingCriterionFA);
    void setStoppingCriterionAngle(float fiberTrackingStoppingCriterionAngle);
    void setWpunct(float value);

private:
    void checkEndianess();
    void createFibers();
    void setFiberStepFiberTracking();
    void createTensorImages();

    template<typename T> void deleteGLBuffer(T *id_buffer);
   template<typename T> void deleteGLTexture(T *id_texture);

    unsigned int mFiberTrackingMaxSteps;
    unsigned int mFiberTrackingMinSteps;

    float mFiberTrackingStoppingCriterionFA;
    float mFiberTrackingStoppingCriterionAngle;
    float mFiberTrackingStoppingCriterionAngleCos;
    unsigned int mNumFilterNOT;
    unsigned int mNumFilterAND;
    int mSizeFilterNOT;
    int mSizeFilterAND;

    short mAttribLocPosition;
    short mAttribFilterNOTLocPosition;
    short mAttribFilterANDLocPosition;

    vmtkManagementShader *mProgramFiberTracking;

    vmtkDiffAcquisition *mDiffusionAcquisition;

    unsigned int m_vao;

    unsigned int mTexBufferIdSeedPosition;
    unsigned int mTexBufferIdFiberInitIndex;
    unsigned int mTexBufferIdFiberVertices;
    unsigned int mTexBufferIdFiberDirections;

    MyVec3 *mSeedVolumeCoord;
};

#endif // FIBER_TRACKING_ALGORITHM_COMPUTE_SHADER_H
