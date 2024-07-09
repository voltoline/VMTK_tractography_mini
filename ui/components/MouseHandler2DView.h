#ifndef MOUSE_HANDLER_2D_VIEW_H
#define MOUSE_HANDLER_2D_VIEW_H

//#include <glm/glm.hpp>

//#include "model/Model2D3DInteraction.h"
//#include "model/Model2DTransformation.h"

//class MouseEvent;
//class RenderingSurfaceSet;

//template <class renderer_typer>
//class MouseHandler2DView:
//        public ModelObserver<Model2D3DGUIState, Model2D3DGUIStateEvent>,
//        public ModelObserver<Model2DTransformation, Model2DTransformationEvent>
//{
//public:
//    MouseHandler2DView();

//    void update(Model2D3DGUIState *subject, Model2D3DGUIStateEvent event_type);
//    void update(Model2DTransformation *subject, Model2DTransformationEvent event_type);

//    void mousePressEvent(MouseEvent &mouse_event);
//    void mouseMoveEvent(MouseEvent &mouse_event);
//    void mouseDoubleClickEvent(MouseEvent &mouse_event);

//    glm::mat4 getRotationMatrix() const;
//    glm::mat4 getScalingMatrix() const;
//    glm::mat4 getTranslationMatrix() const;

//    bool rotationMatrixChanged() const;
//    bool scalingMatrixChanged() const;
//    bool translationMatrixChanged() const;

//    void setStatusDrawLine(bool state);
//    void setMouseState(bool state);
//    void setMouseMode(short mode);

//    bool isMultiPlanarReformatingEnabled() const;
//    bool isCursor2DEnabled() const;

//    void setRenderer(renderer_typer *renderer);

//    void setMouseStateROI(bool state);
//    bool isCursor2DROIEnabled() const;

//    void setMouseStateFreeDrawROI(bool state);

//    void setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState);

//    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

//private:

//    enum class MouseMode
//    {
//        NONE = 0,
//        ROTATION = -2,
//        SCALING = -3,
//        TRANSLATION = -4,
//        TRANSLATION_MPR = -5,
//        ROTATION_MPR = -6
//    };

//    void handleMouseMovement();
//    void createRotationMatrix();
//    void createScalingMatrix();
//    void createTranslationMatrix();

//    MouseMode mMouseMode;
//    renderer_typer *mRenderer;

//    glm::vec2 mMousePressPosition;
//    glm::vec2 mMouseCurrentPosition;

//    glm::mat4 mTranslationMatrix;
//    glm::mat4 mRotationMatrix;
//    glm::mat4 mScalingMatrix;

//    bool mFlagRotationMatrixChanged;
//    bool mFlagScalingMatrixChanged;
//    bool mFlagTranslationMatrixChanged;

//    bool mMouseState;
//    bool mMouseStateROI;
//    bool mMouseStateFreeDrawROI;
//    bool m_isMultiplanarEnabled;

//    Model2D3DGUIState *m_model2D3DGUIState;
//    Model2DTransformation *m_model2DTransformation;

//    RenderingSurfaceSet *mRenderingSurfaceSet;
//};

//#include "MouseHandler2DView.tpp"

#endif // MOUSE_HANDLER_2D_VIEW_H
