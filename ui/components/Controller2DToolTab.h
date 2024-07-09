#ifndef CONTROLLER_2D_TOOL_TAB_H
#define CONTROLLER_2D_TOOL_TAB_H

//#include "IToolTab2DController.h"
//#include "model/ModelObserver.h"
//#include "model/Model2DSlice.h"
//#include "model/Model2DTransformation.h"
//#include "Cursor/model/ModelCursor.h"
//#include "modules/visualization/common/VisSubModuleCursor/ModelCursor.h"
//#include "MultiplanarReformation/model/ModelMPR.h"
//#include "modules/visualization/common/VisSubModule2DSlice/SliceSynchronizer.h"
//#include "modules/visualization/common/VisSubModule2DTransformation/Model2DTransformation.h"
//#include "modules/visualization/common/VisSubModuleMPR/ModelMPR.h"
//#include "model/SliceSynchronizer.h"


//class Controller2DMPRToolTab:
//        public vmtk::IToolTab2DMPRController,
//        public ModelObserver<ModelMPR, ModelMPREvent>
//{
//public:
//    Controller2DMPRToolTab();

//    void update(ModelMPR*, ModelMPREvent event);

//    short getMouseModeMultiplanar();
//    bool isMultiplanarActivated();

//    void setMouseModeMultiplanar(short mode);
//    void setMultiplanarReformatingActivated(bool checked);

//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
//    void setModelMPR(ModelMPR *modelMPR);

//private:
//    void reset2DTransformations();

//    ModelMPR *m_modelMPR;
//    Model2DTransformation *m_model2DTransformation;
//};

//class Controller2DCursorToolTab:
//        public vmtk::IToolTab2DCursorController
//{
//public:
//    Controller2DCursorToolTab();

//    void handleCursorStateChangedEvent(ModelCursor *, ModelCursorEvent event);
//    void handleCursorModeChangedEvent(ModelCursor *, ModelCursorEvent event);

//    bool isCursorActivated();
//    void setCursorActivated(bool state);

//    void setCursorMovable();
//    void setCursorLocked();

//    bool isCursorMovable();
//    bool isCursorLocked();
////    void setCursorLocked(bool state);

//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
//    void setModelCursor(ModelCursor *modelCursor);

//private:
//    void reset2DTransformations();

//    ModelCursor *m_modelCursor;
//    int m_cursorStateChangedHandlerSlotID;
//    int m_cursorModeChangedHandlerSlotID;
//    Model2DTransformation *m_model2DTransformation;
//};

//class Controller2DTransformationToolTab:
//        public vmtk::IToolTab2DTransformationController,
//        public ModelObserver<Model2DTransformation, Model2DTransformationEvent>
//{
//public:
//    Controller2DTransformationToolTab();

//    void update(Model2DTransformation*, Model2DTransformationEvent event);

//    short getMouseModeTransformation();
//    void setMouseModeTransformations(short mode);
//    void reset2DTransformations();

//    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

//private:
//    Model2DTransformation *m_model2DTransformation;
//};

//class Controller2DSliceToolTab:
//        public vmtk::IToolTab2DSliceController,
//        public ModelObserver<Model2DSlice, Model2DSliceEvent>,
//        public ModelObserver<SliceSynchronizer, SliceSynchronizerEvent>
//{
//public:
//    Controller2DSliceToolTab();
//    ~Controller2DSliceToolTab();

//    void update(Model2DSlice *subject, Model2DSliceEvent event_type);
//    void update(SliceSynchronizer *subject, SliceSynchronizerEvent event_type);

//    void resetSlices();
//    void setAxialSlice(int value);
//    void setCoronalSlice(int value);
//    void setSagittalSlice(int value);

//    void setSliceSyncronizationActivated(bool state);

//    void setRadiologicalMode();
//    void setNeurologicalMode();

//    void getSlicingIntervals(int& axial_min, int& axial_max, int& coronal_min, int& coronal_max, int& sagittal_min, int& sagittal_max);
//    void getSlicingData(int& axial, int& coronal, int& sagittal, bool &sync_state);

//    bool isRadiological();
//    bool isNeurological();

//    bool isOrientationEnabled();
//    bool is2DSliceEnabled();
//    bool isSliceSyncronizationEnabled();

//    void setModel2DSlice(Model2DSlice *model);
//    void setSliceSynchronizer(SliceSynchronizer *slice_synchronizer);

//private:
//    Model2DSlice *m_model2DSlice;
//    SliceSynchronizer *m_sliceSynchronizer;
//};

#endif // CONTROLLER_2D_TOOL_TAB_H
