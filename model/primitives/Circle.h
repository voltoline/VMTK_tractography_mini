#ifndef CIRCLE_H
#define CIRCLE_H

#include "renderer/vmtkManagementShader.h"
#include "utilities/QManagementResources.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "utilities/functions/vmtkGeneralFunctions.h"
#include "utilities/QLoadImage.h"


namespace TypeDrawCircleObject {
/**
 * @brief The type of the geometry enum
 */
typedef enum TypeDrawCircle
{
    polygon_geo    = 0,     /**< type polygon (use triangles) */
    line_geo       = 1,     /**< type line (use line)) */
    lines_geo      = 2,     /**< type lines (use lines)) */
    polygon_geo_fan =3,     /**< type polygon: A kind polygon made with triangles with a common vertex  (use triangles) */
    polygon_geo_strip =4,     /**< type polygon: A kind polygon made with triangles strip  (use triangles) */
    polygon_geo_fan_strip =5,     /**< type polygon: A kind polygon made with triangles fan and strip (use triangles) */
    polygon_geo_fan_strip_fan =6     /**< type polygon: A kind polygon made with triangles fan, strip and fan (use triangles) */

}TypeDrawCircle;
}
using namespace TypeDrawCircleObject;



class Circle
{

public:
    Circle();
    ~Circle();


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


    /**
     * @struct PrimitiveData
     */

    typedef struct
    {
        glm::vec3 verCoord; /** < position coordinates */
        glm::vec2 texCoord; /** < texture coordinates */
        glm::vec3 normal; /** < normals */
    }PrimitiveData;

    void defaultResourceFile();
    void makeCircle(GLushort resolution);

    void addCirclesDefaultForAll(GLushort numCircles, GLfloat scaleRadius, GLushort resolution, glm::vec4 color);
    void addFirstCircleSettingResolution(GLfloat scaleRadius, GLushort resolution, glm::vec4 color);
    void addCircle(GLfloat scaleRadius, glm::vec4 color);

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);






    void setColorForAll(glm::vec4 color);
    void setColorForEach(int id, glm::vec4 color);
    void setColorForLast(glm::vec4 color);
    void setEnableViewForAll(bool enable);
    void setScaleRadiusForAll(GLfloat scaleRadius);
    void setScaleRadiusForEach(int id, GLfloat scaleRadius);
    void setScaleRadiusForLast(GLfloat scaleRadius);

    void setInitialScaleForAll(glm::vec3 vS);
    void setInitialScaleForEach(int id, glm::vec3 vS);
    void setInitialScaleForLast(glm::vec3 vS);
    void setInitialPositionForAll(glm::vec3 vT);
    void setInitialPositionForEach(int id, glm::vec3 vT);
    void setInitialPositionForLast(glm::vec3 vT);
    void setInitialRotateForAll(glm::vec3 vR);
    void setInitialRotateForEach(int id, glm::vec3 vR);
    void setInitialRotateForLast(glm::vec3 vR);
    void setScaleForAll(glm::vec3 vS);
    void setScaleForEach(int id, glm::vec3 vS);
    void setTranslateForAll(glm::vec3 vT);
    void setTranslateForEach(int id, glm::vec3 vT);
    void setRotateForAll(glm::vec3 vR);
    void setRotateForEach(int id, glm::vec3 vR);


    void updatingBuffersPrimitive();
    void initializePrimitive(TypeFilled mode);
    void loadImageTexture(const char *image_path);
    void deleteImageTexture();
    void enableImageTexture(bool state);
    void drawPrimitive(GLint idActivateTexture);

    void primitiveRender();


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);


    bool bindingRender();
    void releasingRender();

    bool setShaderProgram(vmtkManagementShader *mShaderProgram);
    vmtkManagementShader *getManagementShader() const;

    void setIDTextureActivated(GLint lta);


    GLint getLastTextureActivated();
    void setDepthTexture(GLuint idTexture);
    void enableDepthTexture(bool state);
    void setDepthTextureBack(GLuint idTexture);
    void enableDepthTextureBack(bool state);

    void setPointSize(GLfloat pointSize);

    void setStatePointSize(bool statePointSize);

    void setEnableFixedAlpha(bool state);

    void updatingInstance();

    int getNumTotalObjects() const;

    void removeAllCircles();

    void removeCircle(int id);

    void removeLastCircle();





    void setEnableViewForEach(int id, bool enable);
    void setEnableViewForLast(bool enable);
    QList<bool> getEnableViewList();
protected:
    void createLoadImagesToTexture(int numImages);

    void createAndInitializeSettings(glm::vec4 color, GLfloat scaleRadius);
    void createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleRadius);

    void createAndInitializeMatrices();
    void createAndInitializeNewMatrix();

    void clearMatrices();
    void clearSettings();
    void removeMatriz(int id);
    void removeSetting(int id);
    void removeLastMatriz();
    void removeLastSetting();
private:

    void initPrimitive();
    void createBuffers();

    vmtkManagementShader *m_shaderProgram; /** pointer to vmtkManagementShader class */
    QList<QLoadImage> m_qImageToTexture;
    GLuint  m_depthTexture, m_depthTextureBack , m_texture3D;    /** Texture name of the image 2D. */


    std::vector<PrimitiveData> m_gData;
    std::vector<GLushort> m_indices;
    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    ushort m_sizeData;
    ushort m_sizeIndices;
    int m_numTotalObjects;
    bool m_dataCircleCreated;
    bool m_stateAddedShader;
    bool m_stateShader;

    bool m_statePointSize;
    bool m_stateTextureImage, m_stateDepthTexture, m_stateDepthTextureBack;
    bool m_fixedAlpha;
    glm::vec3 m_depthScaleFactor;
    GLfloat m_pointSize;

    GLint m_enableViewSamplerLocationID;
    GLint m_colorSamplerLocationID;
    GLint m_modelVewMatrixSamplerLocationID;
    QVector<bool> vectorEnableViewTexture;
    std::vector<glm::vec4> vectorColorTexture;
    std::vector<glm::mat4> vectorModelViewMatrixTexture;
    GLuint modelViewMatrixBuffer, colorBuffer, enableViewBuffer; /**< has been created and sized to store one vmath::Matrix4f per-instance. */
    GLuint modelVewMatrixTextureBufferObject, colorTextureBufferObject, enableViewBufferObject;

    QList<glm::vec4> m_color;
    QList<glm::mat4> m_scaleRadiusList;
    QList<bool> m_enableViewList;
    QList<glm::mat4> m_initialPositionMatrixPrimitive;   /** Initial matrix position for geometry */
    QList<glm::mat4> m_initialScaleMatrixPrimitive;      /** Initial matrix scale for geometry */
    QList<glm::mat4> m_initialRotateMatrixPrimitive;     /** Initial matrix rotation for geometry */
    QList<glm::mat4> m_scaleMatrixPrimitive;             /** Matrix scale for geometry */
    QList<glm::mat4> m_translateMatrixPrimitive;         /** Matrix translation for geometry  */
    QList<glm::mat4> m_rotateMatrixPrimitive;            /** Matrix rotation for geometry  */
    glm::mat4 m_modelViewMatrixPrimitive;                /** local model view matrix (for all scene)  */
    glm::mat4 m_projectionMatrix;                       /** Global model view matrix (for all scene)  */
    glm::mat4 m_modelViewMatrix;                        /** Global model view matrix (for all scene)  */
    GLint m_getIDTextureActivated;
    GLint m_lastIDTextureActivated;
    TypeFilled m_modeDraw;




};

#endif // CIRCLE_H
