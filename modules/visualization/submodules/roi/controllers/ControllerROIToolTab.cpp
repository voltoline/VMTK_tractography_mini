#include "ControllerROIToolTab.h"

#include <modules/visualization/submodules/dti/models/ModelDTI.h>
#include "Controller3DViewROI.h"
#include "Controller2DViewROI.h"
#include "ui/components/RenderingSurfaceSet.h"
#include "modules/visualization/submodules/dti/ui/ToolTractography.h"
#include "../event_handler/EventHandlerROI.h"

ControllerROIToolTab::ControllerROIToolTab():
    ControllerTool("ToolTractography")
{
    mEnableAutomaticRender = true;

    mRenderingSurfaceSet = nullptr;
    mModelROI = nullptr;
    m_modelDTI = nullptr;
    m2DViewControllerAxialROI = nullptr;
    m2DViewControllerCoronalROI = nullptr;
    m2DViewControllerSagittalROI = nullptr;
    m_eventHandlerROI = nullptr;
    m3DViewControllerROI = nullptr;
}

void ControllerROIToolTab::update(ModelROI *, ModelROISetEvent event)
{
    switch(event)
    {
    case ModelROISetEvent::ROI_STATE_CHANGED:
    {
        controllerROIchangePertinence(mModelROI->isEnabled());
        setRenderingController(mModelROI->isEnabled());
        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateTractography();
        break;
    }

    case ModelROISetEvent::ROI_ADDED:
    case ModelROISetEvent::ROI_LOADED:
    {
        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateTractography();
        break;
    }

    case ModelROISetEvent::ROI_REMOVED:
    {
        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateTractography();
        break;
    }
    case ModelROISetEvent::LAST_ROI_REMOVED:
    {
        if(getTool())
            static_cast<ToolTractography*>(getTool())->updateTractography();
        break;
    }
    case ModelROISetEvent::ROI_MODEL_CHANGED:
    {
        if(mEnableAutomaticRender)
            getPoints(getActiveROIs(m_modelDTI->getFiberBundleID(m_modelDTI->getCurrentFiberBundleEditionIndex())));
        else
        {
            if(getTool())
                static_cast<ToolTractography*>(getTool())->updateTractography();
        }
        break;
    }

    default:
        break;
    }
}

void ControllerROIToolTab::setModelROI(ModelROI * roi)
{
    mModelROI = roi;
    roi->subscribe(this, ModelROISetEvent::ROI_STATE_CHANGED);
    roi->subscribe(this, ModelROISetEvent::ROI_ADDED);
    roi->subscribe(this, ModelROISetEvent::ROI_LOADED);
    roi->subscribe(this, ModelROISetEvent::ROI_REMOVED);
    roi->subscribe(this, ModelROISetEvent::LAST_ROI_REMOVED);
    roi->subscribe(this, ModelROISetEvent::ROI_MODEL_CHANGED);

}


void ControllerROIToolTab::setModelDTI(ModelDTI *dti)
{
    m_modelDTI = dti;
}

void ControllerROIToolTab::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
     mRenderingSurfaceSet = renderingSurfaceSet;
}

void ControllerROIToolTab::setFreeDrawROI(bool state)
{
    ModelROISetEvent event = mModelROI->setEnabled(state);
    controllerROIchangePertinence(state);

    setRenderingController(state);

    mModelROI->notify(event, this);
}

bool ControllerROIToolTab::getFreeDrawControllerState()
{
    return mModelROI->isEnabled();
}

int ControllerROIToolTab::createNewROI(ModelROI::roi_type type, std::string tract_name, int tract_id)
{
    ModelROISetEvent event = mModelROI->newROI(type, tract_name, tract_id);
    int id = mModelROI->getCurrentActiveROIid();

    mModelROI->notify(event, this);

    event = mModelROI->setROIEditionActivated(true);
    mModelROI->notify(event, this);

    setROIactivity(id, true);

    return id;
}


void ControllerROIToolTab::setNameROI(int id, std::string name)
{
    mModelROI->setNameROI(id, name);
}

void ControllerROIToolTab::loadROIbasedOnTractID(int /*tract_id*/)
{

}


void ControllerROIToolTab::removeROI(int id, bool tract_removed)
{
    ModelROISetEvent event = mModelROI->removeROI(id, tract_removed);
    mModelROI->notify(event, this);
}


bool ControllerROIToolTab::saveROI(int id_tract, std::string name_tract, std::string filename)
{
    std::string filename_upper(filename);
    std::string filename_no_ext(filename);
    std::transform(filename_upper.begin(), filename_upper.end(), filename_upper.begin(), [](unsigned char c){ return std::toupper(c); });

    if(filename_upper.compare(filename_upper.size() - 4, filename_upper.size(), ".ROI") != 0)
        filename += ".roi";
    else
        filename_no_ext = filename.substr(0, filename_upper.size() - 4);

    m_modelDTI->getFiberBundleDataByID(id_tract).m_fiberClusterSet->saveFibers(filename_no_ext);
    return mModelROI->save_v2(id_tract, name_tract, filename);
}


void ControllerROIToolTab::loadROI(std::vector<std::string> text)
{
    std::string current_tract_name = m_modelDTI->getFiberBundleName(m_modelDTI->getCurrentFiberBundleEditionIndex());
    short fiber_bundle_id = m_modelDTI->getFiberBundleID(m_modelDTI->getCurrentFiberBundleEditionIndex());

    ModelROISetEvent event = mModelROI->load(text, current_tract_name, fiber_bundle_id);
    mModelROI->notify(event);
}

std::vector<std::string> ControllerROIToolTab::getROInameInformation()
{
    return mModelROI->getROInameInformation();
}


std::vector<int> ControllerROIToolTab::getROItractInformation()
{
    return mModelROI->getROItractIdInfo();
}


std::string ControllerROIToolTab::getROItractName(unsigned int id_roi)
{
    return mModelROI->getROItractNameInfo(id_roi);
}


std::vector<bool> ControllerROIToolTab::getROIActivityInformation()
{
    return mModelROI->getROIActivityInformation();
}


std::vector<int> ControllerROIToolTab::getROIidInformation()
{
    return mModelROI->getROIidInformation();
}


std::vector<ModelROI::roi_type> ControllerROIToolTab::getROITypeInformation()
{
    std::vector<ModelROI::roi_type> types;
    std::vector<int> ids = mModelROI->getROIidInformation();
    for(unsigned int i = 0; i < ids.size(); i++)
    {
        types.push_back(mModelROI->getType(ids[i]));
    }

    return types;
}


void ControllerROIToolTab::setChangeROI(bool change)
{
    ModelROISetEvent event = mModelROI->setModelChange(change);
    mModelROI->notify(event);
}


void ControllerROIToolTab::set2DViewControllerROIAxial(Controller2DViewROI *value)
{
    m2DViewControllerAxialROI = value;
}

void ControllerROIToolTab::set2DViewControllerROICoronal(Controller2DViewROI *value)
{
    m2DViewControllerCoronalROI = value;
}

void ControllerROIToolTab::set2DViewControllerROISagittal(Controller2DViewROI *value)
{
    m2DViewControllerSagittalROI = value;
}

void ControllerROIToolTab::set3DViewControllerROI(Controller3DViewROI *value)
{
    m3DViewControllerROI = value;
}


void ControllerROIToolTab::setCurrentActiveROI_id(unsigned int id)
{
    ModelROISetEvent event = mModelROI->setCurrentActiveROIid(id);
    mModelROI->notify(event, this);
    mRenderingSurfaceSet->update2DRendering();
    mRenderingSurfaceSet->update3DRendering();
}

void ControllerROIToolTab::setROIactivity(int id, bool active)
{
    mModelROI->setROIactivity(active, id);
}

void ControllerROIToolTab::setROIactivityBasedOnTract(int tract_id, bool active)
{
    mModelROI->setROIactivityTractBased(active, tract_id);
}


std::vector<int> ControllerROIToolTab::getActiveROIs()
{
    return mModelROI->getActiveROIs();
}

std::vector<int> ControllerROIToolTab::getActiveROIs(short tract_id)
{
    return mModelROI->getActiveROIs(tract_id);
}

std::vector<int> ControllerROIToolTab::getLoadedROIs()
{
    return mModelROI->getLoadedROIs();
}


int ControllerROIToolTab::getCurrentActiveROI_id()
{
    return mModelROI->getCurrentActiveROIid();
}

void ControllerROIToolTab::getPoints(std::vector<int> ids)
{
    std::vector<int> ids_valid;
    for(unsigned int index = 0; index < ids.size(); index++)
    {
        if(mModelROI->getROIpoints(ids[index]).size() > 0)
        {
            mModelROI->computeIndexes(ids[index]);
            ids_valid.push_back(ids[index]);
        }

    }

    mModelROI->multipleROIindexesComputed(ids_valid);

    std::vector<int> ids_ = mModelROI->getMultipleROIidsVisualization();
    mModelROI->setMultipleIndexes(ids_);
    std::vector<int> indexes = mModelROI->getIndexes_seed_concatenation();
    std::vector<int> not_indexes = mModelROI->getIndexes_not_concatenation();
    std::vector<int> and_indexes = mModelROI->getIndexes_and_concatenation();
    std::vector<int> and_number_index = mModelROI->getNumberFilterAND();
    std::vector<int> not_number_index = mModelROI->getNumberFilterNOT();

    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setROISeedAndFilters(indexes, and_indexes, and_number_index, not_indexes, not_number_index);

    m_modelDTI->notify();
}

void ControllerROIToolTab::generateGlobalTractographyFromROIs(std::vector<int> ids)
{
    std::vector<int> ids_valid;
    for(unsigned int index = 0; index < ids.size(); index++)
    {
        if(mModelROI->getROIpoints(ids[index]).size() > 0)
        {
            mModelROI->computeIndexes(ids[index]);
            ids_valid.push_back(ids[index]);
        }

    }

    mModelROI->multipleROIindexesComputed(ids_valid);

    std::vector<int> ids_ = mModelROI->getMultipleROIidsVisualization();
    mModelROI->setMultipleIndexes(ids_);
    std::vector<int> not_indexes = mModelROI->getIndexes_not_concatenation();
    std::vector<int> and_indexes = mModelROI->getIndexes_and_concatenation();
    std::vector<int> and_number_index = mModelROI->getNumberFilterAND();
    std::vector<int> not_number_index = mModelROI->getNumberFilterNOT();

    mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

    m_modelDTI->setFilters(and_indexes, and_number_index, not_indexes, not_number_index);
    m_modelDTI->notify();
}

void ControllerROIToolTab::setROIType(ModelROI::roi_type type)
{
    unsigned int id  = mModelROI->getCurrentActiveROIid();
    ModelROISetEvent event = mModelROI->setType(id, type);
    mModelROI->notify(event, this);
}

void ControllerROIToolTab::controllerROIchangePertinence(bool state)
{
    if(state)
    {
        //Add Controllers
        mRenderingSurfaceSet->getRenderingSurfaceAxial()->addController(m2DViewControllerAxialROI);
        mRenderingSurfaceSet->getRenderingSurfaceCoronal()->addController(m2DViewControllerCoronalROI);
        mRenderingSurfaceSet->getRenderingSurfaceSagittal()->addController(m2DViewControllerSagittalROI);
    }
    else
    {
        //Remove Controllers
        mRenderingSurfaceSet->getRenderingSurfaceAxial()->removeController(m2DViewControllerAxialROI);
        mRenderingSurfaceSet->getRenderingSurfaceCoronal()->removeController(m2DViewControllerCoronalROI);
        mRenderingSurfaceSet->getRenderingSurfaceSagittal()->removeController(m2DViewControllerSagittalROI);
    }

}

void ControllerROIToolTab::setEventHandlerROI(EventHandlerROI *eventHandlerROI)
{
    m_eventHandlerROI = eventHandlerROI;
}

void ControllerROIToolTab::setRenderingController(bool state)
{
    if(state)
    {
        m_eventHandlerROI->setActivationFreeDrawROI(true);

        m2DViewControllerAxialROI->setActivationFreeDrawROI(true);
        m2DViewControllerCoronalROI->setActivationFreeDrawROI(true);
        m2DViewControllerSagittalROI->setActivationFreeDrawROI(true);

        if(m3DViewControllerROI)
            m3DViewControllerROI->setActivationFreeDraw3D(true);
    }
    else
    {
        m_eventHandlerROI->setActivationFreeDrawROI(false);

        m2DViewControllerAxialROI->setActivationFreeDrawROI(false);
        m2DViewControllerCoronalROI->setActivationFreeDrawROI(false);
        m2DViewControllerSagittalROI->setActivationFreeDrawROI(false);

        if(m3DViewControllerROI)
                m3DViewControllerROI->setActivationFreeDraw3D(false);
    }
}
