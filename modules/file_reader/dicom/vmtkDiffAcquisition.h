#ifndef VMTK_DIFFUSION_ACQUISITION_H
#define VMTK_DIFFUSION_ACQUISITION_H

#include "vmtkAcquisition.h"
#include <iostream>
#include <limits>
#include <mutex>

class vmtkDiffAcquisition: public vmtkAcquisitionAbstract
{
public:
    vmtkDiffAcquisition();

    unsigned short getDimensions(unsigned short index) const;
    void setDimensions(unsigned short index, unsigned short value);

    double getPixelSpacing(unsigned short index) const;
    void setPixelSpacing(unsigned short index, double value);

    double getOrigin(unsigned short index) const;
    void setOrigin(unsigned short index, double value);

    double getDirCos(unsigned short rol, unsigned short col) const;
    void setDirCos(unsigned short rol, unsigned short col, double value);

    unsigned short getSamplesPerPixel() const;
    void setSamplesPerPixel(unsigned short value);

    unsigned short getBitsAllocated() const;
    void setBitsAllocated(unsigned short value);

    unsigned short getBitsStored() const;
    void setBitsStored(unsigned short value);

    unsigned short getHighBit() const;
    void setHighBit(unsigned short value);

    float getRescaleSlope() const;
    void setRescaleSlope(float value);

    float getRescaleIntercept() const;
    void setRescaleIntercept(float value);

    /**
     * @brief get the photometric interpretation of the stored pixel values.
     * @return gdcm::PhotometricInterpretation::PIType: 0 (UNKNOW), 1 (MONOCHROME1), 2 (MONOCHROME2),
     * 3 (PALETTE_COLOR), 4 (RGB), 5 (HSV), 6 (ARGB), 7 (CMYK), 8 (YBR_FULL), 9 (YBR_FULL_422),
     * 10 (YBR_PARTIAL_422), 11 (YBR_PARTIAL_420), 12 (YBR_ICT), 13 (YBR_RCT), or 14 (PI_END).
     */
    unsigned short getPhotometricInterpretation() const;    //Included by Ting (17/11/2019)
    /**
     * @brief set the photometric interpretation of the stored pixel values.
     * @param value photometric interpretation (PIType) read from the DICOM file.
     */
    void setPhotometricInterpretation (unsigned short value); //Included by Ting (17/11/2019)

    /**
     * @brief get the list of window centers of implicit linear transformation to the presentation of the stored pixel values.
     * @return pointer to the list of window centers.
     */
    std::vector<int> &getVOILUTWindowCenter ();    //Included by Ting (17/11/2019)
    /**
     * @brief set the list of window centers of implicit linear transformation to the presentation of the stored pixel values.
     * @param value_array pointer to the list of window centers.
     */
    void setVOILUTWindowCenter (std::vector<int> value_array); //Included by Ting (17/11/2019)
    /**
     * @brief clear the vector m_VOILUT_win_center
     */
    void clearVOILUTWindowCenter (); //Included by Ting (24/11/2019)

    /**
     * @brief get the list of window widths of implicit linear transformation to the presentation of the stored pixel values.
     * @return pointer to the list of window widths.
     */
    std::vector<int> &getVOILUTWindowWidth ();    //Included by Ting (17/11/2019)
    /**
     * @brief set the list of window widths of implicit linear transformation to the presentation of the stored pixel values.
     * @param value_array pointer to the list of window widths.
     */
    void setVOILUTWindowWidth (std::vector<int> value_array); //Included by Ting (17/11/2019)
    /**
     * @brief clear the vector m_VOILUT_win_width
     */
    void clearVOILUTWindowWidth (); //Included by Ting (24/11/2019)

    unsigned int getImageBufferLength() const;
    void setImageBufferLength(unsigned int value);

    const char *getImageBuffer(unsigned short index) const;
    char *getImageBuffer(unsigned short index);
    void setImageBuffer(unsigned short index, char *value);


    int getUmin() const;
    void setUmin(int value);

    int getUmax() const;
    void setUmax(int value);

    int getUmin(unsigned short index) const;
    void setUmin(unsigned short index, int value);

    int getUmax(unsigned short index) const;
    void setUmax(unsigned short index, int value);

    /**
     * @brief get the minimum of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @return the minimum of the (stored) raw data.
     */
    int getRawUmin() const;
    /**
     * @brief get the minimum of the volume <index> of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param index the index of the diffusion volume (per gradient)
     * @return the minimum of the (stored) raw data in the volume.
     */
    int getRawUmin(unsigned short index) const;
    /**
     * @brief set the minimum of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param value the minimum of the (stored) raw data in the volume.
     */
    void setRawUmin(int value);
    /**
     * @brief get the minimum of the volume <index> of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param index the index of the diffusion volume (per gradient)
     * @param value the minimum of the (stored) raw data in the volume.
     */
    void setRawUmin(unsigned short index, int value);

    /**
     * @brief get the maximum of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @return the maximum of the (stored) raw data.
     */
    int getRawUmax() const;
    /**
     * @brief get the maximum of the volume <index> of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param index the index of the diffusion volume (per gradient)
     * @return the maximum of the (stored) raw data in the volume.
     */
    int getRawUmax(unsigned short index) const;
    /**
     * @brief set the maximum of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param value the maximum of the (stored) raw data.
     */
    void setRawUmax(int value);
    /**
     * @brief get the maximum of the volume <index> of the (stored) raw data. This function is written to make the module compatible with the other functions. It must be revised.
     * @param index the index of the diffusion volume (per gradient)
     * @param value the maximum of the (stored) raw data in the volume.
     */
    void setRawUmax(unsigned short index, int value);

    void setBValue(unsigned short index, float value);
    float getBValue(unsigned short index) const;

    unsigned short *getADC(unsigned short index, unsigned short *umax);

    void printAttributes();

    void setGradientOrientation(unsigned short index, float x, float y, float z);
    void getGradientOrientation(unsigned int index, float &x, float &y, float &z) const;
    float* getGradientOrientations( ) const;

    vmtkAcquisition *getSingleAcquisition(unsigned short index);
    void setFilterVolume(unsigned short *buffer);

    unsigned short getNumB0Vol() const;
    void setNumB0Vol(unsigned short num_b0_img);

    unsigned short getNumGradientVol() const;
    void setNumGradientVol(unsigned short num_gradient_img);

    bool isDWI(unsigned short index) const;
    bool isB0(unsigned short index) const;
    void setIsDWI(unsigned short index, bool value);
    void setIsB0(unsigned short index, bool value);

    unsigned short getNumDiffusionMeasures() const;
    float* getBValues( ) const;


    unsigned short getPixelRepresentation() const;    //Included by Ting (5/2/2018)
    void setPixelRepresentation(unsigned short value); //Included by Ting (5/2/2018)

    unsigned short *getMask() const;

    void findMinMaxValues();
    void removeVolume(unsigned short volume_index);

private:

    typedef struct _diffusion_volumes
    {
        int m_bvalue;
        float m_gradient[3];
        int m_umin;
        int m_umax;
        char *m_volume_buffer;
        bool m_is_DWI;
        bool m_is_b0;

        _diffusion_volumes()
        {
            m_bvalue = 0;
            m_gradient[0] = m_gradient[1] = m_gradient[2] = 0;
            m_umin = std::numeric_limits<int>::max();
            m_umax = std::numeric_limits<int>::min();
            m_volume_buffer = NULL;
            m_is_DWI = false;
            m_is_b0 = false;
        }

        void copyData(_diffusion_volumes &data)
        {
            m_bvalue = data.m_bvalue;
            m_gradient[0] = data.m_gradient[0];
            m_gradient[1] = data.m_gradient[1];
            m_gradient[2] = data.m_gradient[2];
            m_umin = data.m_umin;
            m_umax = data.m_umax;
            m_volume_buffer = data.m_volume_buffer;
            m_is_DWI = data.m_is_DWI;
            m_is_b0 = data.m_is_b0;
        }

        ~_diffusion_volumes()
        {
            if(m_volume_buffer != NULL)
                delete m_volume_buffer;
            m_volume_buffer = NULL;
        }

    }DiffusionVolumes;

    DiffusionVolumes *m_diff_volumes;

    unsigned short m_dimension[4];          // m_dimension[0]: rows; m_dimension[1]: cols; m_dimension[2]: slices per volume; m_dimension[3]: volumes
    double m_pixel_spacing[3];              // m_pixel_spacing[0]: width; m_pixel_spacing[1]: height; m_pixel_spacing[2]: depth
    double m_origin[3];                     // Image Position (Patient)
    double m_dircos[3][3];                  // Image Orientation (Patient) basis vectors with respect to LPS
    unsigned short m_samples_per_pixel;     // Samples per pixel
    unsigned short m_bits_allocated;        // Bits allocated
    unsigned short m_bits_stored;           // Bits stored
    unsigned short m_high_bit;              // High bit
    unsigned short m_pixel_representation;  // Pixel Representation (0028,0103): 0 unsigned integer, 1 signed integer
    float m_rescale_slope;                  // Rescale slope
    float m_rescale_intercept;              // Rescale intercept
    unsigned short m_photometric_interpretation; //Photometric Interpreation of the pixel values (addes by Ting in 17/11/2019)
    unsigned int m_VOILUT_win_number; // Number of windowing (linear) transformation for pixel storage value interpretation (added by Ting in 17/11/2019)
    std::vector<int> m_VOILUT_win_center;         // Center of the windowing function for pixel storage value interpretation (added by Ting in 17/11/2019)
    std::vector<int> m_VOILUT_win_width;          // Width of the windowing function for pixel storage value interpretation (added by Ting in 17/11/2019)
    unsigned int m_image_buffer_length;

    bool *m_filter_volume;

    int m_umin;
    int m_umax;

    unsigned short m_num_b0_vol;
    unsigned short m_num_gradient_vol;

    std::mutex m_mutex;

    unsigned short *m_mask = NULL;
};

#endif // VMTK_DIFFUSION_ACQUISITION_H
