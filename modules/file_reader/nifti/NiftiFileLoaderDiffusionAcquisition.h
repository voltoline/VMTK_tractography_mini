/**
  * @file NiftiFileLoader3DAcquisition.h
  * @author Raphael Voltoline
  * @date 2019
  */
#ifndef NIFTI_FILE_LOADER_DIFFUSION_ACQUISITION_H
#define NIFTI_FILE_LOADER_DIFFUSION_ACQUISITION_H

#include <iostream>
#include "tipl/tipl.hpp"

class vmtkDiffAcquisition;
class vmtkAcquisitionAbstract;

/**
 * @ingroup Volume_Import
 *
 * @brief Import a scanned DWI volume in the nifti format.
 *
 */
class NiftiFileLoaderDiffusionAcquisition
{
public:
    /**
     * @brief Constructor of an object of the class NiftiFileLoaderDiffusionAcquisition.
     *
     */
    NiftiFileLoaderDiffusionAcquisition();

    /**
     * @brief Load the raw data and the metadata of a medical volume.
     *
     * The imaging raw data are casted into the datatype specified in the metadata.
     *
     * @param file_path file.
     * @return vmtkAcquisitionAbstract object with loaded data.
     */
    vmtkAcquisitionAbstract *loadImage(std::string file_path);

private:
    template <typename nifti_image_type>
    /**
     * @brief Extract the raw data and metadata from a file.
     *
     * @param nifti_file_parser parser interpreter of nifti format.
     * @param file_path file.
     * @return vmtkAcquisitionAbstract object with loaded data.
     */
    vmtkAcquisitionAbstract* extractDiffusionImage(tipl::io::nifti &nifti_file_parser, std::string &file_path);

    /**
     * @brief Load the factor that reflects the strength and timing of the gradients used to generate
     * diffusion weighted images.
     *
     * @param diff_acq  DWI volume object.
     * @param file_path file.
     * @return bool loaded (1) or not (0).
     */
    bool loadBvalsAndBvecs(vmtkDiffAcquisition *diff_acq, std::string file_path);
    /**
     * @brief Reorient the volume to the orientation LPS with respect to the patient reference.
     *
     * @param diff_acq DWI volume object.
     */
    void reorientVolumes(vmtkDiffAcquisition *diff_acq);
    /**
     * @brief Recompute the gradients with respect to a transformation.
     *
     * @param diff_acq DWI volume object.
     * @param matrix[] transformation in 3x3 matrix.
     */
    void recalculateGradients(vmtkDiffAcquisition *diff_acq, float matrix[9]);
};

#endif // NIFTI_FILE_LOADER_DIFFUSION_ACQUISITION_H
