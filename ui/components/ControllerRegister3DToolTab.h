#ifndef CONTROLLER_REGISTER_3D_TOOLTAB_H
#define CONTROLLER_REGISTER_3D_TOOLTAB_H

//#include "IToolTab3DController.h"
//#include "model/ModelObserver.h"
//#include "model/Model3DTransformation.h"
//#include "model/Model3DClipping.h"

//class Model2D3DGUIState;

//class ControllerRegister3DToolTab:
//        public ModelObserver<Model3DClipping, Model3DClippingEvent>,
//        public vmtk::IToolTab3DClippingController
//{
//public:
//    ControllerRegister3DToolTab();
//    ~ControllerRegister3DToolTab() {}

//    void update(Model3DClipping *subject, Model3DClippingEvent event_type);

//    void setLeftClipping(int value);
//    void setRightClipping(int value);
//    void setBottomClipping(int value);
//    void setTopClipping(int value);
//    void setBackClipping(int value);
//    void setFrontClipping(int value);

//    void getClippingIntervals(int& left_right_min, int& left_right_max, int& bottom_top_min, int& bottom_top_max, int& back_front_min, int& back_front_max);
//    void getClippingData(int& clipLeft, int& clipRight, int& clipBottom, int& clipTop, int& clipBack, int& clipFront);

//    void setModel3DClipping(Model3DClipping *model3DClipping);

//private:
//    Model3DClipping *m_model3DClipping;
//};

////======================================================================================================================================================================

//class ControllerRegister3DTransformationToolTab:
//        public ModelObserver<Model3DTransformation, Model3DTransformationEvent>,
//        public vmtk::IToolTab3DTransformationController
//{
//public:
//    ControllerRegister3DTransformationToolTab();
//    ~ControllerRegister3DTransformationToolTab() {}

//    void update(Model3DTransformation *subject, Model3DTransformationEvent event_type);

//    void setMouseMode(short mode);
//    short int getMouseMode();
//    void reset3DTransformations();

//    void getThresholdData(int& minThreshold, int& maxThreshold, int& valueThreshold, bool& stateThreshold);
//    void setThresholdState(bool state);
//    void setThreshold(int threshold);

//    void setModel3D(Model3DTransformation *model3D);

//private:
//    Model3DTransformation *m_model3DTransformation;
//};

#endif // CONTROLLER_REGISTER_3D_TOOLTAB_H
