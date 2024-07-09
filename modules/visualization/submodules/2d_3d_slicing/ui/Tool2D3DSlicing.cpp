#include "Tool2D3DSlicing.h"

Tool2D3DSlicing::Tool2D3DSlicing():
    Tool("Tool2D3DSlicing")
{
    m_ptrController2D3DSlicing = nullptr;
}

void Tool2D3DSlicing::updateController(Controller2D3DSlicingTool *controller)
{
    m_ptrController2D3DSlicing = controller;
}

void Tool2D3DSlicing::updateComponents()
{
    /** 3D mouse tools buttons **/
    updateComponentsEnabled();

    if(m_ptrController2D3DSlicing == nullptr)
        return;

    if(m_ptrController2D3DSlicing != nullptr && (m_ptrController2D3DSlicing->is3DClippingActivated() || m_ptrController2D3DSlicing->is2DSliceActivated()))
    {
        updateIntervals();
        updateClipping();
    }
}

Controller2D3DSlicingTool *Tool2D3DSlicing::getController() const
{
    return m_ptrController2D3DSlicing;
}
