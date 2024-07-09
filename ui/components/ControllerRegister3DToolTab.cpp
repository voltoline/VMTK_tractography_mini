//#include "ControllerRegister3DToolTab.h"

//#include <model/Model2D3DInteraction.h>

//ControllerRegister3DToolTab::ControllerRegister3DToolTab():
//    m_model3DClipping(nullptr)
//{

//}

//void ControllerRegister3DToolTab::update(Model3DClipping *, Model3DClippingEvent event_type)
//{
//    switch(event_type)
//    {

//    case Model3DClippingEvent::CLIPPING_CHANGED:
//    {
//        updateToolTab3DClippingData();
//        break;
//    }

//    default:
//        break;

//    }
//}


//void ControllerRegister3DToolTab::setModel3DClipping(Model3DClipping *model3DClipping)
//{
//    m_model3DClipping = model3DClipping;
//    m_model3DClipping->subscribe(this, Model3DClippingEvent::CLIPPING_CHANGED);
//}

//void ControllerRegister3DToolTab::setLeftClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setLeftClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::setRightClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setRightClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::setBottomClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setBottomClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::setTopClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setTopClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::setBackClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setBackClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::setFrontClipping(int value)
//{
//    Model3DClippingEvent event = m_model3DClipping->setFrontClipping(value);
//    m_model3DClipping->notify(event, this);
//}

//void ControllerRegister3DToolTab::getClippingIntervals(int &left_right_min, int &left_right_max, int &bottom_top_min, int &bottom_top_max, int &back_front_min, int &back_front_max)
//{
//    unsigned short left_right_min_;
//    unsigned short left_right_max_;
//    unsigned short bottom_top_min_;
//    unsigned short bottom_top_max_;
//    unsigned short back_front_min_;
//    unsigned short back_front_max_;

//    m_model3DClipping->getLeftRightInterval(left_right_min_, left_right_max_);
//    m_model3DClipping->getBottomTopInterval(bottom_top_min_, bottom_top_max_);
//    m_model3DClipping->getBackFrontInterval(back_front_min_, back_front_max_);

//    left_right_min = left_right_min_;
//    left_right_max = left_right_max_;
//    bottom_top_min = bottom_top_min_;
//    bottom_top_max = bottom_top_max_;
//    back_front_min = back_front_min_;
//    back_front_max = back_front_max_;
//}

//void ControllerRegister3DToolTab::getClippingData(int &clipLeft, int &clipRight, int &clipBottom, int &clipTop, int &clipBack, int &clipFront)
//{
//    clipLeft = m_model3DClipping->getLeftClipping();
//    clipRight = m_model3DClipping->getRightClipping();
//    clipBottom = m_model3DClipping->getBottomClipping();
//    clipTop = m_model3DClipping->getTopClipping();
//    clipBack = m_model3DClipping->getBackClipping();
//    clipFront = m_model3DClipping->getFrontClipping();
//}

////======================================================================================================================================================================

//ControllerRegister3DTransformationToolTab::ControllerRegister3DTransformationToolTab():
//    m_model3DTransformation(nullptr)
//{

//}

//void ControllerRegister3DTransformationToolTab::update(Model3DTransformation *, Model3DTransformationEvent event_type)
//{
//    switch(event_type)
//    {
//    case Model3DTransformationEvent::THRESHOLD_DATA_CHANGED:
//    {
//        updateToolTab3DThresholdData();
//        break;
//    }

//    default:
//        break;

//    }
//}

//void ControllerRegister3DTransformationToolTab::setModel3D(Model3DTransformation *model3D)
//{
//    m_model3DTransformation = model3D;
//    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::THRESHOLD_DATA_CHANGED);
//}

//void ControllerRegister3DTransformationToolTab::setMouseMode(short mode)
//{
//    Model3DTransformationEvent event ;
//    switch(mode)
//    {
//    case -2:
//        event = m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::ROTATION);
//        break;

//    case -3:
//        event = m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::ZOOM);
//        break;

//    case -4:
//        event = m_model3DTransformation->setMouseMode(Model3DTransformation::MouseMode::MOVE);
//        break;
//    }

//    m_model3DTransformation->notify(event);
//}

//short ControllerRegister3DTransformationToolTab::getMouseMode()
//{
//    switch(m_model3DTransformation->getMouseMode())
//    {
//    case Model3DTransformation::MouseMode::ROTATION:
//        return -2;

//    case Model3DTransformation::MouseMode::ZOOM:
//        return -3;

//    case Model3DTransformation::MouseMode::MOVE:
//        return -4;

//    default:
//        return 0;

//    }
//}

//void ControllerRegister3DTransformationToolTab::reset3DTransformations()
//{
//    Model3DTransformationEvent event = m_model3DTransformation->resetTransformations();
//    m_model3DTransformation->notify(event);
//}

//void ControllerRegister3DTransformationToolTab::getThresholdData(int &minThreshold, int& maxThreshold, int &valueThreshold, bool &stateThreshold)
//{
//    m_model3DTransformation->getThresholdInterval(minThreshold, maxThreshold);
//    valueThreshold = m_model3DTransformation->getThreshold();
//    stateThreshold = m_model3DTransformation->getThresholdState();
//}

//void ControllerRegister3DTransformationToolTab::setThresholdState(bool state)
//{
//    Model3DTransformationEvent event = m_model3DTransformation->setThresholdState(state);
//    m_model3DTransformation->notify(event, this);
//}

//void ControllerRegister3DTransformationToolTab::setThreshold(int threshold)
//{
//    Model3DTransformationEvent event = m_model3DTransformation->setThreshold(threshold);
//    m_model3DTransformation->notify(event, this);
//}
