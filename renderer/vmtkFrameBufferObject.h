/**
  * @file vmtkFrameBufferObject.h
  * @brief Manage GL frame buffer object.
  * @author José Angel Ivan Rubianes Silva
  * @date February 2015
  * @note This code is revised by Wu Shin-Ting.
  */

#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H
#include "utilities/QCheckingOpenGL.h"

/**
 * @brief The FrameBufferObject class
 * @details manages GL framebuffer object
 */
class vmtkFrameBufferObject
{
public:

    /**
     * @brief destructor
     */
    ~vmtkFrameBufferObject();

    /**
     *@enum Attachment
     *@brief Possible images to be attached to the FBO
    */
    enum Attachment {
        NoAttachment,     /**< no attachment */
        DepthStencil,     /**< depth and stencil buffer */
        Depth,            /**< depth buffer */
        Stencil           /**< stencil buffer */
    };

    /**
     * @brief constructs a FrameBufferObject without arguments
     */
    vmtkFrameBufferObject();

    /**
     * @brief constructs a FrameBufferObject with specified dimensions
     * @param[in] width image width
     * @param[in] height image height
     * @param[in] target of the texture bound to the framebufferobject
     */
    vmtkFrameBufferObject(int width, int height,
                          unsigned short num_render_buffer = 1,
                          vmtkFrameBufferObject::Attachment attachment = NoAttachment,
                          GLenum target = GL_TEXTURE_2D,
                          GLenum internal_format = GL_RGBA32F);

    GLenum target;          /**< target of the image to be bound to the attachment point in the FBO */
    Attachment attachment;  /**< attachment point to which an image from texture should be attached */


    /**
     * @brief gets the Fbo name.
     * @return the FBO name
     */
    GLuint getFbo() const;

    /**
     * @brief gets the texture bound to the FBO.
     * @return the texture object name
     */
    GLuint getTexture(unsigned short render_buffer_tex_index = 0) const;

    GLuint getDepthTexture() const;

    float* getDataFromTextureAttach(unsigned short attach_index);

    /**
    * @brief binds the FBO to the framebuffer target.
    * @return true (successful) or false (failed)
    */
    bool binding();

    /**
    * @brief releases the FBO from the framebuffer target.
    * @return true (successful) or false (failed)
    */
    bool releasing();
    /**
     * @brief resizing FrameBufferObject dimensions ... It does not work!!!! (Ting)
     * @param[in] width buffer image width
     * @param[in] height buffer image height
     */
    void resizing(int ,int );

    unsigned short getWidth() const;
    unsigned short getHeight() const;

private:
    /**
     * @brief initialize the created FBO.
     * @param[in] width image width
     * @param[in] height image height
     * @param[in] attachment attachment point
     * @param[in] internal_format internal format of the texture image to be attached to FBO
     * @param[in] target texture target of the attached texture image
     * @param[in] samples number of samples in each pixel
     * @param[in] mipmap with or without level-of-detail. The number of levels is computed dynamically.
     */
    void init(int width, int height,
          vmtkFrameBufferObject::Attachment attachment,
              GLenum internal_format, GLenum target,
              GLint samples = 0, bool mipmap = false);

    GLuint bound(const int &min, const int &val, const int &max);

    GLuint fbo;                  /**< FBO name */
    GLuint *texture;              /**< the bound texture name */
    GLuint depthTexture;
    unsigned short m_num_render_buffer_tex;

    unsigned short m_width, m_height;
    GLint m_previous_fbo;

    GLuint m_color_buffer;
    GLuint m_depth_buffer;
    GLuint m_stencil_buffer;
};

#endif // FRAMEBUFFEROBJECT_H
