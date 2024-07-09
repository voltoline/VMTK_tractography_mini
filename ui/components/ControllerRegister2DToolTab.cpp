//#include "ControllerRegister2DToolTab.h"

//#include "model/Model2DSlice.h"
//#include <model/Model2D3DInteraction.h>

//ControllerRegister2DToolTab::ControllerRegister2DToolTab()
//{

//}

//void ControllerRegister2DToolTab::setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState)
//{
//    m_model2D3DGUIState = model2D3DGUIState;
//}

//void ControllerRegister2DToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//}

//short ControllerRegister2DToolTab::getMouseModeMultiplanar()
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

//bool ControllerRegister2DToolTab::isMultiplanarEnabled()
//{
//    return m_model2D3DGUIState->isMultiPlanarReformatingEnabled();
//}

//void ControllerRegister2DToolTab::setMouseModeMultiplanar(short mode)
//{
//        ModelMPREvent event;
//        switch(mode)
//        {
//        case -6:
//            event = m_modelMPR->setMouseMode(ModelMPR::MouseMode::ROTATION);
//            break;

//        case -5:
//            event = m_modelMPR->setMouseMode(ModelMPR::MouseMode::MOVE);
//            break;

//        default:
//            return;

//        }

//        m_modelMPR->notify(event);
//}

//void ControllerRegister2DToolTab::setMultiplanarReformatingEnabled(bool state)
//{
//    if(state)
//    {
//        reset2DTransformations();

//        Model2D3DGUIStateEvent event = m_model2D3DGUIState->setMouseMode2D(Model2D3DGUIState::MouseMode2D::MULTIPLANAR);
//        m_model2D3DGUIState->notify(event);

//    } else {

//        Model2D3DGUIStateEvent event = m_model2D3DGUIState->setMouseMode2D(Model2D3DGUIState::MouseMode2D::TRANSFORMATION);
//        m_model2D3DGUIState->notify(event);
//    }
//}

//void ControllerRegister2DToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event);
//}

//void ControllerRegister2DToolTab::setModelMPR(ModelMPR *modelMPR)
//{
//    m_modelMPR = modelMPR;
//}

////======================================================================================================================================================

//ControllerRegister2DCursorToolTab::ControllerRegister2DCursorToolTab()
//{

//}

//void ControllerRegister2DCursorToolTab::setModel2D3DGUIState(Model2D3DGUIState *model2D3DGUIState)
//{
//    m_model2D3DGUIState = model2D3DGUIState;
//}

//void ControllerRegister2DCursorToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//}

//void ControllerRegister2DCursorToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event);
//}

//bool ControllerRegister2DCursorToolTab::isCursorEnabled()
//{
//    return m_model2D3DGUIState->isCursorEnabled();
//}

//void ControllerRegister2DCursorToolTab::setCursorEnabled(bool state)
//{
//    if(state)
//    {
//        reset2DTransformations();

//        Model2D3DGUIStateEvent event = m_model2D3DGUIState->setMouseMode2D(Model2D3DGUIState::MouseMode2D::CURSOR);
//        m_model2D3DGUIState->notify(event);

//    } else {

//        Model2D3DGUIStateEvent event = m_model2D3DGUIState->setMouseMode2D(Model2D3DGUIState::MouseMode2D::TRANSFORMATION);
//        m_model2D3DGUIState->notify(event);
//    }
//}

////======================================================================================================================================================

//ControllerRegister2DTransformationToolTab::ControllerRegister2DTransformationToolTab()
//{

//}

//void ControllerRegister2DTransformationToolTab::setModel2DTransformation(Model2DTransformation *model2DTransformation)
//{
//    m_model2DTransformation = model2DTransformation;
//}

//short ControllerRegister2DTransformationToolTab::getMouseModeTransformation()
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

//void ControllerRegister2DTransformationToolTab::setMouseModeTransformations(short mode)
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

//    m_model2DTransformation->notify(event);
//}

//void ControllerRegister2DTransformationToolTab::reset2DTransformations()
//{
//    Model2DTransformationEvent event = m_model2DTransformation->resetAxialTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetCoronalTransformations();
//    m_model2DTransformation->notify(event);

//    event = m_model2DTransformation->resetSagittalTransformations();
//    m_model2DTransformation->notify(event);
//}

////======================================================================================================================================================

//ControllerRegister2DSliceToolTab::ControllerRegister2DSliceToolTab()
//{

//}

//void ControllerRegister2DSliceToolTab::update(Model2DSlice *, Model2DSliceEvent event_type)
//{
//    if(event_type == Model2DSliceEvent::SLICING_CHANGED)
//    {
//        updateToolTab2DSlicingData();
//    }
//}

//void ControllerRegister2DSliceToolTab::setModel2DSlice(Model2DSlice *model2DSlice)
//{
//    m_model2DSlice = model2DSlice;
//    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);
//}

//void ControllerRegister2DSliceToolTab::setSliceSynchronizer(SliceSynchronizer *slice_synchronizer)
//{
//    m_sliceSynchronizer = slice_synchronizer;
//}

//void ControllerRegister2DSliceToolTab::resetSlices()
//{
//    Model2DSliceEvent event = m_model2DSlice->resetSlices();
//    m_model2DSlice->notify(event);
//}

//void ControllerRegister2DSliceToolTab::setAxialSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setAxialSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void ControllerRegister2DSliceToolTab::setCoronalSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setCoronalSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void ControllerRegister2DSliceToolTab::setSagittalSlice(int value)
//{
//    Model2DSliceEvent event = m_model2DSlice->setSagittalSlice(value);
//    m_model2DSlice->notify(event, this);
//}

//void ControllerRegister2DSliceToolTab::setSyncState(bool state)
//{
//    m_sliceSynchronizer->setEnabledSynchronization(state);
//}

//void ControllerRegister2DSliceToolTab::setRadiologicalMode()
//{
//    Model2DSliceEvent event = m_model2DSlice->setRadiologicalVoxelOrder();
//    m_model2DSlice->notify(event, this);
//}

//void ControllerRegister2DSliceToolTab::setNeurologicalMode()
//{
//    Model2DSliceEvent event = m_model2DSlice->setNeurologicalVoxelOrder();
//    m_model2DSlice->notify(event, this);
//}

//void ControllerRegister2DSliceToolTab::getSlicingIntervals(int &axial_min, int &axial_max, int &coronal_min, int &coronal_max, int &sagittal_min, int &sagittal_max)
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

//void ControllerRegister2DSliceToolTab::getSlicingData(int &axial, int &coronal, int &sagittal, bool &sync_state)
//{
//    axial = m_model2DSlice->getAxialSlice();
//    coronal = m_model2DSlice->getCoronalSlice();
//    sagittal = m_model2DSlice->getSagittalSlice();
//    sync_state = m_sliceSynchronizer->isSynchronizationEnabled();
//}

//bool ControllerRegister2DSliceToolTab::isRadiological()
//{
//    return m_model2DSlice->isRadiological();
//}

//bool ControllerRegister2DSliceToolTab::isNeurological()
//{
//    return m_model2DSlice->isNeurological();
//}
