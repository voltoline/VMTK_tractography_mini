#include "Controller2DToolTab.h"

//Controller2DMPRToolTab::Controller2DMPRToolTab()
//{

//}

//void Controller2DMPRToolTab::update(ModelMPR *, ModelMPREvent event)
//{
//    if(event == ModelMPREvent::MULTIPLANAR_STATE_CHANGED)
//    {
//        setUIEnabled(m_modelMPR->isEnabled());
//        updateUI();
//    }
//}

//void Controller2DMPRToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//}

//short Controller2DMPRToolTab::getMouseModeMultiplanar()
//{
//    switch(m_modelMPR->getMouseMode())
//    {
//    case ModelMPR::MouseMode::ROTATION:
//        return -6;

//    case ModelMPR::MouseMode::MOVE:
//        return -5;

//    default:
//        return 0;
//    }
//}

//bool Controller2DMPRToolTab::isMultiplanarActivated()
//{
//    return m_modelMPR->isActivated();
//}

//void Controller2DMPRToolTab::setMouseModeMultiplanar(short mode)
//{
//    ModelMPREvent event;
//    switch(mode)
//    {
//    case -6:
//        event = m_modelMPR->setMouseMode(ModelMPR::MouseMode::ROTATION);
//        break;

//    case -5:
//        event = m_modelMPR->setMouseMode(ModelMPR::MouseMode::MOVE);
//        break;

//    default:
//        return;

//    }

//    m_modelMPR->notify(event, this);
//}

//void Controller2DMPRToolTab::setMultiplanarReformatingActivated(bool checked)
//{
//    if(checked)
//        reset2DTransformations();

//    ModelMPREvent event = m_modelMPR->setActivated(checked);
//    m_modelMPR->notify(event, this);
//}

//void Controller2DMPRToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event);
//}

//void Controller2DMPRToolTab::setModelMPR(ModelMPR *modelMPR)
//{
//    m_modelMPR = modelMPR;
//    m_modelMPR->subscribe(this, ModelMPREvent::MULTIPLANAR_STATE_CHANGED);
//}

//====================================================================================================================================================

//Controller2DCursorToolTab::Controller2DCursorToolTab()
//{
//    m_cursorStateChangedHandlerSlotID = -1;
//    m_cursorModeChangedHandlerSlotID = -1;
//}

//void Controller2DCursorToolTab::handleCursorStateChangedEvent(ModelCursor *, ModelCursorEvent event)
//{
//    if(event == ModelCursorEvent::CURSOR_STATE_CHANGED)
//    {
//        setUIEnabled(m_modelCursor->isEnabled());
//        updateUI();
//    }
//}

//void Controller2DCursorToolTab::handleCursorModeChangedEvent(ModelCursor *, ModelCursorEvent event)
//{
//    if(event == ModelCursorEvent::CURSOR_MODE_CHANGED)
//        updateUI();
//}

//void Controller2DCursorToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//}

//bool Controller2DCursorToolTab::isCursorActivated()
//{
//    return m_modelCursor->isActivated();
//}

//void Controller2DCursorToolTab::setCursorActivated(bool state)
//{
//    if(state)
//        reset2DTransformations();

//    ModelCursorEvent event = m_modelCursor->setActivated(state);
//    m_modelCursor->notify(event, m_cursorStateChangedHandlerSlotID, m_modelCursor, event);
//}

//void Controller2DCursorToolTab::setCursorMovable()
//{
//    reset2DTransformations();

//    ModelCursorEvent event = m_modelCursor->setCursorMode(ModelCursor::Mode::MOVE);
//    m_modelCursor->notify(event, m_cursorModeChangedHandlerSlotID, m_modelCursor, event);
//}

//void Controller2DCursorToolTab::setCursorLocked()
//{
//    ModelCursorEvent event = m_modelCursor->setCursorMode(ModelCursor::Mode::LOCK);
//    m_modelCursor->notify(event, m_cursorModeChangedHandlerSlotID, m_modelCursor, event);
//}

//bool Controller2DCursorToolTab::isCursorMovable()
//{
//    return m_modelCursor->isMovable();
//}

//bool Controller2DCursorToolTab::isCursorLocked()
//{
//    return m_modelCursor->isLocked();
//}

////void Controller2DCursorToolTab::setCursorLocked(bool state)
////{
////    ModelCursorEvent event = m_modelCursor->setLocked(state);
////    m_modelCursor->notify(event, m_cursorLockStateChangedHandlerSlotID, m_modelCursor, event);
////}

//void Controller2DCursorToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event);
//}

//void Controller2DCursorToolTab::setModelCursor(ModelCursor *modelCursor)
//{
//    m_modelCursor = modelCursor;
//    m_cursorStateChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_STATE_CHANGED, CREATE_SLOT(this, &Controller2DCursorToolTab::handleCursorStateChangedEvent));
//    m_cursorModeChangedHandlerSlotID = m_modelCursor->subscribe(ModelCursorEvent::CURSOR_MODE_CHANGED, CREATE_SLOT(this, &Controller2DCursorToolTab::handleCursorModeChangedEvent));
//}

//====================================================================================================================================================

//Controller2DTransformationToolTab::Controller2DTransformationToolTab()
//{

//}

//void Controller2DTransformationToolTab::update(Model2DTransformation *, Model2DTransformationEvent event)
//{
//    if(event == Model2DTransformationEvent::_2D_TRANSFORMATION_STATE_CHANGED)
//    {
//        set2DTransformationUIEnabled(m_model2DTransformation->isTransformationActivated());
//    }
//}

//void Controller2DTransformationToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//    m_model2DTransformation->subscribe(this, Model2DTransformationEvent::_2D_TRANSFORMATION_STATE_CHANGED);

//    set2DTransformationUIEnabled(m_model2DTransformation->isTransformationActivated());
//}

//short Controller2DTransformationToolTab::getMouseModeTransformation()
//{
//    switch(m_model2DTransformation->getMouseMode())
//    {
//    case Model2DTransformation::MouseMode::ROTATION:
//        return -2;

//    case Model2DTransformation::MouseMode::ZOOM:
//        return -3;

//    case Model2DTransformation::MouseMode::MOVE:
//        return -4;

//    default:
//        return 0;
//    }
//}

//void Controller2DTransformationToolTab::setMouseModeTransformations(short mode)
//{
//    Model2DTransformationEvent event ;
//    switch(mode)
//    {
//    case -2:
//        event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::ROTATION);
//        break;

//    case -3:
//        event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::ZOOM);
//        break;

//    case -4:
//        event = m_model2DTransformation->setMouseMode(Model2DTransformation::MouseMode::MOVE);
//        break;

//    default:
//        return;
//    }

//    m_model2DTransformation->notify(event, this);
//}

//void Controller2DTransformationToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event, this);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event, this);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event, this);
//}

//====================================================================================================================================================

//Controller2DSliceToolTab::Controller2DSliceToolTab()
//{
//    m_model2DSlice = nullptr;
//}

//Controller2DSliceToolTab::~Controller2DSliceToolTab()
//{
//    if(m_model2DSlice != nullptr)
//        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);

//}

//void Controller2DSliceToolTab::update(Model2DSlice *, Model2DSliceEvent event_type)
//{
//    if(event_type == Model2DSliceEvent::SLICING_CHANGED)
//    {
////        updateToolTab2DSlicingData();
//        updateUI();
//    }
//}

//void Controller2DSliceToolTab::update(SliceSynchronizer *, SliceSynchronizerEvent event_type)
//{
//    if(event_type == SliceSynchronizerEvent::SYNC_STATE_CHANGED)
//    {
//        updateUI();
//    }
//}

//void Controller2DSliceToolTab::setModel2DSlice(Model2DSlice *model)
//{
//    if(m_model2DSlice == model)
//        return;

//    if(m_model2DSlice != nullptr)
//        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);

//    m_model2DSlice = model;

//    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);
//}

//void Controller2DSliceToolTab::setSliceSynchronizer(SliceSynchronizer *slice_synchronizer)
//{
//    m_sliceSynchronizer = slice_synchronizer;
//    m_sliceSynchronizer->subscribe(this, SliceSynchronizerEvent::SYNC_STATE_CHANGED);
//}

//void Controller2DSliceToolTab::resetSlices()
//{
//    Model2DSliceEvent event = m_model2DSlice->resetSlices();
//    m_model2DSlice->notify(event);
//}

//void Controller2DSliceToolTab::setAxialSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setAxialSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void Controller2DSliceToolTab::setCoronalSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setCoronalSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void Controller2DSliceToolTab::setSagittalSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setSagittalSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void Controller2DSliceToolTab::setSliceSyncronizationActivated(bool state)
//{
//    SliceSynchronizerEvent event = m_sliceSynchronizer->setSynchronizationActivated(state);
//    m_sliceSynchronizer->notify(event, this);
//}

//void Controller2DSliceToolTab::setRadiologicalMode()
//{
//    Model2DSliceEvent event = m_model2DSlice->setRadiologicalVoxelOrder();
//    m_model2DSlice->notify(event, this);
//}

//void Controller2DSliceToolTab::setNeurologicalMode()
//{
//    Model2DSliceEvent event = m_model2DSlice->setNeurologicalVoxelOrder();
//    m_model2DSlice->notify(event, this);
//}

//void Controller2DSliceToolTab::getSlicingIntervals(int &axial_min, int &axial_max, int &coronal_min, int &coronal_max, int &sagittal_min, int &sagittal_max)
//{
//    unsigned short sagittal_min_;
//    unsigned short sagittal_max_;
//    unsigned short coronal_min_;
//    unsigned short coronal_max_;
//    unsigned short axial_min_;
//    unsigned short axial_max_;

//    m_model2DSlice->getAxialInterval(axial_min_, axial_max_);
//    m_model2DSlice->getCoronalInterval(coronal_min_, coronal_max_);
//    m_model2DSlice->getSagittalInterval(sagittal_min_, sagittal_max_);

//    sagittal_min = sagittal_min_;
//    sagittal_max = sagittal_max_;
//    coronal_min = coronal_min_;
//    coronal_max = coronal_max_;
//    axial_min = axial_min_;
//    axial_max = axial_max_;
//}

//void Controller2DSliceToolTab::getSlicingData(int &axial, int &coronal, int &sagittal, bool &sync_state)
//{
//    axial = m_model2DSlice->getAxialSlice();
//    coronal = m_model2DSlice->getCoronalSlice();
//    sagittal = m_model2DSlice->getSagittalSlice();
//    sync_state = m_sliceSynchronizer->isSynchronizationActivated();
//}

//bool Controller2DSliceToolTab::isRadiological()
//{
//    return m_model2DSlice->isRadiological();
//}

//bool Controller2DSliceToolTab::isNeurological()
//{
//    return m_model2DSlice->isNeurological();
//}

//bool Controller2DSliceToolTab::isOrientationEnabled()
//{
//    return m_model2DSlice->isActivated();
//}

//bool Controller2DSliceToolTab::is2DSliceEnabled()
//{
//    return m_model2DSlice->isActivated();
//}

//bool Controller2DSliceToolTab::isSliceSyncronizationEnabled()
//{
//    return m_sliceSynchronizer->isEnabled();
//}
