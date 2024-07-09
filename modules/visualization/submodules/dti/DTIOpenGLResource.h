/**
  * @file VisibleVoxelPreProcessingGeometryShader.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef DTI_OPENGL_RESOURCE_H
#define DTI_OPENGL_RESOURCE_H

#include "utilities/QCheckingOpenGL.h"
#include "models/ModelDTI.h"
#include "utilities/vmath.h"   ///Ting: to be replaced by glm ... (30/04/2020)

/**
 * @ingroup DWI_Volume_processing
 *
 * @brief Transfer the diffusion tensor data as colored textures to the GPU.
 *
 */
class DTIOpenGLResource
{
public:
    /**
     * @brief Constructor of an object of the class DTIOpenGLResource.
     *
     * @param model_dti diffusion tensor estimator.
     */
    DTIOpenGLResource(ModelDTI *model_dti);

    /**
     * @brief Get the diffusion tensor texture buffer.
     *
     * @return GLuint identifier of buffer.
     */
    GLuint getTensorColorTextureBuffer();
    /**
     * @brief Create the diffusion tensor texture buffer.
     *
     */
    void createTensorColorTextureBuffer();

    /**
     * @brief  Get the texture buffer of diffusion tensor glyphes.
     *
     * @return GLuint identifier of buffer.
     */
    GLuint getSuperquadricAttribTextureBuffer();
    /**
     * @brief Create the texture buffer of diffusion tensor glyphes.
     *
     */
    void createSuperquadricAttribTextureBuffer();

    /**
     * @brief  Get the texture buffer of spatial transformations on the tensor glyphs.
     *
     * @return GLuint  identifier of buffer.
     */
    GLuint getSuperquadricTransformationTextureBuffer();
    /**
     * @brief Create the texture buffer of geometric transformations on the basic tensor glyphs.
     *
     * @param reorient_point_matrix
     */
    void createSuperquadricTransformationTextureBuffer(const float *scale_factors);

private:
    bool m_isTensorColorTextureBufferCreated; /**< is the buffer of tensor data created (1) or not (0) */
    GLuint m_glyphColorTexId; /**< identifier of the texture of tensor data */
    GLuint m_glyphColorBufferId; /**< identifier of the texture buffer */

    bool m_isSuperquadricAttribTextureBufferCreated; /**< is the buffer of superquadric glyph data created (1) or not (0) */
    GLuint m_tensorAttrTexId; /**< identifier of the texture of superquadrics glyph data */
    GLuint m_tensorAttrBufferId; /**< identifier of the texture buffer */

    bool m_isTensorTransformationTextureBufferCreated; /**< is the buffer of Rotation matrices created (1) or not (0) */
    GLuint m_tensorTransformationTexId; /**< identifier of the texture of Rotation matrices */
    GLuint m_tensorTransformationBufferId; /**< identifier of the texture buffer */

    ModelDTI *m_modelDTI; /**< identifier of the DTI estimator */
};

#endif // DTIOPENGLRESOURCE_H
