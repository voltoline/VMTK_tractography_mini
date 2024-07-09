#ifndef CURSOR2D_H
#define CURSOR2D_H
#include "model/primitives/Rectangle_Square.h"
#include "CursorRenderer2D.h"
#include "model/primitives/Sphere.h"
#include "model/primitives/LineP2P.h"
#include "model/primitives/Circle.h"

class Cursor2D
{
public:

    /**
     * @brief Cursor2D (constructor)
     */
    Cursor2D();

    /**
     * @brief initializing 2D cursor settings.
     */
    void initCursor();

    /**
     * @brief drawCursor 2D
     */
    void drawCursor();

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

    void setEnableCursor(bool enable);

    glm::mat4 getProjectionMatrix();
    void setProjectionMatrix(glm::mat4 projectionMatrix);

    glm::vec3 getPos() const;
    void setPos(const glm::vec3 &pos);

    void setDrawCursorType(DrawCursorType newDrawCursorType);

protected:
    /**
     * @brief initCrossCursor: Initialization of 2D target settings for cross-type rendering.
     */
    void initCrossCursor();
    void initRadiusCursor();
    void initLineCursor();
    /**
     * @brief drawCrossCursor: 2d target rendering.
     */
    void drawCrossCursor();
    void drawRadiusCursor();
    void drawLineCursor();
private:
    Rectangle_Square* m_cursorCross;             /** < group of primitives (rectangles).*/
    Circle* m_cursorRadius;             /** < Sphere primitive.*/
    LineP2P* m_cursorLine;             /** < Sphere primitive.*/
    glm::mat4 m_projectionMatrix;   /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;    /**< modelview transformation matrix */
    bool m_enableCursor;
    glm::vec3 m_pos;
    DrawCursorType m_drawCursorType;
};

#endif // Cursor2D_H
