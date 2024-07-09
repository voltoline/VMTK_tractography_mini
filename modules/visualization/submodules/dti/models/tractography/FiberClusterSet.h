#ifndef FIBER_CLUSTER_SET_H
#define FIBER_CLUSTER_SET_H

#include "FiberCluster.h"

#include <vector>
#include <memory>

class FiberClusterSet
{
public:
    typedef std::shared_ptr<FiberClusterSet> sPtr;

    enum class Orientation
    {
        RAS,
        LPS
    };

    FiberClusterSet();
    FiberCluster& operator [](unsigned int i);
    void add(FiberCluster::sPtr fiber_cluster);
    unsigned int size();
    void retrieveFibersFromGPU();
    void saveFibers(std::string file_name);
    void setOutputFileOrientation(FiberClusterSet::Orientation orientation_output);
    void setPhysicalDimension(float *physicalDim);

private:
    std::vector<FiberCluster::sPtr> m_fiberClusterVector;
    void toBigEndian(unsigned char big_endian_buffer[4], unsigned char *buffer);
    void checkEndianess();

    float mPhysicalDimension[3];

    Orientation mOutputFileOrientation;
    bool mSystemByteOrderIsBigEndian;
};

#endif // FIBER_CLUSTER_SET_H
