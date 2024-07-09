#include "ControllerMouseHandlerCursor3D.h"
#include "CursorRenderer3D.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "utilities/functions/vmtkGeneralFunctions.h"
#include "modules/visualization/StateManager.h"
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/SliceSynchronizer.h"
#else
#include "modules/visualization/submodules/2d_slice/SliceSynchronizer.h"
#endif

ControllerMouseHandlerCursor3D:: ControllerMouseHandlerCursor3D()
{
    m_mouseCursorEnable = false;

    m_renderer = nullptr;
    m_renderingSurfaceSet = nullptr;
    m_modelCursor = nullptr;
    m_sliceSynchronizer = nullptr;
    m_raycastingVolumeRenderer3D = nullptr;
}

void ControllerMouseHandlerCursor3D::handleCursorStateChangeEvent(ModelCursor *, ModelCursorEvent event_type){
    m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();
    if(event_type == ModelCursorEvent::CURSOR_STATE_CHANGED){
        if(m_modelCursor->isActivated()){
            m_mouseCursorEnable = true;
            m_renderer->setEnableCursor(m_mouseCursorEnable);
            m_renderingSurfaceSet->update3DRendering();

        } else {
            m_mouseCursorEnable = false;
            m_renderer->setEnableCursor(m_mouseCursorEnable);
            m_renderingSurfaceSet->update3DRendering();
        }
        return;
    }
}

void ControllerMouseHandlerCursor3D::handleCursorOffScreenChangeEvent(ModelCursor *modelCursor, ModelCursorEvent event_type){
    if(event_type == ModelCursorEvent::CURSOR_OFFSCREEN_CHANGED){
        QString nameOffScreen = modelCursor->getOffScreenEnablePair().first;
        bool enableOffScreen = modelCursor->getOffScreenEnablePair().second;
        m_raycastingVolumeRenderer3D->setEnableOffScreen(nameOffScreen, enableOffScreen);
    }
}

void ControllerMouseHandlerCursor3D::handleCursorDrawTypeChangeEvent(ModelCursor *, ModelCursorEvent event_type){
    m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();
    if(event_type == ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED) {
        m_renderer->setDrawCursorType(m_modelCursor->getDrawCursorType());
        m_renderingSurfaceSet->update3DRendering();
    }
}

void ControllerMouseHandlerCursor3D::handleCursorPositionChangeEvent(ModelCursor *, ModelCursorEvent event_type){
    if(event_type == ModelCursorEvent::CURSOR_POSITION_CHANGED){
        m_renderer->setCursorPosition3D(m_modelCursor->getCursorPosition3D());
        //Rendering if the cursor 3D is associated to a canvas (Raphael and Ting, 4/8/2023)
        m_renderingSurfaceSet->update3DRendering();
    }
}

void ControllerMouseHandlerCursor3D::handleCursorLockStateChangeEvent(ModelCursor *, ModelCursorEvent event_type)
{
    if(event_type == ModelCursorEvent::CURSOR_MODE_CHANGED){
        if(m_modelCursor->isActivated())
        {
            if(m_modelCursor->isLocked())
            {
                //Set flag to ignore mouse events
                m_mouseEventLock = true;
            }
            else
            {
                //Set flag to habilitate mouse events
                m_mouseEventLock = false;
            }
        }
    }
}

void ControllerMouseHandlerCursor3D::mousePressEvent(MouseEvent &mouse_event, RenderingSurface *rendering_surface){

    if(m_renderer)
        m_renderingSurfaceSet->setFocus3D();

    if(m_mouseEventLock)
        return;

    if(m_sliceSynchronizer != nullptr)
    {
        if(m_sliceSynchronizer->isSynchronizationActivated())
        {
            SliceSynchronizerEvent event = m_sliceSynchronizer->stopSyncAllSlices();
            m_sliceSynchronizer->notify(event);
        }
    }

    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCursorEnable){

        glm::ivec2 mousePos (mouse_event.getPosX(), mouse_event.getPosY() );
        glm::ivec2 screen_size(rendering_surface->getWidth(), rendering_surface->getHeight());
        glm::vec4  viewport = glm::make_vec4(m_renderer->getViewport());
        int height = screen_size.y;
        glm::ivec2 win = vgf::orientedMousePosition(mousePos, height);
        GLfloat depthZ;
        if( m_modelCursor->getOffScreenEnablePair().second ){

            QString nameOS = m_modelCursor->getOffScreenEnablePair().first;
            if( !m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCastingOffScreen(nameOS, win.x, win.y, viewport, depthZ) ){
                m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCasting(win.x, win.y, viewport, depthZ);
            }
        }else{
            m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCasting(win.x, win.y, viewport, depthZ);
        }

        ModelCursorEvent event = m_modelCursor->setCursorPositionFrom3DView(mousePos, viewport, screen_size, depthZ);
        m_renderer->setCursorPosition3D(m_modelCursor->getCursorPosition3D());
        rendering_surface->updateRendering();
        m_modelCursor->notify(event, m_cursorEventSlotID, m_modelCursor, event);


    }
}

void ControllerMouseHandlerCursor3D::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *rendering_surface)
{
    if (m_mouseEventLock)
        return;

    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCursorEnable){
        glm::ivec2 mousePos (mouse_event.getPosX(), mouse_event.getPosY() );
        glm::ivec2 screen_size(rendering_surface->getWidth(), rendering_surface->getHeight());
        glm::vec4  viewport = glm::make_vec4(m_renderer->getViewport());
        int height = screen_size.y;
        glm::ivec2 win = vgf::orientedMousePosition(mousePos, height);
        GLfloat depthZ;
        if( m_modelCursor->getOffScreenEnablePair().second ){
            QString nameOS = m_modelCursor->getOffScreenEnablePair().first;
            if( !m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCastingOffScreen(nameOS, win.x, win.y, viewport, depthZ) ){
                m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCasting(win.x, win.y, viewport, depthZ);
            }
        } else{
            m_raycastingVolumeRenderer3D->getZDepthFrontFromRayCasting(win.x, win.y, viewport, depthZ);
        }

        ModelCursorEvent event;
        event = m_modelCursor->setCursorPositionFrom3DView(mousePos, viewport, screen_size, depthZ);
        m_renderer->setCursorPosition3D(m_modelCursor->getCursorPosition3D());
        rendering_surface->updateRendering();
        m_modelCursor->notify(event, m_cursorEventSlotID, m_modelCursor, event);
    }
}

void ControllerMouseHandlerCursor3D::updateRenderingSurfaceController(ControllerRenderingSurface *controller)
{
    if(controller == nullptr)
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->removeController(this);
    }
    else{
        m_renderingSurfaceSet->getRenderingSurface3D()->addController(this);
    }
}

void ControllerMouseHandlerCursor3D::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D)
{
    m_raycastingVolumeRenderer3D = raycastingVolumeRenderer3D;
}

void ControllerMouseHandlerCursor3D::setCursorRenderer(CursorRenderer3D *cursorRenderer)
{
    m_renderer = cursorRenderer;
}

void ControllerMouseHandlerCursor3D::setSliceSynchronizer(SliceSynchronizer *sliceSynchronizer)
{
    m_sliceSynchronizer = sliceSynchronizer;
}

void ControllerMouseHandlerCursor3D::setModelCursor(ModelCursor *modelCursor)
{
    m_modelCursor = modelCursor;
    m_cursorEventSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_POSITION_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor3D::handleCursorPositionChangeEvent));
    m_cursorStateChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_STATE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor3D::handleCursorStateChangeEvent));
    m_cursorLockStateChangedHandlerSlotId = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_MODE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor3D::handleCursorLockStateChangeEvent));

    m_cursorDrawTypeChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor3D::handleCursorDrawTypeChangeEvent));

    m_cursorOffScreenSlotID  =  m_modelCursor->subscribe(ModelCursorEvent::CURSOR_OFFSCREEN_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor3D::handleCursorOffScreenChangeEvent));

    m_mouseCursorEnable = m_modelCursor->isActivated();
    m_mouseEventLock = m_modelCursor->isLocked();
}

void ControllerMouseHandlerCursor3D::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;

    if(m_modelCursor->isEnabled())
        updateRenderingSurfaceController(this);
    else
        updateRenderingSurfaceController(nullptr);
}
