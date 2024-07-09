#ifndef FIBER_CLUSTER_H
#define FIBER_CLUSTER_H

#include <memory>

class FiberCluster
{
public:
    typedef std::shared_ptr<FiberCluster> sPtr;

    enum class StorageLocation
    {
        NONE,
        CPU,
        GPU,
        CPU_GPU
    };

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

    FiberCluster();
    ~FiberCluster();

    unsigned int getNumSeeds() const;

    FiberCluster::MyVec4 *getSeedPositions() const;
    void setSeedPositions(FiberCluster::MyVec4 *seedPositions, unsigned int numSeeds);

    unsigned int getNumFibers() const;
    void setNumFibers(unsigned int numFibers);

    unsigned int *getNumVerticesPerFiber() const;
    void setNumVerticesPerFiber(unsigned int *numVerticesPerFiber);

    MyVec4 *getFiberVertices() const;
    void setFiberVertices(MyVec4 *fiberVertices);

    unsigned int getNumFiberSegmentPoints() const;
    void setNumFiberSegmentPoints(unsigned int numFiberSegmentPoints);

    unsigned int getVBOFiberVertices() const;
    void setVBOFiberVertices(const unsigned int &VBOFiberVertices);

    unsigned int getVBOFiberDirections() const;
    void setVBOFiberDirections(const unsigned int &VBOFiberDirections);

    StorageLocation getFiberVerticeStorageLocation() const;

    void updateGPUVertices();

private:
    template<typename T> void deleteGLBuffer(T *id_buffer);
    void deleteBuffers();

    unsigned int mNumSeeds;
    MyVec4 *mSeedPositions;

    unsigned int mNumFibers;
    unsigned int *mNumVerticesPerFiber;
    MyVec4 *mFiberVertices;

    unsigned int mNumFiberSegmentPoints;

    unsigned int m_VBOFiberVertices;
    unsigned int m_VBOFiberDirections;

    StorageLocation m_fiberVerticeStorageLocation;
    StorageLocation m_fiberDirectionStorageLocation;

    bool m_wasDelete;
};

#endif // FIBER_CLUSTER_H
