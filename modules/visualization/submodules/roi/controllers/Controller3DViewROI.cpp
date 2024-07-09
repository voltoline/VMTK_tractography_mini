#include "Controller3DViewROI.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "../renderer/ROIRenderer3D.h"

Controller3DViewROI::Controller3DViewROI()
{
    mActiveFreeLine3D = false;
    mEnableRenderFreeDraw3D = false;
    m_cursorEventSlotID = -1;

    mRenderingSurfaceSet = nullptr;
    mRenderer = nullptr;
    m_model3DTransformation = nullptr;
    mModelROI = nullptr;
}

void Controller3DViewROI::afterRendering()
{
    mRenderingSurfaceSet->getRenderingSurface3D()->saveImage(true);
}

void Controller3DViewROI::update(Model3DTransformation *, Model3DTransformationEvent event)
{
    switch(event)
    {

    case Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED:
    {
        mRenderer->setModelViewMatrix(m_model3DTransformation->getModelViewMatrix());

        mRenderingSurfaceSet->update3DRendering();
        break;
    }

    default:
        break;

    }
}

void Controller3DViewROI::update(ModelROI *, ModelROISetEvent event)
{
    switch(event)
    {
    case ModelROISetEvent::ROI_CREATED:
    {
        //Add NEW ROI on Render
        setContext();
        if (mActiveFreeLine3D)
        {
            int id_retrieve = mModelROI->getCurrentActiveROIid();
            mRenderer->addNewFreeDraw3D(id_retrieve, mModelROI->getTractID(id_retrieve));
            std::cout << "New Free Draw ROI 3D Created, ID Retrieval: " << id_retrieve << std::endl;
            mRenderer->setCurrentActiveFreeDraw3D(id_retrieve);
        }
        break;
    }
    case ModelROISetEvent::POINT_ADDED:
    {
        setContext();
        if (mActiveFreeLine3D)
        {
            glm::vec3 point_add = mModelROI->getMousePosition3D();
            mRenderer->addNewPointFreeDraw3D(point_add);
        }
        mRenderingSurfaceSet->update3DRendering();
        break;
    }
    case ModelROISetEvent::ROI_ENDED:
    {
        setContext();
        if(mActiveFreeLine3D)
        {
            //DO something
        }
        break;
    }
    case ModelROISetEvent::CHANGE_TYPE_ROI:
    {
        break;
    }
    case ModelROISetEvent::RENDER_ROI:
    {
        setContext();
        if (mActiveFreeLine3D)
        {
            setStateRenderingFreeDraw3D(true);
        }
        break;
    }
    case ModelROISetEvent::NOT_RENDER_ROI:
    {
        if (mActiveFreeLine3D)
        {
            setStateRenderingFreeDraw3D(false);
        }
        break;
    }
    case ModelROISetEvent::UPDATE_RENDER:
    {
        setContext();
        if (mActiveFreeLine3D)
        {
            mRenderer->setCurrentActiveFreeDraw3D(mModelROI->getCurrentActiveROIid());
        }
        break;
    }
    case ModelROISetEvent::ROI_REMOVED:
    {
        setContext();
        if (mActiveFreeLine3D)
        {
            mRenderer->removeFreeDraw3D(mModelROI->getLastRemovedROIid());
            std::cout << "Removed Free Line 3D with id: " << mModelROI->getLastRemovedROIid() << std::endl;
        }
        break;
    }
    case ModelROISetEvent::LAST_ROI_REMOVED:
    {
        setContext();
        if (mActiveFreeLine3D)
        {
            setStateRenderingFreeDraw3D(false);
            mRenderer->removeFreeDraw3D(mModelROI->getLastRemovedROIid());
        }
        break;
    }
    default:
        break;
    }
}

void Controller3DViewROI::updateMousePosition(ModelCursor * model, ModelCursorEvent event)
{
    switch(event)
    {
    case ModelCursorEvent::CURSOR_POSITION_CHANGED:
    {
        mRenderingSurfaceSet->update3DRendering();
        break;
    }
    //To make compiler happy (Ting, 3/5/2022)
    case ModelCursorEvent::CURSOR_MODE_CHANGED:
    case ModelCursorEvent::CURSOR_STATE_CHANGED:
    case ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED:
    case ModelCursorEvent::NONE:
        break;
    }

}

void Controller3DViewROI::setContext()
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();
}

void Controller3DViewROI::mousePressEvent(MouseEvent &/*mouse_event*/, RenderingSurface *renderingSurface)
{
    renderingSurface->updateRendering();

    mRenderingSurfaceSet->setFocus3D();

}

void Controller3DViewROI::mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface)
{
    renderingSurface->updateRendering();

    //Capture movement of Mouse when pressed to create a Free Line Draw
    if(mouse_event.getMouseButton() == MouseEvent::MouseButton::LEFT)
    {
        //Save values on list to be created
    }

}

void Controller3DViewROI::setRenderer(ROIRenderer3D *renderer)
{
    mRenderer = renderer;
}

void Controller3DViewROI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    mRenderingSurfaceSet = renderingSurfaceSet;
}

void Controller3DViewROI::setActivationFreeDraw3D(bool state)
{
    mActiveFreeLine3D = state;
}

void Controller3DViewROI::setStateRenderingFreeDraw3D(bool state)
{
    mEnableRenderFreeDraw3D = state;
    mRenderer->setEnableFreeDraw(state);
    mRenderingSurfaceSet->update3DRendering();
}

void Controller3DViewROI::setModel3DTransformation(Model3DTransformation *model3DTransformation)
{
    m_model3DTransformation = model3DTransformation;
    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void Controller3DViewROI::setModelROI(ModelROI *modelROI)
{
    mModelROI = modelROI;
    mModelROI->subscribe(this, ModelROISetEvent::ROI_CREATED);
    mModelROI->subscribe(this, ModelROISetEvent::POINT_ADDED);
    mModelROI->subscribe(this, ModelROISetEvent::ROI_ENDED);
    mModelROI->subscribe(this, ModelROISetEvent::CHANGE_TYPE_ROI);
    mModelROI->subscribe(this, ModelROISetEvent::RENDER_ROI);
    mModelROI->subscribe(this, ModelROISetEvent::NOT_RENDER_ROI);
    mModelROI->subscribe(this, ModelROISetEvent::UPDATE_RENDER);
    mModelROI->subscribe(this, ModelROISetEvent::ROI_REMOVED);
    mModelROI->subscribe(this, ModelROISetEvent::LAST_ROI_REMOVED);

    m_cursorEventSlotID = mModelROI->getModelCursor()->subscribe(ModelCursorEvent::CURSOR_POSITION_CHANGED, CREATE_SLOT(this, &Controller3DViewROI::updateMousePosition));
}
