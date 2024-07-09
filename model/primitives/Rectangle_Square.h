#ifndef RECTANGLE_SQUARE_H
#define RECTANGLE_SQUARE_H

#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"
#include "utilities/functions/vmtkGeneralFunctions.h"
#include "utilities/QLoadImage.h"




class Rectangle_Square
{

public:
    /**
     * @brief The ModeFilled enum
     */
    typedef enum TypeFilled
    {
         solid          = 0,    /**< drawing solid */
         wire           = 1,    /**< drawing wire */
         points         = 2,    /**< drawing points */
         line_strip     = 3      /**< drawing line or lines */
    }TypeFilled;

    Rectangle_Square();
    ~Rectangle_Square();
    /**
     * @struct VertexData
     */
    struct PrimitiveData
    {
        glm::vec3 verCoord; /** < position coordinates */
        glm::vec2 texCoord; /** < texture coordinates */
        glm::vec3 normal; /** < normals */
    };

    void defaultResourceFile();
    void makeRectangle();

    void addRectanglesDefaultForAll(GLushort numRectangles, GLfloat scaleWidth, GLfloat scaleHeight, glm::vec4 color);
    void addRectangleCustomized(GLfloat scaleRadius, GLfloat scaleHeight, glm::vec4 color);

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);

    void setColorForAll(glm::vec4 color);
    void setColorForEach(GLuint id, glm::vec4 color);
    void setScaleWidthForAll(GLfloat scaleWidth);
    void setScaleWidthForEach(GLuint id, GLfloat scaleWidth);
    void setScaleHeigthForAll(GLfloat scaleHeight);
    void setScaleHeigthForEach(GLuint id, GLfloat scaleHeight);

    void setInitialScaleForAll(glm::vec3 vS);
    void setInitialScaleForEach(GLuint id, glm::vec3 vS);
    void setInitialPositionForAll(glm::vec3 vT);
    void setInitialPositionForEach(GLuint id, glm::vec3 vT);
    void setInitialRotateForAll(glm::vec3 vR);
    void setInitialRotateForEach(GLuint id, glm::vec3 vR);
    void setScaleForAll(glm::vec3 vS);
    void setScaleForEach(GLuint id, glm::vec3 vS);
    void setTranslateForAll(glm::vec3 vT);
    void setTranslateForEach(GLuint id, glm::vec3 vT);
    void setRotateForAll(glm::vec3 vR);
    void setRotateForEach(GLuint id, glm::vec3 vR);


    void updatingBuffersGeometry();
    void initializePrimitive();
    void loadImageTexture(const char *image_path);
    void deleteImageTexture();
    void enableImageTexture(bool state);
    void drawGeometry(TypeFilled mode, GLuint idActivateTexture);
    void geometryRender( TypeFilled mode = solid );


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);

    bool bindingRender();
    void releasingRender();

    bool setShaderProgram(vmtkManagementShader *mShaderProgram);
    vmtkManagementShader *getShaderProgram() const;

    void setIDTextureActivated(GLuint lta);


    GLuint getLastTextureActivated();
    void setDepthTexture(GLuint idTexture);
    void enableDepthTexture(bool state);
    void setDepthTextureBack(GLuint idTexture);
    void enableDepthTextureBack(bool state);

    void setPointSize(GLfloat pointSize);

    void setStatePointSize(bool statePointSize);

protected:
    void createLoadImageToTexture(QLoadImage* qli);


    void createAndInitializeMatrices();
    void createAndInitializeNewMatrix();

    void createAndInitializeSettings(glm::vec4 color, GLfloat scaleWidth, GLfloat scaleHeight);
    void createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleWidth,GLfloat scaleHeight);

private:

    void initPrimitive();
    void createBuffers();

    vmtkManagementShader *m_shaderProgram; /** pointer to vmtkManagementShader class */
    QLoadImage *m_qImageToTexture;
    GLuint  m_depthTexture, m_depthTextureBack , m_texture3D;    /** Texture name of the image 2D. */

    std::vector<PrimitiveData> m_gData;
    std::vector<GLushort> m_indices;
    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    ushort m_sizeData;
    ushort m_sizeIndices;
    GLuint m_numTotalObjects;

    bool m_dataRectangleCreated;
    bool m_stateAddedShader;
    bool m_stateShader;

    bool m_statePointSize;
    bool m_stateTextureImage, m_stateDepthTexture, m_stateDepthTextureBack;
    glm::vec3 m_depthScaleFactor;
    GLfloat m_pointSize;


    GLint m_colorSamplerLocationID;
    GLint m_modelVewMatrixSamplerLocationID;
    std::vector<glm::mat4> vectorModelViewMatrixTexture;
    std::vector<glm::vec4> vectorColorTexture;
    GLuint modelViewMatrixBuffer, colorBuffer, radiusBuffer; /**< has been created and sized to store one vmath::Matrix4f per-instance. */
    GLuint modelVewMatrixTextureBufferObject, colorTextureBufferObject, radiusBufferObject;

    std::vector<glm::vec4> m_color;
    std::vector<glm::mat4> m_scaleWidthForAll;
    std::vector<glm::mat4> m_scaleHeigthForAll;
    std::vector<glm::mat4> m_initialPositionMatrixGeometry;   /** Initial matrix position for geometry */
    std::vector<glm::mat4> m_initialScaleMatrixGeometry;      /** Initial matrix scale for geometry */
    std::vector<glm::mat4> m_initialRotateMatrixGeometry;     /** Initial matrix rotation for geometry */
    std::vector<glm::mat4> m_scaleMatrixGeometry;             /** Matrix scale for geometry */
    std::vector<glm::mat4> m_translateMatrixGeometry;         /** Matrix translation for geometry  */
    std::vector<glm::mat4> m_rotateMatrixGeometry;            /** Matrix rotation for geometry  */
    glm::mat4 m_modelViewMatrixGeometry;                /** local model view matrix (for all scene)  */
    glm::mat4 m_projectionMatrix;                       /** Global model view matrix (for all scene)  */
    glm::mat4 m_modelViewMatrix;                        /** Global model view matrix (for all scene)  */
    GLuint m_getIDTextureActivated;
    GLuint m_lastIDTextureActivated;
};

#endif // Rectangle_Square_H
