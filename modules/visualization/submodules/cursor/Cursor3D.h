#ifndef CURSOR3D_H
#define CURSOR3D_H
#include "model/primitives/Cylinder.h"
#include "model/primitives/Sphere.h"
#include "model/primitives/LineP2P.h"
#include "CursorRenderer3D.h"
#include "glm/detail/type_vec3.hpp"


class Cursor3D
{
public:

    /**
     * @brief Cursor3D (constructor)
     */
    Cursor3D();

    /**
     * @brief initializing 3D cursor settings.
     */
    void initCursor3D();

    /**
     * @brief drawCursor3D
     * @param[in] pos: position of cursor in vertex space.
     */
    void drawCursor3D(glm::vec3 pos);

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

    void setDepthTexture(GLuint idDepthTexture);
    void setDepthBackTexture(GLuint idDepthBackTexture);

    void setEnableFixedAlpha(bool setEnableFixedAlpha);

    Cylinder *getPrimitivesCursor() const
    {
        return m_cursorCylinder;
    }

    void setPrimitivesCursor(Cylinder *cursor)
    {
        m_cursorCylinder = cursor;
    }

    void setDrawCursorType(DrawCursorType newDrawCursorType);

protected:
    void initCrossCursor();
    void initRadiusCursor();
    void initLineCursor();
    void drawCrossCursor(glm::vec3 pos);
    void drawRadiusCursor(glm::vec3 pos);
    void drawLineCursor(glm::vec3 pos);
private:
    Cylinder* m_cursorCylinder;             /** < group of primitives (Cylinders).*/
    Sphere* m_cursorRadius;             /** < Sphere primitive.*/
    LineP2P* m_cursorLine;             /** < Sphere primitive.*/
    glm::mat4 m_projectionMatrix;   /**< projection transformation matrix */
    glm::mat4 m_modelViewMatrix;    /**< modelview transformation matrix */
    glm::vec3 m_normalDirection;     /**< Orientation of Cursor based on 2D Focus */
    bool m_enableCursor;
    bool m_enableFixedAlpha;
    GLuint m_idDepthTexture;
    GLuint m_idDepthBackTexture;
    DrawCursorType m_drawCursorType;

};

#endif // CURSOR3D_H
