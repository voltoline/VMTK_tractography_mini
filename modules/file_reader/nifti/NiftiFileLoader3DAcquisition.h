/**
  * @file NiftiFileLoader3DAcquisition.h
  * @author Raphael Voltoline
  * @date 2019
  */
#ifndef NIFTI_FILE_LOADER_3D_ACQUISITION_H
#define NIFTI_FILE_LOADER_3D_ACQUISITION_H

#include <iostream>
#include "tipl/tipl.hpp"

class vmtkDiffAcquisition;
class vmtkAcquisitionAbstract;

/**
 * @ingroup Volume_Import
 *
 * @brief Import a scanned volume in the nifti format.
 *
 */
class NiftiFileLoader3DAcquisition
{
public:
    /**
     * @brief Constructor of an object of the class NiftiFileLoader3DAcquisition.
     *
     */
    NiftiFileLoader3DAcquisition();

    /**
     * @brief Load the raw data and the metadata of a medical volume.
     *
     * The imaging raw data are casted into the datatype specified in the metadata.
     *
     * @param file_path     file
     * @return vmtkAcquisitionAbstract  pointer to an imported volume.
     */
    vmtkAcquisitionAbstract *loadImage(std::string file_path);

private:
    template <typename nifti_image_type>
    /**
     * @brief Load the raw data as a strem of bytes and the metadata of a medical volume.
     *
     * @param nifti_file_parser parser
     * @param file_path  file
     * @return vmtkAcquisitionAbstract pointer to an imported volume.
     */
    vmtkAcquisitionAbstract* extract3DImage(tipl::io::nifti &nifti_file_parser, std::string &file_path);

    template <typename nifti_image_type>
    float getVolumeMinValue(tipl::image<nifti_image_type, 3> image, unsigned int image_dim[], float scl_slope, float scl_inter, float cal_max, float cal_min);
};

#endif // NIFTI_FILE_LOADER_3D_ACQUISITION_H
