#pragma once

#ifndef CONTROLLERROITOOLTAB_H
#define CONTROLLERROITOOLTAB_H

#include "../ModelROI.h"

class Controller3DViewROI;
class Controller2DViewROI;
class RenderingSurfaceSet;
class ModelDTI;
class EventHandlerROI;

class ControllerROIToolTab:
        public ControllerTool,
        public ModelObserver<ModelROI, ModelROISetEvent>
{
public:
    ControllerROIToolTab();

    void update(ModelROI *subject, ModelROISetEvent event);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setFreeDrawROI(bool state);

    bool saveROI(int id_tract, std::string name_tract, std::string filename);
    void loadROI(std::vector<std::string> text);

    void setModelROI(ModelROI * roi);
    void setModelDTI(ModelDTI * dti);

    void set2DViewControllerROIAxial(Controller2DViewROI *value);
    void set2DViewControllerROICoronal(Controller2DViewROI *value);
    void set2DViewControllerROISagittal(Controller2DViewROI *value);

    void set3DViewControllerROI(Controller3DViewROI *value);

    int  createNewROI(ModelROI::roi_type type, std::string tract_name, int tract_id);
    void setCurrentActiveROI_id(unsigned int id);
    int  getCurrentActiveROI_id();
    void removeROI(int id, bool tract_removed);
    void setNameROI(int id, std::string name);
    void setTractROI(int id, std::string tract_name, int tract_id);
    void loadROIbasedOnTractID(int tract_id);
    void setROIType(ModelROI::roi_type type);

    std::vector<std::string>        getROInameInformation();
    std::vector<int>                getROIidInformation();
    std::vector<ModelROI::roi_type> getROITypeInformation();
    std::vector<int>                getROItractInformation();

    bool getFreeDrawControllerState();
    void getPoints(std::vector<int> ids);
    void generateGlobalTractographyFromROIs(std::vector<int> ids);

    void setRenderingController(bool state);

    void setROIactivity(int id, bool visible);
    void setROIactivityBasedOnTract(int tract_id, bool visible);
    std::vector<bool> getROIActivityInformation();
    std::vector<int>  getActiveROIs();
    std::vector<int>  getActiveROIs(short tract_id);
    std::string       getROItractName(unsigned int id_roi);
    std::vector<int>  getLoadedROIs();

    void setChangeROI(bool change);
    void setROIModelChange(bool set);

    void setEventHandlerROI(EventHandlerROI *eventHandlerROI);

private:
    void controllerROIchangePertinence(bool state);

    RenderingSurfaceSet *mRenderingSurfaceSet;

    //Model
    ModelROI *mModelROI;
    ModelDTI *m_modelDTI;
    /* New 2D Controller for ROI application */
    Controller2DViewROI *m2DViewControllerAxialROI;
    Controller2DViewROI *m2DViewControllerCoronalROI;
    Controller2DViewROI *m2DViewControllerSagittalROI;

    EventHandlerROI *m_eventHandlerROI;

    /* New 3D Controller for ROI Application */
    Controller3DViewROI *m3DViewControllerROI;

    bool mEnableAutomaticRender;
};

#endif // CONTROLLERROITOOLTAB_H
