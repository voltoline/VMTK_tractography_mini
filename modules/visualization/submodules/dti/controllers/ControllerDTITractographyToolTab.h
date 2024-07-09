#ifndef CONTROLLER_DTI_TRACTOGRAPHY_TOOL_TAB_H
#define CONTROLLER_DTI_TRACTOGRAPHY_TOOL_TAB_H

#include "model/ModelObserver.h"
#include <core/ToolBase.h>

class RenderingSurfaceSet;
class ModelDTI;
class ModelUIStateDTI;

enum class ModelDTIEvent;

class ControllerDTITractographyToolTab:
        public ModelObserver<ModelDTI, ModelDTIEvent>,
        public ControllerTool
{
public:
    ControllerDTITractographyToolTab();
    ~ControllerDTITractographyToolTab() {}

    void update(ModelDTI *subject, ModelDTIEvent event);

    void computeGlobalTractography();
    void computeTractographyFromROIs();

    void getTrackingParameters(unsigned int &fiber_length_min, unsigned int &fiber_length_max,
                               unsigned short &stopping_criterion_angle, float &stopping_criterion_fa,
                               float &seed_creation_fa, float &wpunct);

    void getROIPlaneFilterParameters(bool &roi_plane_filter_activated, int &roi_angle_filter);

    void setFiberLengthMin(unsigned int fiber_length_min);
    void setFiberLengthMax(unsigned int fiber_length_max);
    void setStoppingCriteriaAngle(unsigned short stopping_criterion_angle);
    void setStoppingCriteriaFA(float stopping_criterion_fa);
    void setSeedCreationParameters(float seed_creation_fa);
    void setWpunctParameter(float wpunct);
    void setROIPlaneAngle(float value);
    void setActivatedROIPlaneFilter(bool value);

    void addFiberBundle(bool is_visible, std::string tract_name, bool current_edition);
    void deleteFiberBundle(unsigned short fiber_bundle_index);
    short getCurrentFiberBundleEditinIndex();
    void setCurrentFiberBundleEditinIndex(short fiber_bundle_index);
    void setFiberBundleVisible(short fiber_bundle_index, bool visible);
    bool isFiberBundleVisible(short fiber_bundle_index);
    bool isFiberBundleReconstructed(short fiber_bundle_index);
    unsigned short getFiberBundlesCount();
    void setFiberBundleName(short fiber_bundle_index, std::string tract_name);
    std::string getFiberBundleName(short fiber_bundle_index);
    short getFiberBundleID(short fiber_bundle_index);
    void getFiberBundleTrackingParameters(short fiber_bundle_index,
                                          unsigned int &fiber_length_min, unsigned int &fiber_length_max,
                                          unsigned short &stopping_criterion_angle, float &stopping_criterion_fa,
                                          float &seed_creation_fa, float &wpunct);

    void enableROIBasedFiberTracking(bool state);

    void setAutomaticROIFiberTrackingEnabled(bool state);
    bool isAutomaticROIFiberTrackingEnabled();

    bool isFiberBundleUpdated(unsigned short fiber_bundle_index);

    bool isTractographyEnabled();
    bool isTractographyActivated();
    bool isDTIComputed();

    void setTractographyActivated(bool state);

    void setModelDTI(ModelDTI *modelDTI);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModelUIStateDTI(ModelUIStateDTI *modelUIStateDTI);

private:
    ModelDTI *m_modelDTI;
    ModelUIStateDTI *m_modelUIStateDTI;
    RenderingSurfaceSet *mRenderingSurfaceSet;
    bool m_enable_DTI;
    bool m_isTractographyEnabled;
};

#endif // CONTROLLER_DTI_TRACTOGRAPHY_TOOL_TAB_H
