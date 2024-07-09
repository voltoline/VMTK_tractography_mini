#ifndef MOUSE_HANDLER_3D_VIEW_H
#define MOUSE_HANDLER_3D_VIEW_H

//#include <glm/glm.hpp>
//#include "modules/visualization/submodules/3d_transformation/Model3DTransformation.h"

//class MouseEvent;

//template <class renderer_type>
//class MouseHandler3DView: public ModelObserver<Model3DTransformation, Model3DTransformationEvent>
//{
//public:
//    MouseHandler3DView();

//    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type);

//    void mousePressEvent(MouseEvent &mouse_event);
//    void mouseMoveEvent(MouseEvent &mouse_event);

//    glm::mat4 getRotationMatrix() const;
//    glm::mat4 getScalingMatrix() const;
//    glm::mat4 getTranslationMatrix() const;

//    bool rotationMatrixChanged() const;
//    bool scalingMatrixChanged() const;
//    bool translationMatrixChanged() const;

////    void setMouseMode(short mode);
//    void setRenderer(renderer_type *renderer);

//    void setModel3DTransformation(Model3DTransformation *model3DTransformation);

//private:
//    enum class MouseMode
//    {
//        ROTATION = -2,
//        SCALING = -3,
//        TRANSLATION = -4
//    };

//    void handleMouseMovement();
//    void createRotationMatrix();
//    void createScalingMatrix();
//    void createTranslationMatrix();

//    renderer_type *mRenderer;
//    MouseMode mMouseMode;

//    glm::vec2 mMousePressPosition; /**< mouse position vector */
//    glm::vec2 mMouseCurrentPosition;

//    glm::mat4 mRotationMatrix;
//    glm::mat4 mScalingMatrix;
//    glm::mat4 mTranslationMatrix;

//    bool mFlagRotationMatrixChanged;
//    bool mFlagScalingMatrixChanged;
//    bool mFlagTranslationMatrixChanged;

//    Model3DTransformation *m_model3DTransformation;
//};

#include "MouseHandler3DView.tpp"

#endif // MOUSE_HANDLER_3D_VIEW_H
