#include "DTI.h"

#include <cmath>
#include <fstream>
#include <ctime>
#include <iomanip>

#if stateExternalEigen
#include "eigen3/Eigen/Cholesky"
#else

#include "algorithms/Eigen/Cholesky"
#endif

#include "tipl/tipl.hpp"

#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"

#include "../color_map/VEC2RGBAbsoluteColorMap.h"

#ifndef __APPLE__
#include <omp.h>
#endif

#include <cmath>

#ifdef Success
#if Success == 0
#undef Success
#else
#error "Success #defined with non-zero value,  possibly in X.h included by CImg.h"
#endif
#endif

DTI::DTI( ):
    mTensorCoefficients(NULL),
    m_eigen_01(NULL),
    m_eigen_02(NULL),
    m_eigen_03(NULL),
    m_color_map(NULL),
    m_mask(NULL),
    m_mask_size(0)
{
    m_progressBar = nullptr;

    m_minMeanDiffusivity = std::numeric_limits<float>::max();
    m_maxMeanDiffusivity = std::numeric_limits<float>::min();

    m_minRadialDiffusivity = std::numeric_limits<float>::max();
    m_maxRadialDiffusivity = std::numeric_limits<float>::min();

    m_minL1Diffusivity = std::numeric_limits<float>::max();
    m_maxL1Diffusivity = std::numeric_limits<float>::min();

    m_minL2Diffusivity = std::numeric_limits<float>::max();
    m_maxL2Diffusivity = std::numeric_limits<float>::min();

    m_minL3Diffusivity = std::numeric_limits<float>::max();
    m_maxL3Diffusivity = std::numeric_limits<float>::min();

    m_faBuffer = nullptr;
    m_mdBuffer = nullptr;
    m_rdBuffer = nullptr;
}

DTI::~DTI( )
{
    if(m_color_map != NULL)
        delete[] m_color_map;

    if(m_mask != NULL)
        delete[] m_mask;

    cleanAndReset();
}

void DTI::cleanAndReset()
{
    if(mTensorCoefficients != NULL)
        delete[] mTensorCoefficients;

    if(m_eigen_01 != NULL)
        delete[] m_eigen_01;

    if(m_eigen_02 != NULL)
        delete[] m_eigen_02;

    if(m_eigen_03 != NULL)
        delete[] m_eigen_03;

    m_minMeanDiffusivity = std::numeric_limits<float>::max();
    m_maxMeanDiffusivity = std::numeric_limits<float>::min();

    m_minRadialDiffusivity = std::numeric_limits<float>::max();
    m_maxRadialDiffusivity = std::numeric_limits<float>::min();

    m_minL1Diffusivity = std::numeric_limits<float>::max();
    m_maxL1Diffusivity = std::numeric_limits<float>::min();

    m_minL2Diffusivity = std::numeric_limits<float>::max();
    m_maxL2Diffusivity = std::numeric_limits<float>::min();

    m_minL3Diffusivity = std::numeric_limits<float>::max();
    m_maxL3Diffusivity = std::numeric_limits<float>::min();
}

unsigned short DTI::getDimensions(unsigned short index) const
{
    if(index == 0)
        return m_width;

    if(index == 1)
        return m_height;

    if(index == 2)
        return m_slices;
}

float DTI::getPixelSpacing(unsigned short index) const
{
    if(index <= 2)
        return m_pixelSpacing[index];
    else {
        std::cerr << "DTI::getPixelSpacing -> Index out of range! Value = " << index << std::endl;
        return 0;
    }
}

float DTI::getMaxL3Diffusivity() const
{
    return m_maxL3Diffusivity;
}

float DTI::getMinL3Diffusivity() const
{
    return m_minL3Diffusivity;
}

float DTI::getMaxL2Diffusivity() const
{
    return m_maxL2Diffusivity;
}

float DTI::getMinL2Diffusivity() const
{
    return m_minL2Diffusivity;
}

float DTI::getMaxL1Diffusivity() const
{
    return m_maxL1Diffusivity;
}

float DTI::getMinL1Diffusivity() const
{
    return m_minL1Diffusivity;
}

float DTI::getMaxMeanDiffusivity() const
{
    return m_maxMeanDiffusivity;
}

float DTI::getMinMeanDiffusivity() const
{
    return m_minMeanDiffusivity;
}

float DTI::getMaxRadialDiffusivity() const
{
    return m_maxRadialDiffusivity;
}

float DTI::getMinRadialDiffusivity() const
{
    return m_minRadialDiffusivity;
}

DTI::TensorCoefficient *DTI::getTensorCoefficients() const
{
    return mTensorCoefficients;
}

void DTI::setProgressBar(ProgressDialog *progress_bar)
{
    m_progressBar = progress_bar;
}

void DTI::setNumMeasures(unsigned short measures)
{
    m_num_measures = measures;
}

unsigned DTI::offset(short x, short y, short z)
{
    if((x >= 0) && (x < m_width) && (y >= 0) && (y < m_height) &&  (z >= 0) && (z < m_slices))
        return (x + (y * m_width) + (m_width * m_height * z));
    else
        return -1;
}

void DTI::loadTensorCoefficientsBinary(const std::string file_path)
{
    std::cout << "DTI::loadTensorCoefficients --> Loading tensors coefficients from " << file_path << " ..." << std::endl;

    std::ifstream file(file_path, std::ios_base::binary);

    if(!file.good())
    {
        std::cerr << "\tDTI::loadTensorCoefficients --> It was not possible to open the file!" << std::endl;
        return;
    }

    setColorCodeAlgorithm(AbsoluteColorMap);

    cleanAndReset();

    std::string fitter_algorithm_name;

    std::getline(file, fitter_algorithm_name, '\0');

    file.read((char*)&m_width, sizeof(m_width));
    file.read((char*)&m_height, sizeof(m_height));
    file.read((char*)&m_slices, sizeof(m_slices));

    file.read((char*)&m_pixelSpacing[0], sizeof(m_pixelSpacing[0]));
    file.read((char*)&m_pixelSpacing[1], sizeof(m_pixelSpacing[1]));
    file.read((char*)&m_pixelSpacing[2], sizeof(m_pixelSpacing[2]));

    m_num_voxels = m_width * m_height * m_slices;

    mTensorCoefficients = new TensorCoefficient[m_num_voxels];
    m_eigen_01 = new EigenStruct[m_num_voxels];
    m_eigen_02 = new EigenStruct[m_num_voxels];
    m_eigen_03 = new EigenStruct[m_num_voxels];

    for(unsigned i = 0; i < m_num_voxels; i++)
    {

        float m_coeff[6];
        file.read((char*)m_coeff, sizeof(m_coeff));

        if(m_mask == NULL || m_mask[i] > 0)
        {
            mTensorCoefficients[i].m_coeff[0] = m_coeff[0];
            mTensorCoefficients[i].m_coeff[1] = m_coeff[1];
            mTensorCoefficients[i].m_coeff[2] = m_coeff[2];
            mTensorCoefficients[i].m_coeff[3] = m_coeff[3];
            mTensorCoefficients[i].m_coeff[4] = m_coeff[4];
            mTensorCoefficients[i].m_coeff[5] = m_coeff[5];
        }

    }

#pragma omp parallel for
    for(unsigned i = 0; i < m_num_voxels; i++)
    {
        Eigen::VectorXd tensor_coef(6);
        tensor_coef.setZero(6);
        Eigen::MatrixXd tensor(3, 3);
        tensor.setZero(3, 3);

        tensor_coef(0) = mTensorCoefficients[i].m_coeff[0];
        tensor_coef(1) = mTensorCoefficients[i].m_coeff[1];
        tensor_coef(2) = mTensorCoefficients[i].m_coeff[2];
        tensor_coef(3) = mTensorCoefficients[i].m_coeff[3];
        tensor_coef(4) = mTensorCoefficients[i].m_coeff[4];
        tensor_coef(5) = mTensorCoefficients[i].m_coeff[5];

        if(m_mask == NULL || m_mask[i] > 0)
        {

            tensor <<
                      tensor_coef(0), tensor_coef(3), tensor_coef(4),
                    tensor_coef(3), tensor_coef(1), tensor_coef(5),
                    tensor_coef(4), tensor_coef(5), tensor_coef(2);

            Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver;
            solver.compute(tensor);

            if(solver.info() != Eigen::Success)
            {
                std::cerr << "solver.info() -> not success!" << std::endl;

                Eigen::MatrixXd zero_tensor(3, 3);
                zero_tensor.setZero(3, 3);
                Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> new_solver;
                new_solver.compute(zero_tensor);
                solver = new_solver;
            }

            Eigen::VectorXd eigen_values = solver.eigenvalues();
            Eigen::MatrixXd eigen_vectors = solver.eigenvectors();

            unsigned short max_ind, med_ind, min_ind;
            sortEigenvalues(eigen_values, max_ind, med_ind, min_ind);

            m_eigen_01[i].m_eigenvalue = fabs(static_cast<float>(eigen_values(max_ind)));
            m_eigen_01[i].m_eigenvector[0] = static_cast<float>(eigen_vectors(0, max_ind));
            m_eigen_01[i].m_eigenvector[1] = static_cast<float>(eigen_vectors(1, max_ind));
            m_eigen_01[i].m_eigenvector[2] = static_cast<float>(eigen_vectors(2, max_ind));

            m_eigen_02[i].m_eigenvalue = fabs(static_cast<float>(eigen_values(med_ind)));
            m_eigen_02[i].m_eigenvector[0] = static_cast<float>(eigen_vectors(0, med_ind));
            m_eigen_02[i].m_eigenvector[1] = static_cast<float>(eigen_vectors(1, med_ind));
            m_eigen_02[i].m_eigenvector[2] = static_cast<float>(eigen_vectors(2, med_ind));

            m_eigen_03[i].m_eigenvalue = fabs(static_cast<float>(eigen_values(min_ind)));
            m_eigen_03[i].m_eigenvector[0] = static_cast<float>(eigen_vectors(0, min_ind));
            m_eigen_03[i].m_eigenvector[1] = static_cast<float>(eigen_vectors(1, min_ind));
            m_eigen_03[i].m_eigenvector[2] = static_cast<float>(eigen_vectors(2, min_ind));

            //===================================================================================================================

            Eigen::Vector3f eigenvector_1(m_eigen_01[i].m_eigenvector[0],
                    m_eigen_01[i].m_eigenvector[1],
                    m_eigen_01[i].m_eigenvector[2]);

            Eigen::Vector3f eigenvector_2(m_eigen_02[i].m_eigenvector[0],
                    m_eigen_02[i].m_eigenvector[1],
                    m_eigen_02[i].m_eigenvector[2]);

            Eigen::Vector3f eigenvector_3(m_eigen_03[i].m_eigenvector[0],
                    m_eigen_03[i].m_eigenvector[1],
                    m_eigen_03[i].m_eigenvector[2]);

            Eigen::Vector3f cross_ = eigenvector_1.cross(eigenvector_2);

            float dot_ = eigenvector_3.dot(cross_);
            if(dot_ < 0)
            {
                m_eigen_03[i].m_eigenvector[0] = -m_eigen_03[i].m_eigenvector[0];
                m_eigen_03[i].m_eigenvector[1] = -m_eigen_03[i].m_eigenvector[1];
                m_eigen_03[i].m_eigenvector[2] = -m_eigen_03[i].m_eigenvector[2];
            }
        }
    }

    file.close();

    for(unsigned int i = 0; i < m_num_voxels; i++)
    {
        if(m_eigen_01[i].m_eigenvalue < m_minL1Diffusivity)
            m_minL1Diffusivity = m_eigen_01[i].m_eigenvalue;

        if(m_eigen_01[i].m_eigenvalue > m_maxL1Diffusivity)
            m_maxL1Diffusivity = m_eigen_01[i].m_eigenvalue;

        if(m_eigen_02[i].m_eigenvalue < m_minL2Diffusivity)
            m_minL2Diffusivity = m_eigen_02[i].m_eigenvalue;

        if(m_eigen_02[i].m_eigenvalue > m_maxL2Diffusivity)
            m_maxL2Diffusivity = m_eigen_02[i].m_eigenvalue;

        if(m_eigen_03[i].m_eigenvalue < m_minL3Diffusivity)
            m_minL3Diffusivity = m_eigen_03[i].m_eigenvalue;

        if(m_eigen_03[i].m_eigenvalue > m_maxL3Diffusivity)
            m_maxL3Diffusivity = m_eigen_03[i].m_eigenvalue;
    }
}

float DTI::getFractionalAnisotropy(unsigned index) const
{
    if(index < m_num_voxels)
    {
        float eigenvalue_01 = fabs(m_eigen_01[index].m_eigenvalue);
        float eigenvalue_02 = fabs(m_eigen_02[index].m_eigenvalue);
        float eigenvalue_03 = fabs(m_eigen_03[index].m_eigenvalue);
        float d = (eigenvalue_01 + eigenvalue_02 + eigenvalue_03) / 3.f;
        float term00 = pow(eigenvalue_01 - d, 2);
        float term01 = pow(eigenvalue_02 - d, 2);
        float term02 = pow(eigenvalue_03 - d, 2);
        float term03 = pow(m_eigen_01[index].m_eigenvalue, 2) + pow(m_eigen_02[index].m_eigenvalue, 2) + pow(m_eigen_03[index].m_eigenvalue, 2);

        if(term03 > 0.f)
            return sqrt((3.f * (term00 + term01 + term02)) / (2.f * term03));

    } else {
        std::cerr << "DTI::FractionalAnisotropy --> Index out of range!  Value = " << index << std::endl;
    }

    return 0.f;
}

float* DTI::getEigenVectorBuffer(unsigned short eigen_index) const
{
    EigenStruct *eigen_struct;
    switch(eigen_index)
    {
    case 1:
        eigen_struct = m_eigen_01;
        break;

    case 2:
        eigen_struct = m_eigen_02;
        break;

    case 3:
        eigen_struct = m_eigen_03;
        break;

    default:
        return NULL;
    }

    float *eigen_buffer = new float[m_num_voxels * 3];
    for(unsigned int i = 0; i < m_num_voxels; i++)
    {
        eigen_buffer[i * 3] = eigen_struct[i].m_eigenvector[0];
        eigen_buffer[i * 3 + 1] = eigen_struct[i].m_eigenvector[1];
        eigen_buffer[i * 3 + 2] = eigen_struct[i].m_eigenvector[2];
    }

    return eigen_buffer;
}

float* DTI::getEigenValueBuffer(unsigned short eigen_index) const
{
    EigenStruct *eigen_struct;
    switch(eigen_index)
    {
    case 1:
        eigen_struct = m_eigen_01;
        break;

    case 2:
        eigen_struct = m_eigen_02;
        break;

    case 3:
        eigen_struct = m_eigen_03;
        break;

    default:
        return NULL;
    }

    float *eigen_buffer = new float[m_num_voxels];
    for(unsigned int i = 0; i < m_num_voxels; i++)
        eigen_buffer[i] = eigen_struct[i].m_eigenvalue;

    return eigen_buffer;
}

void DTI::getEigenVector01(unsigned int index, float &x, float &y, float &z) const
{
    if(index < m_num_voxels)
    {
        x = m_eigen_01[index].m_eigenvector[0];
        y = m_eigen_01[index].m_eigenvector[1];
        z = m_eigen_01[index].m_eigenvector[2];

    } else {

        std::cerr << "DTI::getEigenVector01 --> Index out of range!  Value = " << index << std::endl;
        x = y = z = 0;
    }
}

void DTI::getEigenVector02(unsigned int index, float &x, float &y, float &z) const
{
    if(index < m_num_voxels)
    {
        x = m_eigen_02[index].m_eigenvector[0];
        y = m_eigen_02[index].m_eigenvector[1];
        z = m_eigen_02[index].m_eigenvector[2];

    } else {

        std::cerr << "DTI::getEigenVector02 --> Index out of range!  Value = " << index << std::endl;
        x = y = z = 0;
    }
}

void DTI::getEigenVector03(unsigned int index, float &x, float &y, float &z) const
{
    if(index < m_num_voxels)
    {
        x = m_eigen_03[index].m_eigenvector[0];
        y = m_eigen_03[index].m_eigenvector[1];
        z = m_eigen_03[index].m_eigenvector[2];

    } else {

        std::cerr << "DTI::getEigenVector03 --> Index out of range!  Value = " << index << std::endl;
        x = y = z = 0;
    }
}

float DTI::getEigenValue01(unsigned int index) const
{
    if(index < m_num_voxels)
        return m_eigen_01[index].m_eigenvalue;
    else {
        std::cerr << "DTI::getEigenValue01 --> Index out of range!  Value = " << index << std::endl;
        return 0;
    }
}

float DTI::getEigenValue02(unsigned int index) const
{
    if(index < m_num_voxels)
        return m_eigen_02[index].m_eigenvalue;
    else {
        std::cerr << "DTI::getEigenValue02 --> Index out of range!  Value = " << index << std::endl;
        return 0;
    }
}

float DTI::getEigenValue03(unsigned int index) const
{
    if(index < m_num_voxels)
        return m_eigen_03[index].m_eigenvalue;
    else {
        std::cerr << "DTI::getEigenValue03 --> Index out of range!  Value = " << index << std::endl;
        return 0;
    }
}

float DTI::getFractionalAnisotropy(unsigned short x, unsigned short y, unsigned short z) const
{
    return getFractionalAnisotropy(x + (m_width * y) + (m_width * m_height * z));
}

GrayscaleImage3DFloat::sPtr DTI::getFractionalAnisotropy( )
{
    if(m_faBuffer == nullptr)
    {
        float *buffer = new float[m_num_voxels];

        for(unsigned i = 0; i < m_num_voxels; i++)
            buffer[i] = getFractionalAnisotropy(i);

        m_faBuffer = std::make_shared<GrayscaleImage3DFloat>(buffer, m_width, m_height, m_slices);
    }

    return m_faBuffer;
}

GrayscaleImage3DFloat::sPtr DTI::getFractionalAnisotropy( ) const
{
    if(m_faBuffer == nullptr)
    {
        DTI* ptr =  const_cast<DTI*> (this);
        ptr->getFractionalAnisotropy();
    }

    return m_faBuffer;
}

void DTI::setColorCodeAlgorithm(vmtkColorCodeAlgorithm color_code_algorithm)
{
    if(m_color_map != NULL)
    {
        delete m_color_map;
        m_color_map = NULL;
    }

    switch(color_code_algorithm)
    {
    case AbsoluteColorMap:
        std::cout << "DTI::setColorCodeAlgorithm-> AbsoluteColorMap Algorithm!" << std::endl;
        m_color_map = new VEC2RGBAbsoluteColorMap;
        break;
    }

}

void DTI::ColorCodedFractionalAnisotropy(unsigned index, float &r, float &g, float &b) const
{
    if(index < m_num_voxels)
    {
        float fa = getFractionalAnisotropy(index);

        m_color_map->map(m_eigen_01[index].m_eigenvector[0] * fa,
                m_eigen_01[index].m_eigenvector[1] * fa,
                m_eigen_01[index].m_eigenvector[2] * fa,
                r, g, b);

    } else {
        std::cerr << "DTI::ColorCodedFractionalAnisotropy --> Index out of range!  Value = " << index << std::endl;
    }
}

void DTI::ColorCodedFractionalAnisotropy(unsigned short x, unsigned short y, unsigned short z, float &r, float &g, float &b) const
{
    ColorCodedFractionalAnisotropy(x + (m_width * y) + (m_width * m_height * z), r, g, b);
}

float* DTI::ColorCodedFractionalAnisotropy( ) const
{
    unsigned buffer_size = m_num_voxels * 3;
    float *color_fa = new float[buffer_size];

    for(unsigned i = 0; i < m_num_voxels; i ++)
        ColorCodedFractionalAnisotropy(i, color_fa[i * 3], color_fa[i * 3 + 1], color_fa[i * 3 + 2]);

    return color_fa;
}

float DTI::getMeanDiffusivity(unsigned index) const
{
    if(index < m_num_voxels)
    {
        float eigenvalue_01 = fabs(m_eigen_01[index].m_eigenvalue);
        float eigenvalue_02 = fabs(m_eigen_02[index].m_eigenvalue);
        float eigenvalue_03 = fabs(m_eigen_03[index].m_eigenvalue);
        float d = (eigenvalue_01 + eigenvalue_02 + eigenvalue_03) / 3.f;
        return d;
    } else {
        std::cerr << "DTI::getMeanDiffusivity --> Index out of range!  Value = " << index << std::endl;
    }

    return 0.f;
}

float DTI::getRadialDiffusivity(unsigned index) const
{
    if(index < m_num_voxels)
    {
        float eigenvalue_02 = fabs(m_eigen_02[index].m_eigenvalue);
        float eigenvalue_03 = fabs(m_eigen_03[index].m_eigenvalue);
        float d = (eigenvalue_02 + eigenvalue_03) / 2.f;
        return d;

    } else {
        std::cerr << "DTI::getRadialDiffusivity --> Index out of range!  Value = " << index << std::endl;
    }

    return 0.f;
}

float DTI::getMeanDiffusivity(unsigned short x, unsigned short y, unsigned short z) const
{
    return getMeanDiffusivity(x + (m_width * y) + (m_width * m_height * z));
}

float DTI::getRadialDiffusivity(unsigned short x, unsigned short y, unsigned short z) const
{
    return getRadialDiffusivity(x + (m_width * y) + (m_width * m_height * z));
}

const float* DTI::getMeanDiffusivity( )
{
    if(m_mdBuffer == nullptr)
    {
        m_mdBuffer = new float[m_num_voxels];

        for(unsigned i = 0; i < m_num_voxels; i++)
        {
            m_mdBuffer[i] = getMeanDiffusivity(i);

            if(m_mdBuffer[i] < m_minMeanDiffusivity)
                m_minMeanDiffusivity = m_mdBuffer[i];

            if(m_mdBuffer[i] > m_maxMeanDiffusivity)
                m_maxMeanDiffusivity = m_mdBuffer[i];
        }
    }

    return m_mdBuffer;
}

const float* DTI::getRadialDiffusivity( )
{
    if(m_rdBuffer == nullptr)
    {
        m_rdBuffer = new float[m_num_voxels];

        for(unsigned i = 0; i < m_num_voxels; i++)
        {
            m_rdBuffer[i] = getRadialDiffusivity(i);

            if(m_rdBuffer[i] < m_minRadialDiffusivity)
                m_minRadialDiffusivity = m_rdBuffer[i];

            if(m_rdBuffer[i] > m_maxRadialDiffusivity)
                m_maxRadialDiffusivity = m_rdBuffer[i];
        }
    }

    return m_rdBuffer;
}

const float* DTI::getMeanDiffusivity( ) const
{
    if(m_mdBuffer == nullptr)
    {
        DTI* ptr =  const_cast<DTI*> (this);
        ptr->getMeanDiffusivity();
    }

    return m_mdBuffer;
}

const float* DTI::getRadialDiffusivity( ) const
{
    if(m_rdBuffer == nullptr)
    {
        DTI* ptr =  const_cast<DTI*> (this);
        ptr->getRadialDiffusivity();
    }

    return m_rdBuffer;
}

GrayscaleImage3DFloat::sPtr DTI::MeanDiffusivityNormalized( ) const
{
    float *md = new float[m_num_voxels];
    float md_max = 0;

    for(unsigned i = 0; i < m_num_voxels; i++)
    {
        md[i] = getMeanDiffusivity(i);
        if(md[i] > md_max)
            md_max = md[i];
    }

    for(unsigned i = 0; i < m_num_voxels; i++)
        md[i] = md[i] / md_max;

    return std::make_shared<GrayscaleImage3DFloat>(md, m_width, m_height, m_slices);
}

void DTI::sortEigenvalues(Eigen::VectorXd &eigen_values, unsigned short &max_ind, unsigned short &med_ind, unsigned short &min_ind)
{
    if(fabs(eigen_values(0)) > fabs(eigen_values(1)))
    {
        if(fabs(eigen_values(0)) > fabs(eigen_values(2)))
        {
            max_ind = 0;

            if(fabs(eigen_values(1)) > fabs(eigen_values(2)))
            {
                med_ind = 1;
                min_ind = 2;

            } else {

                med_ind = 2;
                min_ind = 1;
            }

        } else {

            max_ind = 2;
            med_ind = 0;
            min_ind = 1;
        }

    } else if(fabs(eigen_values(1)) > fabs(eigen_values(2))) {

        max_ind = 1;

        if(fabs(eigen_values(0)) > fabs(eigen_values(2)))
        {
            med_ind = 0;
            min_ind = 2;

        } else {

            med_ind = 2;
            min_ind = 0;
        }

    } else {

        max_ind = 2;
        med_ind = 1;
        min_ind = 0;
    }
}

void DTI::setNIfTIMask(const std::string &mask_path)
{
    if(m_mask != NULL)
    {
        delete[] m_mask;
        m_mask_size = 0;
    }

    tipl::io::nifti nifti_file_fa;
    std::string path_file_fa(mask_path);
    tipl::image<float, 3>  image_fa;

    std::cout << "DTI::setNIfTIMask -->> Load mask from nii file..." << std::endl;
    if(!nifti_file_fa.load_from_file(path_file_fa))
    {
        std::cout << "DTI::setNIfTIMask -->> Load mask from nii file failed!" << std::endl;
        return;
    }

    nifti_file_fa.toLPS(image_fa);
    nifti_file_fa >> image_fa;

    m_mask_size = image_fa.width() * image_fa.height() * image_fa.depth();
    m_mask = new float[m_mask_size];

    std::copy(image_fa.begin(), image_fa.begin() + m_mask_size, m_mask);

    unsigned int count_voxel = 0;
    for(unsigned int i = 0; i < m_mask_size; i++)
        if(m_mask[i] > 0)
            count_voxel++;

    std::cout << "DTI::setNIfTIMask::num valid voxels --> " << count_voxel << std::endl;
}
