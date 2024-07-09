#ifndef LineP2P_H
#define LineP2P_H

#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"
#include "utilities/functions/vmtkGeneralFunctions.h"



class LineP2P
{

public:
    LineP2P();
    ~LineP2P();
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

    void makeLineP2P(glm::vec3 point1, glm::vec3 point2);


    void addLineP2P(glm::vec3 point1, glm::vec3 point2, glm::vec4 color = m_defaultColor, bool view = true);
    void addLineP2P(std::pair<glm::vec3, glm::vec3> points, glm::vec4 color = m_defaultColor, bool view = true);

    void updatePointsLineP2P(size_t id, glm::vec3 point1, glm::vec3 point2);
    void updatePointsLineP2P(size_t id, std::pair<glm::vec3, glm::vec3> points);
    void updateLastPointsLineP2P(glm::vec3 point1, glm::vec3 point2);
    void updateLastPointsLineP2P(std::pair<glm::vec3, glm::vec3> points);
    void updateFirstPointLineP2P(size_t id, glm::vec3 point);
    void updateLastFirstPointsLineP2P(glm::vec3 point);
    void updateSecondPointLineP2P(size_t id, glm::vec3 point);
    void updateLastSecondPointsLineP2P(glm::vec3 point);
    void updateAllPrimitivesP2P(std::vector<glm::vec3> point1Vector, std::vector<glm::vec3> point2Vector, std::vector<glm::vec4> colorVector, std::vector<bool> viewVector);
    void updateAllPrimitivesP2P(std::vector<std::pair<glm::vec3, glm::vec3> > points, std::vector<glm::vec4> colorVector, std::vector<bool> viewVector);

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);

    void setColorForAll(glm::vec4 color);
    void setColorForEach(size_t id, glm::vec4 color);
    void setColorForLast(glm::vec4 color);

    void setInitialScaleForAll(glm::vec3 vS);
    void setInitialScaleForEach(size_t id, glm::vec3 vS);
    void setInitialScaleForLast(glm::vec3 vS);

    void setInitialPositionForAll(glm::vec3 vT);
    void setInitialPositionForEach(size_t id, glm::vec3 vT);
    void setInitialPositionForLast(glm::vec3 vT);

    void setInitialRotateForAll(glm::vec3 vR);
    void setInitialRotateForEach(size_t id, glm::vec3 vR);
    void setInitialRotateForLast(glm::vec3 vR);

    void setScaleForAll(glm::vec3 vS);
    void setScaleForEach(size_t id, glm::vec3 vS);
    void setScaleForLast(glm::vec3 vS);

    void setTranslateForAll(glm::vec3 vT);
    void setTranslateForEach(size_t id, glm::vec3 vT);
    void setTranslateForLast(glm::vec3 vT);

    void setRotateForAll(glm::vec3 vR);
    void setRotateForEach(size_t id, glm::vec3 vR);
    void setRotateForLast(glm::vec3 vR);

    void updatingBuffersPrimitive();
    void initializePrimitive();
    void drawPrimitive();
    void primitiveRender();

    void setEnableViewForAll(bool enable);
    void setEnableViewForEach(size_t id, bool enable);
    void setEnableViewForLast(bool enable);


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);

    bool bindingRender();
    void releasingRender();

    bool setShaderProgram(vmtkManagementShader *mShaderProgram);
    vmtkManagementShader *getShaderProgram() const;

    void setIDTextureActivated(GLuint lta);


    GLuint getLastTextureActivated();
    void setDepthTexture(GLuint idTexture);
    void enableDepthTexture(bool state);
    void setDepthBackTexture(GLuint idTexture);
    void enableDepthTextureBack(bool state);

    void setPointSize(GLfloat pointSize);

    void setStatePointSize(bool statePointSize);


    void setProjectionMatrixGlobal(const glm::mat4 &projectionMatrixGlobal);

    void setModelViewMatrixGlobal(const glm::mat4 &modelViewMatrixGlobal);

    void removeAllPrimitives();

    void removePrimitive(size_t id);

    void removeLastPrimitive();





    void updateMakeLineP2P(size_t id, glm::vec3 point1, glm::vec3 point2);
    void setEnableFixedAlpha(bool enableFixedAlpha);

protected:

    void createAndInitializeMatrices();
    void initializeNewMatrix();


    void clearMatrices();
    void clearSettings();
    void removeMatriz(size_t id);
    void removeSetting(size_t id);
    void removeLastMatriz();
    void removeLastSetting();

    void createAndInitializeSettings(glm::vec4 color, bool view);

    void initializeNewSetting(glm::vec4 color, bool view);

    void makeIndices(GLushort size);
    int sumSizesindices(size_t id);
    void clearData();
private:

    static glm::vec4 m_defaultColor;
    void initPrimitive();
    void createBuffers();

    vmtkManagementShader *m_shaderProgram; /** pointer to vmtkManagementShader class */
    GLuint  m_depthTexture, m_depthTextureBack /*, m_texture3D*/;    /** Texture name of the image 2D. */

    std::vector<PrimitiveData> m_primitiveData;
    std::vector< std::pair<glm::vec3,glm::vec3 > > m_points;
    std::vector<GLushort> m_indices;
    std::vector<int> m_sizeSubindices;

    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    GLushort m_sizeData;
    GLushort m_sizeIndices;
    size_t m_numTotalObjects;
    bool m_dataLineP2PCreated;
    bool initPrimitiveCreated;
    bool m_stateAddedShader;
    bool m_stateShader;

    bool m_statePointSize;
    bool m_stateDepthTexture, m_stateDepthTextureBack;
    glm::vec3 m_depthScaleFactor;

    GLfloat m_pointSize;

    std::vector<glm::vec4> m_colorVector;
    std::vector<bool> m_enableViewVector;

    std::vector<glm::mat4> m_initialScaleMatrixPrimitive;      /** Initial matrix scale for geometry */
    std::vector<glm::mat4> m_initialPositionMatrixPrimitive;   /** Initial matrix position for geometry */
    std::vector<glm::mat4> m_initialRotateMatrixPrimitive;     /** Initial matrix rotation for geometry */
    std::vector<glm::mat4> m_scaleMatrixPrimitive;             /** Matrix scale for geometry */
    std::vector<glm::mat4> m_translateMatrixPrimitive;         /** Matrix translation for geometry  */
    std::vector<glm::mat4> m_rotateMatrixPrimitive;            /** Matrix rotation for geometry  */

    glm::mat4 m_projectionMatrixGlobal;                       /** projection matrix for global scene  */
    glm::mat4 m_modelViewMatrixGlobal;                        /** model view matrix for global scene  */

    GLuint m_getIDTextureActivated;
    GLuint m_lastIDTextureActivated;
    bool m_fixedAlpha;
};




#endif // LineP2P_H
