#include "FiberCluster.h"

#include "utilities/QCheckingOpenGL.h"

FiberCluster::FiberCluster()
{
    mNumSeeds = 0;
    mSeedPositions = nullptr;
    mNumVerticesPerFiber = nullptr;
    mFiberVertices = nullptr;
    mNumFiberSegmentPoints = 0;

    m_VBOFiberVertices = 0;
    m_VBOFiberDirections = 0;

    m_fiberVerticeStorageLocation = StorageLocation::NONE;
    m_fiberDirectionStorageLocation = StorageLocation::NONE;

    m_wasDelete = false;
}

FiberCluster::~FiberCluster()
{
    deleteBuffers();
    std::cout << "m_wasDelete " << m_wasDelete << std::endl;
}

void FiberCluster::deleteBuffers()
{
    if(mSeedPositions != nullptr)
    {
        delete[] mSeedPositions;
        mSeedPositions = nullptr;
    }

    if(mFiberVertices != nullptr)
    {
        delete[] mFiberVertices;
        mFiberVertices = nullptr;
    }

    if(mNumVerticesPerFiber != nullptr)
    {
        delete[] mNumVerticesPerFiber;
        mNumVerticesPerFiber = nullptr;
    }

    if(m_VBOFiberVertices != 0)
        deleteGLBuffer(&m_VBOFiberVertices);

    if(m_VBOFiberDirections != 0)
        deleteGLBuffer(&m_VBOFiberDirections);

    m_fiberVerticeStorageLocation = StorageLocation::NONE;
    m_fiberDirectionStorageLocation = StorageLocation::NONE;

    m_wasDelete = true;
}

template<typename T>
void FiberCluster::deleteGLBuffer(T *id_buffer)
{
    if(*id_buffer != 0)
    {
        glDeleteBuffers(1, id_buffer);
        *id_buffer = 0;
    }
}

unsigned int FiberCluster::getNumSeeds() const
{
    return mNumSeeds;
}

FiberCluster::MyVec4 *FiberCluster::getSeedPositions() const
{
    return mSeedPositions;
}

void FiberCluster::setSeedPositions(MyVec4 *seedPositions, unsigned int numSeeds)
{
    if(mSeedPositions != nullptr)
        delete[] mSeedPositions;

    mNumSeeds = numSeeds;
    mSeedPositions = seedPositions;
}

unsigned int* FiberCluster::getNumVerticesPerFiber() const
{
    return mNumVerticesPerFiber;
}

void FiberCluster::setFiberVertices(MyVec4 *fiberVertices)
{
    if(mFiberVertices != nullptr)
        delete[] mFiberVertices;

    mFiberVertices = fiberVertices;

    if(fiberVertices != nullptr)
    {
        if(m_fiberVerticeStorageLocation == StorageLocation::GPU)
            m_fiberVerticeStorageLocation = StorageLocation::CPU_GPU;

        else if(m_fiberVerticeStorageLocation == StorageLocation::NONE)
            m_fiberVerticeStorageLocation = StorageLocation::CPU;

    } else {

        if(m_fiberVerticeStorageLocation == StorageLocation::CPU_GPU)
            m_fiberVerticeStorageLocation = StorageLocation::GPU;

        else if(m_fiberVerticeStorageLocation == StorageLocation::CPU)
            m_fiberVerticeStorageLocation = StorageLocation::NONE;
    }
}

unsigned int FiberCluster::getNumFiberSegmentPoints() const
{
    return mNumFiberSegmentPoints;
}

void FiberCluster::setNumFiberSegmentPoints(unsigned int numFiberSegmentPoints)
{
    mNumFiberSegmentPoints = numFiberSegmentPoints;
}

unsigned int FiberCluster::getVBOFiberVertices() const
{
    return m_VBOFiberVertices;
}

void FiberCluster::setVBOFiberVertices(const unsigned int &VBOFiberVertices)
{    
    m_VBOFiberVertices = VBOFiberVertices;

    if(VBOFiberVertices != 0)
    {
        if(m_fiberVerticeStorageLocation == StorageLocation::CPU)
            m_fiberVerticeStorageLocation = StorageLocation::CPU_GPU;

        else if(m_fiberVerticeStorageLocation == StorageLocation::NONE)
            m_fiberVerticeStorageLocation = StorageLocation::GPU;

    } else {

        if(m_fiberVerticeStorageLocation == StorageLocation::CPU_GPU)
            m_fiberVerticeStorageLocation = StorageLocation::CPU;

        else if(m_fiberVerticeStorageLocation == StorageLocation::GPU)
            m_fiberVerticeStorageLocation = StorageLocation::NONE;
    }
}

unsigned int FiberCluster::getVBOFiberDirections() const
{
    return m_VBOFiberDirections;
}

void FiberCluster::setVBOFiberDirections(const GLuint &VBOFiberDirections)
{
    m_VBOFiberDirections = VBOFiberDirections;

    if(VBOFiberDirections != 0)
    {
        if(m_fiberDirectionStorageLocation == StorageLocation::CPU)
            m_fiberDirectionStorageLocation = StorageLocation::CPU_GPU;

        else if(m_fiberDirectionStorageLocation == StorageLocation::NONE)
            m_fiberDirectionStorageLocation = StorageLocation::GPU;

    } else {

        if(m_fiberDirectionStorageLocation == StorageLocation::CPU_GPU)
            m_fiberDirectionStorageLocation = StorageLocation::CPU;

        else if(m_fiberDirectionStorageLocation == StorageLocation::GPU)
            m_fiberDirectionStorageLocation = StorageLocation::NONE;
    }
}

FiberCluster::StorageLocation FiberCluster::getFiberVerticeStorageLocation() const
{
    return m_fiberVerticeStorageLocation;
}

void FiberCluster::updateGPUVertices()
{
    if(m_fiberVerticeStorageLocation == StorageLocation::CPU_GPU)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOFiberVertices);
        glBufferData(GL_ARRAY_BUFFER, getNumFiberSegmentPoints() * sizeof(GLfloat) * 4, getFiberVertices(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void FiberCluster::setNumFibers(unsigned int numFibers)
{
    mNumFibers = numFibers;
}

void FiberCluster::setNumVerticesPerFiber(unsigned int *numVerticesPerFiber)
{
    if(mNumVerticesPerFiber != nullptr)
        delete[] mNumVerticesPerFiber;

    mNumVerticesPerFiber = numVerticesPerFiber;
}

FiberCluster::MyVec4 *FiberCluster::getFiberVertices() const
{
    return mFiberVertices;
}

unsigned int FiberCluster::getNumFibers() const
{
    return mNumFibers;
}
