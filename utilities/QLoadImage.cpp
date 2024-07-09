#include "QLoadImage.h"

QLoadImage::QLoadImage()
{
    imageBits=NULL;
    w = 0; h = 0;
    m_textureID = 0;
}

QLoadImage::~QLoadImage()
{
    if(m_textureID != 0){
        glDeleteTextures(1, &m_textureID);
    }
}

void QLoadImage::deleteTextureImage(){
    if(m_textureID != 0){
        glDeleteTextures(1, &m_textureID);
    }
    m_textureID = 0;
}

void QLoadImage::loadImageToTexture(const char * image_path)
{
    QImage img = QImage(image_path);
#ifdef enableOldQGL
    QImage tex = QGLWidget::convertToGLFormat( img );
#else
    QImage tex = img;
#endif
    imageBits = tex.bits();
    glGenTextures(1, &m_textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Return the ID of the texture we just created
}

void QLoadImage::loadBMPImageToTexture(const char * image_path)
{
    // Data read from the header of the BMP file
    unsigned char *header;
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;

    // Actual RGB data
    unsigned char * data = loadResourceFile(image_path);

    if(data == NULL)
        return;

    header = data;
    data = &data[54];

    // A BMP files always begins with "BM"
    if ( header[0] != 'B' || header[1] != 'M' ){
        std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
        return;
    }

    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E]) != 0  )
    {
        std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
        return;
    }

    if ( *(int*)&(header[0x1C]) != 24 )
    {
        std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
        return;
    }

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)
        imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component

    if (dataPos == 0)
        dataPos = 54; // The BMP header is done that way

    unsigned char *data_inv = new unsigned char [imageSize];
    for(unsigned int i = 0; i < height; i++)
    {
        memcpy(&data_inv[width * 3 * i], &data[width * 3 * (height - i - 1)], sizeof(unsigned char) * width * 3);
    }

    // Create one OpenGL texture
    glGenTextures(1, &m_textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete [] header;
    delete [] data_inv;

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

unsigned char *QLoadImage::loadBMPImage(const char *image_path)
{
        // Data read from the header of the BMP file
        unsigned char *header;
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;

        // Actual RGB data
        unsigned char * data = loadResourceFile(image_path);

        if(data == NULL)
            return 0;

        header = data;
        data = &data[54];

        // A BMP files always begins with "BM"
        if ( header[0] != 'B' || header[1] != 'M' ){
            std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
            return 0;
        }

        // Make sure this is a 24bpp file
        if ( *(int*)&(header[0x1E]) != 0  )
        {
            std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
            return 0;
        }

        if ( *(int*)&(header[0x1C]) != 24 )
        {
            std::cerr << "loadBMPImage -> Not a correct BMP file!" << std::endl;
            return 0;
        }

        // Read the information about the image
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        w = width      = *(int*)&(header[0x12]);
        h = height     = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize == 0)
            imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component

        if (dataPos == 0)
            dataPos = 54; // The BMP header is done that way

        unsigned char *data_inv = new unsigned char [imageSize];
        for(unsigned int i = 0; i < height; i++)
        {
            memcpy(&data_inv[width * 3 * i], &data[width * 3 * (height - i - 1)], sizeof(unsigned char) * width * 3);
        }

        delete [] header;
        delete [] data_inv;

        return imageBits = data_inv;
}

unsigned char* QLoadImage::loadResourceFile(const char *file)
{
    QFile infile(file);

    if (!infile.open(QIODevice::ReadOnly))
    {
        std::cerr << "loadResourceFile -> Unable to open file: '" << file<< "'!" <<std::endl;
        return NULL;
    }

    if(infile.size() > 54)
    {
        char *buffer = new char [infile.size()];
        infile.read(buffer, infile.size());
        infile.close();

        return imageBits = reinterpret_cast<unsigned char*>(buffer);

    }else
        std::cerr << "loadResourceFile -> Not a correct BMP file!" << std::endl;

    return NULL;

}

void QLoadImage::imageLoadedToTextureOGL()
{
    if(imageBits == NULL){
        std::cerr << "imageLoadedToTextureOGL -> Not loaded BMP file or is NULL!" << std::endl;
        return;
    }

    // Create one OpenGL texture
    glGenTextures(1, &m_textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, imageBits);

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
