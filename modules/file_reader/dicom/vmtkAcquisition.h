#ifndef VMTK_ACQUISITION_H
#define VMTK_ACQUISITION_H

#include "vmtkAcquisitionAbstract.h"

#include <vector>

class vmtkAcquisition: public vmtkAcquisitionAbstract
{
    friend class vmtkReorientImageVolumeToLPS;

public:
    vmtkAcquisition();
    ~vmtkAcquisition();

    unsigned short getDimensions(unsigned short index) const;
    void setDimensions(unsigned short index, unsigned short value);

    double getPixelSpacing(unsigned short index) const;
    void setPixelSpacing(unsigned short index, double value);

    double getOrigin(unsigned short index) const;
    void setOrigin(unsigned short index, double value);

    double getDirCos(unsigned short rol, unsigned short col) const;
    void setDirCos(unsigned short rol, unsigned short col, double value);

    /**
     * @brief get the type of the representation of pixel data value.
     * @return 0 (unsigned) or 1 (signed)
     */
    unsigned short getPixelRepresentation() const;    //Included by Ting (5/2/2018)
    /**
     * @brief set the type of the representation of pixel data value.
     * @param value 0 (unsigned) or 1 (signed)
     */
    void setPixelRepresentation(unsigned short value); //Included by Ting (5/2/2018)

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

    unsigned int getImageBufferLength() const;
    void setImageBufferLength(unsigned int value);

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
     * @return reference to the list of window centers.
     */
    std::vector<int> &getVOILUTWindowCenter ();    //Included by Ting (17/11/2019)
    /**
     * @brief set the list of window centers of implicit linear transformation to the presentation of the stored pixel values.
     * @param value_array list of window centers.
     */
    void setVOILUTWindowCenter (std::vector<int> value_array); //Included by Ting (17/11/2019)
    /**
     * @brief clear the vector m_VOILUT_win_center
     */
    void clearVOILUTWindowCenter (); //Included by Ting (24/11/2019)

    /**
     * @brief get the list of window widths of implicit linear transformation to the presentation of the stored pixel values.
     * @return reference to the list of window widths.
     */
    std::vector<int> &getVOILUTWindowWidth ();    //Included by Ting (17/11/2019)
    /**
     * @brief set the list of window widths of implicit linear transformation to the presentation of the stored pixel values.
     * @param value_array list of window widths.
     */
    void setVOILUTWindowWidth (std::vector<int> value_array); //Included by Ting (17/11/2019)
    /**
     * @brief clear the vector m_VOILUT_win_width
     */
    void clearVOILUTWindowWidth (); //Included by Ting (24/11/2019)

    char *getImageBuffer() const;
    char *getImageBuffer();
    void setImageBuffer(char *value);

    /**
     * @brief get the minimum (stored) raw data value. If it is negative, it is displaced to 0 in the processed data.
     * @return the minimum raw data value.
     */
    int getRawUmin() const;
    /**
     * @brief set  the minimum (stored) raw data value.
     * @param value the minimum raw data value.
     */
    void setRawUmin(int value);
    /**
     * @brief get the minimum displaced data value.
     * @return the minimum displaced data value.
     */
    int getUmin() const;

    /**
     * @brief get the maximum (stored) raw data value. If Umin is negative, offseting of Umin must be performed.
     * @return the maximum raw data value.
     */
    int getRawUmax() const;
    /**
     * @brief set the maximum (stored) raw data value.
     * @param value the maximum raw data value.
     */
    void setRawUmax(int value);
    /**
     * @brief get the maximum displaced data value.
     * @return the maximum displaced data value.
     */
    int getUmax() const;

    void printAttributes();

    const char *operator()(unsigned short x, unsigned short y, unsigned short z, unsigned short type_size) const
    { return &(m_image_buffer[type_size * (x + y * m_dimension[0] + z * m_dimension[0] * m_dimension[1])]); }

    char *operator()(unsigned short x, unsigned short y, unsigned short z, unsigned short type_size)
    { return &(m_image_buffer[type_size * (x + y * m_dimension[0] + z * m_dimension[0] * m_dimension[1])]); }

private:
    unsigned short m_dimension[3];          // m_dimension[0]: rows; m_dimension[1]: cols; m_dimension[2]: planes
    double m_pixel_spacing[3];              // m_pixel_spacing[0]: width; m_pixel_spacing[1]: height; m_pixel_spacing[2]: depth
    double m_origin[3];                     // Image Position (Patient)
    double m_dircos[3][3];                  // Image Orientation (Patient) basis vectors with respect to LPS
    unsigned short m_pixel_representation;  // Pixel representation  (adde by Ting in 5/2/2018)
    unsigned short m_samples_per_pixel;     // Samples per pixel
    unsigned short m_bits_allocated;        // Bits allocated
    unsigned short m_bits_stored;           // Bits stored
    unsigned short m_high_bit;              // High bit
    float m_rescale_slope;                  // Rescale slope
    float m_rescale_intercept;              // Rescale intercept
    unsigned short m_photometric_interpretation; //Photometric Interpreation of the pixel values (addes by Ting in 17/11/2019)
    std::vector<int> m_VOILUT_win_center;         // Center of the windowing function for pixel storage value interpretation (added by Ting in 17/11/2019)
    std::vector<int> m_VOILUT_win_width;          // Width of the windowing function for pixel storage value interpretation (added by Ting in 17/11/2019)
    unsigned int m_image_buffer_length;
    char *m_image_buffer;

    int m_umin;
    int m_umax;
};

#endif // VMTK_ACQUISITION_H
