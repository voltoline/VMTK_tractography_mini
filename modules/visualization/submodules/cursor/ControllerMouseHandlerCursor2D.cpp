#include "ControllerMouseHandlerCursor2D.h"
#include "CursorRenderer2D.h"

#include "ui/components/RenderingSurfaceSet.h"
#include <glm/gtx/string_cast.hpp>
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/SliceSynchronizer.h"
#else
#include "modules/visualization/submodules/2d_slice/SliceSynchronizer.h"
#endif


ControllerMouseHandlerCursor2D::ControllerMouseHandlerCursor2D(View view)
{
    m_view = view;
    m_mouseCursorEnable = false;
    m_renderer = nullptr;
    m_mouseEventLock = false;
    m_sliceSynchronizer = nullptr;
    m_renderingSurfaceSet = nullptr;
    m_cursorDrawTypeChangedHandlerSlotID = -1;
    m_modelCursor = nullptr;

    m_cursorPositionChangedHandlerSlotID = -1;
    m_cursorStateChangedHandlerSlotID = -1;
    m_cursorLockStateChangedHandlerSlotId = -1;
}

void ControllerMouseHandlerCursor2D::handleCursorStateChangeEvent(ModelCursor *, ModelCursorEvent event_type)
{
    if(event_type == ModelCursorEvent::CURSOR_STATE_CHANGED)
    {
        if(m_mouseCursorEnable == m_modelCursor->isActivated())
            return;

        m_mouseCursorEnable = m_modelCursor->isActivated();

        if(m_mouseCursorEnable)
            updateRenderingSurfaceController(this);
        else
            updateRenderingSurfaceController(nullptr);


        m_renderer->setEnabled(m_mouseCursorEnable);

        if(m_modelCursor->isActivated())
        {
            if(isAxial())
                m_renderer->setPosition(m_modelCursor->getCursorPositionAxial());

            else if(isCoronal())
                m_renderer->setPosition(m_modelCursor->getCursorPositionCoronal());

            else if(isSagittal())
                m_renderer->setPosition(m_modelCursor->getCursorPositionSagittal());
        }

        if(isAxial())
            m_renderingSurfaceSet->updateAxialRendering();

        else if(isCoronal())
            m_renderingSurfaceSet->updateCoronalRendering();

        else if(isSagittal())
            m_renderingSurfaceSet->updateSagittalRendering();
    }
}



void ControllerMouseHandlerCursor2D::handleCursorPositionChangeEvent(ModelCursor *, ModelCursorEvent event_type)
{
    if(event_type == ModelCursorEvent::CURSOR_POSITION_CHANGED)
    {
        if(m_renderer == nullptr)
            return;

        if(isAxial() && m_modelCursor->isActivated())
        {
            m_renderer->setPosition(m_modelCursor->getCursorPositionAxial());

            if(m_renderingSurfaceSet)
                m_renderingSurfaceSet->updateAxialRendering();

        } else if(isCoronal() && m_modelCursor->isActivated()) {

            m_renderer->setPosition(m_modelCursor->getCursorPositionCoronal());

            if(m_renderingSurfaceSet)
                m_renderingSurfaceSet->updateCoronalRendering();

        } else if(isSagittal() && m_modelCursor->isActivated()) {

            m_renderer->setPosition(m_modelCursor->getCursorPositionSagittal());

            if(m_renderingSurfaceSet)
                m_renderingSurfaceSet->updateSagittalRendering();
        }
    }
}

void ControllerMouseHandlerCursor2D::handleCursorLockStateChangeEvent(ModelCursor *, ModelCursorEvent event_type)
{
    if(event_type == ModelCursorEvent::CURSOR_MODE_CHANGED)
    {
        if(m_renderer == nullptr)
            return;

        if(m_modelCursor->isActivated())
        {
            if(m_modelCursor->isLocked())
                m_mouseEventLock = true; //Set flag to ignore mouse events
            else
                m_mouseEventLock = false;//Set flag to habilitate mouse events
        }
    }
}

void ControllerMouseHandlerCursor2D::handleCursorDrawTypeChangeEvent(ModelCursor *, ModelCursorEvent event_type)
{
    if(event_type == ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED)
    {
        m_renderer->setDrawCursorType(m_modelCursor->getDrawCursorType());
        m_renderingSurfaceSet->update2DRendering();
    }
}

void ControllerMouseHandlerCursor2D::mousePressEvent(MouseEvent &mouse_event, RenderingSurface *rendering_surface)
{
    // Begin Code -> added by Iván (03:25 a. m. 18/10/2018)
    // Setting and painting border of the widget in focus.
    if(m_renderer)
    {
        if(isAxial())
        {
            m_renderingSurfaceSet->setFocusAxial();
        } else if(isCoronal()) {
            m_renderingSurfaceSet->setFocusCoronal();
        } else {
            m_renderingSurfaceSet->setFocusSagittal();
        }
    }

    if(m_mouseEventLock)
        return;

    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCursorEnable)
    {
        if(m_modelCursor->getDrawCursorType() == DrawCursorType::invisibleCursor)
        {

        }

        if(m_renderer)
        {
            int screen_size[2] = {rendering_surface->getWidth(), rendering_surface->getHeight()};
            ModelCursorEvent event;

            if(isAxial())
            {
                event = m_modelCursor->setCursorPositionFromAxialView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionAxial());

            } else if(isCoronal()) {

                event = m_modelCursor->setCursorPositionFromCoronalView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionCoronal());

            } else {

                event = m_modelCursor->setCursorPositionFromSagittalView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionSagittal());
            }

            m_modelCursor->notify(event, m_cursorPositionChangedHandlerSlotID, m_modelCursor, event);
        }
    }

    rendering_surface->updateRendering();
}

void ControllerMouseHandlerCursor2D::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *rendering_surface)
{
    if(m_mouseEventLock)
        return;

    if (mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCursorEnable)
    {
        if(m_renderer)
        {
            int screen_size[2] = {rendering_surface->getWidth(), rendering_surface->getHeight()};
            ModelCursorEvent event;

            if(isAxial())
            {
                event = m_modelCursor->setCursorPositionFromAxialView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionAxial());

                rendering_surface->updateRendering();

            } else if(isCoronal()) {

                event = m_modelCursor->setCursorPositionFromCoronalView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionCoronal());

                rendering_surface->updateRendering();

            } else {

                event = m_modelCursor->setCursorPositionFromSagittalView(mouse_event.getPosX(), mouse_event.getPosY(), m_renderer->getViewport(), screen_size);
                m_renderer->setPosition(m_modelCursor->getCursorPositionSagittal());

                rendering_surface->updateRendering();
            }

            m_modelCursor->notify(event, m_cursorPositionChangedHandlerSlotID, m_modelCursor, event);
        }
    }
}

void ControllerMouseHandlerCursor2D::updateRenderingSurfaceController(ControllerRenderingSurface *controller)
{
    if(m_renderingSurfaceSet == nullptr)
        return;

    if(controller == nullptr)
    {
        if(m_renderer)
        {
            if(isAxial())
            {
                if(m_renderingSurfaceSet->getRenderingSurfaceAxial())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceAxial()->removeController(this);
                }

            } else if(isCoronal()) {
                if(m_renderingSurfaceSet->getRenderingSurfaceCoronal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceCoronal()->removeController(this);
                }

            } else {
                if(m_renderingSurfaceSet->getRenderingSurfaceSagittal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceSagittal()->removeController(this);
                }
            }
        }

    } else {

        if(m_renderer)
        {
            if(isAxial())
            {
                if(m_renderingSurfaceSet->getRenderingSurfaceAxial())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceAxial()->addController(this);
                }

            } else if(isCoronal()) {
                if(m_renderingSurfaceSet->getRenderingSurfaceCoronal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceCoronal()->addController(this);
                }

            } else {
                if(m_renderingSurfaceSet->getRenderingSurfaceSagittal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceSagittal()->addController(this);
                }
            }
        }
    }

}

void ControllerMouseHandlerCursor2D::updateRenders()
{
    if(m_renderer && m_renderingSurfaceSet)
    {
        m_renderingSurfaceSet->update2DRendering();
    }
}

void ControllerMouseHandlerCursor2D::setModelCursor(ModelCursor *modelCursor)
{
    m_modelCursor = modelCursor;
    m_cursorPositionChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_POSITION_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor2D::handleCursorPositionChangeEvent));
    m_cursorStateChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_STATE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor2D::handleCursorStateChangeEvent));
    m_cursorLockStateChangedHandlerSlotId = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_MODE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor2D::handleCursorLockStateChangeEvent));

    m_cursorDrawTypeChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED, CREATE_SLOT(this, &ControllerMouseHandlerCursor2D::handleCursorDrawTypeChangeEvent));


    m_mouseCursorEnable = m_modelCursor->isActivated();
    m_mouseEventLock = m_modelCursor->isLocked();

    if(m_modelCursor->isActivated())
        updateRenderingSurfaceController(this);
}

void ControllerMouseHandlerCursor2D::setSliceSynchronizer(SliceSynchronizer *sliceSynchronizer)
{
    m_sliceSynchronizer = sliceSynchronizer;
}

void ControllerMouseHandlerCursor2D::setRenderer(CursorRenderer2D *renderer)
{
    m_renderer = renderer;

    if(m_renderer)
        m_renderer->setEnabled(m_mouseCursorEnable);

    if(m_modelCursor->isActivated())
        updateRenderingSurfaceController(this);
}

void ControllerMouseHandlerCursor2D::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;

    if(m_modelCursor->isActivated())
        updateRenderingSurfaceController(this);
}

bool ControllerMouseHandlerCursor2D::isAxial()
{
    return m_view == ControllerMouseHandlerCursor2D::View::AXIAL;
}

bool ControllerMouseHandlerCursor2D::isCoronal()
{
    return m_view == ControllerMouseHandlerCursor2D::View::CORONAL;
}

bool ControllerMouseHandlerCursor2D::isSagittal()
{
    return m_view == ControllerMouseHandlerCursor2D::View::SAGITTAL;
}
