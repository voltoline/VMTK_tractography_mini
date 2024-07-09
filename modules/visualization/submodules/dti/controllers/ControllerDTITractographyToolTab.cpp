#include "ControllerDTITractographyToolTab.h"

#include "ui/components/RenderingSurfaceSet.h"
#include "ui/RenderingSurface.h"

#include "../models/ModelDTI.h"
#include "../ui/ToolTractography.h"

#include <modules/visualization/submodules/dti/models/ModelUIStateDTI.h>

ControllerDTITractographyToolTab::ControllerDTITractographyToolTab():
    ControllerTool("ToolTractography")
{
    m_modelDTI = nullptr;
    m_enable_DTI = false;

    m_isTractographyEnabled = false;

    m_modelUIStateDTI = nullptr;
    mRenderingSurfaceSet = nullptr;
}

void ControllerDTITractographyToolTab::update(ModelDTI *, ModelDTIEvent event)
{
    switch(event)
    {

    case ModelDTIEvent::TENSORS_COMPUTED:
    {
        m_isTractographyEnabled = true;

        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateComponents();

        m_enable_DTI = true;
        break;
    }

    case ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED:
    case ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED:
    case ModelDTIEvent::FIBER_BUNDLE_ADDED:
    case ModelDTIEvent::ROI_AND_FILTER_CHANGED:
    {
        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateTractography();
        break;
    }

    default:
        break;

    }
}

void ControllerDTITractographyToolTab::computeGlobalTractography()
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->computeGlobalTractography();
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::getTrackingParameters(unsigned int &fiber_length_min, unsigned int &fiber_length_max,
                                                             unsigned short &stopping_criterion_angle, float &stopping_criterion_fa,
                                                             float &seed_creation_fa, float &wpunct)
{
    m_modelDTI->getCurrentTrackingParameters(fiber_length_min, fiber_length_max,
                                      stopping_criterion_angle, stopping_criterion_fa,
                                      seed_creation_fa, wpunct);
}

void ControllerDTITractographyToolTab::getROIPlaneFilterParameters(bool &roi_plane_filter_activated, int &roi_angle_filter)
{
    m_modelDTI->getROIPlaneFilterParameters(roi_plane_filter_activated, roi_angle_filter);
}

void ControllerDTITractographyToolTab::setFiberLengthMin(unsigned int fiber_length_min)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setFiberLengthMin(fiber_length_min);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setFiberLengthMax(unsigned int fiber_length_max)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setFiberLengthMax(fiber_length_max);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setStoppingCriteriaAngle(unsigned short stopping_criterion_angle)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setStoppingCriterionAngle(stopping_criterion_angle);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setStoppingCriteriaFA(float stopping_criterion_fa)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setStoppingCriterionFA(stopping_criterion_fa);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setSeedCreationParameters(float seed_creation_fa)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setSeedCreationFA(seed_creation_fa);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setWpunctParameter(float wpunct)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setWpunct(wpunct);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setROIPlaneAngle(float value)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setROIPlaneAngle(value);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setActivatedROIPlaneFilter(bool value)
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setActivatedROIPlaneFilter(value);
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::addFiberBundle(bool is_visible, std::string tract_name, bool current_edition)
{
    m_modelDTI->addFiberBundle(is_visible, tract_name, current_edition);
    m_modelDTI->notify();
}

void ControllerDTITractographyToolTab::deleteFiberBundle(unsigned short fiber_bundle_index)
{
    m_modelDTI->deleteFiberBundleByIndex(fiber_bundle_index);
    m_modelDTI->notify(this);
}

short ControllerDTITractographyToolTab::getCurrentFiberBundleEditinIndex()
{
    return m_modelDTI->getCurrentFiberBundleEditionIndex();
}

void ControllerDTITractographyToolTab::setCurrentFiberBundleEditinIndex(short fiber_bundle_index)
{
    m_modelDTI->setCurrentFiberBundleEditionIndex(fiber_bundle_index);
    m_modelDTI->notify();
}

void ControllerDTITractographyToolTab::setFiberBundleVisible(short fiber_bundle_index, bool visible)
{
    m_modelDTI->setFiberBundleVisible(fiber_bundle_index, visible);
    m_modelDTI->notify();
}

bool ControllerDTITractographyToolTab::isFiberBundleVisible(short fiber_bundle_index)
{
    return m_modelDTI->isFiberBundleVisible(fiber_bundle_index);
}

bool ControllerDTITractographyToolTab::isFiberBundleReconstructed(short fiber_bundle_index)
{
    return m_modelDTI->isFiberBundleReconstructed(fiber_bundle_index);
}

unsigned short ControllerDTITractographyToolTab::getFiberBundlesCount()
{
    return m_modelDTI->getFiberBundleCount();
}

void ControllerDTITractographyToolTab::setFiberBundleName(short fiber_bundle_index, std::string tract_name)
{
    m_modelDTI->setFiberBundleName(fiber_bundle_index, tract_name);
    m_modelDTI->notify(this);
}

std::string ControllerDTITractographyToolTab::getFiberBundleName(short fiber_bundle_index)
{
    return m_modelDTI->getFiberBundleName(fiber_bundle_index);
}

short ControllerDTITractographyToolTab::getFiberBundleID(short fiber_bundle_index)
{
    return m_modelDTI->getFiberBundleID(fiber_bundle_index);
}

void ControllerDTITractographyToolTab::getFiberBundleTrackingParameters(short fiber_bundle_index, unsigned int &fiber_length_min, unsigned int &fiber_length_max, unsigned short &stopping_criterion_angle, float &stopping_criterion_fa, float &seed_creation_fa, float &wpunct)
{
    m_modelDTI->getFiberBundleTrackingParameters(fiber_bundle_index,
                                                 fiber_length_min, fiber_length_max,
                                                 stopping_criterion_angle, stopping_criterion_fa,
                                                 seed_creation_fa, wpunct);
}

void ControllerDTITractographyToolTab::enableROIBasedFiberTracking(bool state)
{
    m_modelDTI->enableROIBasedFiberTracking(state);
}

void ControllerDTITractographyToolTab::setAutomaticROIFiberTrackingEnabled(bool state)
{
    m_modelDTI->setAutomaticFiberTrackingEnabled(state);
}

bool ControllerDTITractographyToolTab::isAutomaticROIFiberTrackingEnabled()
{
    return m_modelDTI->isAutomaticFiberTrackingEnabled();
}

bool ControllerDTITractographyToolTab::isFiberBundleUpdated(unsigned short fiber_bundle_index)
{
    return m_modelDTI->isFiberBundleUpdated(fiber_bundle_index);
}

bool ControllerDTITractographyToolTab::isTractographyEnabled()
{
    return m_isTractographyEnabled;
}

bool ControllerDTITractographyToolTab::isTractographyActivated()
{
    return m_modelUIStateDTI->isFiberRenderingActivated();
}

void ControllerDTITractographyToolTab::setTractographyActivated(bool state)
{
    ModelUIStateDTIEvent event = m_modelUIStateDTI->setFiberRenderingActivated(state);
    m_modelUIStateDTI->notify(event);
}

bool ControllerDTITractographyToolTab::isDTIComputed()
{
    return m_modelDTI->isDTIComputed();
}

void ControllerDTITractographyToolTab::computeTractographyFromROIs()
{
    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->computeTractographyFromROIs();
    m_modelDTI->notify(this);
}

void ControllerDTITractographyToolTab::setModelDTI(ModelDTI *modelDTI)
{
    m_modelDTI = modelDTI;
    m_modelDTI->subscribe(this, ModelDTIEvent::TENSORS_COMPUTED);
    m_modelDTI->subscribe(this, ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
    m_modelDTI->subscribe(this, ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED);
    m_modelDTI->subscribe(this, ModelDTIEvent::ROI_AND_FILTER_CHANGED);
    m_modelDTI->subscribe(this, ModelDTIEvent::FIBER_BUNDLE_ADDED);

    if(m_modelDTI->isDTIComputed())
    {
        m_isTractographyEnabled = true;
        m_enable_DTI = true;
    }
}

void ControllerDTITractographyToolTab::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    mRenderingSurfaceSet = renderingSurfaceSet;
}

void ControllerDTITractographyToolTab::setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI)
{
    m_modelUIStateDTI = modelUIStateDTI;
}
