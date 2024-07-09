#include "EventHandlerDTIToROI.h"

#include <ui/components/RenderingSurfaceSet.h>
#include <modules/visualization/submodules/dti/ui/ToolTractography.h>
#include <modules/visualization/submodules/roi/ModelROI.h>
#include <modules/visualization/submodules/roi/controllers/ControllerROIToolTab.h>
#include <modules/visualization/submodules/roi/renderer/ROIRenderer2D.h>

EventHandlerDTIToROI::EventHandlerDTIToROI()
{
    m_roiRenderer2DAxial = nullptr;
    m_roiRenderer2DSagittal = nullptr;
    m_roiRenderer2DCoronal = nullptr;

    m_renderingSurfaceSet = nullptr;

    m_modelROI = nullptr;

    m_modelDTI = nullptr;
    m_controllerROIToolTab = nullptr;
}

void EventHandlerDTIToROI::update(ModelDTI *, ModelDTIEvent event)
{
    switch(event)
    {
    case ModelDTIEvent::FIBER_BUNDLE_DELETED:
    {
        std::cout << "ModelDTIEvent::FIBER_BUNDLE_DELETED  " << std::endl;

        int tract_id = m_modelDTI->getDeletedFiberBundleID();
        std::vector<int> roi_ids = m_modelROI->deletedTract(tract_id);

        for (unsigned short i = 0; i < roi_ids.size(); i++)
        {
            ModelROISetEvent event = m_modelROI->removeROI(roi_ids[i], true);
            m_modelROI->notify(event);
        }

        break;
    }

    case ModelDTIEvent::FIBER_BUNDLE_ADDED:
    case ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED:
    {
        short fiber_bundle_id = -1;
        if(m_modelDTI->getCurrentFiberBundleEditionIndex() >= 0)
            fiber_bundle_id = m_modelDTI->getFiberBundleID(m_modelDTI->getCurrentFiberBundleEditionIndex());

        if(m_roiRenderer2DAxial)
            m_roiRenderer2DAxial->setCurrentTractID(fiber_bundle_id);

        if(m_roiRenderer2DSagittal)
            m_roiRenderer2DSagittal->setCurrentTractID(fiber_bundle_id);

        if(m_roiRenderer2DCoronal)
            m_roiRenderer2DCoronal->setCurrentTractID(fiber_bundle_id);

        if(m_renderingSurfaceSet)
            m_renderingSurfaceSet->update2DRendering();

        if(m_modelROI)
            m_modelROI->setCurrentEditableTractID(fiber_bundle_id);
    }

    default:
        break;
    }
}

void EventHandlerDTIToROI::setModelDTI(ModelDTI *modelDTI)
{
    m_modelDTI = modelDTI;

    m_modelDTI->subscribe(this, ModelDTIEvent::FIBER_BUNDLE_ADDED);
    m_modelDTI->subscribe(this, ModelDTIEvent::FIBER_BUNDLE_DELETED);
    m_modelDTI->subscribe(this, ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
}

void EventHandlerDTIToROI::setModelROI(ModelROI *modelROI)
{
    m_modelROI = modelROI;
}

void EventHandlerDTIToROI::setControllerROIToolTab(ControllerROIToolTab *controllerROIToolTab)
{
    m_controllerROIToolTab = controllerROIToolTab;
}

void EventHandlerDTIToROI::setRoiRenderer2DAxial(ROIRenderer2D *roiRenderer2DAxial)
{
    m_roiRenderer2DAxial = roiRenderer2DAxial;
}

void EventHandlerDTIToROI::setRoiRenderer2DSagittal(ROIRenderer2D *roiRenderer2DSagittal)
{
    m_roiRenderer2DSagittal = roiRenderer2DSagittal;
}

void EventHandlerDTIToROI::setRoiRenderer2DCoronal(ROIRenderer2D *roiRenderer2DCoronal)
{
    m_roiRenderer2DCoronal = roiRenderer2DCoronal;
}

void EventHandlerDTIToROI::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}
