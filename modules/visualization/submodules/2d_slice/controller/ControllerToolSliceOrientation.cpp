#include "ControllerToolSliceOrientation.h"

#include "../ui/ToolSliceOrientation.h"
#include "../CommonObjects2DSlice.h"

ControllerToolSliceOrientation::ControllerToolSliceOrientation(CommonObjects2DSlice *common_objects):
    ControllerTool("ToolSliceOrientation")
{
    m_model2DSlice = common_objects->getModel2DSlice();
    m_model2DSliceUIState = common_objects->getModel2DSliceUIState();

    if(m_model2DSlice != nullptr)
        m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
}

ControllerToolSliceOrientation::~ControllerToolSliceOrientation()
{

}

void ControllerToolSliceOrientation::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    if(getTool() == nullptr)
        return;

    switch(event_type)
    {

    case Model2DSliceEvent::VOXEL_ORDER_CHANGED:
        static_cast<ToolSliceOrientation*>(getTool())->updateSliceOrientation();
        break;

    default:
        break;
    }
}

void ControllerToolSliceOrientation::setRadiologicalMode()
{
    Model2DSliceEvent event = m_model2DSlice->setRadiological();
    m_model2DSlice->notify(event, this);
}

void ControllerToolSliceOrientation::setNeurologicalMode()
{
    Model2DSliceEvent event = m_model2DSlice->setNeurological();
    m_model2DSlice->notify(event, this);
}

bool ControllerToolSliceOrientation::isRadiological()
{
    return m_model2DSlice->isRadiological();
}

bool ControllerToolSliceOrientation::isNeurological()
{
    return m_model2DSlice->isNeurological();
}

bool ControllerToolSliceOrientation::isOrientationEnabled()
{
    return m_model2DSliceUIState->isActivated();
}

bool ControllerToolSliceOrientation::is2DSliceActivated()
{
    return m_model2DSliceUIState->isActivated();
}
