#include "Controller2DViewROI.h"

#include "ui/RenderingSurface.h"
#include "ui/components/RenderingSurfaceSet.h"
#include "ControllerROIToolTab.h"
#include "../renderer/ROIRenderer2D.h"

#ifdef usinglib_cursor
#include "internal_libraries/Cursor/include/ModelCursor.h"
#else
#include "modules/visualization/submodules/cursor/ModelCursor.h"
#endif

Controller2DViewROI::Controller2DViewROI(View view)
{
    m_view = view;
    m_enableRenderFreeDrawROI = false;
    m_mouseCaptureEnable = false;
    m_activeFreeDrawROI = false;

    m_renderingSurfaceSet = nullptr;
    m_modelROI = nullptr;
    m_modelCursor = nullptr;
    m_roiRenderer2D = nullptr;

}

Controller2DViewROI::~Controller2DViewROI()
{
}

void Controller2DViewROI::update(ModelROI *, ModelROISetEvent event)
{
    //std::cout << "Controller ROI View Box Update - Observer" << std::endl;
    switch(event)
    {

    case ModelROISetEvent::ROI_ADDED:
    {
        setContext();

        ModelROI::roi_type type = m_modelROI->getType(m_modelROI->getCurrentActiveROIid());
        glm::vec4 color;
        if(type == ModelROI::roi_type::SEED)
        {
              color = glm::vec4(0.0, 0.0, 1.0, 1.0);
        }
        else if (type == ModelROI::roi_type::AND_FILTER)
        {
             color = glm::vec4(0.0, 1.0, 0.0, 1.0);
        }
        else if (type == ModelROI::roi_type::NOT_FILTER)
        {
             color = glm::vec4(1.0, 0.0, 0.0, 1.0);
        }

        m_roiRenderer2D->addNewROI(m_modelROI->getCurrentActiveROIid(), m_modelROI->getTractID(m_modelROI->getCurrentActiveROIid()), color);
        int id_retrieve = m_modelROI->getCurrentActiveROIid();

        m_roiRenderer2D->setCurrentActiveROI(id_retrieve);

        ModelROI::roi_plane plane = m_modelROI->getPlane(m_modelROI->getCurrentActiveROIid());
        if(isAxial() && plane == ModelROI::roi_plane::AXIAL_2D)
        {
            std::vector<glm::vec3> points_ref = m_modelROI->getROIpoints(m_modelROI->getCurrentActiveROIid());
            for(unsigned int i = 0; i < points_ref.size(); i++)
            {
                glm::vec3 point_ax;
                point_ax.x = points_ref[i].x;
                point_ax.y = points_ref[i].y * (-1);
                point_ax.z = points_ref[i].z;

                m_roiRenderer2D->addPoint(point_ax);
            }

        } else if (isSagittal() && plane == ModelROI::roi_plane::SAGITTAL_2D) {

            std::vector<glm::vec3> points_ref = m_modelROI->getROIpoints(m_modelROI->getCurrentActiveROIid());
            for(unsigned int i = 0; i < points_ref.size(); i++)
            {
                glm::vec3 point_sag;
                point_sag.x = points_ref[i].y;
                point_sag.y = points_ref[i].z;
                point_sag.z = points_ref[i].x;

                m_roiRenderer2D->addPoint(point_sag);
            }

        } else if (isCoronal() && plane == ModelROI::roi_plane::CORONAL_2D) {

            std::vector<glm::vec3> points_ref = m_modelROI->getROIpoints(m_modelROI->getCurrentActiveROIid());
            for(unsigned int i = 0; i < points_ref.size(); i++)
            {
                glm::vec3 point_cor;
                point_cor.x = points_ref[i].x;
                point_cor.y = points_ref[i].z;
                point_cor.z = points_ref[i].y;

                m_roiRenderer2D->addPoint(point_cor);
            }
        }

        break;
    }

    case ModelROISetEvent::POINT_ADDED:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
           ModelROI::roi_plane plane = m_modelROI->getPlane(m_modelROI->getCurrentActiveROIid());
            if(isAxial() && plane == ModelROI::roi_plane::AXIAL_2D)
            {
                glm::vec3 point_ref = m_modelROI->getLastPointAdded();
                glm::vec3 point_ax;
                point_ax.x = point_ref.x;
                point_ax.y = point_ref.y * (-1);
                point_ax.z = point_ref.z;

                m_roiRenderer2D->addPoint(point_ax);
            }
            else if (isSagittal() && plane == ModelROI::roi_plane::SAGITTAL_2D)
            {
                glm::vec3 point_ref = m_modelROI->getLastPointAdded();
                glm::vec3 point_sag;
                point_sag.x = point_ref.y;
                point_sag.y = point_ref.z;
                point_sag.z = point_ref.x;

                m_roiRenderer2D->addPoint(point_sag);
            }
            else if (isCoronal() && plane == ModelROI::roi_plane::CORONAL_2D)
            {
                glm::vec3 point_ref = m_modelROI->getLastPointAdded();
                glm::vec3 point_cor;
                point_cor.x = point_ref.x;
                point_cor.y = point_ref.z;
                point_cor.z = point_ref.y;

                m_roiRenderer2D->addPoint(point_cor);
            }
        }
        break;
    }

    case ModelROISetEvent::ROI_ENDED:
    {
        setContext();

        if(m_activeFreeDrawROI)
        {
            //Disable Mouse Capturing
            m_mouseCaptureEnable = false;
        }

        break;
    }

    case ModelROISetEvent::ROI_EDITION:
    {
        if(m_activeFreeDrawROI)
        {
            //Enable Mouse Capture for get points
            m_mouseCaptureEnable = m_modelROI->getRoiEditionActivated();
        }
        break;
    }

    case ModelROISetEvent::ROI_CREATED:
    {
       //Add NEW ROI on Render
       setContext();
       if(m_activeFreeDrawROI)
       {
           ModelROI::roi_type type = m_modelROI->getType(m_modelROI->getCurrentActiveROIid());
           glm::vec4 color;
           if(type == ModelROI::roi_type::SEED)
           {
                 color = glm::vec4(0.0, 0.0, 1.0, 1.0);
           }
           else if (type == ModelROI::roi_type::AND_FILTER)
           {
                color = glm::vec4(0.0, 1.0, 0.0, 1.0);
           }
           else if (type == ModelROI::roi_type::NOT_FILTER)
           {
                color = glm::vec4(1.0, 0.0, 0.0, 1.0);
           }

           m_roiRenderer2D->addNewROI(m_modelROI->getCurrentActiveROIid(), m_modelROI->getTractID(m_modelROI->getCurrentActiveROIid()), color);
           int id_retrieve = m_modelROI->getCurrentActiveROIid();

           m_roiRenderer2D->setCurrentActiveROI(id_retrieve);
       }

       break;
    }
    case ModelROISetEvent::ROI_REMOVED:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
            m_roiRenderer2D->removeROI(m_modelROI->getLastRemovedROIid());

            m_renderingSurfaceSet->update2DRendering();
        }
        break;
    }
    case ModelROISetEvent::LAST_ROI_REMOVED:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
            setStateRenderingFreeDrawCursorROI(false);
            m_roiRenderer2D->removeROI(m_modelROI->getLastRemovedROIid());
        }
        break;
    }
    case ModelROISetEvent::UPDATE_RENDER:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
            m_roiRenderer2D->setCurrentActiveROI(m_modelROI->getCurrentActiveROIid());
        }
        break;
    }
    case ModelROISetEvent::RENDER_ROI:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
            setStateRenderingFreeDrawCursorROI(true);
        }
        break;
    }
    case ModelROISetEvent::NOT_RENDER_ROI:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
            setStateRenderingFreeDrawCursorROI(false);
        }
        break;
    }

    case ModelROISetEvent::ROI_SLICE_CHANGE:
    {
        setContext();
        if (m_activeFreeDrawROI)
        {
           ModelROI::roi_plane plane = m_modelROI->getPlane(m_modelROI->getCurrentActiveROIid());
            if(isAxial() && plane == ModelROI::roi_plane::AXIAL_2D)
            {
                m_roiRenderer2D->setROISlice(m_modelROI->getCurrentActiveROIid(), m_modelROI->getSlice(m_modelROI->getCurrentActiveROIid()));
            }
            else if (isSagittal() && plane == ModelROI::roi_plane::SAGITTAL_2D)
            {
                m_roiRenderer2D->setROISlice(m_modelROI->getCurrentActiveROIid(), m_modelROI->getSlice(m_modelROI->getCurrentActiveROIid()));
            }
            else if (isCoronal() && plane == ModelROI::roi_plane::CORONAL_2D)
            {
                m_roiRenderer2D->setROISlice(m_modelROI->getCurrentActiveROIid(), m_modelROI->getSlice(m_modelROI->getCurrentActiveROIid()));
            }
        }
        break;
    }

    default:
        break;
    }
}

void Controller2DViewROI::setContext()
{
    if(isAxial())
    {
        m_renderingSurfaceSet->getRenderingSurfaceAxial()->setRenderingContext();
    }
    else if(isSagittal())
    {
        m_renderingSurfaceSet->getRenderingSurfaceSagittal()->setRenderingContext();
    }
    else if(isCoronal())
    {
        m_renderingSurfaceSet->getRenderingSurfaceCoronal()->setRenderingContext();
    }
}

void Controller2DViewROI::mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCaptureEnable == true)
    {
            int screen_size[2] = {renderingSurface->getWidth(), renderingSurface->getHeight()};

            std::cout << "Controller2DViewROI::mousePressEvent::Mouse Pressed Inside 2D ROI Controller View Box" << std::endl;

            if(isAxial())
            {
                std::cout << "Controller2DViewROI::mousePressEvent::Change to ROI Controller Axial" << std::endl;

                unsigned int id = m_modelROI->getCurrentActiveROIid();
                ModelROISetEvent event = m_modelROI->setPlane(id, ModelROI::roi_plane::AXIAL_2D);
                m_modelROI->notify(event, nullptr);
                m_renderingSurfaceSet->setFocusAxial();

                m_modelCursor->setCursorPositionFromAxialView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);

            } else if(isCoronal()) {
                std::cout << "Controller2DViewROI::mousePressEvent::Change to ROI Controller Coronal " << std::endl;

                unsigned int id = m_modelROI->getCurrentActiveROIid();
                ModelROISetEvent event = m_modelROI->setPlane(id, ModelROI::roi_plane::CORONAL_2D);
                m_modelROI->notify(event, nullptr);
                m_renderingSurfaceSet->setFocusCoronal();

                m_modelCursor->setCursorPositionFromCoronalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);

            } else if (isSagittal()){   //Check whether it is sagittal view
                std::cout << "Controller2DViewROI::mousePressEvent::Change to ROI Controller Sagittal " << std::endl;

                unsigned int id = m_modelROI->getCurrentActiveROIid();
                ModelROISetEvent event = m_modelROI->setPlane(id, ModelROI::roi_plane::SAGITTAL_2D);
                m_modelROI->notify(event, nullptr);
                m_renderingSurfaceSet->setFocusSagittal();

                m_modelCursor->setCursorPositionFromSagittalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);

            }

            renderingSurface->updateRendering();
    }
}

void  Controller2DViewROI::mouseDoubleClickEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    int screen_size[2] = {renderingSurface->getWidth(), renderingSurface->getHeight()};

    setContext();
    if(isAxial())
    {
        m_modelCursor->setCursorPositionFromAxialView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
    }
    else if(isSagittal())
    {
        m_modelCursor->setCursorPositionFromSagittalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
    }
    else if(isCoronal())
    {
        m_modelCursor->setCursorPositionFromCoronalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
    }

}

void Controller2DViewROI::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    int screen_size[2] = {renderingSurface->getWidth(), renderingSurface->getHeight()};
    glm::vec3 point_position;

    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCaptureEnable == true)
    {
        setContext();
        if(isAxial())
        {
            m_modelCursor->setCursorPositionFromAxialView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
            point_position = m_modelCursor->getCursorPositionAxial();
        }
        else if(isSagittal())
        {
            m_modelCursor->setCursorPositionFromSagittalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
            point_position = m_modelCursor->getCursorPositionSagittal();
        }
        else if(isCoronal())
        {
            m_modelCursor->setCursorPositionFromCoronalView(mouse_event.getPosX(), mouse_event.getPosY(), m_roiRenderer2D->getViewport(), screen_size);
            point_position = m_modelCursor->getCursorPositionCoronal();
        }

        if(mouse_event.getType() == MouseEvent::Type::MOUSE_SINGLE_CLICK)
        {
            if(m_enableRenderFreeDrawROI)
            {
                setContext();

                glm::vec3 point_3D = m_modelCursor->getCursorPosition3D();
                ModelROISetEvent event = m_modelROI->addPoint(m_modelROI->getCurrentActiveROIid(), point_3D);
                m_modelROI->notify(event, this);
                m_roiRenderer2D->addPoint(point_position);
            }
        }

        renderingSurface->updateRendering();
    }
}

void Controller2DViewROI::mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::RIGHT && m_mouseCaptureEnable == true)
    {
        if(mouse_event.getType() == MouseEvent::Type::MOUSE_SINGLE_CLICK && m_enableRenderFreeDrawROI)
        {
            ModelROISetEvent event = m_modelROI->endROI(m_modelROI->getCurrentActiveROIid());
            m_modelROI->notify(event, nullptr);
            std::cout << "Controller2DViewROI::mouseReleaseEvent: Release Event Capture Single Click" << std::endl;
        }

        renderingSurface->updateRendering();
    }
}

void Controller2DViewROI::setModelROI(ModelROI *modelROI)
{
    m_modelROI = modelROI;
    m_modelCursor = m_modelROI->getModelCursor();
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_ADDED);
    m_modelROI->subscribe(this, ModelROISetEvent::POINT_ADDED);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_CREATED);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_ENDED);
    m_modelROI->subscribe(this, ModelROISetEvent::RENDER_ROI);
    m_modelROI->subscribe(this, ModelROISetEvent::NOT_RENDER_ROI);
    m_modelROI->subscribe(this, ModelROISetEvent::UPDATE_RENDER);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_REMOVED);
    m_modelROI->subscribe(this, ModelROISetEvent::LAST_ROI_REMOVED);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_SLICE_CHANGE);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_EDITION);
}

void Controller2DViewROI::setActivationFreeDrawROI(bool activation)
{
    m_activeFreeDrawROI = activation;
    if(!activation)
        setStateRenderingFreeDrawCursorROI(false);
    else
        setStateRenderingFreeDrawCursorROI(true);
}

void Controller2DViewROI::setStateRenderingFreeDrawCursorROI(bool state)
{
    m_enableRenderFreeDrawROI = state;
    m_roiRenderer2D->setROIDrawingActivated(m_enableRenderFreeDrawROI);
    update2DRendering();
}

void Controller2DViewROI::update2DRendering()
{
    if(isAxial())
        m_renderingSurfaceSet->updateAxialRendering();

    else if(isCoronal())
        m_renderingSurfaceSet->updateCoronalRendering();

    else
        m_renderingSurfaceSet->updateSagittalRendering();
}

void Controller2DViewROI::setRoiRenderer2D(ROIRenderer2D *roiRenderer2D)
{
    m_roiRenderer2D = roiRenderer2D;
}

void Controller2DViewROI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

bool Controller2DViewROI::isAxial()
{
    return m_view == Controller2DViewROI::View::AXIAL;
}

bool Controller2DViewROI::isCoronal()
{
    return m_view == Controller2DViewROI::View::CORONAL;
}

bool Controller2DViewROI::isSagittal()
{
    return m_view == Controller2DViewROI::View::SAGITTAL;
}
