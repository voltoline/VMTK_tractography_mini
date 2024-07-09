#ifndef QLOADIMAGE_H
#define QLOADIMAGE_H
#include "utilities/QCheckingOpenGL.h"
#include "utilities/QManagementResources.h"
#include <QImage>
#include <QFile>
#include <QOpenGLWidget>

class QLoadImage
{
public:
    QLoadImage();
    ~QLoadImage();

    /**
     * @brief verified image data and extension
     * @param image_path the path of the image
     * @return
     */
    void loadBMPImageToTexture(const char *image_path);

    unsigned char *loadBMPImage(const char *image_path);

    void imageLoadedToTextureOGL();

    GLuint getTextureID() const{ return m_textureID; }

    void loadImageToTexture(const char *image_path);

    void deleteTextureImage();
private:

    /**
     * @brief load file from the resource
     * @param file the file path
     * @return a pointer to the buffer of the loaded image
     */
    unsigned char* loadResourceFile(const char *file);

    unsigned char* imageBits;
    int w,h;

    GLuint m_textureID;
};

#endif // QLOADIMAGE_H
