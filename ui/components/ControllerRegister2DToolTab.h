#ifndef CONTROLLER_REGISTER_2D_TOOL_TAB_H
#define CONTROLLER_REGISTER_2D_TOOL_TAB_H

//#include "IToolTab2DController.h"
//#include "model/ModelObserver.h"
//#include "model/Model2DSlice.h"
//#include "model/Model2DTransformation.h"

//#include <MultiplanarReformation/model/ModelMPR.h>

//#include <model/SliceSynchronizer.h>

//class Model2D3DGUIState;

//class ControllerRegister2DToolTab: public vmtk::IToolTab2DMPRController
//{
//public:
//    ControllerRegister2DToolTab();

//    short getMouseModeMultiplanar();
//    bool isMultiplanarEnabled();

//    void setMouseModeMultiplanar(short mode);
//    void setMultiplanarReformatingEnabled(bool state);

//    void setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState);
//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
//    void setModelMPR(ModelMPR *modelMPR);

//private:
//    void reset2DTransformations();

//    ModelMPR *m_modelMPR;
//    Model2D3DGUIState *m_model2D3DGUIState;
//    Model2DTransformation *m_model2DTransformation;
//};

//class ControllerRegister2DCursorToolTab: public vmtk::IToolTab2DCursorController
//{
//public:
//    ControllerRegister2DCursorToolTab();

//    bool isCursorEnabled();
//    void setCursorEnabled(bool state);

//    void setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState);
//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

//private:
//    void reset2DTransformations();

//    Model2D3DGUIState *m_model2D3DGUIState;
//    Model2DTransformation *m_model2DTransformation;
//};

//class ControllerRegister2DTransformationToolTab: public vmtk::IToolTab2DTransformationController
//{
//public:
//    ControllerRegister2DTransformationToolTab();

//    short getMouseModeTransformation();
//    void setMouseModeTransformations(short mode);
//    void reset2DTransformations();

//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

//private:
//    Model2DTransformation *m_model2DTransformation;
//};

//class ControllerRegister2DSliceToolTab: public ModelObserver<Model2DSlice, Model2DSliceEvent>, public vmtk::IToolTab2DSliceController
//{
//public:
//    ControllerRegister2DSliceToolTab();

//    void update(Model2DSlice *subject, Model2DSliceEvent event_type);

//    void resetSlices();
//    void setAxialSlice(int value);
//    void setCoronalSlice(int value);
//    void setSagittalSlice(int value);

//    void setSyncState(bool state);

//    void setRadiologicalMode();
//    void setNeurologicalMode();

//    void getSlicingIntervals(int& axial_min, int& axial_max, int& coronal_min, int& coronal_max, int& sagittal_min, int& sagittal_max);
//    void getSlicingData(int& axial, int& coronal, int& sagittal, bool &sync_state);

//    bool isRadiological();
//    bool isNeurological();

//    void setModel2DSlice(Model2DSlice *model);
//    void setSliceSynchronizer(SliceSynchronizer *slice_synchronizer);

//private:
//    Model2DSlice *m_model2DSlice;
//    SliceSynchronizer *m_sliceSynchronizer;
//};

#endif // CONTROLLER_REGISTER_2D_TOOL_TAB_H
