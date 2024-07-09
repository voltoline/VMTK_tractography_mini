#include "vmtkTextRenderer.h"

#include <cstring>
#include <iostream>
#include <QFile>

#include "utilities/QManagementResources.h"
#include "renderer/vmtkManagementShader.h"

vmtkTextRenderer::vmtkTextRenderer():
    m_reg(NULL),
    m_shader_program(NULL)
{
    m_program = 0;
    m_vao = m_vbo = m_text_tex_name = 0;
    m_reg = NULL;
    m_reg = new std::vector<TextInfo*>();

    m_default_height = 1;
    m_default_width = 1;

    setGrid(4, 4);
}

vmtkTextRenderer::~vmtkTextRenderer()
{
    short vector_size = m_reg->size();
    TextInfo *text_info = NULL;
    for(unsigned short i = 0; i < vector_size; i++)
    {
        text_info = (*m_reg).at(i);
        delete[] text_info->m_text;
    }

    if(m_reg != NULL)
        delete m_reg;

    if(m_shader_program != NULL)
        delete m_shader_program;

    if(m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);

    if(m_vbo != 0)
        glDeleteBuffers(1, &m_vbo);

    if(m_text_tex_name != 0)
        glDeleteTextures(1, &m_text_tex_name);
}

void vmtkTextRenderer::initialize()
{
    QManagementResource *vs = new QManagementResource;
    QManagementResource *geom = new QManagementResource;
    QManagementResource *fs = new QManagementResource;

    GLuint vertex = vs->compileResourceFileShader(QManagementResource::vertex, ":/text_vert");
    GLuint geometry = geom->compileResourceFileShader(QManagementResource::geometry, ":/text_geom");
    GLuint fragment = fs->compileResourceFileShader(QManagementResource::fragment, ":/text_frag");

    m_shader_program = new vmtkManagementShader;
    m_shader_program->setShader(vertex);
    m_shader_program->setShader(geometry);
    m_shader_program->setShader(fragment);
    m_shader_program->linking();

    m_program = m_shader_program->getProgram();
    m_shader_program->binding();

    glUniform2f(glGetUniformLocation(m_program, "CellSize"), 1.0f / 16, (300.0f / 384) / 6);
    glUniform2f(glGetUniformLocation(m_program, "CellOffset"), (0.5 / 256.0), (0.5 / 256.0));
    glUniform1i(glGetUniformLocation(m_program, "Sampler"), 0);

    m_text_tex_name = loadBMPImage(":/text_bmp");

    if(m_text_tex_name != 0)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_text_color_loc = glGetUniformLocation(m_program, "TextColor");
        m_text_size_loc = glGetUniformLocation(m_program, "RenderSize");
        m_text_origin_loc = glGetUniformLocation(m_program, "RenderOrigin");
        m_text_character_loc = glGetAttribLocation(m_program, "Character");
    }

    m_shader_program->releasing();

    delete vs;
    delete geom;
    delete fs;
}

void vmtkTextRenderer::setGrid(unsigned short columns, unsigned short rows)
{
    m_rows = rows;
    m_cell_height = 1.f / m_rows;

    m_columns = columns;
    m_cell_width = 1.f / m_columns;

    updateAllText();
}

void vmtkTextRenderer::updateAllText( )
{
    short vector_size = m_reg->size();
    TextInfo *text_info = NULL;
    for(unsigned short i = 0; i < vector_size; i++)
    {
        text_info = (*m_reg).at(i);
        setTextSize(text_info->m_height, text_info->m_width, i);

        if(text_info->m_is_right_aligned)
            rightAligneText(i, text_info->m_right_dist);
    }
}

void vmtkTextRenderer::addText(const char *text)
{
    TextInfo *text_info = new TextInfo();
    text_info->m_text_len = strlen(text);
    text_info->m_text = new char[text_info->m_text_len + 1];
    strcpy(text_info->m_text, text);

    m_reg->push_back(text_info);

    setTextSize(m_default_height, m_default_width);
}

vmtkTextRenderer::TextInfo* vmtkTextRenderer::getTextInfo(short index)
{
    short i = m_reg->size();

    if(index < 0)
    {
        i--;
        if(i == -1)
            return NULL;
        else
            return (*m_reg).at(i);
    }

    if(index <= (i - 1))
        return (*m_reg).at(index);
    else
        return NULL;
}

void vmtkTextRenderer::rightAligneText(short text_index, unsigned short distance)
{
    TextInfo *text_info = getTextInfo(text_index);
    if(text_info != NULL)
    {
        text_info->m_is_right_aligned = true;
        text_info->m_right_dist = distance;
        setTextOrigin(text_info->m_origin_row, m_columns - (text_info->m_text_len * text_info->m_width) - distance, text_index);
    }
}

void vmtkTextRenderer::setTextColor(float r, float g, float b, float a, short text_index)
{
    TextInfo *text_info = getTextInfo(text_index);
    if(text_info != NULL)
    {
        text_info->m_color[0] = r;
        text_info->m_color[1] = g;
        text_info->m_color[2] = b;
        text_info->m_color[3] = a;
    }
}

void vmtkTextRenderer::setTextSize(unsigned short height, unsigned short width, short text_index)
{
    TextInfo *text_info = getTextInfo(text_index);
    if(text_info != NULL)
    {
        text_info->m_size[0] = width * m_cell_width;
        text_info->m_size[1] = height * m_cell_height;

        text_info->m_width = width;
        text_info->m_height = height;

        setTextOrigin(text_info->m_origin_row, text_info->m_origin_col, text_index);
    }
}

void vmtkTextRenderer::setDefaultSize(unsigned short height, unsigned short width)
{
    m_default_height = height;
    m_default_width = width;
}

void vmtkTextRenderer::setTextOrigin(unsigned short row, unsigned short col, short text_index)
{
    TextInfo *text_info = getTextInfo(text_index);
    if(text_info != NULL)
    {
        text_info->m_origin[0] = 2 * m_cell_width * (text_info->m_width * 0.5f + col) - 1.f;
        text_info->m_origin[1] = 1.f - 2 * m_cell_height * (text_info->m_height * 0.5f + row);

        text_info->m_origin_row = row;
        text_info->m_origin_col = col;
    }
}

void vmtkTextRenderer::changeText(const char* text, short text_index)
{
    TextInfo *text_info = getTextInfo(text_index);
    if(text_info != NULL)
    {
        delete[] text_info->m_text;

        text_info->m_text_len = strlen(text);
        text_info->m_text = new char[text_info->m_text_len + 1];
        strcpy(text_info->m_text, text);

        if(text_info->m_is_right_aligned)
            rightAligneText(text_index);
    }
}

void vmtkTextRenderer::deleteText(short text_index)
{
    unsigned short vector_size = m_reg->size();
    if( 0 <= text_index && text_index < vector_size)
    {
        TextInfo *text_info = getTextInfo(text_index);
        delete text_info->m_text;
        m_reg->erase(m_reg->begin() + text_index);
    }
}

void vmtkTextRenderer::setTextBuffer(const char* text, unsigned short len)
{
    if(m_vbo == 0)
    {
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, len, text, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(m_text_character_loc);
        glVertexAttribIPointer(m_text_character_loc, 1, GL_UNSIGNED_BYTE, 0, 0);
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, len, text, GL_DYNAMIC_DRAW);

}

void vmtkTextRenderer::render()
{
    if(m_text_tex_name != 0)
    {
        glBindVertexArray(m_vao);
        m_shader_program->binding();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_text_tex_name);

        for(unsigned int i = 0; i < m_reg->size(); i++)
        {
            TextInfo *text_info = (*m_reg).at(i);
            setTextBuffer(text_info->m_text, text_info->m_text_len);

            glUniform2fv(m_text_origin_loc, 1, text_info->m_origin);
            glUniform4fv(m_text_color_loc, 1, text_info->m_color);
            glUniform2fv(m_text_size_loc, 1, text_info->m_size);

            glDrawArrays(GL_POINTS, 0, text_info->m_text_len);
        }

        m_shader_program->releasing();;
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

}

GLuint vmtkTextRenderer::loadBMPImage(const char * image_path)
{

//    printf("Reading image %s\n", image_path);

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
        std::cerr << "vmtkTextRenderer::loadBMPImage -> Not a correct BMP file!" << std::endl;
        return 0;
    }

    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E]) != 0  )
    {
        std::cerr << "vmtkTextRenderer::loadBMPImage -> Not a correct BMP file!" << std::endl;
        return 0;
    }

    if ( *(int*)&(header[0x1C]) != 24 )
    {
        std::cerr << "vmtkTextRenderer::loadBMPImage -> Not a correct BMP file!" << std::endl;
        return 0;
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
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data_inv);

    // OpenGL has now copied the data. Free our own version
    delete [] header;
    delete [] data_inv;

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Return the ID of the texture we just created
    return textureID;
}

unsigned char* vmtkTextRenderer::loadResourceFile(const char *file)
{
    QFile infile(file);

    if (!infile.open(QIODevice::ReadOnly))
    {
        std::cerr << "vmtkTextRenderer::loadResourceFile -> Unable to open file: '" << file<< "'!" <<std::endl;
        return NULL;
    }

    if(infile.size() > 54)
    {
        char *buffer = new char [infile.size()];
        infile.read(buffer, infile.size());
        infile.close();

        return reinterpret_cast<unsigned char*>(buffer);

    }else
        std::cerr << "vmtkTextRenderer::loadResourceFile -> Not a correct BMP file!" << std::endl;

    return NULL;

}
