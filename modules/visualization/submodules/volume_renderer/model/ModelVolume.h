#ifndef MODEL_VOLUME_H
#define MODEL_VOLUME_H

#include <glm/glm.hpp>
#include <utilities/ImageBuffer.h>
#include "model/ModelSubject.h"
#include "../transfer_function/vmtkTFBuffer.h"

enum class ModelVolumeEvent
{
    NONE,
    FLOAT_RGB_VOLUME_DATA_CHANGED,
    FLOAT_GRAYSCALE_VOLUME_DATA_CHANGED,
    USHORT_GRAYSCALE_VOLUME_DATA_CHANGED,
    TRANSFER_FUNCTION_CHANGED,
    THRESHOLD_DATA_CHANGED,
    SAMPLE_INTERPOLATION_CHANGED
};

class ModelVolume: public ModelSubject<ModelVolume, ModelVolumeEvent>
{
public:
    enum class SampleInterpolation
    {
        NEAREST,
        LINEAR
    };

    enum class VolumeType
    {
        NONE,
        FLOAT_RGB,
        FLOAT_GRAYSCALE,
        USHORT_GRAYSCALE
    };

    ModelVolume(unsigned short volume_dim[], double pixel_spacing[]);

    void getScaleFactors(float scale_factors[4]);
    void getPhyScale(float phy_scale[3]);

    vmtkTFBuffer *getTransferFunction() const;
    ModelVolumeEvent setTransferFunction(vmtkTFBuffer *transferFunction);

    void createDefaultTransferFunctionBuffer(unsigned short size, int umax);

    /**
   * @brief setNoiseThreshold
   * @param[in] threshold noise threshold
   */
    ModelVolumeEvent setThreshold(int value);

    /**
    * @brief Enable or disable Threshold
    * @param[in] state: true or false
    */
    ModelVolumeEvent setThresholdState(bool state);
    ModelVolumeEvent setThresholdInterval(int min, int max);

    int getThreshold( ) const;
    bool getThresholdState() const;
    void getThresholdInterval(int &min, int &max);
    float getMappedThreshold();

    ModelVolumeEvent setGrayscaleVolumeData(unsigned short *volumeData, unsigned short bits_allocated, int umax);
    ModelVolumeEvent setGrayscaleVolumeData(GrayscaleImage3DFloat::sPtr volume);

    ModelVolumeEvent setRGBVolumeData(float *volume_data);

    DataBuffer::sPtr getIntensityMap() const;
    void setIntensityMap(DataBuffer::sPtr intensity_map);

    /**
     * @brief Get voxel values mapped (normalized or equalized) with respect to the size of the allocated bits.
     * @return pointer to the volume.
     */
    GrayscaleImage3DUshort::sPtr getMappedGrayscaleVolume();

    GrayscaleImage3DFloat::sPtr getGrayscaleVolumeDataFloat() const;
    float *getRGBVolumeData() const;

    ModelVolumeEvent setSampleInterpolation(const SampleInterpolation &sampleInterpolation);
    SampleInterpolation getSampleInterpolation() const;

    unsigned short getBitsAllocated() const;
    void setBitsAllocated(unsigned short bitsAllocated);

    /**
     * @brief Get the four sequences of points that define the piecewise linear color mapping.
     * @param red sequnce of red coordinates.
     * @param green sequence of green coordinates.
     * @param blue sequence of blue coordiantes.
     * @param alpha sequence of alpha coordinates.
     */
    void getTransferFunctionPoints(std::vector<float> &red, std::vector<float> &green, std::vector<float> &blue, std::vector<float> &alpha); //added by Ting (23/11/2019)

    /**
     * @brief Set the four sequences of points that define the piecewise linear color mapping
     * @param red sequence of red coordinates.
     * @param green sequence of green coorindates.
     * @param blue sequence of blue coordinates.
     * @param alpha sequence of alpha coordinates
     */
    void setTransferFunctionPoints(std::vector<float> red, std::vector<float> green, std::vector<float> blue, std::vector<float> alpha); //added by Ting (23/11/2019)

    void estimateDefaultRanges(int uMim, int uMax, std::vector<int> center, std::vector<int> width);

    void getEstimateDefaultRanges(int &tfMin, int &tfMax);

    void createDefaultTransferFunctionPoints(int tfMin, int tfMax, int uMax);

    std::vector<float> getDefaultTransferFunctionPoints();

    VolumeType getVolumeType() const;

    bool isVolumeLoadedAsTexture() const;
    void setIsVolumeLoadedAsTexture(bool isVolumeLoadedAsTexture);

    ModelVolumeEvent useNormalizedVolume();
    ModelVolumeEvent useEqualizedVolume();

    bool isUsingEqualizedVolume() const;

private:
    void computedScaleFactors();

    void computeIntensityMap();

    unsigned short m_volumeDimension[3];
    double m_pixelSpacing[3];
    float m_phyScale[3];

    bool m_isScaleFactorsComputed;
    float m_scaleFactors[4];

    vmtkTFBuffer *m_transferFunction;

    typedef struct
    {
        int mValue;
        int mMax;
        int mMin;
        bool mState;

    } ThresholdData;
    ThresholdData mThresholdData;

    unsigned short *m_grayscaleVolumeDataUshort;
    GrayscaleImage3DUshort::sPtr m_mappedGrayscaleVolume;

    GrayscaleImage3DFloat::sPtr m_grayscaleVolumeDataFloat;
    float *m_rgbVolumeData;

    DataBuffer::sPtr m_intensityMap;

    unsigned short m_bitsAllocated;
    int m_umax;

    VolumeType m_volumeType;

    SampleInterpolation m_sampleInterpolation;

    std::vector<float> mTranferFunctionRedPoints;   /**< sequence of points along the red color piecewise linear mapping */
    std::vector<float> mTranferFunctionGreenPoints; /**< sequence of points along the green color piecewise linear mapping */
    std::vector<float> mTranferFunctionBluePoints;  /**< sequence of points along the blue color piecewise linear mapping */
    std::vector<float> mTranferFunctionAlphaPoints; /**< sequence of points along the alpha piecewise linear mapping */

    int m_tfMin;
    int m_tfMax;
    std::vector<float> m_defaultTransferFunctionPoints;

    bool m_isVolumeLoadedAsTexture;

    bool m_useEqualizedVolume;

};

#endif // MODEL_VOLUME_H
