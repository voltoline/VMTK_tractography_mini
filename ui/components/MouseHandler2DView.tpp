//#include "MouseHandler2DView.h"
//#include "RenderingSurfaceSet.h"

//#include <glm/gtx/quaternion.hpp>
//#include "ui/vmtkControl.h"
//#include "ui/RenderingSurface.h"

//template <class renderer_typer>
//MouseHandler2DView<renderer_typer>::MouseHandler2DView()
//{
//    mRotationMatrix = glm::mat4(1.0f);
//    mScalingMatrix = glm::mat4(1.0f);
//    mTranslationMatrix = glm::mat4(1.0f);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;

//    mMouseState = false;
//    mMouseStateROI = false;
//    mMouseStateFreeDrawROI = false;
//    m_isMultiplanarEnabled = false;

//    mMouseMode = MouseMode::ROTATION;
//}

//template<class renderer_typer>
//void MouseHandler2DView<renderer_typer>::update(Model2D3DGUIState *, Model2D3DGUIStateEvent event_type)
//{
//    if(event_type == Model2D3DGUIStateEvent::MOUSE_MODE_2D_CHANGED)
//    {
//        if(m_model2D3DGUIState->getMouseMode2D() == Model2D3DGUIState::MouseMode2D::MULTIPLANAR)
//        {
//            m_isMultiplanarEnabled = true;
//            mRenderer->setStatusDrawLine(m_isMultiplanarEnabled);

//            if(m_model2D3DGUIState->getMouseModeMultiPlanar() == Model2D3DGUIState::MouseModeMultiplanar::MOVE)
//            {
//                mMouseMode = MouseMode::TRANSLATION_MPR;
//                vmtkControl::m_stateMPRrotation = false;

//            } else {

//                mMouseMode = MouseMode::ROTATION_MPR;
//                vmtkControl::m_stateMPRrotation = true;
//            }
//        }

//        if(m_model2D3DGUIState->getMouseMode2D() == Model2D3DGUIState::MouseMode2D::TRANSFORMATION)
//        {
//            m_isMultiplanarEnabled = false;
//            mMouseState = false;
//            mRenderer->setStatusDrawLine(m_isMultiplanarEnabled);
//            mRenderer->setEnableCursor(mMouseState);
//        }

//        if(m_model2D3DGUIState->getMouseMode2D() == Model2D3DGUIState::MouseMode2D::CURSOR)
//        {
//            mMouseState = true;
//            mRenderer->setEnableCursor(mMouseState);
//        }

//        return;
//    }

//    if(event_type == Model2D3DGUIStateEvent::MOUSE_MODE_MULTIPLANAR_CHANGED)
//    {
//        if(m_model2D3DGUIState->getMouseModeMultiPlanar() == Model2D3DGUIState::MouseModeMultiplanar::MOVE)
//        {
//            mMouseMode = MouseMode::TRANSLATION_MPR;
//            vmtkControl::m_stateMPRrotation = false;

//        } else {

//            mMouseMode = MouseMode::ROTATION_MPR;
//            vmtkControl::m_stateMPRrotation = true;
//        }
//    }

//    mRenderingSurfaceSet->update2DRendering();
//    mRenderingSurfaceSet->update3DRendering();
//}

//template<class renderer_typer>
//void MouseHandler2DView<renderer_typer>::update(Model2DTransformation *, Model2DTransformationEvent event_type)
//{
//    if(event_type == Model2DTransformationEvent::MOUSE_MODE_CHANGED)
//    {
//        switch(m_model2DTransformation->getMouseMode())
//        {
//        case Model2DTransformation::MouseMode::ROTATION:
//            mMouseMode = MouseMode::ROTATION;
//            break;

//        case Model2DTransformation::MouseMode::ZOOM:
//            mMouseMode = MouseMode::SCALING;
//            break;

//        case Model2DTransformation::MouseMode::MOVE:
//            mMouseMode = MouseMode::TRANSLATION;
//            break;
//        }
//    }
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setMouseMode(short mode)
//{
//    mMouseMode = static_cast<MouseMode>(mode);
//    // Toggle the MPR variables. It should be moved to Viewer Tools! (Ting, 14/6/2015)

//    if (mMouseMode == MouseMode::TRANSLATION_MPR)
//    {

////        vmtkControl::m_stateMPRrotation = true;

//        vmtkControl::m_stateMPRrotation = false;

//    } else if (mMouseMode == MouseMode::ROTATION_MPR) {

////        if (vmtkControl::m_stateMPRrotation)
////            vmtkControl::m_stateMPRrotation = false;

//        vmtkControl::m_stateMPRrotation = true;
//    }
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::mousePressEvent(MouseEvent &mouse_event)
//{
////    int height = mRenderer->getH();
////    mMousePressPosition = vgf::toCoordVert( vgf::getNormalizedPositionFromMouseCoordinate(mouse_event.getPosX(), mouse_event.getPosY(), height) );
////    mMousePressPosition2D = vgf::toCoordVert(vgf::getNormalizedPositionFromMouseCoordinate(mouse_event.mPositionX, mouse_event.mPositionY, height));

//    mMousePressPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

//    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT && (mMouseState == true || m_isMultiplanarEnabled == true ||  mMouseStateROI == true || mMouseStateFreeDrawROI == true))
//    {
//        mRenderer->setMouseInitPosition(mouse_event.getPosX(), mouse_event.getPosY());
////        vmtkControl::mousePressed = true;
//        return;
//    }

//    mRotationMatrix = glm::mat4(1.0f);
//    mScalingMatrix = glm::mat4(1.0f);
//    mTranslationMatrix = glm::mat4(1.0f);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::mouseMoveEvent(MouseEvent &mouse_event)
//{
////    int height = mRenderer->getH();
////    mMouseCurrentPosition = vgf::toCoordVert( vgf::getNormalizedPositionFromMouseCoordinate(mouse_event.getPosX(), mouse_event.getPosY(), height) );
////    mMouseCurrentPosition = glm::vec2(mouse_event.mPositionX, mouse_event.mPositionY);

//    mMouseCurrentPosition = glm::vec2(mouse_event.getPosX(), mouse_event.getPosY());

//    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT && (mMouseState==true || m_isMultiplanarEnabled==true ||  mMouseStateROI == true || mMouseStateFreeDrawROI == true))
//    {
//        // Action is taken only when the left button is pressed
//        mRenderer->setMousePosition(mouse_event.getPosX(), mouse_event.getPosY());

//        return;
//    }

//    mRotationMatrix = glm::mat4(1.0f);
//    mScalingMatrix = glm::mat4(1.0f);
//    mTranslationMatrix = glm::mat4(1.0f);

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;

//    handleMouseMovement();
//    mMousePressPosition = mMouseCurrentPosition;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::mouseDoubleClickEvent(MouseEvent &mouse_event)
//{
//    std::cout << "Double Click Captured at point X:" << mouse_event.getPosX() << " Y: " << mouse_event.getPosY() << std::endl;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::handleMouseMovement()
//{
//    switch (mMouseMode)
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

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::createRotationMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    glm::vec3 n = glm::vec3(0.0f, 0.0f, diff[0]);
//    n = glm::normalize(n);
//    glm::vec3 rotationAxis;
//    rotationAxis += n;
//    rotationAxis = glm::normalize(rotationAxis);
//    glm::quat tempQuaternionRotation = glm::angleAxis(glm::radians(3.0f), rotationAxis);
//    if(std::isnan(tempQuaternionRotation.w) || std::isnan(tempQuaternionRotation.x) || std::isnan(tempQuaternionRotation.y) || std::isnan(tempQuaternionRotation.z) )
//        tempQuaternionRotation = glm::quat(1,0,0,0);

//    mRotationMatrix = glm::toMat4(tempQuaternionRotation) * mRenderer->getRotationMatrix();
//    mRenderer->setRotationMatrix(mRotationMatrix);
////    mMousePressPosition = mMouseCurrentPosition;

//    mFlagRotationMatrixChanged = true;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::createScalingMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    float zoom_factor = 1.0f + diff.y / 500.f;
//    if(zoom_factor < 0.f)
//        return;

//    glm::mat4 tempScalationMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoom_factor));
//    mScalingMatrix = tempScalationMatrix * mRenderer->getScalingMatrix();
//    mRenderer->setScalingMatrix(mScalingMatrix);
////    mMousePressPosition = mMouseCurrentPosition; //Update old mouse pos

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = true;
//    mFlagTranslationMatrixChanged = false;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::createTranslationMatrix()
//{
//    glm::vec2 diff = mMouseCurrentPosition - mMousePressPosition;
//    glm::mat4 tempTranslationMatrix = glm::translate(glm::mat4(1.0),  glm::vec3(diff.x / 100.f, -diff.y / 100.f, 0.0f));
//    mTranslationMatrix = tempTranslationMatrix * mRenderer->getTranslationMatrix();
//    mRenderer->setTranslationMatrix(mTranslationMatrix);
////    mMousePressPosition = mMouseCurrentPosition;

//    mFlagRotationMatrixChanged = false;
//    mFlagScalingMatrixChanged = false;
//    mFlagTranslationMatrixChanged = true;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::MOUSE_MODE_CHANGED);
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
//{
//    mRenderingSurfaceSet = renderingSurfaceSet;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState)
//{
//    m_model2D3DGUIState = model2D3DGUIState;
//    m_model2D3DGUIState->subscribe(this, Model2D3DGUIStateEvent::MOUSE_MODE_2D_CHANGED);
//    m_model2D3DGUIState->subscribe(this, Model2D3DGUIStateEvent::MOUSE_MODE_MULTIPLANAR_CHANGED);
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setRenderer(renderer_typer *renderer)
//{
//    mRenderer = renderer;

//    mRenderer->setEnableCursor(mMouseState);
//    mRenderer->setStatusDrawLine(m_isMultiplanarEnabled);
//}

//template <class renderer_typer>
//glm::mat4 MouseHandler2DView<renderer_typer>::getRotationMatrix() const
//{
//    return mRotationMatrix;
//}

//template <class renderer_typer>
//glm::mat4 MouseHandler2DView<renderer_typer>::getScalingMatrix() const
//{
//    return mScalingMatrix;
//}

//template <class renderer_typer>
//glm::mat4 MouseHandler2DView<renderer_typer>::getTranslationMatrix() const
//{
//    return mTranslationMatrix;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::rotationMatrixChanged() const
//{
//    return mFlagRotationMatrixChanged;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::scalingMatrixChanged() const
//{
//    return mFlagScalingMatrixChanged;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::translationMatrixChanged() const
//{
//    return mFlagTranslationMatrixChanged;
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setStatusDrawLine(bool state)
//{
//    m_isMultiplanarEnabled = state;
//    mRenderer->setStatusDrawLine(m_isMultiplanarEnabled);
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setMouseState(bool state)
//{
//    mMouseState = state;
//    mRenderer->setEnableCursor(mMouseState);
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setMouseStateROI(bool state)
//{
//    mMouseStateROI = state;
//    mRenderer->setStroke2D(mMouseStateROI);
//}

//template <class renderer_typer>
//void MouseHandler2DView<renderer_typer>::setMouseStateFreeDrawROI(bool state)
//{
//    mMouseStateFreeDrawROI = state;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::isMultiPlanarReformatingEnabled() const
//{
//    return m_isMultiplanarEnabled;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::isCursor2DEnabled() const
//{
//    return mMouseState;
//}

//template <class renderer_typer>
//bool MouseHandler2DView<renderer_typer>::isCursor2DROIEnabled() const
//{
//    return mMouseStateROI;
//}
