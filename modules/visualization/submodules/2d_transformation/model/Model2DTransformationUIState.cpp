#include "Model2DTransformationUIState.h"

Model2DTransformationUIState::Model2DTransformationUIState()
{
    m_stateManager = nullptr;
    m_states2DTransformation = nullptr;

}

Model2DTransformationUIState::~Model2DTransformationUIState()
{

}

bool Model2DTransformationUIState::isTransformationActivated() const
{
    if(m_states2DTransformation != nullptr)
        return m_states2DTransformation->isActivated();
    else
        return false;
}

void Model2DTransformationUIState::setTransformationActivated(bool isTransformationEnabled)
{
    if(isNotifying())
    {
        std::cerr << "Model2DTransformation::setTransformationActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_states2DTransformation != nullptr)
        m_states2DTransformation->setActivated(isTransformationEnabled);
}

void Model2DTransformationUIState::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    m_stateManager = state_manager;

    if(m_stateManager != nullptr)
    {
        if(m_stateManager->addDefaultFeature("2D_TRANSFORMATION"))
        {
            m_states2DTransformation = m_stateManager->getStatesOf("2D_TRANSFORMATION");
            m_states2DTransformation->setStateChangedCallback([this]()
            {
                this->notify(Model2DTransformationUIStateEvent::TRANSFORMATION_2D_STATE_CHANGED);
            });

        }
    }
}

bool Model2DTransformationUIState::isEnabled() const
{
    if(m_states2DTransformation != nullptr)
        return m_states2DTransformation->isEnabled();
    else
        return false;
}
