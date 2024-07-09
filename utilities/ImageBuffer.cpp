#include "ImageBuffer.h"

#include <assert.h>
#include <iostream>

ImageBuffer::ImageBuffer(char *data, ImageBuffer::Type type, unsigned int width, unsigned int height, unsigned int depth, unsigned short num_samples):
    m_dataBuffer(data, getDataBufferType(type), (width * height * depth * num_samples))
{
    if(width < 1 || height < 1 || depth < 1)
        assert(!"ImageBuffer::ImageBuffer -->> Invalid image dimension!");

    m_dimension[0] = width;
    m_dimension[1] = height;
    m_dimension[2] = depth;

    m_samplesPerPixel = num_samples;
    m_numPixels = width * height * depth;

    m_dataType = type;
}

ImageBuffer::ImageBuffer(ImageBuffer::Type type, unsigned int width, unsigned int height, unsigned int depth, unsigned short num_samples):
    m_dataBuffer(getDataBufferType(type), (width * height * depth * num_samples))
{
    if(width < 1 || height < 1 || depth < 1)
        assert(!"ImageBuffer::ImageBuffer -->> Invalid image dimension!");

    m_dimension[0] = width;
    m_dimension[1] = height;
    m_dimension[2] = depth;

    m_samplesPerPixel = num_samples;
    m_numPixels = width * height * depth;

    m_dataType = type;
}

unsigned short ImageBuffer::getSampleAsUshort(unsigned int row, unsigned int col, unsigned int plane, unsigned short sample_index)
{
    unsigned int index = computeIndex(row, col, plane);
    return m_dataBuffer.getValueAsUshort(index + sample_index);
}

short ImageBuffer::getSampleAsShort(unsigned int row, unsigned int col, unsigned int plane, unsigned short sample_index)
{
    unsigned int index = computeIndex(row, col, plane);
    return m_dataBuffer.getValueAsShort(index + sample_index);
}

void ImageBuffer::getPixelAsUshort(unsigned short *pixel, unsigned int row, unsigned int col, unsigned int plane)
{
    unsigned int index = computeIndex(row, col, plane);
    for(unsigned short i = 0; i < m_samplesPerPixel; i++)
        pixel[i] = m_dataBuffer.getValueAsUshort(index + i);
}

void ImageBuffer::getPixelAsShort(short *pixel, unsigned int row, unsigned int col, unsigned int plane)
{    
    unsigned int index = computeIndex(row, col, plane);
    for(unsigned short i = 0; i < m_samplesPerPixel; i++)
        pixel[i] = m_dataBuffer.getValueAsShort(index + i);
}

bool ImageBuffer::isValidPixelCoordinate(unsigned int row, unsigned int col, unsigned int plane)
{
    if(row > m_dimension[0] || col > m_dimension[1] || plane > m_dimension[2])
    {
        std::cerr << "ImageBuffer::isValidPixelCoordinate -->> Invalid pixel coordinate!" << std::endl;
        return false;
    }

    return true;
}

bool ImageBuffer::isValidPixelIndex(unsigned int index)
{
    if(index > m_numPixels)
    {
        std::cerr << "ImageBuffer::isValidPixelIndex -->> Invalid pixel index!" << std::endl;
        return false;
    }

    return true;
}

unsigned int ImageBuffer::computeIndex(unsigned row, unsigned int col, unsigned int plane)
{
    if(!isValidPixelCoordinate(row, col, plane))
        return 0;

    return (row + col * m_dimension[0] + plane * m_dimension[0] * m_dimension[1]) * m_samplesPerPixel;
}

DataBuffer::Type ImageBuffer::getDataBufferType(ImageBuffer::Type type)
{
    switch(type)
    {
    case ImageBuffer::Type::USHORT:
        return DataBuffer::Type::USHORT;

    case ImageBuffer::Type::FLOAT:
        return DataBuffer::Type::FLOAT;

    default:
        assert(!"ImageBuffer::getDataBufferType -->> Type not handled!");
    }

    return DataBuffer::Type::NONE;
}

unsigned int ImageBuffer::getNumPixels() const
{
    return m_numPixels;
}

ImageBuffer::Type ImageBuffer::getDataType() const
{
    return m_dataType;
}

char* ImageBuffer::getBuffer() const
{
    return m_dataBuffer.getBuffer();
}

//===========================================================================================================================================================================

GrayscaleImage3DFloat::GrayscaleImage3DFloat(float *data, unsigned int width, unsigned int height, unsigned int depth):
    ImageBuffer(reinterpret_cast<char*>(data), ImageBuffer::Type::FLOAT, width, height, depth)
{
    m_data = data;
}

GrayscaleImage3DFloat::GrayscaleImage3DFloat(unsigned int width, unsigned int height, unsigned int depth):
    ImageBuffer(ImageBuffer::Type::FLOAT, width, height, depth)
{
    m_data = reinterpret_cast<float*>(getBuffer());
}

float& GrayscaleImage3DFloat::at(unsigned int index)
{
    if(isValidPixelIndex(index))
        return m_data[index];

    return m_data[0];
}

float& GrayscaleImage3DFloat::at(unsigned row, unsigned int col, unsigned int plane)
{
    return m_data[computeIndex(row, col, plane)];
}

//===========================================================================================================================================================================

GrayscaleImage3DUshort::GrayscaleImage3DUshort(unsigned short *data, unsigned int width, unsigned int height, unsigned int depth):
    ImageBuffer(reinterpret_cast<char*>(data), ImageBuffer::Type::USHORT, width, height, depth)
{
    m_data = data;
}

GrayscaleImage3DUshort::GrayscaleImage3DUshort(unsigned int width, unsigned int height, unsigned int depth):
    ImageBuffer(ImageBuffer::Type::USHORT, width, height, depth)
{
    m_data = reinterpret_cast<unsigned short*>(getBuffer());
}

unsigned short& GrayscaleImage3DUshort::at(unsigned int index)
{
    if(isValidPixelIndex(index))
        return m_data[index];

    return m_data[0];
}

unsigned short& GrayscaleImage3DUshort::at(unsigned row, unsigned int col, unsigned int plane)
{
    return m_data[computeIndex(row, col, plane)];
}
