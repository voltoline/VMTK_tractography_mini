/**
  * @file DTI.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef DTI_H
#define DTI_H

#if stateExternalEigen
#include "eigen3/Eigen/Eigenvalues"
#else
#include "algorithms/Eigen/Eigenvalues"
#endif

#include "core/ProgressDialog.h"
#include <iostream>
#include <queue>
#include <utilities/ImageBuffer.h>

class vmtkDiffAcquisition;
class ColorMapBase;

/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Compute the maps reflecting diffusion properties represented in a DTI (diffusion tensor imaging) volume.
 *
 * Different tensor estimators included in the class TensorFitter are available for the computation
 * of the diffusion tensors. From the eigenvalues (01, 02, 03) and eigenvectors (01, 02, 03) of these diffusion tensors
 * various <a href="http://mriquestions.com/dti-tensor-imaging.html" target=_complement>maps</a> of diffusion properties,
 * such as the mean diffusivity, or aparrent diffusion coefficient (ADC), and the fractional anisotropy, are generated.
 *
 * The diffusion tensor matrix can be factored as a product of three 4x4 matrices. Rotation matrix built up from the eigenvectors,
 * Lambda diagonal matrix composed by the eigenvalues and the inverse of the Rotation matrix. Representing a diffusion tensor as
 * a glyph, we can consider that the product of this inverse with the glyph's geometry centered at the origin
 * aligns the principal axes of the glyph with the axes of a reference system. We call it the basic glyph instance G.
 * If we scale this basic instance with Lambda and rotate it with Rotation, we recover the individual diffusion glyph
 * within a voxel.
 *
 */
class DTI
{
public:
    /**
     * @enum vmtkColorCodeAlgorithm
     * Modes of coding the spatial directions in colors.
     */
    enum vmtkColorCodeAlgorithm
    {
        AbsoluteColorMap,           /**< TODO: describe */
    };

    /**
     * @brief Constructor of an object of the class DTI
     *
     */
    DTI();
    /**
     * @brief Destructor of an object of the class DTI
     *
     */
    ~DTI();

    /**
     * @brief Compute the fractional anisotropy index within a voxel in terms of its eigenvalues.
     *
     * @param x x-coordinate of the voxel.
     * @param y y-coordinate of the voxel.
     * @param z z-coordinate of the voxel.
     * @return float fractional anisotropy index between 0 and 1.
     */
    float getFractionalAnisotropy(unsigned short x, unsigned short y, unsigned short z) const;
    /**
     * @brief Compute the fractional anisotropy index within a voxel in terms of its eigenvalues.
     *
     * @param index index of the voxel in an array.
     * @return float fractional anisotropy index between 0 and 1.
     */
    float getFractionalAnisotropy(unsigned index) const;
    /**
     * @brief Compute the fractional anisotropy index of all voxels.
     *
     * @return float* pointer to the array of fractional anisotropy indices.
     */
    GrayscaleImage3DFloat::sPtr getFractionalAnisotropy( );
    GrayscaleImage3DFloat::sPtr getFractionalAnisotropy( ) const;

    /**
     * @brief Set color coding mode of directions
     *
     * @param color_code_algorithm
     */
    void setColorCodeAlgorithm(vmtkColorCodeAlgorithm color_code_algorithm);
    /**
     * @brief Weigh the color-coded direction of a voxel by the fractional anisotropy index.
     *
     * @param index index of the voxel in an array.
     * @param r red component of the color-coded direction.
     * @param g green component of the color-coded direction.
     * @param b blue component of the color-coded direction.
     */
    void ColorCodedFractionalAnisotropy(unsigned index, float &r, float &g, float &b) const;
    /**
     * @brief Weigh the color-coded direction of a voxel by the fractional anisotropy index.
     *
     * @param x x-coordinate of the voxel.
     * @param y x-coordinate of the voxel.
     * @param z x-coordinate of the voxel.
     * @param r red component of the color-coded direction.
     * @param g green component of the color-coded direction.
     * @param b blue component of the color-coded direction.
     */
    void ColorCodedFractionalAnisotropy(unsigned short x, unsigned short y, unsigned short z, float &r, float &g, float &b) const;
    /**
     * @brief Weigh the color-coded directions by the corresponding fractional anisotropy to all voxels.
     *
     * @return float* pointer to the array of weighted color-coded dirctions of all voxels.
     */
    float* ColorCodedFractionalAnisotropy() const;

    /**
     * @brief Compute the mean diffusivity, or apparent diffusion coefficient (ADC), to all voxels, in terms of the corresponding eigenvalues.
     *
     * @return float* pointer to the array of ADC of all voxels.
     */
    const float* getMeanDiffusivity();
    const float* getMeanDiffusivity() const;

    /**
     * @brief Compute the radial diffusivity to all voxels, in terms of the corresponding eigenvalues.
     *
     * @return float* pointer to the array of radial diffusivity of all voxels.
     */
    const float* getRadialDiffusivity();
    const float* getRadialDiffusivity() const;

    /**
     * @brief Normalize the mean diffusivity in terms of the greatest mean diffusivity to all voxels.
     *
     * @return float* pointer to the array of normalized ADC of all voxels.
     */
    GrayscaleImage3DFloat::sPtr MeanDiffusivityNormalized() const;

    /**
     * @brief Compute the mean diffusivity within a voxel in terms of the eigenvalues.
     *
     * @param index index of the voxel in an array.
     * @return float mean diffusivity.
     */
    float getMeanDiffusivity(unsigned index) const;

    /**
     * @brief Compute the mean diffusivity within a voxel in terms of the eigenvalues.
     *
     * @param x x-coordinate of the voxel.
     * @param y x-coordinate of the voxel.
     * @param z x-coordinate of the voxel.
     * @return float mean diffusivity.
     */
    float getMeanDiffusivity(unsigned short x, unsigned short y, unsigned short z) const;

    /**
     * @brief Compute the radial diffusivity within a voxel in terms of the eigenvalues.
     *
     * @param index index of the voxel in an array.
     * @return float radial diffusivity.
     */
    float getRadialDiffusivity(unsigned index) const;

    /**
     * @brief Compute the radial diffusivity within a voxel in terms of the eigenvalues.
     *
     * @param x x-coordinate of the voxel.
     * @param y x-coordinate of the voxel.
     * @param z x-coordinate of the voxel.
     * @return float radial diffusivity.
     */
    float getRadialDiffusivity(unsigned short x, unsigned short y, unsigned short z) const;

    /**
     * @brief Load from a binary file the (6) coefficients of a symmetric diffusion tensor.
     *
     * @param file_path path of the file.
     */
    void loadTensorCoefficientsBinary(const std::string file_path);

    /**
     * @brief Load from a file the brain mask in the nifti format.
     *
     * @param mask_path path of the file.
     */
    void setNIfTIMask(const std::string &mask_path);

    /**
     * @brief Get the eigen vectors within a voxel.
     *
     * @param eigen_index index of the voxel.
     * @return float* pointer to the three principal vecotrs.
     */
    float *getEigenVectorBuffer(unsigned short eigen_index) const;
    /**
     * @brief Get the eigenvector with the largest eigenvalue within a voxel.
     *
     * @param index index of the voxel in an array.
     * @param x x-coordinate of the eigenvector.
     * @param y y-coordinate of the eigenvector.
     * @param z z-coordinate of the eigenvector.
     */
    void getEigenVector01(unsigned int index, float &x, float &y, float &z) const;
    /**
     * @brief Get the eigenvector with the middle eigenvalue within a voxel.
     *
     * @param index index of the voxel in an array.
     * @param x x-coordinate of the eigenvector.
     * @param y y-coordinate of the eigenvector.
     * @param z z-coordinate of the eigenvector.
     */
    void getEigenVector02(unsigned int index, float &x, float &y, float &z) const;
    /**
     * @brief Get the eigen vector with the smallest eigenvalue within a voxel.
     *
     * @param index index of the voxel in an array.
     * @param x x-coordinate of the eigenvector.
     * @param y y-coordinate of the eigenvector.
     * @param z y-coordinate of the eigenvector.
     */
    void getEigenVector03(unsigned int index, float &x, float &y, float &z) const;

    /**
     * @brief Get the eigenvector corresponding to the given index.
     *
     * @param eigen_index index of the eigenvectors (01=the largest; 03=the smallest)
     * @return float* pointer to the eigenvector.
     */
    float *getEigenValueBuffer(unsigned short eigen_index) const;
    /**
     * @brief Get the largest eigenvalue within a voxel.
     *
     * @param index index of the voxel.
     * @return float eigenvalue.
     */
    float getEigenValue01(unsigned int index) const;
    /**
     * @brief Get the middle eigenvalue within a voxel.
     *
     * @param index index of the voxel.
     * @return float eigenvalue.
     */
    float getEigenValue02(unsigned int index) const;
    /**
     * @brief Get the smallest eigenvalue within a voxel.
     *
     * @param index index of the voxel.
     * @return float eigenvalue.
     */
    float getEigenValue03(unsigned int index) const;

    /**
     * @brief Elements of a diffusion tensor.
     */
    typedef struct _tensor_coeff
    {
        float m_coeff[6]; /**< six coefficients of a symmetric diffusion tensor */
    }TensorCoefficient;

    /**
     * @brief Get the tensor coefficients of all voxels.
     *
     * @return TensorCoefficient* pointer to the tensor coefficients.
     */
    TensorCoefficient *getTensorCoefficients() const;

    /**
     * @brief Set progress bar associated to the computation of the diffusion tensors.
     *
     * @param progress_bar reference to the progress_bar object.
     */
    void setProgressBar(ProgressDialog* progress_bar);

    /**
     * @brief Ge the smallest mean diffusivity among the voxels.
     *
     * @return float mean diffusivity.
     */
    float getMinMeanDiffusivity() const;

    /**
     * @brief Get the largest mean diffusivity among the voxels.
     *
     * @return float mean diffusivity.
     */
    float getMaxMeanDiffusivity() const;

    /**
     * @brief Ge the smallest radial diffusivity among the voxels.
     *
     * @return float mean diffusivity.
     */
    float getMinRadialDiffusivity() const;

    /**
     * @brief Get the largest radial diffusivity among the voxels.
     *
     * @return float mean diffusivity.
     */
    float getMaxRadialDiffusivity() const;

    /**
     * @brief Get the minimum of the largest eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMinL1Diffusivity() const;

    /**
     * @brief Get the maximum of the largest eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMaxL1Diffusivity() const;

    /**
     * @brief Get the minimum of the middle eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMinL2Diffusivity() const;
    /**
     * @brief Get the maximum of the middle eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMaxL2Diffusivity() const;

    /**
     * @brief Get the minimum of the smallest eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMinL3Diffusivity() const;
    /**
     * @brief Get the maximum of the smallest eigenvalues among the voxels.
     *
     * @return float eigenvalue.
     */
    float getMaxL3Diffusivity() const;

    unsigned short getDimensions(unsigned short index) const;
    float getPixelSpacing(unsigned short index) const;

private:

    /**
     * @struct EigenStruct
     * Eigenvector associated to the corresponding eigenvalue.
     */
    typedef struct _eigen_components
    {
        float m_eigenvalue; /**< eigenvalue */
        float m_eigenvector[3]; /**< eigenvector */
    }EigenStruct;

    /**
     * @brief Set the number of gradients.
     *
     * @param measures number of gradients applied during the acquisition.
     */
    void setNumMeasures(unsigned short measures);
    /**
     * @brief Sort the eigenvalues.
     *
     * @param eigen_values reference to the eigenvalues.
     * @param max_ind the maximum value.
     * @param med_ind the middle value.
     * @param min_ind the minimum value.
     */
    void sortEigenvalues(Eigen::VectorXd &eigen_values, unsigned short &max_ind, unsigned short &med_ind, unsigned short &min_ind);
    /**
     * @brief Get the index of a voxel in a clipped DWI volume [0,m_width;0,m_height;0,m_slices].
     *
     * @param x x-coordinate of the voxel.
     * @param y y-coordinate of the voxel.
     * @param z z-coordinate of the voxel.
     * @return unsigned the index, if it is inside the range; otherwise, -1
     */
    unsigned offset(short x, short y, short z);
    /**
     * @brief Remove the allocated buffers and reset all the variables.
     *
     */
    void cleanAndReset();

    unsigned short m_num_measures; /**< number of gradients */
    unsigned short m_width; /**< width of a DWI volume */
    unsigned short m_height; /**< height of a DWI volume */
    unsigned short m_slices; /**< number of slices in the DWI volumes */
    unsigned int m_num_voxels; /**< total number of voxels in a DWI volume */
    float m_pixelSpacing[3];              // m_pixel_spacing[0]: width; m_pixel_spacing[1]: height; m_pixel_spacing[2]: depth

    TensorCoefficient *mTensorCoefficients; /**< buffer of the tensor coefficients of all voxels */
    EigenStruct *m_eigen_01; /**< The largest eigenvalue with respective eigenvector */
    EigenStruct *m_eigen_02; /**< The middle eigenvalue with respective eigenvector */
    EigenStruct *m_eigen_03; /**< The smallest eigenvalue with respective eigenvector */
    ColorMapBase *m_color_map; /**< buffer of the color map for directions */

    float *m_mask; /**< brain mask buffer */
    unsigned int m_mask_size; /**< size of the brain mask array */

    ProgressDialog *m_progressBar; /**< progress bar widget */

    float m_minMeanDiffusivity; /**< the minimum of the mean diffusivities among all the voxels */
    float m_maxMeanDiffusivity; /**< the minimum of the smallest mean diffusivity in each voxel */

    float m_minRadialDiffusivity; /**< the minimum of the radial diffusivities among all the voxels */
    float m_maxRadialDiffusivity; /**< the minimum of the smallest radial diffusivity in each voxel */

    float m_minL1Diffusivity; /**< the minimum of the largest mean diffusivity in each voxel */
    float m_maxL1Diffusivity; /**< the maximum of the largest mean diffusivity in each voxel */

    float m_minL2Diffusivity; /**< the minimum of the middle mean diffusivity in each voxel */
    float m_maxL2Diffusivity; /**< the maximum of the middle mean diffusivity in each voxel */

    float m_minL3Diffusivity; /**< the minimum of the smallest mean diffusivity in each voxel */
    float m_maxL3Diffusivity; /**< the maximum of the smallest mean diffusivity in each voxel */

    GrayscaleImage3DFloat::sPtr m_faBuffer;
    float *m_mdBuffer;
    float *m_rdBuffer;
};

#endif // DTI_H
