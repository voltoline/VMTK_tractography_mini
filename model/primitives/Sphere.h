#ifndef SPHERE_H
#define SPHERE_H

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


namespace TypeDrawSphereObject {
/**
 * @brief The type of the Primitive enum
 */
typedef enum TypeDrawSphere
{
    polygon_geo    = 0,     /**< type polygon (use triangles) */
    line_geo       = 1,     /**< type line (use line)) */
    lines_geo      = 2,     /**< type lines (use lines)) */
    polygon_geo_fan =3,     /**< type polygon: A kind polygon made with triangles with a common vertex  (use triangles) */
    polygon_geo_strip =4,     /**< type polygon: A kind polygon made with triangles strip  (use triangles) */
    polygon_geo_fan_strip =5,     /**< type polygon: A kind polygon made with triangles fan and strip (use triangles) */
    polygon_geo_fan_strip_fan =6     /**< type polygon: A kind polygon made with triangles fan, strip and fan (use triangles) */

}TypeDrawSphere;
}
using namespace TypeDrawSphereObject;



class Sphere
{

public:
    Sphere();
    ~Sphere();


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
    void makeSphere(GLushort verticalResolution, GLushort horizontalResolution);

    void addSpheresDefaultForAll(GLushort numSpheres, GLfloat scaleRadius, GLushort verticalResolution, GLushort horizontalResolution, glm::vec4 color = glm::vec4(0.f,0.f,1.f,0.5f));
    void addFirstSphereCustomized(GLfloat scaleRadius, GLushort verticalResolution, GLushort horizontalResolution, glm::vec4 color = glm::vec4(0.f,0.f,1.f,0.5f));
    void addSphereCustomized(GLfloat scaleRadius, glm::vec4 color = glm::vec4(0.f,0.f,1.f,0.5f));

    bool addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile);

    void setProjectionMatrixGlobal( glm::mat4 projectionMatrixGlobal);

    void setModelViewMatrixGlobal( glm::mat4 modelViewMatrixGlobal);

    void setColorForAll(glm::vec4 color);
    void setColorForEach(unsigned short id, glm::vec4 color);
    void setColorForLast(glm::vec4 color);
    void setEnableViewForAll(bool enable);
    void setScaleRadiusForAll(GLfloat scaleRadius);
    void setScaleRadiusForEach(unsigned short id, GLfloat scaleRadius);
    void setScaleRadiusForLast(GLfloat scaleRadius);

    void setInitialScaleForAll(glm::vec3 vS);
    void setInitialScaleForEach(unsigned short id, glm::vec3 vS);
    void setInitialScaleForLast(glm::vec3 vS);
    void setInitialPositionForAll(glm::vec3 vT);
    void setInitialPositionForEach(int id, glm::vec3 vT);
    void setInitialPositionForLast(glm::vec3 vT);
    void setInitialRotateForAll(glm::vec3 vR);
    void setInitialRotateForEach(unsigned short id, glm::vec3 vR);
    void setInitialRotateForLast(glm::vec3 vR);
    void setScaleForAll(glm::vec3 vS);
    void setScaleForEach(unsigned short id, glm::vec3 vS);

    void setTranslateForAll(glm::vec3 vT);

    void setTranslateForAll(glm::mat4 mT);

    void setTranslateForEach(unsigned short id, glm::vec3 vT);

    void setTranslateForEach(unsigned short id, glm::mat4 mT);

    void setRotateForAll(glm::vec3 vR);

    void setRotateForAll(glm::mat4 mR);

    void setRotateForEach(unsigned short id, glm::vec3 vR);

    void setRotateForEach(unsigned short id, glm::mat4 mR);



    void setTranslateForLast(glm::vec3 vT);

    void setTranslateForLast(glm::mat4 mT);

    void setInitialRotateForLast(glm::mat4 mR);


    void updatingBuffersPrimitive();
    void initializePrimitive();
    void loadImageTexture(const char *image_path);

    void deleteImageTexture();
    void enableImageTexture(bool state);
    void drawPrimitive(TypeFilled mode, GLushort idActivateTexture);

    void primitiveRender( TypeFilled mode = solid );


    void setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix);


    bool bindingRender();
    void releasingRender();

    bool setShaderProgram(vmtkManagementShader *mShaderProgram);
    vmtkManagementShader *getManagementShader() const;

    void setIDTextureActivated(unsigned short idta);


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

    void removeAllSpheres();

    void removeSphere(unsigned short id);

    void removeLastSphere();

    void setEnableViewForEach(unsigned short id, bool enable);

    void setEnableViewForLast(bool enable);

    std::vector<bool> getEnableViewVector();



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

    std::vector<QLoadImage> m_qImageToTexture;
    GLuint  m_depthTexture, m_depthTextureBack , m_texture3D;    /** Texture name of the image 2D. */


    std::vector<PrimitiveData> m_gData;
    std::vector<GLushort> m_indices;

    GLuint vaos,            /**< render: vertex array object */
    vbo,                    /**< render: data buffer */
    ebo;                    /**< render: element array buffer */
    ushort m_sizeData;
    ushort m_sizeIndices;
    unsigned short m_numTotalObjects;
    bool m_dataSphereCreated;
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
    std::vector<int> vectorEnableViewTexture;
    std::vector<glm::vec4> vectorColorTexture;
    std::vector<glm::mat4> vectorModelViewMatrixTexture;
    GLuint modelViewMatrixBuffer, colorBuffer, enableViewBuffer; /**< has been created and sized to store one vmath::Matrix4f per-instance. */
    GLuint modelVewMatrixTextureBufferObject, colorTextureBufferObject, enableViewBufferObject;

    std::vector<glm::vec4> m_colorVector;
    std::vector<glm::mat4> m_scaleRadiusVector;
    std::vector<bool> m_enableViewVector;
    std::vector<glm::mat4> m_initialPositionMatrixPrimitive;   /** Initial matrix position for Primitive */
    std::vector<glm::mat4> m_initialScaleMatrixPrimitive;      /** Initial matrix scale for Primitive */
    std::vector<glm::mat4> m_initialRotateMatrixPrimitive;     /** Initial matrix rotation for Primitive */
    std::vector<glm::mat4> m_scaleMatrixPrimitive;             /** Matrix scale for Primitive */
    std::vector<glm::mat4> m_translateMatrixPrimitive;         /** Matrix translation for Primitive  */
    std::vector<glm::mat4> m_rotateMatrixPrimitive;            /** Matrix rotation for Primitive  */
    glm::mat4 m_modelViewMatrixPrimitive;                /** local model view matrix (for all scene)  */
    glm::mat4 m_projectionMatrixGlobal;                       /** Global model view matrix (for all scene)  */
    glm::mat4 m_modelViewMatrixGlobal;                        /** Global model view matrix (for all scene)  */
    GLushort m_getIDTextureActivated;
    GLushort m_lastIDTextureActivated;


};

#endif // SPHERE_H
