#ifndef OPENGL_VOLUME_RENDERING_RESOURCE_H
#define OPENGL_VOLUME_RENDERING_RESOURCE_H

#include "utilities/QCheckingOpenGL.h"
#include <modules/file_reader/dicom/vmtkAcquisition.h>
#include "transfer_function/vmtkTFBuffer.h"
#include <utilities/ImageBuffer.h>

class OpenGLVolumeRenderingResource
{
public:
    typedef std::shared_ptr<OpenGLVolumeRenderingResource> sPtr;

    enum class SampleType
    {
        GRAYSCALE,
        RGB
    };

    enum class SampleInterpolation
    {
        NEAREST,
        LINEAR
    };

    OpenGLVolumeRenderingResource();
    ~OpenGLVolumeRenderingResource();

    GLuint getVolumeTextureID();
    GLuint getTransferFunctionTextureID();

    void setTransferFunctionData(vmtkTFBuffer *new_tf);
    void setGrayscaleVolumeData(GrayscaleImage3DFloat::sPtr volume_data);
    void setGrayscaleVolumeData(GrayscaleImage3DUshort::sPtr volume_data);
    void setRGBVolumeData(float *volume_data);

    void setSampleInterpolation(const SampleInterpolation &sampleInterpolation);

    void setVolumeDimension(unsigned short volume_dimension[3]);

    /**
     * @brief get a rough estimate of the largest texture that the GL can handle. The value must be at least 64.
     * @return the estimated size
     */
    int getMax1D2DTextureSize(); // added by Ting (22/11/2019)

    /**
     * @brief get a rough estimat of the largest 3D texture that the GL can handle.
     * @return the estimated size
     */
    int getMax3DTextureSize(); //added by Ting (22/11/2019)

    void setTransferFuntionSampleInterpolation(const SampleInterpolation &transferFuntionSampleInterpolation);

private:
    //Provisory solution to optmize the usage of color mapping texture
    const unsigned short MAX_1D2D_TEXTURE_SIZE=16384; // added by Ting (22/11/2019)
    const unsigned short MAX_3D_TEXTURE_SIZE=2048; // added by Ting (22/11/2019)

    void loadGrayscaleVolumeTexture();
    void loadRGBVolumeTexture();
    void loadTransferFunctionTexture();

    GLuint m_volumeTextureID;
    bool m_isGrayscaleVolumeTextureLoaded;
    bool m_isRGBVolumeTextureLoaded;

    GLuint m_transferFunctionTextureID;
    bool m_isTransferFunctionTextureLoaded;

    SampleType m_sampleType;
    SampleInterpolation m_sampleInterpolation;
    SampleInterpolation m_transferFuntionSampleInterpolation;

    GrayscaleImage3DUshort::sPtr m_grayscaleVolumeDataUshort;
    GrayscaleImage3DFloat::sPtr m_grayscaleVolumeDataFloat;
    float *m_rgbVolumeData;
    unsigned short m_volumeDimension[3];
    int m_max1D2DTextureSize = 0;  //added by Ting (22/11/2019)
    int m_max3DTextureSize = 0;    //added by Ting (22/11/2019)

    vmtkTFBuffer *m_transferFunctionData;
};

#endif // OPENGL_VOLUME_RENDERING_RESOURCE_H
