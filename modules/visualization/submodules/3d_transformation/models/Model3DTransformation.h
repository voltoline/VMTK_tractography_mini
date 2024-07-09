#ifndef MODEL_3D_TRANSFORMATION_H
#define MODEL_3D_TRANSFORMATION_H

//#include "model/StateManager.h"
#include "modules/visualization/StateManager.h"
#include "model/ModelSubject.h"
#include <glm/glm.hpp>

enum class Model3DTransformationEvent
{
//    NONE,
    _3D_TRANSFORMATIONS_CHANGED,
    MOUSE_MODE_CHANGED,
    _3D_TRANSFORMATION_STATE_CHANGED
};

class Model3DTransformation: public ModelSubject<Model3DTransformation, Model3DTransformationEvent>

{
public:

    enum class MouseMode
    {
        ROTATION,
        MOVE,
        ZOOM
    };

    Model3DTransformation();
    ~Model3DTransformation();

    MouseMode getMouseMode() const;
    void setMouseMode(const MouseMode _3D_mouse_mode);

    void setScalingMatrix(glm::mat4 scaling);
    void setRotationMatrix(glm::mat4 rotation);
    void setTranslationMatrix(glm::mat4 translation);
//    Model3DTransformationEvent setProjectionMatrix(glm::mat4 projection);
    void setPreTransformationMatrix(const glm::mat4 &preTransformationMatrix);
    void setPostTransformationMatrix(const glm::mat4 &postTransformationMatrix);

    glm::mat4 getScalingMatrix() const;
    glm::mat4 getRotationMatrix() const;
    glm::mat4 getTranslationMatrix() const;   
    glm::mat4 getProjectionMatrix(bool additional_transf=false) const;
    glm::mat4 getPreTransformationMatrix() const;
    glm::mat4 getPostTransformationMatrix() const;

    glm::mat4 getMVPMatrix(bool additional_transf=false) const;
    glm::mat4 getModelViewMatrix(bool additional_transf=false) const;

    void resetTransformations();

    bool isTransformationActivated() const;
    void setTransformationActivated(bool isTransformationActivated);

    void setStateManager(StateManager *state_manager);
    void handle3DTransformationStateChanged(std::string feature_name, FeatureState state);

    bool isEnabled() const;

private:

    glm::mat4 m_preTransformationMatrix;
    glm::mat4 m_postTransformationMatrix;

    glm::mat4 mRotationMatrix;           /**< rotationMatrix transformation matrix */
    glm::mat4 mScalingMatrix;          /**< scalationMatrix transformation matrix */
    glm::mat4 mTranslationMatrix;        /**< traslationMatrix transformation matrix */
//    glm::mat4 mProjectionMatrix;       /**< projectionMatrix transformation matrix */

    MouseMode m_mouseMode;

//    TransformationData mTransformationData;
    bool m_isTransformationActivated;
    bool m_isTransformationEnabled;

    StateManager *m_stateManager;
    int m_3DTransformationStateChangedHandlerSlotID;

    float m_projLeft, m_projRight, m_projBottom, m_projTop;
};

#endif // MODEL_3D_TRANSFORMATION_H
