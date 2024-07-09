#ifndef CYLINDER_H
#define CYLINDER_H

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


namespace TypeDrawGeometryObject {
/**
 * @brief The type of the geometry enum
 */
typedef enum TypeDrawGeometry
{
    polygon_geo    = 0,     /**< type polygon (use triangles) */
    line_geo       = 1,     /**< type line (use line)) */
    lines_geo      = 2,     /**< type lines (use lines)) */
    polygon_geo_fan =3,     /**< type polygon: A kind polygon made with triangles with a common vertex  (use triangles) */
    polygon_geo_strip =4,     /**< type polygon: A kind polygon made with triangles strip  (use triangles) */
    polygon_geo_fan_strip =5,     /**< type polygon: A kind polygon made with triangles fan and strip (use triangles) */
    polygon_geo_fan_strip_fan =6     /**< type polygon: A kind polygon made with triangles fan, strip and fan (use triangles) */

}TypeDrawGeometry;
}
using namespace TypeDrawGeometryObject;

namespace ModeTypeFilled{
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
}
using namespace ModeTypeFilled;

class Cylinder
{

public:
    Cylinder();
    ~Cylinder();
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
    void makeCylinder(GLushort resolution);

    void addCylindersDefaultForAll(GLushort numCylinders, GLfloat scaleRadius, GLfloat scaleHeight, GLushort resolution, glm::vec4 color);
    void addCylinderCustomized(GLfloat scaleRadius, GLfloat scaleHeight, GLushort resolution, glm::vec4 color);

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);






    void setColorForAll(glm::vec4 color);
    void setColorForEach(int id, glm::vec4 color);
    void setScaleRadiusForAll(GLfloat scaleRadius);
    void setScaleRadiusForEach(int id, GLfloat scaleRadius);
    void setScaleHeigthForAll(GLfloat scaleHeight);
    void setScaleHeigthForEach(int id, GLfloat scaleHeight);

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
    void loadImageTexture(const char *image_path);
    void loadImageTexture(const char *image_path1, const char *image_path2);
    void loadImageTexture(const char *image_path1, const char *image_path2, const char *image_path3);
    void deleteImageTexture();
    void enableImageTexture(bool state);
    void drawGeometry(TypeFilled mode, GLint idActivateTexture);

    void geometryRender( TypeFilled mode = ModeTypeFilled::solid );


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);


    void bindingRender();
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
protected:
    void createLoadImagesToTexture(int numImages);

    void createAndInitializeSettings(glm::vec4 color, GLfloat scaleRadius, GLfloat scaleHeight);
    void createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleRadius,GLfloat scaleHeight);

    void createAndInitializeMatrices();
    void createAndInitializeNewMatrix();

private:

    void initPrimitive();
    void createBuffers();

    vmtkManagementShader *m_shaderProgram; /** pointer to vmtkManagementShader class */
    TypeDrawGeometry m_typeDraw;
    QList<QLoadImage> m_qImageToTexture;
    GLuint  m_depthTexture, m_depthTextureBack , m_texture3D;    /** Texture name of the image 2D. */


    std::vector<PrimitiveData> m_gData;
    std::vector<GLushort> m_indices;
    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    ushort m_sizeData;
    ushort m_sizeIndices;
    int *m_vectorSizeSubindices;
    int m_numTotalObjects;
    int m_numImages;
    bool m_dataCylinderCreated;
    bool m_stateAddedShader;
    bool m_stateShader;

    bool m_statePointSize;
    bool m_stateTextureImage, m_stateDepthTexture, m_stateDepthTextureBack;
    bool m_fixedAlpha;
    glm::vec3 m_depthScaleFactor;
    GLfloat m_pointSize;

    GLint m_colorSamplerLocationID;
    GLint m_modelVewMatrixSamplerLocationID;
    std::vector<glm::mat4> vectorModelViewMatrixTexture;
    std::vector<glm::vec4> vectorColorTexture;
    GLuint modelViewMatrixBuffer, colorBuffer, radiusBuffer; /**< has been created and sized to store one vmath::Matrix4f per-instance. */
    GLuint modelVewMatrixTextureBufferObject, colorTextureBufferObject, radiusBufferObject;

    QList<glm::vec4> m_color;
    QList<glm::mat4> m_scaleRadiusForAll;
    QList<glm::mat4> m_scaleHeigthForAll;
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





};

#endif // CYLINDER_H
