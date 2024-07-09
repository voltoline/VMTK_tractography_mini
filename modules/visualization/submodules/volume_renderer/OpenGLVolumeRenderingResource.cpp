#include "OpenGLVolumeRenderingResource.h"

#include "transfer_function/vmtkGrayScale.h"
#include <math.h>

OpenGLVolumeRenderingResource::OpenGLVolumeRenderingResource()
{
    m_volumeTextureID = 0;
    m_transferFunctionTextureID = 0;

    m_isRGBVolumeTextureLoaded = false;
    m_isGrayscaleVolumeTextureLoaded = false;
    m_isTransferFunctionTextureLoaded = false;

    m_sampleType = SampleType::GRAYSCALE;
    m_sampleInterpolation = SampleInterpolation::LINEAR;
    m_transferFuntionSampleInterpolation = SampleInterpolation::LINEAR;

    m_grayscaleVolumeDataUshort = nullptr;
    m_grayscaleVolumeDataFloat = nullptr;
    m_rgbVolumeData = nullptr;
    m_transferFunctionData = nullptr;
}

OpenGLVolumeRenderingResource::~OpenGLVolumeRenderingResource()
{
    if(m_volumeTextureID > 0)
        glDeleteTextures(1, &m_volumeTextureID);

    if(m_transferFunctionTextureID > 0)
        glDeleteTextures(1, &m_transferFunctionTextureID);
}

GLuint OpenGLVolumeRenderingResource::getVolumeTextureID()
{
    if(m_sampleType == SampleType::GRAYSCALE && !m_isGrayscaleVolumeTextureLoaded)
        loadGrayscaleVolumeTexture();

    else if(m_sampleType == SampleType::RGB && !m_isRGBVolumeTextureLoaded)
        loadRGBVolumeTexture();

    glBindTexture(GL_TEXTURE_3D, m_volumeTextureID);

    if(m_sampleInterpolation == SampleInterpolation::LINEAR)
    {
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {

        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return m_volumeTextureID;
}

GLuint OpenGLVolumeRenderingResource::getTransferFunctionTextureID()
{
    if(!m_isTransferFunctionTextureLoaded)
        loadTransferFunctionTexture();

    glBindTexture(GL_TEXTURE_1D, m_transferFunctionTextureID);

    if(m_transferFuntionSampleInterpolation == SampleInterpolation::LINEAR)
    {
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {

        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);

    return m_transferFunctionTextureID;
}

void OpenGLVolumeRenderingResource::setTransferFunctionData(vmtkTFBuffer *new_tf)
{
    m_isTransferFunctionTextureLoaded = false;

    if(m_transferFunctionData)
    {
        delete m_transferFunctionData;
        m_transferFunctionData = nullptr;
    }

    m_transferFunctionData = new_tf;
}

void OpenGLVolumeRenderingResource::setGrayscaleVolumeData(GrayscaleImage3DFloat::sPtr volume_data)
{
    m_isGrayscaleVolumeTextureLoaded = false;

    if(m_grayscaleVolumeDataFloat)
    {
        m_grayscaleVolumeDataFloat = nullptr;
    }

    if(m_grayscaleVolumeDataUshort)
    {
        m_grayscaleVolumeDataUshort = nullptr;
    }

    m_grayscaleVolumeDataFloat = volume_data;

    m_sampleType = SampleType::GRAYSCALE;
}

void OpenGLVolumeRenderingResource::setGrayscaleVolumeData(GrayscaleImage3DUshort::sPtr volume_data)
{
    m_isGrayscaleVolumeTextureLoaded = false;

    if(m_grayscaleVolumeDataFloat)
    {
        m_grayscaleVolumeDataFloat = nullptr;
    }

    if(m_grayscaleVolumeDataUshort)
    {
        m_grayscaleVolumeDataUshort = nullptr;
    }

    m_grayscaleVolumeDataUshort = volume_data;

    m_sampleType = SampleType::GRAYSCALE;
}

void OpenGLVolumeRenderingResource::setRGBVolumeData(float *volume_data)
{
    m_isRGBVolumeTextureLoaded = false;

    if(m_rgbVolumeData)
    {
        delete[] m_rgbVolumeData;
        m_rgbVolumeData = nullptr;
    }

    m_rgbVolumeData = volume_data;

    m_sampleType = SampleType::RGB;
}

void OpenGLVolumeRenderingResource::loadGrayscaleVolumeTexture()
{
    if(m_isGrayscaleVolumeTextureLoaded)
    {
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // pixel store alignment per 1 byte

    if(m_volumeTextureID == 0)
        glGenTextures(1, &m_volumeTextureID);

    glBindTexture(GL_TEXTURE_3D, m_volumeTextureID);
    {
        if(m_grayscaleVolumeDataUshort)
            glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, m_volumeDimension[0], m_volumeDimension[1], m_volumeDimension[2], 0, GL_RED, GL_UNSIGNED_SHORT, m_grayscaleVolumeDataUshort->getBuffer());

        else if(m_grayscaleVolumeDataFloat)
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_volumeDimension[0], m_volumeDimension[1], m_volumeDimension[2], 0, GL_RED, GL_FLOAT, m_grayscaleVolumeDataFloat->getBuffer());

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    glBindTexture(GL_TEXTURE_3D, 0);

    m_isGrayscaleVolumeTextureLoaded = true;
}

void OpenGLVolumeRenderingResource::loadRGBVolumeTexture()
{
    if(m_isRGBVolumeTextureLoaded)
    {
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // pixel store alignment per 1 byte

    if(m_volumeTextureID == 0)
        glGenTextures(1, &m_volumeTextureID);

    glBindTexture(GL_TEXTURE_3D, m_volumeTextureID);
    {
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, m_volumeDimension[0], m_volumeDimension[1], m_volumeDimension[2], 0, GL_RGB, GL_FLOAT, m_rgbVolumeData);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    glBindTexture(GL_TEXTURE_3D, 0);

    m_isRGBVolumeTextureLoaded = true;
}

void OpenGLVolumeRenderingResource::loadTransferFunctionTexture()
{
    if(m_isTransferFunctionTextureLoaded)
    {
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(m_transferFunctionTextureID == 0)
        glGenTextures(1, &m_transferFunctionTextureID);

    glBindTexture(GL_TEXTURE_1D, m_transferFunctionTextureID);
    {        
        if(m_transferFunctionData)
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, m_transferFunctionData->getBufferSize(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_transferFunctionData->getTFBuffer());

        else {

            std::cerr << "OpenGLVolumeRenderingResource::loadTransferFunctionTexture -->> Transfer function was not set! " << std::endl;

            glBindTexture(GL_TEXTURE_1D, 0);
            return;
        }

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_1D, 0);

    m_isTransferFunctionTextureLoaded = true;
    QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
}

void OpenGLVolumeRenderingResource::setTransferFuntionSampleInterpolation(const SampleInterpolation &transferFuntionSampleInterpolation)
{
    m_transferFuntionSampleInterpolation = transferFuntionSampleInterpolation;
}

void OpenGLVolumeRenderingResource::setSampleInterpolation(const SampleInterpolation &sampleInterpolation)
{
    m_sampleInterpolation = sampleInterpolation;
}

void OpenGLVolumeRenderingResource::setVolumeDimension(unsigned short volume_dimension[3])
{
    m_volumeDimension[0] = volume_dimension[0];
    m_volumeDimension[1] = volume_dimension[1];
    m_volumeDimension[2] = volume_dimension[2];
}

int OpenGLVolumeRenderingResource::getMax1D2DTextureSize() {
    int max;

    max = 0;
    if (!m_max1D2DTextureSize) {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!max) max = MAX_1D2D_TEXTURE_SIZE;
        m_max1D2DTextureSize = max;
    }

    return m_max1D2DTextureSize;
}

int OpenGLVolumeRenderingResource::getMax3DTextureSize() {
    int max;

    max = 0;
    if (!m_max3DTextureSize) {
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max);
        if (!max) max = MAX_3D_TEXTURE_SIZE;
        m_max3DTextureSize = max;
    }

    return m_max3DTextureSize;
}
