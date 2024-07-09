#ifndef FreeLineClosed_H
#define FreeLineClosed_H

#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"
#include "utilities/functions/vmtkGeneralFunctions.h"
#include "utilities/QLoadImage.h"
#include <QVector>

class FreeLineClosed
{

public:
    FreeLineClosed();
    ~FreeLineClosed();
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
    void makeFreeLineClosed(QVector<glm::vec3> points);
    void addFreeLineClosed(QVector<glm::vec3> points, glm::vec4 color = glm::vec4(1.0,0.0,0.0,1.0));

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);

    void setColorForAll(glm::vec4 color);
    void setColorForEach(int id, glm::vec4 color);

    void setInitialScaleForAll(glm::vec3 vS);
    void setInitialScaleForEach(int id, glm::vec3 vS);
    void setInitialPositionForAll(glm::vec3 vT);
    void setInitialPositionForEach(int id, glm::vec3 vT);
    void setInitialRotateForAll(glm::vec3 vR);
    void setInitialRotateForEach(int id, glm::vec3 vR);
    void setScaleForAll(glm::vec3 vS);
    void setScaleForEach(int id, glm::vec3 vS);
    void setTranslateForAll(glm::vec3 vT);
    void setTranslateForEach(int id, glm::vec3 vT);
    void setRotateForAll(glm::vec3 vR);
    void setRotateForEach(int id, glm::vec3 vR);


    void updatingBuffersGeometry();
    void initializePrimitive();
    void drawGeometry();
    void geometryRender();


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);

    bool bindingRender();
    void releasingRender();

    bool setShaderProgram(vmtkManagementShader *mShaderProgram);
    vmtkManagementShader *getShaderProgram() const;

    void setIDTextureActivated(GLint lta);


    GLint getLastTextureActivated();
    void setDepthTexture(GLuint idTexture);
    void enableDepthTexture(bool state);
    void setDepthTextureBack(GLuint idTexture);
    void enableDepthTextureBack(bool state);

    void setPointSize(GLfloat pointSize);

    void setStatePointSize(bool statePointSize);

    void removeFreeLineClosed(int id);
    void removeLastFreeLineClosed();
protected:

    void createAndInitializeMatrices();
    void createAndInitializeNewMatrix();


    void makeIndices(int size);
    void removeMatriz(int id);
    int sumSizesindices(int id);
    void removeLastMatriz();
private:

    void initPrimitive();
    void createBuffers();

    vmtkManagementShader *m_shaderProgram; /** pointer to vmtkManagementShader class */
    GLuint  m_depthTexture, m_depthTextureBack , m_texture3D;    /** Texture name of the image 2D. */

    QVector<PrimitiveData> m_gData;
    QVector<GLushort> m_indices;
    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    ushort m_sizeData;
    ushort m_sizeIndices;
    int m_numTotalObjects;

    bool m_stateAddedShader;
    bool m_stateShader;

    bool m_statePointSize;
    bool m_stateDepthTexture, m_stateDepthTextureBack;
    glm::vec3 m_depthScaleFactor;
    GLfloat m_pointSize;


    std::vector<glm::mat4> vectorModelViewMatrixTexture;
    std::vector<glm::vec4> vectorColorTexture;
    GLuint modelViewMatrixBuffer, colorBuffer, radiusBuffer; /**< has been created and sized to store one vmath::Matrix4f per-instance. */


    QList<glm::vec4> m_color;
    QList<glm::mat4> m_initialPositionMatrixGeometry;   /** Initial matrix position for geometry */
    QList<glm::mat4> m_initialScaleMatrixGeometry;      /** Initial matrix scale for geometry */
    QList<glm::mat4> m_initialRotateMatrixGeometry;     /** Initial matrix rotation for geometry */
    QList<glm::mat4> m_scaleMatrixGeometry;             /** Matrix scale for geometry */
    QList<glm::mat4> m_translateMatrixGeometry;         /** Matrix translation for geometry  */
    QList<glm::mat4> m_rotateMatrixGeometry;            /** Matrix rotation for geometry  */
    glm::mat4 m_modelViewMatrixGeometry;                /** local model view matrix (for all scene)  */
    glm::mat4 m_projectionMatrix;                       /** Global model view matrix (for all scene)  */
    glm::mat4 m_modelViewMatrix;                        /** Global model view matrix (for all scene)  */
    GLint m_getIDTextureActivated;
    GLint m_lastIDTextureActivated;
    QVector<int> m_sizeSubindices;
};




#endif // FreeLineClosed_H
