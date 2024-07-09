/**
  *
  *
  *
  *
  */

#include "vmtkDiffAcquisition.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <cmath>
#include <cstring>
#include "modules/file_reader/dicom/vmtkAcquisition.h"


vmtkDiffAcquisition::vmtkDiffAcquisition( ):
    vmtkAcquisitionAbstract("vmtkDiffAcquisition"),
    m_diff_volumes(NULL),
    m_filter_volume(NULL),
    m_num_b0_vol(0),
    m_num_gradient_vol(0)
{
    setUmax(std::numeric_limits<int>::min( ));
    setUmin(std::numeric_limits<int>::max( ));
}

/**
 * @brief vmtkDiffAcquisition::getDimensions
 * @param index
 * @return
 */
unsigned short vmtkDiffAcquisition::getDimensions(unsigned short index) const
{
    if(index <= 3)
        return m_dimension[index];
    else{
        std::cerr << "vmtkDiffAcquisition::getDimensions -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkDiffAcquisition::setDimensions(unsigned short index, unsigned short value)
{
    if(index <= 3)
    {
        if(index == 3 && value > 0)
        {
            if(m_diff_volumes != NULL)
                delete[ ] m_diff_volumes;

            m_diff_volumes = new DiffusionVolumes[value];
        }

        this->m_dimension[index] = value;

    } else
        std::cerr << "vmtkDiffAcquisition::setDimensions -> Index out of range! Value = " << index << std::endl;
}

double vmtkDiffAcquisition::getPixelSpacing(unsigned short index) const
{
    if(index <= 2)
        return m_pixel_spacing[index];
    else {
        std::cerr << "vmtkDiffAcquisition::getPixelSpacing -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkDiffAcquisition::setPixelSpacing(unsigned short index, double value)
{
    if(index <= 2)
        this->m_pixel_spacing[index] = value;
    else
        std::cerr << "vmtkDiffAcquisition::setPixelSpacing -> Index out of range! Value = " << index << std::endl;
}

double vmtkDiffAcquisition::getOrigin(unsigned short index) const
{
    if(index <= 2)
        return m_origin[index];
    else {
        std::cerr << "vmtkDiffAcquisition::getOrigin -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

void vmtkDiffAcquisition::setOrigin(unsigned short index, double value)
{
    if(index <= 2)
        this->m_origin[index] = value;
    else
        std::cerr << "vmtkDiffAcquisition::setOrigin -> Index out of range! Value = " << index << std::endl;
}

double vmtkDiffAcquisition::getDirCos(unsigned short row, unsigned short col) const
{
    if((row <= 2) && (col <= 2))
        return m_dircos[row][col];
    else {
        std::cerr << "vmtkDiffAcquisition::getDirCos -> Index out of range! Value = [" << row << "][" << col << "]" << std::endl;
        return 0;
    }
}

void vmtkDiffAcquisition::setDirCos(unsigned short row, unsigned short col, double value)
{
    if((row <= 2) && (col <= 2))
        m_dircos[row][col] = value;
    else
        std::cerr << "vmtkDiffAcquisition::setDirCos -> Index out of range! Value = [" << row << "][" << col << "]" << std::endl;
}

unsigned short vmtkDiffAcquisition::getSamplesPerPixel() const
{
    return m_samples_per_pixel;
}

void vmtkDiffAcquisition::setSamplesPerPixel(unsigned short value)
{
    m_samples_per_pixel = value;
}

unsigned short vmtkDiffAcquisition::getBitsAllocated() const
{
    return m_bits_allocated;
}

void vmtkDiffAcquisition::setBitsAllocated(unsigned short value)
{
    m_bits_allocated = value;
}

unsigned short vmtkDiffAcquisition::getBitsStored() const
{
    return m_bits_stored;
}

void vmtkDiffAcquisition::setBitsStored(unsigned short value)
{
    m_bits_stored = value;
}

unsigned short vmtkDiffAcquisition::getHighBit() const
{
    return m_high_bit;
}

void vmtkDiffAcquisition::setHighBit(unsigned short value)
{
    m_high_bit = value;
}

float vmtkDiffAcquisition::getRescaleSlope() const
{
    return m_rescale_slope;
}

void vmtkDiffAcquisition::setRescaleSlope(float value)
{
    m_rescale_slope = value;
}

float vmtkDiffAcquisition::getRescaleIntercept() const
{
    return m_rescale_intercept;
}

void vmtkDiffAcquisition::setRescaleIntercept(float value)
{
    m_rescale_intercept = value;
}

unsigned short vmtkDiffAcquisition::getPhotometricInterpretation () const
{
    return m_photometric_interpretation;
}

void vmtkDiffAcquisition::setPhotometricInterpretation (unsigned short value)
{
    m_photometric_interpretation = value;
}

std::vector<int> &vmtkDiffAcquisition::getVOILUTWindowCenter ()
{
    return m_VOILUT_win_center;
}

void vmtkDiffAcquisition::setVOILUTWindowCenter (std::vector<int> value_array)
{
    for (int i = 0; i < value_array.size(); i++) {
         m_VOILUT_win_center.push_back(value_array[i]);
    }
}

void vmtkDiffAcquisition::clearVOILUTWindowCenter ()
{
         m_VOILUT_win_center.clear();
}

std::vector<int> &vmtkDiffAcquisition::getVOILUTWindowWidth ()
{
    return m_VOILUT_win_width;
}

void vmtkDiffAcquisition::setVOILUTWindowWidth (std::vector<int> value_array)
{
    for (int i = 0; i < value_array.size(); i++) {
         m_VOILUT_win_width.push_back(value_array[i]);
    }
}

void vmtkDiffAcquisition::clearVOILUTWindowWidth ()
{
         m_VOILUT_win_width.clear();
}

unsigned int vmtkDiffAcquisition::getImageBufferLength() const
{
    return m_image_buffer_length;
}

void vmtkDiffAcquisition::setImageBufferLength(unsigned int value)
{
    m_image_buffer_length = value;
}


const char* vmtkDiffAcquisition::getImageBuffer(unsigned short index) const
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            return m_diff_volumes[index].m_volume_buffer;
        else
            std::cerr << "vmtkDiffAcquisition::getImageBuffer -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::getImageBuffer -> Diffusion volumes buffer not allocated!" << std::endl;

    return NULL;
}


char* vmtkDiffAcquisition::getImageBuffer(unsigned short index)
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            return m_diff_volumes[index].m_volume_buffer;
        else
            std::cerr << "vmtkDiffAcquisition::getImageBuffer -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::getImageBuffer -> Diffusion volumes buffer not allocated!" << std::endl;

    return NULL;
}

void vmtkDiffAcquisition::setImageBuffer(unsigned short index, char *value)
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            m_diff_volumes[index].m_volume_buffer = value;
        else
            std::cerr << "vmtkDiffAcquisition::setImageBuffer -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::setImageBuffer -> Diffusion volumes buffer not allocated!" << std::endl;

}

void vmtkDiffAcquisition::findMinMaxValues()
{
    for(unsigned short v = 0; v < m_dimension[3]; v++)
    {
        unsigned int iz, iy, ix, index;

        unsigned short *volume_buffer = reinterpret_cast<unsigned short*>(m_diff_volumes[v].m_volume_buffer);
        for (iz = 0; iz < getDimensions(2); iz++)
            for (iy =0; iy < getDimensions(1); iy++)
                for (ix =0; ix < getDimensions(0); ix++)
                {
                    index = ix + iy * m_dimension[0] + iz * m_dimension[0] * m_dimension[1];
                    unsigned short value = volume_buffer[index];

                    if(getRawUmax() < (int)value)
                        setRawUmax((int)value);

                    if(getRawUmin() > (int)value)
                        setRawUmin((int)value);

                    if(getRawUmax(v) < (int)value)
                        setRawUmax(v, (int)value);

                    if(getRawUmin(v) > (int)value)
                        setRawUmin(v, (int)value);

                }
    }
}

void vmtkDiffAcquisition::removeVolume(unsigned short volume_index)
{
    if(m_diff_volumes != nullptr)
    {
        if(volume_index < m_dimension[3] - 1)
        {
            delete[] m_diff_volumes[volume_index].m_volume_buffer;
            m_diff_volumes[volume_index].copyData(m_diff_volumes[m_dimension[3] - 1]);

            m_diff_volumes[m_dimension[3] - 1].m_volume_buffer = nullptr;
            m_dimension[3] = m_dimension[3] - 1;

        } else if(volume_index == m_dimension[3] - 1) {

            delete[] m_diff_volumes[volume_index].m_volume_buffer;
            m_diff_volumes[m_dimension[3] - 1].m_volume_buffer = nullptr;
            m_dimension[3] = m_dimension[3] - 1;

        } else
            std::cerr << "vmtkDiffAcquisition::removeVolume -> Volume index " << volume_index << "is out of range!" << std::endl;

    } else
        std::cerr << "vmtkDiffAcquisition::removeVolume -> Diffusion volumes not allocated!" << std::endl;
}


int vmtkDiffAcquisition::getRawUmin() const
{
    return getUmin();
}

void vmtkDiffAcquisition::setRawUmin(int value)
{
    setUmin(value);
}

int vmtkDiffAcquisition::getRawUmin(unsigned short index) const
{
    return getUmin(index);
}

void vmtkDiffAcquisition::setRawUmin(unsigned short index, int value)
{
    setUmin(index, value);
}

int vmtkDiffAcquisition::getUmin() const
{
    return m_umin;
}

int vmtkDiffAcquisition::getUmin(unsigned short index) const
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            return m_diff_volumes[index].m_umin;
        else
            std::cerr << "vmtkDiffAcquisition::getUmin -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::getUmin -> Diffusion volumes buffer not allocated!" << std::endl;

    return 0;
}

void vmtkDiffAcquisition::setUmin(int value)
{
    m_umin = value;
}

void vmtkDiffAcquisition::setUmin(unsigned short index, int value)
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            m_diff_volumes[index].m_umin = value;
        else
            std::cerr << "vmtkDiffAcquisition::setUmin -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::setUmin -> Diffusion volumes buffer not allocated!" << std::endl;

}

int vmtkDiffAcquisition::getRawUmax() const
{
    return getUmax();
}

void vmtkDiffAcquisition::setRawUmax(int value)
{
    setUmax(value);
}

int vmtkDiffAcquisition::getRawUmax(unsigned short index) const
{
    return getUmax(index);
}

void vmtkDiffAcquisition::setRawUmax(unsigned short index, int value)
{
    setUmax(index, value);
}

int vmtkDiffAcquisition::getUmax() const
{
    return m_umax;
}

int vmtkDiffAcquisition::getUmax(unsigned short index) const
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            return m_diff_volumes[index].m_umax;
        else
            std::cerr << "vmtkDiffAcquisition::getUmax -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::getUmax -> Diffusion volumes buffer not allocated!" << std::endl;

    return 0;
}

void vmtkDiffAcquisition::setUmax(int value)
{
    m_umax = value;
}

void vmtkDiffAcquisition::setUmax(unsigned short index, int value)
{
    if(m_diff_volumes != NULL)
    {
        if(index < m_dimension[3])
            m_diff_volumes[index].m_umax = value;
        else
            std::cerr << "vmtkDiffAcquisition::setUmax -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::setUmax-> Diffusion volumes buffer not allocated!" << std::endl;

}


float vmtkDiffAcquisition::getBValue(unsigned short index) const
{
    if(index <= m_dimension[3])
        return m_diff_volumes[index].m_bvalue;
    else{
        std::cerr << "vmtkDiffAcquisition::getBValue -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

float* vmtkDiffAcquisition::getBValues( ) const
{
    unsigned short numDiffusionMeasures = getNumDiffusionMeasures( );
    float *bvalues = new float[numDiffusionMeasures];

    for (unsigned short i = 0; i < numDiffusionMeasures; i++)
        bvalues[i] = getBValue(i);

    return bvalues;
}

void vmtkDiffAcquisition::setBValue(unsigned short index, float value)
{
    if(index <= m_dimension[3])
        m_diff_volumes[index].m_bvalue = value;
    else
        std::cerr << "vmtkDiffAcquisition::setBValue -> Index out of range! Value = " << index << std::endl;

}

void vmtkDiffAcquisition::setGradientOrientation(unsigned short index, float x, float y, float z)
{
    if(index < m_dimension[3])
    {
        if(m_diff_volumes != NULL)
        {
            m_diff_volumes[index].m_gradient[0] = x;
            m_diff_volumes[index].m_gradient[1] = y;
            m_diff_volumes[index].m_gradient[2] = z;

        } else
            std::cerr << "vmtkDiffAcquisition::setGradientOrientation -> Diffusion volumes buffer not allocated!" << std::endl;

    } else
        std::cerr << "vmtkDiffAcquisition::setGradientOrientation -> Index out of range! Value = " << index << std::endl;
}

void vmtkDiffAcquisition::getGradientOrientation(unsigned int index, float &x, float &y, float &z) const
{
    if(index < m_dimension[3])
    {
        if(m_diff_volumes != NULL)
        {
            x = m_diff_volumes[index].m_gradient[0];
            y = m_diff_volumes[index].m_gradient[1];
            z = m_diff_volumes[index].m_gradient[2];

            return;

        } else
            std::cerr << "vmtkDiffAcquisition::setGradientOrientation -> Diffusion volumes buffer not allocated!" << std::endl;

    } else
        std::cerr << "vmtkDiffAcquisition::setGradientOrientation -> Index out of range! Value = " << index << std::endl;

    x = y = z = 0;
}

float* vmtkDiffAcquisition::getGradientOrientations( ) const
{
    unsigned short numDiffusionMeasures = getNumDiffusionMeasures( );
    float *bvectors = new float[numDiffusionMeasures * 3];

    for (unsigned short i = 0; i < numDiffusionMeasures; i++)
        getGradientOrientation(i, bvectors[i * 3], bvectors[i * 3 + 1], bvectors[i * 3 + 2]);

    return bvectors;
}

vmtkAcquisition *vmtkDiffAcquisition::getSingleAcquisition(unsigned short index)
{
    vmtkAcquisition *single_acq = new vmtkAcquisition();

    single_acq->setAcquisitionPath(getacquisitionPath());
    single_acq->setModalityStr(getModalityStr());
    single_acq->setInstitutionName(getInstitutionName());
    single_acq->setManufacturesModel(getManufacturesModel());
    single_acq->setSeriesDescription(getSeriesDescription());
    single_acq->setPatientID(getPatientID());
    single_acq->setPatientBirthDate(getPatientBirthDate());
    single_acq->setStudyDescription(getStudyDescription());

    single_acq->setType(VMTK_MRI_T1_T2_ACQUISITION);
    single_acq->setId(getId());

    single_acq->setDimensions(0, getDimensions(0));
    single_acq->setDimensions(1, getDimensions(1));
    single_acq->setDimensions(2, getDimensions(2));

    single_acq->setOrigin(0, getOrigin(0));
    single_acq->setOrigin(1, getOrigin(1));
    single_acq->setOrigin(2, getOrigin(2));

    single_acq->setPixelSpacing(0, getPixelSpacing(0));
    single_acq->setPixelSpacing(1, getPixelSpacing(1));
    single_acq->setPixelSpacing(2, getPixelSpacing(2));

    single_acq->setDirCos(0, 0, getDirCos(0, 0)); single_acq->setDirCos(0, 1, getDirCos(0, 1)); single_acq->setDirCos(0, 2, getDirCos(0, 2));
    single_acq->setDirCos(1, 0, getDirCos(1, 0)); single_acq->setDirCos(1, 1, getDirCos(1, 1)); single_acq->setDirCos(1, 2, getDirCos(1, 2));
    single_acq->setDirCos(2, 0, getDirCos(2, 0)); single_acq->setDirCos(2, 1, getDirCos(2, 1)); single_acq->setDirCos(2, 2, getDirCos(2, 2));

    single_acq->setBitsAllocated(getBitsAllocated());
    single_acq->setBitsStored(getBitsStored());
    single_acq->setHighBit(getHighBit());

    single_acq->setImageBufferLength(getImageBufferLength());
    single_acq->setImageBuffer(getImageBuffer(index));

    single_acq->setRawUmin(getUmin(index));
    single_acq->setRawUmax(getUmax(index));

    return single_acq;
}

void vmtkDiffAcquisition::setFilterVolume(unsigned short *buffer)
{
    if(m_filter_volume != NULL)
    {
        delete[] m_filter_volume;
        m_filter_volume = NULL;
    }

    if(buffer != NULL)
    {
        unsigned int iz, iy, ix, volSize;

        volSize = getDimensions( 0 ) * getDimensions( 1 ) * getDimensions( 2 );
        m_filter_volume = new bool[volSize];
        memset(m_filter_volume, 0x0, volSize * sizeof( bool ) );

        for (iz = 0; iz < getDimensions(2); iz++)
            for (iy =0; iy < getDimensions(1); iy++)
                for (ix =0; ix < getDimensions(0); ix++)
                {
                    unsigned int index = ix + iy * m_dimension[0] + iz * m_dimension[0] * m_dimension[1];
                    if(buffer[index] > 0)
                        m_filter_volume[index] = true;

                }
    }

}

unsigned short *vmtkDiffAcquisition::getADC(unsigned short index, unsigned short *umax)
{
    if(index <= 0)
        index = 1;

    std::cout << "inicio adc" << std::endl;

    unsigned short *b0_vol = reinterpret_cast<unsigned short*>(getImageBuffer(0));
    unsigned short *dwi_vol = reinterpret_cast<unsigned short*>(getImageBuffer(index));
    unsigned short *adc_vol = reinterpret_cast<unsigned short*>(new char[m_image_buffer_length]);

    int num_pixels = m_dimension[0] * m_dimension[1] * m_dimension[2];

    unsigned short aux_umax = 0;
    double max_adc = 0;

    for(int i = 0; i < num_pixels; i++)
    {

        if(b0_vol[i] != 0 && dwi_vol[i] != 0)
        {
            unsigned short dwi_value;

            if(dwi_vol[i] > b0_vol[i])
                dwi_value = b0_vol[i];
            else
                dwi_value = dwi_vol[i];

            double pixel_adc = -m_diff_volumes[index].m_bvalue * log(double(dwi_value) / b0_vol[i]);

            if(pixel_adc > max_adc)
                max_adc = pixel_adc;

            if(pixel_adc < 0)
            {
                std::cerr << "pixel_adc = " << pixel_adc <<
                             " b0_vol[" << i << "] = " << b0_vol[i] <<
                             " dwi_vol[" << i << "] = " << dwi_vol[i] << std::endl;
            }

            adc_vol[i] = static_cast<unsigned short>(pixel_adc);
        }else
            adc_vol[i] = 0;

        if(adc_vol[i] > aux_umax)
            aux_umax = adc_vol[i];

    }

    std::cout << "aux_umax " << (aux_umax) << std::endl;
    std::cout << "max_adc " << max_adc << std::endl;
    std::cout << "fim adc" << std::endl;

    *umax = aux_umax;
    std::cout << "umax " << (*umax) << std::endl;

    return adc_vol;
}


void vmtkDiffAcquisition::printAttributes()
{
    std::cout << "*********************** vmtkDiffAcquisition Attributes ***********************" << std::endl;
    vmtkAcquisitionAbstract::printAttributes();

    std::cout << "Dimensions: (" << m_dimension[0] << "," << m_dimension[1] << "," << m_dimension[2] << "," << m_dimension[3] << ")" << std::endl ;
    std::cout << "Origin: (" << m_origin[0] << "," << m_origin[1] << "," << m_origin[2] << ")" << std::endl;
    std::cout << "Spacing: (" << m_pixel_spacing[0] << "," << m_pixel_spacing[1] << "," << m_pixel_spacing[2] << ")" << std::endl;
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

bool vmtkDiffAcquisition::isDWI( unsigned short index ) const
{
    if( m_diff_volumes != NULL )
    {
        if( index < m_dimension[ 3 ] )
            return m_diff_volumes[ index ].m_is_DWI;
        else
            std::cerr << "vmtkDiffAcquisition::isDWI -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::isDWI -> Diffusion volumes buffer not allocated!" << std::endl;

    return false;
}

bool vmtkDiffAcquisition::isB0( unsigned short index ) const
{
    if( m_diff_volumes != NULL )
    {
        if( index < m_dimension[ 3 ] )
            return m_diff_volumes[ index ].m_is_b0;
        else
            std::cerr << "vmtkDiffAcquisition::isB0 -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::isB0 -> Diffusion volumes buffer not allocated!" << std::endl;

    return false;
}

void vmtkDiffAcquisition::setIsDWI( unsigned short index, bool value )
{
    if( m_diff_volumes != NULL )
    {
        if( index < m_dimension[ 3 ] )
            m_diff_volumes[ index ].m_is_DWI = value;
        else
            std::cerr << "vmtkDiffAcquisition::setIsDWI -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::setIsDWI -> Diffusion volumes buffer not allocated!" << std::endl;

}

void vmtkDiffAcquisition::setIsB0( unsigned short index, bool value )
{
    if( m_diff_volumes != NULL )
    {
        if( index < m_dimension[ 3 ] )
            m_diff_volumes[ index ].m_is_b0 = value;
        else
            std::cerr << "vmtkDiffAcquisition::setIsB0 -> Index out of range! Value = " << index << std::endl;
    } else
        std::cerr << "vmtkDiffAcquisition::setIsB0 -> Diffusion volumes buffer not allocated!" << std::endl;

}

unsigned short vmtkDiffAcquisition::getNumDiffusionMeasures() const
{
    return (getNumB0Vol( ) + getNumGradientVol( ));
}

unsigned short *vmtkDiffAcquisition::getMask() const
{
    return m_mask;
}

unsigned short vmtkDiffAcquisition::getNumGradientVol() const
{
    return m_num_gradient_vol;
}

void vmtkDiffAcquisition::setNumGradientVol(unsigned short num_gradient_img)
{
    m_num_gradient_vol = num_gradient_img;
}

unsigned short vmtkDiffAcquisition::getNumB0Vol() const
{
    return m_num_b0_vol;
}

void vmtkDiffAcquisition::setNumB0Vol(unsigned short num_b0_img)
{
    m_num_b0_vol = num_b0_img;
}

unsigned short vmtkDiffAcquisition::getPixelRepresentation() const      //Included by Ting (5/2/2018)
{
    return m_pixel_representation;
}

void vmtkDiffAcquisition::setPixelRepresentation(unsigned short value)   //Included by Ting (5/2/2018)
{
    m_pixel_representation = value;
}
