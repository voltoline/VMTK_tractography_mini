#include "vmtkAcquisition.h"
#include <iostream>
#include <cstring>
#include <limits>

vmtkAcquisition::vmtkAcquisition():
    vmtkAcquisitionAbstract("vmtkAcquisition"),
    m_image_buffer(NULL)
{
    setRawUmax( std::numeric_limits<int>::min() );
    setRawUmin( std::numeric_limits<int>::max() );
}

vmtkAcquisition::~vmtkAcquisition()
{
    if(m_image_buffer != NULL)
        delete[] m_image_buffer;
}

unsigned short vmtkAcquisition::getDimensions(unsigned short index) const
{
    if(index <= 2)
        return m_dimension[index];
    else{
        std::cerr << "vmtkAcquisition::getDimensions -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkAcquisition::setDimensions(unsigned short index, unsigned short value)
{
    if(index <= 2)
        this->m_dimension[index] = value;
    else
        std::cerr << "vmtkAcquisition::setDimensions -> Index out of range! Value = " << index << std::endl;
}

double vmtkAcquisition::getPixelSpacing(unsigned short index) const
{
    if(index <= 2)
        return m_pixel_spacing[index];
    else {
        std::cerr << "vmtkAcquisition::getPixelSpacing -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkAcquisition::setPixelSpacing(unsigned short index, double value)
{
    if(index <= 2)
        this->m_pixel_spacing[index] = value;
    else
        std::cerr << "vmtkAcquisition::setPixelSpacing -> Index out of range! Value = " << index << std::endl;
}

double vmtkAcquisition::getOrigin(unsigned short index) const
{
    if(index <= 2)
        return m_origin[index];
    else {
        std::cerr << "vmtkAcquisition::getOrigin -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkAcquisition::setOrigin(unsigned short index, double value)
{
    if(index <= 2)
        this->m_origin[index] = value;
    else
        std::cerr << "vmtkAcquisition::setOrigin -> Index out of range! Value = " << index << std::endl;
}

double vmtkAcquisition::getDirCos(unsigned short row, unsigned short col) const
{
    if((row <= 2) && (col <= 2))
        return m_dircos[row][col];
    else {
        std::cerr << "vmtkAcquisition::getDirCos -> Index out of range! Value = [" << row << "][" << col << "]" << std::endl;
        return 0;
    }
}

void vmtkAcquisition::setDirCos(unsigned short row, unsigned short col, double value)
{
    if((row <= 2) && (col <= 2))
        m_dircos[row][col] = value;
    else
        std::cerr << "vmtkAcquisition::setDirCos -> Index out of range! Value = [" << row << "][" << col << "]" << std::endl;
}

unsigned short vmtkAcquisition::getPixelRepresentation() const      //Included by Ting (5/2/2018)
{
    return m_pixel_representation;
}

void vmtkAcquisition::setPixelRepresentation(unsigned short value)   //Included by Ting (5/2/2018)
{
    m_pixel_representation = value;
}


unsigned short vmtkAcquisition::getSamplesPerPixel() const
{
    return m_samples_per_pixel;
}

void vmtkAcquisition::setSamplesPerPixel(unsigned short value)
{
    m_samples_per_pixel = value;
}

unsigned short vmtkAcquisition::getBitsAllocated() const
{
    return m_bits_allocated;
}

void vmtkAcquisition::setBitsAllocated(unsigned short value)
{
    m_bits_allocated = value;
}

unsigned short vmtkAcquisition::getBitsStored() const
{
    return m_bits_stored;
}

void vmtkAcquisition::setBitsStored(unsigned short value)
{
    m_bits_stored = value;
}

unsigned short vmtkAcquisition::getHighBit() const
{
    return m_high_bit;
}

void vmtkAcquisition::setHighBit(unsigned short value)
{
    m_high_bit = value;
}

float vmtkAcquisition::getRescaleSlope() const
{
    return m_rescale_slope;
}

void vmtkAcquisition::setRescaleSlope(float value)
{
    m_rescale_slope = value;
}

float vmtkAcquisition::getRescaleIntercept() const
{
    return m_rescale_intercept;
}

void vmtkAcquisition::setRescaleIntercept(float value)
{
    m_rescale_intercept = value;
}

unsigned short vmtkAcquisition::getPhotometricInterpretation () const
{
    return m_photometric_interpretation;
}

void vmtkAcquisition::setPhotometricInterpretation (unsigned short value)
{
    m_photometric_interpretation = value;
}

std::vector<int> &vmtkAcquisition::getVOILUTWindowCenter ()
{
    return m_VOILUT_win_center;
}

void vmtkAcquisition::setVOILUTWindowCenter (std::vector<int> value_array)
{
    for (int i = 0; i < value_array.size(); i++) {
         m_VOILUT_win_center.push_back(value_array[i]);
    }
}

void vmtkAcquisition::clearVOILUTWindowCenter ()
{
    m_VOILUT_win_center.clear();
}

std::vector<int> &vmtkAcquisition::getVOILUTWindowWidth ()
{
    return m_VOILUT_win_width;
}

void vmtkAcquisition::setVOILUTWindowWidth (std::vector<int> value_array)
{
    for (int i = 0; i < value_array.size(); i++) {
         m_VOILUT_win_width.push_back(value_array[i]);
    }
}

void vmtkAcquisition::clearVOILUTWindowWidth ()
{
    m_VOILUT_win_width.clear();
}

unsigned int vmtkAcquisition::getImageBufferLength() const
{
    return m_image_buffer_length;
}

void vmtkAcquisition::setImageBufferLength(unsigned int value)
{
    m_image_buffer_length = value;
}

char* vmtkAcquisition::getImageBuffer() const
{
    return m_image_buffer;
}

char* vmtkAcquisition::getImageBuffer()
{
    return m_image_buffer;
}

void vmtkAcquisition::setImageBuffer(char *value)
{
    m_image_buffer = value;
}

int vmtkAcquisition::getRawUmin() const
{
    return m_umin;
}
void vmtkAcquisition::setRawUmin(int value)
{
    m_umin = value;
}

int vmtkAcquisition::getUmin() const
{
    if (m_umin < 0) return m_umin-m_umin;
    else return m_umin;
}

int vmtkAcquisition::getRawUmax() const
{
    return m_umax;
}

void vmtkAcquisition::setRawUmax(int value)
{
    m_umax = value;
}

int vmtkAcquisition::getUmax() const
{
    if (m_umin < 0) return m_umax-m_umin;
    else return m_umax;
}

void vmtkAcquisition::printAttributes()
{
    std::cout << "************************* vmtkAcquisition Attributes *************************" << std::endl;
    vmtkAcquisitionAbstract::printAttributes();

    std::cout << "Dimensions: (" << m_dimension[0] << "," << m_dimension[1] << "," << m_dimension[2] << ")" << std::endl ;
    std::cout << "Origin: (" << m_origin[0] << "," << m_origin[1] << "," << m_origin[2] << ")" << std::endl;
    std::cout << "Spacing: (" << m_pixel_spacing[0] << "," << m_pixel_spacing[1] << "," << m_pixel_spacing[2] << ")" << std::endl;
    std::cout << "Pixel Representation: " << m_pixel_representation << std::endl;
    std::cout << "Samples per pixel: " << m_samples_per_pixel << std::endl;
    std::cout << "Bits Allocated: " << m_bits_allocated << std::endl;
    std::cout << "Bits Stored: " << m_bits_stored << std::endl;
    std::cout << "High Bit: " << m_high_bit << std::endl;
    std::cout << "X: (" << m_dircos[0][0] << "," << m_dircos[0][1] << "," << m_dircos[0][2] << ")" << std::endl;
    std::cout << "Y: (" << m_dircos[1][0] << "," << m_dircos[1][1] << "," << m_dircos[1][2] << ")" << std::endl;
    std::cout << "Z: (" << m_dircos[2][0] << "," << m_dircos[2][1] << "," << m_dircos[2][2] << ")" << std::endl;
    std::cout << "Rescale Slope: " << m_rescale_slope << std::endl;
    std::cout << "Rescale Intercept: " << m_rescale_intercept << std::endl;
    std::cout << "umin : " << m_umin << "; umax : "    << m_umax << std::endl;

    std::cout << "******************************************************************************" << std::endl;
}
