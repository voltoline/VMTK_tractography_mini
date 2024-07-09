#include "Model2DSliceUIState.h"

Model2DSliceUIState::Model2DSliceUIState()
{
    m_stateManager = nullptr;
    m_states2DSlice = nullptr;
}

Model2DSliceUIState::~Model2DSliceUIState()
{

}

bool Model2DSliceUIState::isActivated() const
{
    if(m_states2DSlice != nullptr)
        return m_states2DSlice->isActivated();
    else
        return false;
}

bool Model2DSliceUIState::isEnabled() const
{
    if(m_states2DSlice != nullptr)
        return m_states2DSlice->isActivated();
    else
        return false;
}

void Model2DSliceUIState::setActivated(bool isActivated)
{
    if(m_states2DSlice != nullptr)
        m_states2DSlice->setActivated(isActivated);
}

void Model2DSliceUIState::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    m_stateManager = state_manager;

    if(m_stateManager != nullptr)
    {
        if(m_stateManager->addDefaultFeature("2D_SLICE"))
        {
            m_states2DSlice = m_stateManager->getStatesOf("2D_SLICE");
            m_states2DSlice->setStateChangedCallback([this]()
            {
                this->notify(Model2DSliceUIStateEvent::SLICE_STATE_CHANGED);
            });
        }
    }

}
