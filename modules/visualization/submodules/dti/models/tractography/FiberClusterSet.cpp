#include "FiberClusterSet.h"

#include <stdexcept>
#include <fstream>
#include "utilities/QCheckingOpenGL.h"

FiberClusterSet::FiberClusterSet()
{
    mSystemByteOrderIsBigEndian = false;
    checkEndianess();
    mOutputFileOrientation = Orientation::LPS;
}

FiberCluster& FiberClusterSet::operator [](unsigned int i)
{
    if(i < m_fiberClusterVector.size())
        return *m_fiberClusterVector[i];
    else
        throw std::out_of_range (std::string("FiberClusterSet::operator [] -->> Out of Range error: ") + std::to_string(i));
}

void FiberClusterSet::add(FiberCluster::sPtr fiber_cluster)
{
    m_fiberClusterVector.push_back(fiber_cluster);
}

unsigned int FiberClusterSet::size()
{
    return m_fiberClusterVector.size();
}

void FiberClusterSet::retrieveFibersFromGPU()
{
    for(unsigned short i = 0; i < m_fiberClusterVector.size(); i++)
    {
        if(m_fiberClusterVector[i]->getFiberVerticeStorageLocation() == FiberCluster::StorageLocation::GPU)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_fiberClusterVector[i]->getVBOFiberVertices());
            {
                std::cout << "group " << i << std::endl;

                FiberCluster::MyVec4 *tracts = (FiberCluster::MyVec4 *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
                {
                    unsigned int num_remaining_fiber_segments_points = m_fiberClusterVector[i]->getNumFiberSegmentPoints();
                    FiberCluster::MyVec4 *final_fiber_vertices = new FiberCluster::MyVec4[m_fiberClusterVector[i]->getNumFiberSegmentPoints()];

                    for(unsigned int k = 0; k < num_remaining_fiber_segments_points; k++)
                    {
                        final_fiber_vertices[k] = tracts[k];
                    }

                    m_fiberClusterVector[i]->setFiberVertices(final_fiber_vertices);

                }
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FiberClusterSet::saveFibers(std::string file_name)
{
    if(m_fiberClusterVector.size() > 0)
    {
        retrieveFibersFromGPU();

        std::fstream myfile;
        if(mOutputFileOrientation == Orientation::RAS)
            myfile = std::fstream(file_name + "_RAS.vtk", std::ios::out | std::ios::binary);
        else
            myfile = std::fstream(file_name + "_LPS.vtk", std::ios::out | std::ios::binary);

        if(!myfile.is_open())
        {
            std::cout << "FiberTrackingAlgorithmComputeShader -->> Unable to create file " << file_name << "!" << std::endl;
            return;
        }

        float orientation_inversor = 1.f;
        if(mOutputFileOrientation == Orientation::RAS)
            orientation_inversor = -1.f;

        unsigned int num_total_fiber_vertices = 0;
        unsigned int num_total_fiber = 0;
        for(unsigned short i = 0; i < m_fiberClusterVector.size(); i++)
        {
            unsigned int num_fiber = m_fiberClusterVector[i]->getNumFibers() / 2;
            num_total_fiber += num_fiber;
            num_total_fiber_vertices += m_fiberClusterVector[i]->getNumFiberSegmentPoints() - num_fiber;
        }

        myfile << "# vtk DataFile Version 1.0\n";
        myfile << "vtk output\n";
        myfile << "BINARY\n";
        myfile << "DATASET POLYDATA\n";
        myfile << "POINTS " << num_total_fiber_vertices << " float\n";

        for(unsigned short i = 0; i < m_fiberClusterVector.size(); i++)
        {
            unsigned int vertice_count = 0;
            for(unsigned int j = 0; j < m_fiberClusterVector[i]->getNumFibers(); j+=2)
            {
                for(int k = m_fiberClusterVector[i]->getNumVerticesPerFiber()[j] - 1; k >= 0; k--)
                {
                    unsigned char big_endian_buf[4];
                    float x = orientation_inversor * (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].x * mPhysicalDimension[0]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&x));
                    myfile.write((char*)big_endian_buf, sizeof(float));

                    float y = orientation_inversor * (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].y * mPhysicalDimension[1]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&y));
                    myfile.write((char*)big_endian_buf, sizeof(float));

                    float z = (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].z * mPhysicalDimension[2]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&z));
                    myfile.write((char*)big_endian_buf, sizeof(float));
                }
                vertice_count += m_fiberClusterVector[i]->getNumVerticesPerFiber()[j];

                for(unsigned int k = 1; k < m_fiberClusterVector[i]->getNumVerticesPerFiber()[j + 1]; k++)
                {
                    unsigned char big_endian_buf[4];
                    float x = orientation_inversor * (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].x * mPhysicalDimension[0]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&x));
                    myfile.write((char*)big_endian_buf, sizeof(float));

                    float y = orientation_inversor * (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].y * mPhysicalDimension[1]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&y));
                    myfile.write((char*)big_endian_buf, sizeof(float));

                    float z = (m_fiberClusterVector[i]->getFiberVertices()[vertice_count + k].z * mPhysicalDimension[2]);
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&z));
                    myfile.write((char*)big_endian_buf, sizeof(float));
                }

                vertice_count += m_fiberClusterVector[i]->getNumVerticesPerFiber()[j + 1];
            }
        }

        myfile << "\n";
        myfile << "LINES " << num_total_fiber << " " << (num_total_fiber + num_total_fiber_vertices) << "\n";

        unsigned int count_vertices = 0;
        for(unsigned int i = 0; i < m_fiberClusterVector.size(); i++)
        {
            for(unsigned int j = 0; j < m_fiberClusterVector[i]->getNumFibers(); j+=2)
            {
                unsigned char big_endian_buf[4];
                int value = m_fiberClusterVector[i]->getNumVerticesPerFiber()[j] + m_fiberClusterVector[i]->getNumVerticesPerFiber()[j + 1] - 1;
                toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&value));
                myfile.write((char*)big_endian_buf, sizeof(int));

                for(int k = 0; k < value; k++)
                {
                    toBigEndian(big_endian_buf, reinterpret_cast<unsigned char*>(&count_vertices));
                    myfile.write((char*)&big_endian_buf, sizeof(int));

                    count_vertices++;
                }
            }
        }

        myfile.close();
    }
}

void FiberClusterSet::toBigEndian(unsigned char big_endian_buffer[4], unsigned char *buffer)
{
    if(mSystemByteOrderIsBigEndian)
    {
        big_endian_buffer[0] = buffer[0];
        big_endian_buffer[1] = buffer[1];
        big_endian_buffer[2] = buffer[2];
        big_endian_buffer[3] = buffer[3];

    } else {

        big_endian_buffer[0] = buffer[3];
        big_endian_buffer[1] = buffer[2];
        big_endian_buffer[2] = buffer[1];
        big_endian_buffer[3] = buffer[0];
    }
}

void FiberClusterSet::setOutputFileOrientation(FiberClusterSet::Orientation orientation_output)
{
    mOutputFileOrientation = orientation_output;
}

void FiberClusterSet::checkEndianess()
{
    int num = 1;

    if (*(char *)&num == 1)
    {
        printf("FiberTrackingAlgorithmComputeShader::checkEndianess() -->> System is Little-Endian!\n");
        mSystemByteOrderIsBigEndian = false;

    } else {

        printf("FiberTrackingAlgorithmComputeShader::checkEndianess() -->> System is Big-Endian!\n");
        mSystemByteOrderIsBigEndian = true;
    }
}

void FiberClusterSet::setPhysicalDimension(float * physicalDim)
{
    mPhysicalDimension[0] = physicalDim[0];
    mPhysicalDimension[1] = physicalDim[1];
    mPhysicalDimension[2] = physicalDim[2];
}

