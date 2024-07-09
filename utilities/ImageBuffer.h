#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include "DataBuffer.h"

class ImageBuffer
{
public:
    typedef std::shared_ptr<ImageBuffer> sPtr;
    typedef std::unique_ptr<ImageBuffer> uPtr;

    enum class Type
    {
        USHORT,
        FLOAT,
        NONE
    };

    ImageBuffer(char *data, Type type, unsigned int width, unsigned int height = 1, unsigned int depth = 1, unsigned short num_samples = 1);
    ImageBuffer(Type type, unsigned int width, unsigned int height = 1, unsigned int depth = 1, unsigned short num_samples = 1);
    ~ImageBuffer() { }

    unsigned short getSampleAsUshort(unsigned int row, unsigned int col = 0, unsigned int plane = 0, unsigned short sample_index = 0);
    short getSampleAsShort(unsigned int row, unsigned int col = 0, unsigned int plane = 0, unsigned short sample_index = 0);

    void getPixelAsUshort(unsigned short *pixel, unsigned int row, unsigned int col = 0, unsigned int plane = 0);
    void getPixelAsShort(short *pixel, unsigned int row, unsigned int col = 0, unsigned int plane = 0);

    Type getDataType() const;
    char* getBuffer() const;

    unsigned int getNumPixels() const;

protected:
    bool isValidPixelCoordinate(unsigned int row, unsigned int col, unsigned int plane);
    bool isValidPixelIndex(unsigned int index);
    unsigned int computeIndex(unsigned row, unsigned int col, unsigned int plane);

private:
    DataBuffer::Type getDataBufferType(Type type);

    Type m_dataType;
    DataBuffer m_dataBuffer;

    unsigned short m_samplesPerPixel;
    unsigned int m_numPixels;
    unsigned int m_dimension[3];        // m_dimension[0]: rows, width; m_dimension[1]: cols, height; m_dimension[2]: planes, depth
};

//===========================================================================================================================================================================

class GrayscaleImage3DFloat: public ImageBuffer
{
public:
    typedef std::shared_ptr<GrayscaleImage3DFloat> sPtr;
    typedef std::unique_ptr<GrayscaleImage3DFloat> uPtr;

    GrayscaleImage3DFloat(float *data, unsigned int width, unsigned int height = 1, unsigned int depth = 1);
    GrayscaleImage3DFloat(unsigned int width, unsigned int height = 1, unsigned int depth = 1);

    float& at(unsigned int index);
    float& at(unsigned row, unsigned int col, unsigned int plane);

private:
    float *m_data;
};

//===========================================================================================================================================================================

class GrayscaleImage3DUshort: public ImageBuffer
{
public:
    typedef std::shared_ptr<GrayscaleImage3DUshort> sPtr;
    typedef std::unique_ptr<GrayscaleImage3DUshort> uPtr;

    GrayscaleImage3DUshort(unsigned short *data, unsigned int width, unsigned int height = 1, unsigned int depth = 1);
    GrayscaleImage3DUshort(unsigned int width, unsigned int height = 1, unsigned int depth = 1);

    unsigned short& at(unsigned int index);
    unsigned short& at(unsigned row, unsigned int col, unsigned int plane);

private:
    unsigned short *m_data;
};
#endif // IMAGE_BUFFER_H
