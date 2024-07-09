#include "Controller2D3DSlicingTool.h"

#include "../ui/Tool2D3DSlicing.h"

#include <modules/visualization/submodules/volume_renderer/model/Model3DRendering.h>

Controller2D3DSlicingTool::Controller2D3DSlicingTool():
    ControllerTool("Tool2D3DSlicing"),
    m_model3DClipping(nullptr)
{
    m_model2DSlice = nullptr;
    m_model2DSliceUIState = nullptr;
    m_sliceSynchronizer = nullptr;
    m_model3DRendering = nullptr;

    m_renderingSurfaceSet = nullptr;
}

void Controller2D3DSlicingTool::update(Model3DClipping *, Model3DClippingEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {

    case Model3DClippingEvent::CLIPPING_CHANGED:
    {
        static_cast<Tool2D3DSlicing*>(getTool())->updateClipping();
        break;
    }

    case Model3DClippingEvent::CLIPPING_STATE_CHANGED:
    {
        static_cast<Tool2D3DSlicing*>(getTool())->updateComponentsEnabled();
        break;
    }

    default:
        break;

    }
}

void Controller2D3DSlicingTool::update(Model3DRendering */*subject*/, Model3DRenderingEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {

    case Model3DRenderingEvent::HIGHLIGHTED_PLANE_STATE_CHAGNED:
    {
        static_cast<Tool2D3DSlicing*>(getTool())->updateComponentsEnabled();
        break;
    }

    default:
        break;
    }
}

void Controller2D3DSlicingTool::setLeftClipping(int value)
{
        setSagittalSlice(value);

    if(!m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
    }

    if(m_model3DRendering->isHighlightedPlanesActivated() == false)
        m_model3DClipping->setLeftClipping(value);

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::setRightClipping(int value)
{
        setSagittalSlice(value);

    if(!m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
    }
    if(m_model3DRendering->isHighlightedPlanesActivated() == false)
        m_model3DClipping->setRightClipping(value);

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::setBottomClipping(int value)
{
    setCoronalSlice(value);

    if(m_model3DClipping->isActivated())
    {
        m_model3DClipping->setBottomClipping(value);
        m_model3DClipping->notify();
    }
}

void Controller2D3DSlicingTool::setTopClipping(int value)
{
    setCoronalSlice(value);

    if(!m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
    }

    if(m_model3DRendering->isHighlightedPlanesActivated() == false)
        m_model3DClipping->setTopClipping(value);

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::setBackClipping(int value)
{
    setAxialSlice(value);

    if(!m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
    }

    if(m_model3DRendering->isHighlightedPlanesActivated() == false)
        m_model3DClipping->setBackClipping(value);

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::setFrontClipping(int value)
{
    setAxialSlice(value);

    if(!m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
    }

    if(m_model3DRendering->isHighlightedPlanesActivated() == false)
        m_model3DClipping->setFrontClipping(value);

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::getClippingIntervals(int &left_right_min, int &left_right_max, int &bottom_top_min, int &bottom_top_max, int &back_front_min, int &back_front_max)
{
    unsigned short left_right_min_;
    unsigned short left_right_max_;
    unsigned short bottom_top_min_;
    unsigned short bottom_top_max_;
    unsigned short back_front_min_;
    unsigned short back_front_max_;

    m_model3DClipping->getLeftRightInterval(left_right_min_, left_right_max_);
    m_model3DClipping->getBottomTopInterval(bottom_top_min_, bottom_top_max_);
    m_model3DClipping->getBackFrontInterval(back_front_min_, back_front_max_);

    left_right_min = left_right_min_;
    left_right_max = left_right_max_;
    bottom_top_min = bottom_top_min_;
    bottom_top_max = bottom_top_max_;
    back_front_min = back_front_min_;
    back_front_max = back_front_max_;
}

void Controller2D3DSlicingTool::getClippingData(int &clipLeft, int &clipRight, int &clipBottom, int &clipTop, int &clipBack, int &clipFront)
{
    if(m_model3DClipping->isActivated())
    {
        clipLeft = m_model3DClipping->getLeftClipping();
        clipRight = m_model3DClipping->getRightClipping();

        clipTop = m_model3DClipping->getTopClipping();
        clipBack = m_model3DClipping->getBackClipping();

    } else {

        if(m_model2DSlice->isRadiological())
        {
            clipLeft = m_model2DSlice->getSagittalSlice();
            clipRight = m_model3DClipping->getRightClipping();

        } else {

            clipRight = m_model2DSlice->getSagittalSlice();
            clipLeft = m_model3DClipping->getLeftClipping();
        }

        clipBack = m_model2DSlice->getAxialSlice();
        clipTop = m_model2DSlice->getCoronalSlice();
    }

    clipBottom = m_model3DClipping->getBottomClipping();
    clipFront = m_model3DClipping->getFrontClipping();

}

bool Controller2D3DSlicingTool::is3DClippingEnabled()
{
    return m_model3DClipping->isEnabled();
}

bool Controller2D3DSlicingTool::is3DClippingActivated()
{
    return m_model3DClipping->isActivated();
}

void Controller2D3DSlicingTool::set3DClippingActivated(bool state)
{
    if(state == false)
    {
        m_model3DClipping->resetClipping();
    }

    m_model3DClipping->setActivated(state);

    if(state)
    {
        m_model3DClipping->setTopClipping(m_model2DSlice->getCoronalSlice());
        m_model3DClipping->setBackClipping(m_model2DSlice->getAxialSlice());

        if(m_model2DSlice->isNeurological())
            m_model3DClipping->setRightClipping(m_model2DSlice->getSagittalSlice());
        else
            m_model3DClipping->setLeftClipping(m_model2DSlice->getSagittalSlice());
    }

    m_model3DClipping->notify();
}

void Controller2D3DSlicingTool::resetToEdge()
{
    if(m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();
        m_model3DClipping->notify();
    }

    if(m_model2DSlice->isNeurological())
        setSagittalSlice(m_model3DClipping->getRightClipping());

    if(m_model2DSlice->isRadiological())
        setSagittalSlice(m_model3DClipping->getLeftClipping());

    setCoronalSlice(m_model3DClipping->getTopClipping());

    setAxialSlice(m_model3DClipping->getBackClipping());
}

void Controller2D3DSlicingTool::setModel3DClipping(Model3DClipping *model3DClipping)
{
    m_model3DClipping = model3DClipping;
    m_model3DClipping->subscribe(this, Model3DClippingEvent::CLIPPING_CHANGED);
    m_model3DClipping->subscribe(this, Model3DClippingEvent::CLIPPING_STATE_CHANGED);
}

void Controller2D3DSlicingTool::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {
    case Model2DSliceEvent::SLICING_CHANGED:
        static_cast<Tool2D3DSlicing*>(getTool())->updateClipping();
        break;

    case Model2DSliceEvent::VOXEL_ORDER_CHANGED:
        static_cast<Tool2D3DSlicing*>(getTool())->updateComponentsEnabled();
        break;

    default:
        break;
    }
}

void Controller2D3DSlicingTool::update(Model2DSliceUIState *subject, Model2DSliceUIStateEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {
    case Model2DSliceUIStateEvent::SLICE_STATE_CHANGED:
        static_cast<Tool2D3DSlicing*>(getTool())->updateComponentsEnabled();
        break;

    default:
        break;
    }
}

void Controller2D3DSlicingTool::update(SliceSynchronizer *, SliceSynchronizerEvent event_type)
{
    if(event_type == SliceSynchronizerEvent::SYNC_STATE_CHANGED)
    {
        if(getTool())
            static_cast<Tool2D3DSlicing*>(getTool())->updateClipping();
    }
}

void Controller2D3DSlicingTool::setModel2DSlice(Model2DSlice *model)
{
    if(m_model2DSlice == model)
        return;

    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }

    m_model2DSlice = model;

    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
}

void Controller2D3DSlicingTool::setSliceSynchronizer(SliceSynchronizer *slice_synchronizer)
{
    if(m_sliceSynchronizer == slice_synchronizer)
        return;

    if(m_sliceSynchronizer != nullptr)
        m_sliceSynchronizer->unsubscribe(this, SliceSynchronizerEvent::SYNC_STATE_CHANGED);

    m_sliceSynchronizer = slice_synchronizer;

    if(m_sliceSynchronizer != nullptr)
        m_sliceSynchronizer->subscribe(this, SliceSynchronizerEvent::SYNC_STATE_CHANGED);
}

void Controller2D3DSlicingTool::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void Controller2D3DSlicingTool::setModel3DRendering(Model3DRendering *model3DRendering)
{
    m_model3DRendering = model3DRendering;

    m_model3DRendering->subscribe(this, Model3DRenderingEvent::HIGHLIGHTED_PLANE_STATE_CHAGNED);
}

void Controller2D3DSlicingTool::setModel2DSliceUIState(Model2DSliceUIState *model2DSliceUIState)
{
    m_model2DSliceUIState = model2DSliceUIState;

    m_model2DSliceUIState->subscribe(this, Model2DSliceUIStateEvent::SLICE_STATE_CHANGED);
}

void Controller2D3DSlicingTool::resetToMiddle()
{
    Model2DSliceEvent event = m_model2DSlice->resetSlices();
    m_model2DSlice->notify(event);

    if(m_model3DClipping->isActivated())
    {
        m_model3DClipping->resetClipping();

        m_model3DClipping->setBackClipping(m_model2DSlice->getAxialSlice());

        m_model3DClipping->setTopClipping(m_model2DSlice->getCoronalSlice());

        if(m_model2DSlice->isRadiological())
            m_model3DClipping->setLeftClipping(m_model2DSlice->getSagittalSlice());

        if(m_model2DSlice->isNeurological())
            m_model3DClipping->setRightClipping(m_model2DSlice->getSagittalSlice());

        m_model3DClipping->notify();
    }
}

void Controller2D3DSlicingTool::setAxialSlice(int value)
{
    m_renderingSurfaceSet->setFocusAxial();

    Model2DSliceEvent event = m_model2DSlice->setAxialSlice(value);
    m_model2DSlice->notify(event);
}

void Controller2D3DSlicingTool::setCoronalSlice(int value)
{
    m_renderingSurfaceSet->setFocusCoronal();

    Model2DSliceEvent event = m_model2DSlice->setCoronalSlice(value);
    m_model2DSlice->notify(event);
}

void Controller2D3DSlicingTool::setSagittalSlice(int value)
{
    m_renderingSurfaceSet->setFocusSagittal();

    Model2DSliceEvent event = m_model2DSlice->setSagittalSlice(value);
    m_model2DSlice->notify(event);
}

bool Controller2D3DSlicingTool::isRadiological()
{
    return m_model2DSlice->isRadiological();
}

bool Controller2D3DSlicingTool::isNeurological()
{
    return m_model2DSlice->isNeurological();
}

void Controller2D3DSlicingTool::getSlicingIntervals(int &axial_min, int &axial_max, int &coronal_min, int &coronal_max, int &sagittal_min, int &sagittal_max)
{
    unsigned short sagittal_min_;
    unsigned short sagittal_max_;
    unsigned short coronal_min_;
    unsigned short coronal_max_;
    unsigned short axial_min_;
    unsigned short axial_max_;

    m_model2DSlice->getAxialInterval(axial_min_, axial_max_);
    m_model2DSlice->getCoronalInterval(coronal_min_, coronal_max_);
    m_model2DSlice->getSagittalInterval(sagittal_min_, sagittal_max_);

    sagittal_min = sagittal_min_;
    sagittal_max = sagittal_max_;
    coronal_min = coronal_min_;
    coronal_max = coronal_max_;
    axial_min = axial_min_;
    axial_max = axial_max_;
}

void Controller2D3DSlicingTool::getSlicingData(int &axial, int &coronal, int &sagittal, bool &sync_state)
{
    axial = m_model2DSlice->getAxialSlice();
    coronal = m_model2DSlice->getCoronalSlice();
    sagittal = m_model2DSlice->getSagittalSlice();

    if(m_sliceSynchronizer != nullptr)
        sync_state = m_sliceSynchronizer->isSynchronizationActivated();
    else
        sync_state = false;
}

bool Controller2D3DSlicingTool::is2DSliceEnabled()
{
    return m_model2DSliceUIState->isEnabled();
}

bool Controller2D3DSlicingTool::is2DSliceActivated()
{
    return m_model2DSliceUIState->isActivated();
}

void Controller2D3DSlicingTool::set2DSliceActivated(bool state)
{
    m_model2DSliceUIState->setActivated(state);
}

bool Controller2D3DSlicingTool::isSliceSyncronizationEnabled()
{
    if(m_sliceSynchronizer != nullptr)
        return m_sliceSynchronizer->isEnabled();
    else
        return false;
}

void Controller2D3DSlicingTool::set3DSliceActivated(bool state)
{
    m_model3DRendering->setHighlightedPlanesActivated(state);
}

bool Controller2D3DSlicingTool::is3DSliceActivated()
{
    return m_model3DRendering->isHighlightedPlanesActivated();
}
