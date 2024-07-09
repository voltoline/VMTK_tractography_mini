/**
  * @file vmtkTextRenderer.h
  * @brief renders text on the screen by using an image texture and geometry shader.
  * @author Raphael Voltoline
  * @date June 2015
  * @note This code is adapted from the code available at http://github.prideout.net/strings-inside-vertex-buffers/.
  */

#ifndef VMTK_TEXT_RENDERER_H
#define VMTK_TEXT_RENDERER_H

#include "utilities/QCheckingOpenGL.h"
#include <vector>

class vmtkManagementShader;

/**
 * @brief The vmtkTextRenderer class
 * @details This class is for rendering texts on the screen
 */
class vmtkTextRenderer
{
public:
    vmtkTextRenderer();
    ~vmtkTextRenderer();

    /**
     * @brief initializes the text renderer by compiling and linking its shader.
     */
    void initialize();

    /**
     * @brief renders all texts that were added.
     */
    void render();

    /**
     * @brief adds a new text to be rendered
     * @param text the text to be added
     */
    void addText(const char *text);

    /**
     * @brief sets the text especified by text_index to be right-aligned.
     * @param text_index especifies the index of text that will be right-aligned. If it is not especified, it will consider the last text added.
     * @param distance distance in number of cells between the last letter of the text and the edge
     */
    void rightAligneText(short text_index = -1, unsigned short distance = 0);

    /**
     * @brief sets the size of the letters of the text
     * @param height height of the text in cells of the grid
     * @param width width of the text in cells of the grid
     * @param text_index especifies the index of text. If it is not especified, it will consider the last text added.
     */
    void setTextSize(unsigned short height, unsigned short width, short text_index = -1);

    /**
     * @brief sets the default size of the letters of the text
     * @param height default height of the text in cells of the grid
     * @param width default width of the text in cells of the grid
     */
    void setDefaultSize(unsigned short height, unsigned short width);

    /**
     * @brief sets the color of the text
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     * @param text_index especifies the index of text. If it is not especified, it will consider the last text added.
     */
    void setTextColor(float r, float g, float b, float a, short text_index = -1);

    /**
     * @brief sets the coordinates of the left upper corner of the text that will be renderer
     * @param row row of the upper left corner
     * @param col column of the upper left corner
     * @param text_index especifies the index of text. If it is not especified, it will consider the last text added.
     */
    void setTextOrigin(unsigned short row, unsigned short col, short text_index = -1);

    /**
     * @brief changes the text
     * @param text the new text
     * @param text_index especifies the index of text that will be changed. If it is not especified, it will consider the last text added.
     */
    void changeText(const char* text, short text_index = -1);

    /**
     * @brief remove one text
     * @param text_index especifies the index of text that will be removed. If it is not especified, it will consider the last text added.
     */
    void deleteText(short text_index = -1);

    /**
     * @brief sets the size of the grid where the texts will be rendered.
     * @param columns number of columns of the grid
     * @param rows number of rows of the grid
     */
    void setGrid(unsigned short columns, unsigned short rows);

private:

    /**
     * @struct TextInfo
     * @brief data struct representing the text and its information for rendering
     */
    typedef struct _text_info
    {
        char* m_text;                       /**< text to be renderer */
        unsigned short m_text_len;          /**< length of the text */
        unsigned short m_origin_row;        /**< row of the left upper corner of the text*/
        unsigned short m_origin_col;        /**< column of the left upper corner of the text*/
        unsigned short m_height;            /**< height in number of cell of the letter */
        unsigned short m_width;             /**< width in number of cell of the letter */
        unsigned short m_right_dist;        /**< distance from the right edge */
        float m_color[4];                   /**< text color */
        float m_size[2];                    /**< m_size[0], m_size[1] are, respectively, the width and height of the letter that are sent to the shader */
        float m_origin[2];                  /**< m_origin[0], m_origin[1] are, respectively, the x and y coordinates of the text origin that are sent to the shader */
        bool m_is_right_aligned;

        _text_info()
        {
            m_text = NULL;
            m_text_len = 0;
            m_origin_row = 0;
            m_origin_col = 0;
            m_height = 1;
            m_width = 1;
            m_right_dist = 0;

            m_color[0] = 1.f;
            m_color[1] = 1.f;
            m_color[2] = 1.f;
            m_color[3] = 1.f;

            m_size[0] = 1.f;
            m_size[1] = 1.f;

            m_origin[0] = 0.f;
            m_origin[1] = 0.f;

            m_is_right_aligned = false;
        }

        ~_text_info()
        {
            if(m_text != NULL)
                delete[] m_text;
        }

    }TextInfo;

    /**
     * @brief loads the image that will be used as the texture where the letters will be fetched
     * @param image_path the path of the image
     * @return
     */
    GLuint loadBMPImage(const char *image_path);

    /**
     * @brief creates and update the content of the buffer that holds the text
     * @param text the text that will be put into the buffer
     * @param len the length of the text
     */
    void setTextBuffer(const char *text, unsigned short len);

    /**
     * @brief retrieves the struct especified by index that holds the text informations
     * @param index index of the struct
     * @return returns the struct especified if it exist or NULL otherwise
     */
    TextInfo *getTextInfo(short index);

    /**
     * @brief updates all text informations if the size of the grid changes
     */
    void updateAllText();

    /**
     * @brief loads the *.bmp image from the resource
     * @param file the file path
     * @return a pointer to the buffer of the loaded image
     */
    unsigned char* loadResourceFile(const char *file);

    GLuint m_program;                           /**< shader program index */
    GLuint m_vao;                               /**< vertex array name */
    GLuint m_vbo;                               /**< vertex buffer name where the text is*/
    GLuint m_text_tex_name;                     /**< texture name where the letters will be fetched */
    GLuint m_text_size_loc;                     /**< uniform location that especifies the size of the letter */
    GLuint m_text_origin_loc;                   /**< uniform location that especifies the origin of the text */
    GLuint m_text_character_loc;                /**< vertex attribute location that will receive the letter of the text */
    GLuint m_text_color_loc;                    /**< uniform location that especifies the text color */

    unsigned short m_rows;                      /**< number of rows of the grid */
    unsigned short m_columns;                   /**< number of columns of the grid */
    unsigned short m_default_width;             /**< default width of the letter when it is not especified */
    unsigned short m_default_height;            /**< default height of the letter when it is not especified */

    float m_cell_width;                         /**< width of the cell on the screen */
    float m_cell_height;                        /**< height of the cell on the screen */

    vmtkManagementShader *m_shader_program;     /**< object that holds the shader program*/

    std::vector<TextInfo*> *m_reg;              /**< vector that hold all texts and their informations*/
};

#endif // VMTK_TEXT_RENDERER_H
