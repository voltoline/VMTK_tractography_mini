#include "DTIOpenGLResource.h"

#include "models/color_map/VEC2RGBAbsoluteColorMap.h"

DTIOpenGLResource::DTIOpenGLResource(ModelDTI *model_dti)
{
    m_modelDTI = model_dti;

    m_isTensorColorTextureBufferCreated = false;
    m_glyphColorTexId = 0;
    m_glyphColorBufferId = 0;

    m_isSuperquadricAttribTextureBufferCreated = false;
    m_tensorAttrTexId = 0;
    m_tensorAttrBufferId = 0;

    m_isTensorTransformationTextureBufferCreated = false;
    m_tensorTransformationTexId = 0;
    m_tensorTransformationBufferId = 0;
}

GLuint DTIOpenGLResource::getTensorColorTextureBuffer()
{
    if(!m_isTensorColorTextureBufferCreated)
        std::cerr << "DTIOpenGLResource::getTensorColorTextureBuffer -->> Tensor color texture buffer not created!" << std::endl;

    return m_glyphColorTexId;
}

void DTIOpenGLResource::createTensorColorTextureBuffer()
{
    if(m_isTensorColorTextureBufferCreated)
        return;

    const DTI *dti = m_modelDTI->getDTI();
    unsigned int slice_num_voxel = m_modelDTI->getAcquisition()->getDimensions(0) * m_modelDTI->getAcquisition()->getDimensions(1);
    unsigned int image_num_voxel = slice_num_voxel * m_modelDTI->getAcquisition()->getDimensions(2);

    VEC2RGBAbsoluteColorMap color_map;
    float *glyph_color_buffer = new float[slice_num_voxel * 4];
    unsigned int image_index_aux = 0;
    const unsigned int attr_slice_buffer_size_bytes = slice_num_voxel * 4 * sizeof(float);

    glGenTextures(1, &m_glyphColorTexId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, m_glyphColorTexId);
    glGenBuffers(1, &m_glyphColorBufferId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_glyphColorBufferId);
    glBufferData(GL_TEXTURE_BUFFER, image_num_voxel * 4 * sizeof(float), NULL, GL_STATIC_DRAW);

    for(unsigned int z = 0; z < m_modelDTI->getAcquisition()->getDimensions(2); z++)
    {
#pragma omp parallel for
        for(unsigned int j = 0; j < slice_num_voxel; j++)
        {
            unsigned int image_index = image_index_aux + j;
            float eigenvector_1[3], eigenvector_2[3], eigenvector_3[3];
            dti->getEigenVector01(image_index, eigenvector_1[0], eigenvector_1[1], eigenvector_1[2]);
            dti->getEigenVector02(image_index, eigenvector_2[0], eigenvector_2[1], eigenvector_2[2]);
            dti->getEigenVector03(image_index, eigenvector_3[0], eigenvector_3[1], eigenvector_3[2]);

            color_map.map(eigenvector_1[0], eigenvector_1[1], eigenvector_1[2],
                    glyph_color_buffer[4 * j],                                       // color.r
                    glyph_color_buffer[4 * j + 1],                                   // color.g
                    glyph_color_buffer[4 * j + 2]);                                  // color.b
            glyph_color_buffer[4 * j + 3] = dti->getFractionalAnisotropy(image_index);  // color.a
        }

        image_index_aux += slice_num_voxel;

        glBindBuffer(GL_TEXTURE_BUFFER, m_glyphColorBufferId);
        glBufferSubData(GL_TEXTURE_BUFFER, z * attr_slice_buffer_size_bytes, attr_slice_buffer_size_bytes, (void*) glyph_color_buffer);

    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, m_glyphColorTexId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_glyphColorBufferId);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_glyphColorBufferId);

    delete[] glyph_color_buffer;

    m_isTensorColorTextureBufferCreated = true;
}

GLuint DTIOpenGLResource::getSuperquadricAttribTextureBuffer()
{
    if(!m_isSuperquadricAttribTextureBufferCreated)
        std::cerr << "DTIOpenGLResource::getSuperquadricAttribTextureBuffer -->> Superquadric attribute texture buffer not created!" << std::endl;

    return m_tensorAttrTexId;
}

void DTIOpenGLResource::createSuperquadricAttribTextureBuffer()
{
    if(m_isSuperquadricAttribTextureBufferCreated)
        return;

    const DTI *dti = m_modelDTI->getDTI();
    unsigned int slice_num_voxel = m_modelDTI->getAcquisition()->getDimensions(0) * m_modelDTI->getAcquisition()->getDimensions(1);
    unsigned int image_num_voxel = slice_num_voxel * m_modelDTI->getAcquisition()->getDimensions(2);

    float *attr_slice_buffer = new float[slice_num_voxel * 4];
    unsigned int image_index_aux = 0;
    const unsigned int attr_slice_buffer_size_bytes = slice_num_voxel * 4 * sizeof(float);

    glGenTextures(1, &m_tensorAttrTexId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, m_tensorAttrTexId);
    glGenBuffers(1, &m_tensorAttrBufferId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_tensorAttrBufferId);
    glBufferData(GL_TEXTURE_BUFFER, image_num_voxel * 4 * sizeof(float), NULL, GL_STATIC_DRAW);

    for(unsigned int z = 0; z < m_modelDTI->getAcquisition()->getDimensions(2); z++)
    {
#pragma omp parallel for
        for(unsigned int j = 0; j < slice_num_voxel; j++)
        {
            unsigned int image_index = image_index_aux + j;

            float eigenvalue_1 = dti->getEigenValue01(image_index),
                    eigenvalue_2 = dti->getEigenValue02(image_index),
                    eigenvalue_3 = dti->getEigenValue03(image_index);

            float sharpness = 3.f;
            float linear_ind = (eigenvalue_1 - eigenvalue_2) / (eigenvalue_1 + eigenvalue_2 + eigenvalue_3);
            float planar_ind = 2 * (eigenvalue_2 - eigenvalue_3) / (eigenvalue_1 + eigenvalue_2 + eigenvalue_3);

            if(linear_ind >= planar_ind)
            {
                attr_slice_buffer[4 * j] = pow((1.f - planar_ind), sharpness);  //alpha
                attr_slice_buffer[4 * j + 1] = pow((1.f - linear_ind), sharpness);  //beta
                attr_slice_buffer[4 * j + 2] = 2.f;                                 //mode 2

            } else {

                attr_slice_buffer[4 * j] = pow((1.f - linear_ind), sharpness);  //alpha
                attr_slice_buffer[4 * j + 1] = pow((1.f - planar_ind), sharpness);  //beta
                attr_slice_buffer[4 * j + 2] = 1.f;                                 //mode 1
            }

            attr_slice_buffer[4 * j + 3] = 0.f;
        }

        image_index_aux += slice_num_voxel;

        glBindBuffer(GL_TEXTURE_BUFFER, m_tensorAttrBufferId);
        glBufferSubData(GL_TEXTURE_BUFFER, z * attr_slice_buffer_size_bytes, attr_slice_buffer_size_bytes, (void*) attr_slice_buffer);

    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, m_tensorAttrTexId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_tensorAttrBufferId);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tensorAttrBufferId);

    delete[] attr_slice_buffer;

    m_isSuperquadricAttribTextureBufferCreated = true;
}

GLuint DTIOpenGLResource::getSuperquadricTransformationTextureBuffer()
{
    if(!m_isTensorTransformationTextureBufferCreated)
        std::cerr << "DTIOpenGLResource::getSuperquadricAttribTextureBuffer -->> Eigenvector texture buffer not created!" << std::endl;

    return m_tensorTransformationTexId;
}

void DTIOpenGLResource::createSuperquadricTransformationTextureBuffer(const float *scale_factors)
{
    if(m_isTensorTransformationTextureBufferCreated)
        return;

    vmath::Matrix4f reorient_point_matrix; /**< geometric transformation matrix on the basic tensor glyph */
    vmath::Matrix4f reorient_point_matrix_scale; /**< scaling factor on the basic tensor glyph */
    vmath::Matrix4f reorient_point_matrix_rotation; /**< rotation on the basic tensor glyph */
    vmath::Matrix4f reorient_point_matrix_translation; /**< displacement on the basic tensor glyph */

    reorient_point_matrix_translation.setTranslation(-0.5f, -0.5f, -0.5f);
    reorient_point_matrix_scale.setScale(2.f * scale_factors[0] , 2.f * scale_factors[1], 2.f * scale_factors[2]);
    reorient_point_matrix_rotation.identity();

    reorient_point_matrix = reorient_point_matrix_rotation * reorient_point_matrix_scale * reorient_point_matrix_translation;

    float voxel_dim[3];
    float voxel_center[3];

    voxel_dim[0] = 1.f / m_modelDTI->getAcquisition()->getDimensions(0);
    voxel_dim[1] = 1.f / m_modelDTI->getAcquisition()->getDimensions(1);
    voxel_dim[2] = 1.f / m_modelDTI->getAcquisition()->getDimensions(2);

    voxel_center[0] = voxel_dim[0] / 2.f;
    voxel_center[1] = voxel_dim[1] / 2.f;
    voxel_center[2] = voxel_dim[2] / 2.f;

    const DTI *dti = m_modelDTI->getDTI();
    unsigned int slice_num_voxel = m_modelDTI->getAcquisition()->getDimensions(0) * m_modelDTI->getAcquisition()->getDimensions(1);
    unsigned int image_num_voxel = slice_num_voxel * m_modelDTI->getAcquisition()->getDimensions(2);

    vmath::Matrix4f *tensor_transformation_buffer = new vmath::Matrix4f[slice_num_voxel];
    const unsigned int tensor_transformation_buffer_size_bytes = slice_num_voxel * sizeof(vmath::Matrix4f);
    unsigned int image_index_aux = 0;

    glGenTextures(1, &m_tensorTransformationTexId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_BUFFER, m_tensorTransformationTexId);
    glGenBuffers(1, &m_tensorTransformationBufferId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_tensorTransformationBufferId);
    glBufferData(GL_TEXTURE_BUFFER, image_num_voxel * sizeof(vmath::Matrix4f), NULL, GL_STATIC_DRAW);

    for(unsigned int z = 0; z < m_modelDTI->getAcquisition()->getDimensions(2); z++)
    {
#pragma omp parallel for
        for(unsigned int j = 0; j < slice_num_voxel; j++)
        {
            unsigned int image_index = image_index_aux + j;
            float eigenvector_1[3], eigenvector_2[3], eigenvector_3[3];
            dti->getEigenVector01(image_index, eigenvector_1[0], eigenvector_1[1], eigenvector_1[2]);
            dti->getEigenVector02(image_index, eigenvector_2[0], eigenvector_2[1], eigenvector_2[2]);
            dti->getEigenVector03(image_index, eigenvector_3[0], eigenvector_3[1], eigenvector_3[2]);

            float eigenvalue_1 = dti->getEigenValue01(image_index),
                    eigenvalue_2 = dti->getEigenValue02(image_index),
                    eigenvalue_3 = dti->getEigenValue03(image_index);

            vmath::Matrix4f eigenvalue;
            eigenvalue.setScale(eigenvalue_1 / eigenvalue_1, eigenvalue_2 / eigenvalue_1, eigenvalue_3 / eigenvalue_1);

            vmath::Matrix4f eigenvector;
            eigenvector.at(0, 0) = eigenvector_1[0];  eigenvector.at(0, 1) = eigenvector_1[1]; eigenvector.at(0, 2) = eigenvector_1[2]; eigenvector.at(0, 3) = 0.f;
            eigenvector.at(1, 0) = eigenvector_2[0];  eigenvector.at(1, 1) = eigenvector_2[1]; eigenvector.at(1, 2) = eigenvector_2[2]; eigenvector.at(1, 3) = 0.f;
            eigenvector.at(2, 0) = eigenvector_3[0];  eigenvector.at(2, 1) = eigenvector_3[1]; eigenvector.at(2, 2) = eigenvector_3[2]; eigenvector.at(2, 3) = 0.f;
            eigenvector.at(3, 0) = 0.f;  eigenvector.at(3, 1) = 0.f; eigenvector.at(3, 2) = 0.f; eigenvector.at(3, 3) = 1;

            unsigned int y = j / m_modelDTI->getAcquisition()->getDimensions(0);
            unsigned int x = j - (y * m_modelDTI->getAcquisition()->getDimensions(0));

            vmath::Vector4f point(static_cast<float>(x) * voxel_dim[0] + voxel_center[0],
                                  static_cast<float>(y) * voxel_dim[1] + voxel_center[1],
                                  static_cast<float>(z) * voxel_dim[2] + voxel_center[2], 1.f);

            point = reorient_point_matrix * point;
            vmath::Matrix4f trans_;
            trans_.setTranslation(point.x, point.y, point.z);
            vmath::Matrix4f aux_trans_ = trans_ /** reorient_point_matrix_rotation*/;

            tensor_transformation_buffer[j] = aux_trans_ * eigenvector * eigenvalue;

        }

        image_index_aux += slice_num_voxel;

        glBindBuffer(GL_TEXTURE_BUFFER, m_tensorTransformationBufferId);
        glBufferSubData(GL_TEXTURE_BUFFER, z * tensor_transformation_buffer_size_bytes, tensor_transformation_buffer_size_bytes, (void*) tensor_transformation_buffer);

    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_BUFFER, m_tensorTransformationTexId);
    glBindBuffer(GL_TEXTURE_BUFFER, m_tensorTransformationBufferId);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tensorTransformationBufferId);

    delete[] tensor_transformation_buffer;

    m_isTensorTransformationTextureBufferCreated = true;
}
