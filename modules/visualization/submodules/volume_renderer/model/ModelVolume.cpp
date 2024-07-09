#include "ModelVolume.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))  /**< maximum among the input values */
#define MIN(a,b) ((a) < (b) ? (a) : (b))  /**< minimum among the input values */

#include <math.h>
#include <cstring>
#include <utilities/vmtkEqualizer.h>
#include "../transfer_function/vmtkGrayScale.h"

ModelVolume::ModelVolume(unsigned short volume_dim[3], double pixel_spacing[3])
{
     m_volumeDimension[0] = volume_dim[0];
     m_volumeDimension[1] = volume_dim[1];
     m_volumeDimension[2] = volume_dim[2];

     m_pixelSpacing[0] = pixel_spacing[0];
     m_pixelSpacing[1] = pixel_spacing[1];
     m_pixelSpacing[2] = pixel_spacing[2];

     computedScaleFactors();

     m_transferFunction = nullptr;

     mThresholdData.mState = false;
     mThresholdData.mValue = 0;
     mThresholdData.mMin = 0;
     mThresholdData.mMax = 0;

     m_grayscaleVolumeDataUshort = nullptr;
     m_intensityMap = nullptr;
     m_mappedGrayscaleVolume = nullptr;

     m_bitsAllocated = 0;
     m_umax = 0;

     m_sampleInterpolation = SampleInterpolation::LINEAR;

     m_tfMin = 0;
     m_tfMax = 0;
     m_defaultTransferFunctionPoints.clear();

     m_volumeType = VolumeType::NONE;

     m_isVolumeLoadedAsTexture = false;

     m_useEqualizedVolume = false;
}

void ModelVolume::getScaleFactors(float scale_factors[4])
{
    scale_factors[0] = m_scaleFactors[0];
    scale_factors[1] = m_scaleFactors[1];
    scale_factors[2] = m_scaleFactors[2];
    scale_factors[3] = m_scaleFactors[3];
}

void ModelVolume::getPhyScale(float phy_scale[])
{
    phy_scale[0] = m_phyScale[0];
    phy_scale[1] = m_phyScale[1];
    phy_scale[2] = m_phyScale[2];
}

void ModelVolume::setTransferFunctionPoints(std::vector<float> red_points, std::vector<float> green_points,
                                                                 std::vector<float> blue_points, std::vector<float> alpha_points)
{
    mTranferFunctionRedPoints = red_points;
    mTranferFunctionGreenPoints = green_points;
    mTranferFunctionBluePoints = blue_points;
    mTranferFunctionAlphaPoints = alpha_points;
}

void ModelVolume::estimateDefaultRanges(int uMin, int uMax, std::vector<int> center, std::vector<int> width)
{
    int tf_min = 0;
    int tf_max = uMax;

    int ind;

    if (center.size() == 0) {
        if (uMin < 0) {
            m_tfMin = 0;
        } else {
            m_tfMin = uMin;
        }
        m_tfMax = tf_max;
        return;
    } else if (center.size() > 0) {
        int max, j;
        for(ind=0, max=width[0], j=0; j < width.size(); j++) {
            //Choose a valid pair with the largest range (Ting, 08/01/2022)
            if (width[j] > max && ((center[j]-0.5)-((width[j]-1.0)/2)) > uMin &&
                    ((center[j]-0.5)+(width[j]-1.0)/2) < uMax) {
                max = width[j];
                ind = j;
            }
        }

        //Offset to get positive values
        if (uMin < 0) center[ind] -= uMin; //(added by Ting, 25/11/2019)
        //Consistency checking to the limit values. The range of width is doubled to cover more intensities in the default mode, taking into account that
        // our alpha function is a ramp function
        tf_min = (center[ind]-0.5)-((width[ind]-1.0)/2); //Ting: (0/01/2022)
        if (tf_min < 0) tf_min = 0;
        if (((center[ind]-0.5)+(width[ind]-1.0)/2) < tf_max) tf_max = (center[ind]-0.5)+(width[ind]-1.)/2;

        m_tfMin = tf_min;
        m_tfMax = tf_max;
    }
}

void ModelVolume::getEstimateDefaultRanges(int &tfMin, int &tfMax)
{
    tfMin = m_tfMin;
    tfMax = m_tfMax;
}

void ModelVolume::createDefaultTransferFunctionPoints(int tfMin, int tfMax, int uMax)
{
    std::vector<float> aux;
    if (tfMin > 0)
    {
        aux.push_back(0.0);
        aux.push_back(1.0);
    }

    //Ting: Values are normalized values in texture. We suppose that the minimal value is 0 (07/01/2022)
    //Ting: the point coordinates must be in the range [0,1.] (09/01/2022)
    aux.push_back((float)(tfMin*1./uMax));
    aux.push_back(1.0);
    aux.push_back((float)(tfMax*1./uMax));
    aux.push_back(0.0);

    if (tfMax < uMax)
    {
        aux.push_back((float)(uMax*1./uMax));
        aux.push_back(0.0);
    }

    m_defaultTransferFunctionPoints = aux;

    setTransferFunctionPoints(aux, aux, aux, aux);
}

std::vector<float> ModelVolume::getDefaultTransferFunctionPoints()
{
    return m_defaultTransferFunctionPoints;
}

void ModelVolume::getTransferFunctionPoints(std::vector<float> &red_points, std::vector<float> &green_points, std::vector<float> &blue_points, std::vector<float> &alpha_points)
{
    red_points = mTranferFunctionRedPoints;
    green_points = mTranferFunctionGreenPoints;
    blue_points = mTranferFunctionBluePoints;
    alpha_points = mTranferFunctionAlphaPoints;
}

void ModelVolume::computedScaleFactors()
{
    float volume_real_dimension[3];
    for(int i = 0; i < 3; i++)
        volume_real_dimension[i] = m_volumeDimension[i] * m_pixelSpacing[i];

    float maxDimension = MAX(MAX(volume_real_dimension[0], volume_real_dimension[1]), volume_real_dimension[2]);
    for (int i = 0; i < 3; i++)
        m_scaleFactors[i] = volume_real_dimension[i] / maxDimension;

    for (int i = 0; i < 3; i++)
        m_phyScale[i] = m_pixelSpacing[i] / maxDimension;

    m_scaleFactors[3] = 1.0;
}

void ModelVolume::computeIntensityMap()
{
    if(m_useEqualizedVolume)
    {
        unsigned short *mapa;
        vmtkEqualizer::EqualizeHistogram (m_volumeDimension[0], m_volumeDimension[1], m_volumeDimension[2],
                                      m_grayscaleVolumeDataUshort, m_bitsAllocated, m_umax, &mapa);
        unsigned int map_length = m_umax + 1;

        m_intensityMap = std::make_shared<DataBuffer>(reinterpret_cast<char*>(mapa), DataBuffer::Type::USHORT, map_length);

    } else {

        unsigned short value = 0;
        unsigned int map_length = m_umax + 1;

        m_intensityMap = std::make_shared<DataBuffer>(DataBuffer::Type::USHORT, map_length);
        m_intensityMap->setValue(0, value);

        for (int i = 1; i <= m_umax; i++)
        {
            //Linear correspondence between x in [0,umax] and y in [0, pow(2,m_bitsAllocated)] (Ting, 07/01/2022)
            value = (unsigned short)((pow(2,m_bitsAllocated) - 1) * ((i /float(m_umax))));
            m_intensityMap->setValue(i, value);
        }
    }
}


bool ModelVolume::isUsingEqualizedVolume() const
{
    return m_useEqualizedVolume;
}

bool ModelVolume::isVolumeLoadedAsTexture() const
{
    return m_isVolumeLoadedAsTexture;
}

void ModelVolume::setIsVolumeLoadedAsTexture(bool isVolumeLoadedAsTexture)
{
    m_isVolumeLoadedAsTexture = isVolumeLoadedAsTexture;
}

ModelVolumeEvent ModelVolume::useNormalizedVolume()
{
    if(m_useEqualizedVolume != false)
    {
        m_useEqualizedVolume = false;
        m_isVolumeLoadedAsTexture = false;
        computeIntensityMap();
        return ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED;
    }

    return ModelVolumeEvent::NONE;
}

ModelVolumeEvent ModelVolume::useEqualizedVolume()
{
    if(m_useEqualizedVolume != true)
    {
        m_useEqualizedVolume = true;
        m_isVolumeLoadedAsTexture = false;
        computeIntensityMap();
        return ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED;
    }

    return ModelVolumeEvent::NONE;
}

ModelVolume::VolumeType ModelVolume::getVolumeType() const
{
    return m_volumeType;
}

unsigned short ModelVolume::getBitsAllocated() const
{
    return m_bitsAllocated;
}

void ModelVolume::setBitsAllocated(unsigned short bitsAllocated)
{
    m_bitsAllocated = bitsAllocated;
}

ModelVolume::SampleInterpolation ModelVolume::getSampleInterpolation() const
{
    return m_sampleInterpolation;
}

ModelVolumeEvent ModelVolume::setSampleInterpolation(const SampleInterpolation &sampleInterpolation)
{
    if(m_sampleInterpolation == sampleInterpolation)
        return ModelVolumeEvent::NONE;

    m_sampleInterpolation = sampleInterpolation;

    return ModelVolumeEvent::SAMPLE_INTERPOLATION_CHANGED;
}

float *ModelVolume::getRGBVolumeData() const
{
    return m_rgbVolumeData;
}

GrayscaleImage3DFloat::sPtr ModelVolume::getGrayscaleVolumeDataFloat() const
{
    return m_grayscaleVolumeDataFloat;
}

DataBuffer::sPtr ModelVolume::getIntensityMap() const
{
    return m_intensityMap;
}

void ModelVolume::setIntensityMap(DataBuffer::sPtr intensity_map)
{
    m_intensityMap = intensity_map;
}

GrayscaleImage3DUshort::sPtr ModelVolume::getMappedGrayscaleVolume()
{
    if(m_grayscaleVolumeDataUshort == nullptr)
        return nullptr;

    if(m_intensityMap == nullptr)
        return nullptr;

    int intensidade, di, iz, iy, ix/*, volSize*/;

    m_mappedGrayscaleVolume = std::make_shared<GrayscaleImage3DUshort>(m_volumeDimension[0], m_volumeDimension[1], m_volumeDimension[2]);

    for(di = 0, iz = 0; iz < m_volumeDimension[2]; iz++)
    {
        for(iy = 0; iy < m_volumeDimension[1]; iy++)
        {
            for(ix = 0; ix < m_volumeDimension[0]; ix++)
            {
                intensidade = m_grayscaleVolumeDataUshort[ix + iy * m_volumeDimension[0] + iz * m_volumeDimension[0] * m_volumeDimension[1]];
                m_mappedGrayscaleVolume->at(di++) = m_intensityMap->getValueAsUshort(intensidade);
            }
        }
    }
    return m_mappedGrayscaleVolume;
}

ModelVolumeEvent ModelVolume::setGrayscaleVolumeData(unsigned short *volumeData, unsigned short bits_allocated, int umax)
{
    if(m_mappedGrayscaleVolume)
        m_mappedGrayscaleVolume = nullptr;

    if(m_intensityMap)
        m_intensityMap = nullptr;

    m_grayscaleVolumeDataFloat = nullptr;
    m_rgbVolumeData = nullptr;
    m_grayscaleVolumeDataUshort = volumeData;
    m_bitsAllocated = bits_allocated;
    m_umax = umax;

    computeIntensityMap();

    m_volumeType = VolumeType::USHORT_GRAYSCALE;

    m_isVolumeLoadedAsTexture = false;

    return ModelVolumeEvent::USHORT_GRAYSCALE_VOLUME_DATA_CHANGED;
}

ModelVolumeEvent ModelVolume::setGrayscaleVolumeData(GrayscaleImage3DFloat::sPtr volume)
{
    if(m_mappedGrayscaleVolume)
        m_mappedGrayscaleVolume = nullptr;

    if(m_intensityMap)
        m_intensityMap = nullptr;

    m_grayscaleVolumeDataUshort = nullptr;
    m_grayscaleVolumeDataFloat = volume;
    m_rgbVolumeData = nullptr;

    m_volumeType = VolumeType::FLOAT_GRAYSCALE;

    m_isVolumeLoadedAsTexture = false;

    return ModelVolumeEvent::FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED;
}

ModelVolumeEvent ModelVolume::setRGBVolumeData(float *volume_data)
{
    if(m_mappedGrayscaleVolume)
        m_mappedGrayscaleVolume = nullptr;

    if(m_intensityMap)
        m_intensityMap = nullptr;

    m_grayscaleVolumeDataUshort = nullptr;
    m_grayscaleVolumeDataFloat = nullptr;
    m_rgbVolumeData = volume_data;

    m_volumeType = VolumeType::FLOAT_RGB;

    m_isVolumeLoadedAsTexture = false;

    return ModelVolumeEvent::FLOAT_RGB_VOLUME_DATA_CHANGED;
}

vmtkTFBuffer *ModelVolume::getTransferFunction() const
{
    return m_transferFunction;
}

ModelVolumeEvent ModelVolume::setTransferFunction(vmtkTFBuffer *transferFunction)
{
    m_transferFunction = transferFunction;
    return ModelVolumeEvent::TRANSFER_FUNCTION_CHANGED;
}

void ModelVolume::createDefaultTransferFunctionBuffer(unsigned short size, int umax)
{
    int tf_min, tf_max;
    getEstimateDefaultRanges(tf_min, tf_max);

    std::cout << "Ting::ModelVolume::createDefaultTransferFunctionBuffer::tf_min = " << tf_min << "; tf_max = " << tf_max << std::endl;

    unsigned char *tf;
    vmtkGrayScale tfunc;
    //Add umax (Ting: 08/1/2022)
    tfunc.GetGrayScaleTF (5, tf_min, tf_max, (unsigned short*) getIntensityMap()->getBuffer(), getBitsAllocated(),
                          umax, &tf);

    //Set transfer function data in RGBA format
    vmtkTFBuffer *transfer_function = new vmtkTFBuffer(tf, size, vmtkTFBuffer::VMTK_TF_RGBA);

    //Set the transfer function texture data
    setTransferFunction(transfer_function);

    //To remove background noise we start with tf_min+1 threshold (tf_min is not visible)
    setThreshold(tf_min+1);

    setThresholdState(true);

    //Set transfer function points (Ting, 23/11/2019)
    createDefaultTransferFunctionPoints(tf_min, tf_max, umax);

}

ModelVolumeEvent ModelVolume::setThresholdInterval(int min, int max)
{
    mThresholdData.mMin = min;
    mThresholdData.mMax = max;

    if(mThresholdData.mMin > mThresholdData.mMax)
        mThresholdData.mMin = mThresholdData.mMax;

    if(mThresholdData.mValue < mThresholdData.mMin)
    {
        mThresholdData.mValue = mThresholdData.mMin;
        return ModelVolumeEvent::THRESHOLD_DATA_CHANGED;
    }

    return ModelVolumeEvent::NONE;
}

void ModelVolume::getThresholdInterval(int &min, int& max)
{
    min = mThresholdData.mMin;
    max = mThresholdData.mMax;
}

ModelVolumeEvent ModelVolume::setThresholdState(bool state)
{
    if(isNotifying())
    {
        std::cerr << "ModelVolume::setThresholdState -->> A notification is in progress!" << std::endl;
    }

    mThresholdData.mState = state;

    return ModelVolumeEvent::THRESHOLD_DATA_CHANGED;
}

ModelVolumeEvent ModelVolume::setThreshold(int value)
{
    if(isNotifying())
    {
        std::cerr << "ModelVolume::setThreshold -->> A notification is in progress!" << std::endl;
    }

    mThresholdData.mValue = value;

    return ModelVolumeEvent::THRESHOLD_DATA_CHANGED;
}

int ModelVolume::getThreshold() const
{
    return mThresholdData.mValue;
}

float ModelVolume::getMappedThreshold()
{
    if(m_intensityMap)
    {
        return (m_intensityMap->getValueAsUshort(mThresholdData.mValue) / (pow(2, m_bitsAllocated) - 1));
    } else {

        if(m_grayscaleVolumeDataFloat)
            return 0;
        else
            return mThresholdData.mValue;
    }
}

bool ModelVolume::getThresholdState() const
{
    return mThresholdData.mState;
}

