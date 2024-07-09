#include "MouseHandler3DView.h"

//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "ui/RenderingSurface.h"

//template <class renderer_type>
//MouseHandler3DView<renderer_type>::MouseHandler3DView()
//{
//    mRotationMatrix = glm::mat4(1.0);
//    mTranslationMatrix = glm::mat4(1.0);
//    mScalingMatrix = glm::mat4(1.0);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;

//    mMouseMode = MouseMode::ROTATION;
//}

//template<class renderer_type>
//void MouseHandler3DView<renderer_type>::update(Model3DTransformation *, Model3DTransformationEvent event_type)
//{
//    if(event_type == Model3DTransformationEvent::MOUSE_MODE_CHANGED)
//    {
//        if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::MOVE)
//            mMouseMode = MouseMode::TRANSLATION;

//        if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ROTATION)
//            mMouseMode = MouseMode::ROTATION;

//        if(m_model3DTransformation->getMouseMode() == Model3DTransformation::MouseMode::ZOOM)
//            mMouseMode = MouseMode::SCALING;
//    }
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::mousePressEvent(MouseEvent &mouse_event)
//{
//    mMousePressPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

//    mRotationMatrix = glm::mat4(1.0f);
//    mScalingMatrix = glm::mat4(1.0f);
//    mTranslationMatrix = glm::mat4(1.0f);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::mouseMoveEvent(MouseEvent &mouse_event)
//{
//    mMouseCurrentPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

//    mRotationMatrix = glm::mat4(1.0f);
//    mScalingMatrix = glm::mat4(1.0f);
//    mTranslationMatrix = glm::mat4(1.0f);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;

//    handleMouseMovement();
//    mMousePressPosition = mMouseCurrentPosition;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::handleMouseMovement()
//{
//    switch(mMouseMode)
//    {
//    case MouseMode::ROTATION:
//        createRotationMatrix();
//        break;

//    case MouseMode::SCALING:
//        createScalingMatrix();
//        break;

//    case MouseMode::TRANSLATION:
//        createTranslationMatrix();
//        break;

//    default:
//        break;
//    }
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::createRotationMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    glm::vec3 n = glm::vec3(diff[1], diff[0], 0.0f);
//    n = glm::normalize(n);
//    glm::vec3 rotationAxis;
//    rotationAxis += n;
//    rotationAxis = glm::normalize(rotationAxis);
//    glm::quat mQuaternionRotation = glm::angleAxis(glm::radians(3.0f), rotationAxis);
//    if(std::isnan(mQuaternionRotation.w) || std::isnan(mQuaternionRotation.x) || std::isnan(mQuaternionRotation.y) || std::isnan(mQuaternionRotation.z) )
//        mQuaternionRotation = glm::quat(1,0,0,0);

//    mRotationMatrix = glm::toMat4(mQuaternionRotation) * mRenderer->getRotationMatrix();
//    mRenderer->setRotationMatrix(mRotationMatrix);
////    mMousePressPosition = mMouseCurrentPosition;

//    mFlagRotationMatrixChanged = true;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::createScalingMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    float zoom_factor = (float) (1.0f + diff.y / 500.f);
//    if(zoom_factor < 0.f)
//        return;

//    glm::mat4 temp_scalationMatrix = glm::scale(glm::mat4(1.0), glm::vec3(float(zoom_factor)));
//    mScalingMatrix = temp_scalationMatrix * mRenderer->getScalingMatrix();
//    mRenderer->setScalingMatrix(mScalingMatrix);
////    mMousePressPosition = mMouseCurrentPosition;

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = true;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::createTranslationMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    glm::mat4 temp_translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(diff.x / 100, -diff.y / 100, 0.0f));
//    mTranslationMatrix = temp_translationMatrix * mRenderer->getTranslationMatrix();
//    mRenderer->setTranslationMatrix(mTranslationMatrix);
////    mMousePressPosition = mMouseCurrentPosition;

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = true;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::setModel3DTransformation(Model3DTransformation *model3DTransformation)
//{
//    m_model3DTransformation = model3DTransformation;
//    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::MOUSE_MODE_CHANGED);
//}

//template <class renderer_type>
//bool MouseHandler3DView<renderer_type>::rotationMatrixChanged() const
//{
//    return mFlagRotationMatrixChanged;
//}

//template <class renderer_type>
//bool MouseHandler3DView<renderer_type>::scalingMatrixChanged() const
//{
//    return mFlagScalingMatrixChanged;
//}

//template <class renderer_type>
//bool MouseHandler3DView<renderer_type>::translationMatrixChanged() const
//{
//    return mFlagTranslationMatrixChanged;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::setRenderer(renderer_type *renderer)
//{
//    mRenderer = renderer;
//}

//template <class renderer_type>
//glm::mat4 MouseHandler3DView<renderer_type>::getRotationMatrix() const
//{
//    return mRotationMatrix;
//}

//template <class renderer_type>
//glm::mat4 MouseHandler3DView<renderer_type>::getScalingMatrix() const
//{
//    return mScalingMatrix;
//}

//template <class renderer_type>
//glm::mat4 MouseHandler3DView<renderer_type>::getTranslationMatrix() const
//{
//    return mTranslationMatrix;
//}

//template <class renderer_type>
//void MouseHandler3DView<renderer_type>::setMouseMode(short mode)
//{
//    mMouseMode = static_cast<MouseMode>(mode);
//}
