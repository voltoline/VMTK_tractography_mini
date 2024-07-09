#ifndef DTI_EVENT_HANDLER_FOR_ROI_H
#define DTI_EVENT_HANDLER_FOR_ROI_H

#include <model/ModelObserver.cpp>
#include <modules/visualization/submodules/dti/models/ModelDTI.h>

class ModelROI;
class ControllerROIToolTab;
class ROIRenderer2D;
class RenderingSurfaceSet;

class EventHandlerDTIToROI: public ModelObserver<ModelDTI, ModelDTIEvent>
{
public:
    EventHandlerDTIToROI();

    void update(ModelDTI *subject, ModelDTIEvent event);

    void setModelDTI(ModelDTI *modelDTI);
    void setModelROI(ModelROI *modelROI);
    void setControllerROIToolTab(ControllerROIToolTab *controllerROIToolTab);

    void setRoiRenderer2DAxial(ROIRenderer2D *roiRenderer2DAxial);
    void setRoiRenderer2DSagittal(ROIRenderer2D *roiRenderer2DSagittal);
    void setRoiRenderer2DCoronal(ROIRenderer2D *roiRenderer2DCoronal);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

private:
    ControllerROIToolTab *m_controllerROIToolTab;

    ROIRenderer2D *m_roiRenderer2DAxial;
    ROIRenderer2D *m_roiRenderer2DSagittal;
    ROIRenderer2D *m_roiRenderer2DCoronal;

    ModelROI *m_modelROI;
    ModelDTI *m_modelDTI;

    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // DTI_EVENT_HANDLER_FOR_ROI_H
