#ifndef FREEDRAWCURSOR_H
#define FREEDRAWCURSOR_H

#include "model/primitives/FreeLineClosed.h"

class FreeDrawCursor
{
public:

    /**
     * @brief Cursor2D (constructor)
     */
    FreeDrawCursor();

    /**
     * @brief initializing ROI cursor settings.
     */
    void initFreeDrawCursor();

    /**
     * @brief drawCursor
     * @param[in] current_slice: slice where cursor has been drawing.
     */
    void drawFreeDrawCursor(unsigned int current_slice, int tract_id);

    /**
     * @brief sets the matrix of ModelView
     * @param[in] modelViewMatrix
     */
    void setModelViewMatrix(glm::mat4 modelViewMatrix);

    /**
    * @brief get the matrix of model view.
    */
    glm::mat4 getModelViewMatrix();

    void setShaderProgram(vmtkManagementShader *shaderProgram);

    void setEnableFreeDrawCursor(bool enable);

    glm::mat4 getProjectionMatrix();
    void setProjectionMatrix(glm::mat4 projectionMatrix);

    glm::vec3 getPos() const;
    void setPos(const glm::vec3 &pos);

    void addPoint(glm::vec3 pos);
    void addFreeDrawCursor(int id, int tract_id, unsigned int slice, glm::vec4 color);

    void setCurrentActiveFreeDrawCursor(int id);
    int getCurrentActiveDFreeDrawCursor();
    void removeFreeDrawCursor(int id);
    void setColorFreeDrawCursor(int id, glm::vec4 color);

    unsigned int getSliceCurrentActiveFreeDrawCursor();
    void setSliceCurrentActiveFreeDrawCursor(int id, unsigned int slice);

private:

    typedef struct structFreeDrawCursor {
        int id;
        unsigned int slice;
        FreeLineClosed * lines;             /** < group of primitives */
        unsigned int num_lines;
        std::vector<glm::vec3> points;
        QVector<glm::vec3> line_points;
        glm::vec4 color;
        int tract_id;
    } FreeDrawCursor_attr;

    std::vector<FreeDrawCursor_attr> m_FreeDrawCursor;

    FreeLineClosed * m_lines_closed_ref;
    glm::mat4 m_projectionMatrix;   /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;    /**< modelview transformation matrix */
    bool m_enableFreeDrawCursor;
    bool m_enableMovement;
    glm::vec3 m_pos;
    unsigned int m_currentActiveFreeLine;

};

#endif // FREEDRAWCURSOR_H
