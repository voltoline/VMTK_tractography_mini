#include "StateManager.h"

FeatureStates::FeatureStates(std::string feature_name, bool is_default, StateManager *state_manager):
    m_featureName(feature_name),
    m_isDefault(is_default)
{
    m_stateManager = state_manager;
    m_isEnabled = true;
    m_autoDeactivate = true;

    if(is_default)
        m_isActivated = true;
    else
        m_isActivated = false;

    m_callbackStateChanged01 = []() { };
    m_callbackStateChanged02 = [](bool, bool) { };
}

bool FeatureStates::isActivated() const
{
    return m_isActivated;
}

void FeatureStates::setActivated(bool is_activated)
{
    if(m_isActivated == is_activated)
        return;

    if(m_isEnabled)
    {
        m_isActivated = is_activated;

        if(m_isActivated)
            m_stateManager->activateFeature(m_featureName);
        else
            m_stateManager->deactivateFeature(m_featureName);

        m_callbackStateChanged01();
        m_callbackStateChanged02(m_isEnabled, m_isActivated);
    }
}

bool FeatureStates::isEnabled() const
{
    return m_isEnabled;
}

void FeatureStates::setEnabled(bool is_enabled)
{
    if(m_isEnabled == is_enabled)
        return;

    m_isEnabled = is_enabled;

    if(m_isEnabled == false && m_isActivated && m_autoDeactivate)
    {
        m_isActivated = false;
        m_stateManager->deactivateFeature(m_featureName);
    }

    if(m_isEnabled)
        m_stateManager->enableFeature(m_featureName);
    else
        m_stateManager->disableFeature(m_featureName);

    m_callbackStateChanged01();
    m_callbackStateChanged02(m_isEnabled, m_isActivated);
}

bool FeatureStates::isDefault() const
{
    return m_isDefault;
}

void FeatureStates::setStateChangedCallback(std::function<void ()> callback_state_changed)
{
    m_callbackStateChanged01 = callback_state_changed;
}

void FeatureStates::setStateChangedCallback(std::function<void (bool, bool)> callback_state_changed)
{
    m_callbackStateChanged02 = callback_state_changed;
}

std::string FeatureStates::getFeatureName() const
{
    return m_featureName;
}

void FeatureStates::setAutoDeactivate(bool autoDeactivate)
{
    m_autoDeactivate = autoDeactivate;
}

//======================================================================================================================================================================

StateManager::StateManager()
{

}

int StateManager::subscribe(std::string feature_name, std::function<void(std::string, FeatureState)> slot)
{
    auto it = getSignals().find(feature_name);
    if (it == getSignals().end())
        return -1;

    return getSignals()[feature_name].connect(slot);
}

bool StateManager::addDefaultFeature(std::string feature_name)
{
    if(isAdded(feature_name))
        return false;

    FeatureStates::sPtr feature_state = std::make_shared<FeatureStates>(feature_name, true, this);
    m_defaultFeatures.push_back(FeatureStateData{feature_state, feature_name});

    getSignals()[feature_name] = Signal<std::string, FeatureState>();

    activateFeature(feature_name);

    return true;
}

bool StateManager::addStateWithRestrictions(std::string feature_name, std::vector<std::string> locked_feature_names)
{
    if(isAdded(feature_name))
        return false;

    FeatureStates::sPtr feature_state = std::make_shared<FeatureStates>(feature_name, false, this);
    m_featuresWithRestrictions.push_back(FeatureWithRestrictions{feature_state, feature_name, locked_feature_names});

    getSignals()[feature_name] = Signal<std::string, FeatureState>();

    return true;
}

void StateManager::removeFeature(std::string feature_name)
{
    for(unsigned short i = 0; i < m_defaultFeatures.size(); i++)
        if(m_defaultFeatures[i].m_name.compare(feature_name) == 0)
        {
            deactivateFeature(feature_name);
            m_defaultFeatures.erase(m_defaultFeatures.begin() + i);
            getSignals().erase(feature_name);
            return;
        }

    for(unsigned short i = 0; i < m_featuresWithRestrictions.size(); i++)
        if(m_featuresWithRestrictions[i].m_featureName.compare(feature_name) == 0)
        {
            deactivateFeature(feature_name);
            m_featuresWithRestrictions.erase(m_featuresWithRestrictions.begin() + i);
            getSignals().erase(feature_name);
            return;
        }
}

FeatureStates::sPtr StateManager::getStatesOf(std::string feature_name)
{
    for(unsigned short i = 0; i < m_defaultFeatures.size(); i++)
        if(m_defaultFeatures[i].m_name.compare(feature_name) == 0)
            return m_defaultFeatures[i].m_state;

    for(unsigned short i = 0; i < m_featuresWithRestrictions.size(); i++)
        if(m_featuresWithRestrictions[i].m_featureName.compare(feature_name) == 0)
            return m_featuresWithRestrictions[i].m_state;

    std::cerr <<"StateManager::getStatesOf -> Feature " << feature_name << " not found!" << std::endl;
    return nullptr;
}

std::vector<std::string> StateManager::findActivatedConflictingFeatures(std::string feature_name)
{
    std::vector<std::string> conflicting_features;

    for(FeatureWithRestrictions &activated_feature: m_currentActivatedFeaturesWithRestrisctions)
    {
        for(std::string &locked_feature: activated_feature.m_lockedFeatureNames)
            if(locked_feature.compare(feature_name) == 0)
            {
                conflicting_features.push_back(activated_feature.m_featureName);
                break;
            }
    }

    for(FeatureWithRestrictions &feature: m_featuresWithRestrictions)
        if(feature.m_featureName.compare(feature_name) == 0)
        {
            for(std::string &locked_feature: feature.m_lockedFeatureNames)
            {
                for(FeatureStateData &activated_feature: m_currentActivatedDefaultFeatures)
                    if(locked_feature.compare(activated_feature.m_name) == 0)
                        conflicting_features.push_back(activated_feature.m_name);

                for(FeatureWithRestrictions &activated_feature: m_currentActivatedFeaturesWithRestrisctions)
                    if(locked_feature.compare(activated_feature.m_featureName) == 0)
                    {
                        conflicting_features.push_back(activated_feature.m_featureName);
                    }
            }
        }

    return conflicting_features;
}

void StateManager::activateFeature(std::string feature_name, int slot_id)
{

    if(!isAdded(feature_name))
        return;

    if(isActivated(feature_name))
        return;

    if(!isEnabled(feature_name))
        return;

    std::vector<std::string> conflicting_features = findActivatedConflictingFeatures(feature_name);
    for(std::string &conflict: conflicting_features)
        deactivateFeature(conflict, -1, false);

    FeatureStates::sPtr feature_state = nullptr;

    if(isDefaultFeature(feature_name))
    {
        for(FeatureStateData &feature: m_defaultFeatures)
            if(feature.m_name.compare(feature_name) == 0)
            {
                feature_state = feature.m_state;
                m_currentActivatedDefaultFeatures.push_back(feature);
            }

    } else {

        for(FeatureWithRestrictions &feature: m_featuresWithRestrictions)
            if(feature.m_featureName.compare(feature_name) == 0)
            {
                feature_state = feature.m_state;
                m_currentActivatedFeaturesWithRestrisctions.push_back(feature);
            }
    }

    if(feature_state != nullptr)
        feature_state->setActivated(true);
    else
        std::cerr << "StateManager::activateFeature -> feature state is null!" << std::endl;

    if(slot_id == -1)
        notifyAll(feature_name, feature_name, FeatureState::ACTIVATED);
    else
        notify(feature_name, slot_id, feature_name, FeatureState::ACTIVATED);

    if(conflicting_features.size() > 0)
        reactivateDefaultFeatures();
}

void StateManager::deactivateFeature(std::string feature_name, int slot_id, bool reactivate_default_features)
{
    for(unsigned short i = 0; i < m_currentActivatedDefaultFeatures.size(); i++)
        if(m_currentActivatedDefaultFeatures[i].m_name.compare(feature_name) == 0)
        {
            FeatureStates::sPtr feature_state = m_currentActivatedDefaultFeatures[i].m_state;

            m_currentActivatedDefaultFeatures.erase(m_currentActivatedDefaultFeatures.begin() + i);

            feature_state->setActivated(false);

            if(slot_id == -1)
                notifyAll(feature_name, feature_name, FeatureState::DEACTIVATED);
            else
                notify(feature_name, slot_id, feature_name, FeatureState::DEACTIVATED);

            return;
        }

    for(unsigned short i = 0; i < m_currentActivatedFeaturesWithRestrisctions.size(); i++)
        if(m_currentActivatedFeaturesWithRestrisctions[i].m_featureName.compare(feature_name) == 0)
        {
            FeatureStates::sPtr feature_state = m_currentActivatedFeaturesWithRestrisctions[i].m_state;

            m_currentActivatedFeaturesWithRestrisctions.erase(m_currentActivatedFeaturesWithRestrisctions.begin() + i);

            feature_state->setActivated(false);

            if(slot_id == -1)
                notifyAll(feature_name, feature_name, FeatureState::DEACTIVATED);
            else
                notify(feature_name, slot_id, feature_name, FeatureState::DEACTIVATED);

            if(reactivate_default_features)
                reactivateDefaultFeatures();

            return;
        }
}

void StateManager::enableFeature(std::string feature_name, int slot_id, bool reactivate_default_features)
{
    for(unsigned short i = 0; i < m_disabledDefaultFeatures.size(); i++)
        if(m_disabledDefaultFeatures[i].m_name.compare(feature_name) == 0)
        {
            FeatureStates::sPtr feature_state = m_disabledDefaultFeatures[i].m_state;

            m_disabledDefaultFeatures.erase(m_disabledDefaultFeatures.begin() + i);

            feature_state->setEnabled(true);

            if(slot_id == -1)
                notifyAll(feature_name, feature_name, FeatureState::ENABLED);
            else
                notify(feature_name, slot_id, feature_name, FeatureState::ENABLED);

            if(reactivate_default_features)
                reactivateDefaultFeatures();

            return;
        }

    for(unsigned short i = 0; i < m_disabledFeaturesWithRestrisctions.size(); i++)
        if(m_disabledFeaturesWithRestrisctions[i].m_featureName.compare(feature_name) == 0)
        {
            FeatureStates::sPtr feature_state = m_disabledFeaturesWithRestrisctions[i].m_state;

            m_disabledFeaturesWithRestrisctions.erase(m_disabledFeaturesWithRestrisctions.begin() + i);

            feature_state->setEnabled(true);

            if(slot_id == -1)
                notifyAll(feature_name, feature_name, FeatureState::ENABLED);
            else
                notify(feature_name, slot_id, feature_name, FeatureState::ENABLED);

            return;
        }
}

void StateManager::disableFeature(std::string feature_name, int slot_id)
{
    if(!isAdded(feature_name))
        return;

    if(!isEnabled(feature_name))
        return;

    FeatureStates::sPtr feature_state = nullptr;

    if(isDefaultFeature(feature_name))
    {
        for(FeatureStateData &feature: m_defaultFeatures)
            if(feature.m_name.compare(feature_name) == 0)
            {
                feature_state = feature.m_state;
                m_disabledDefaultFeatures.push_back(feature);
            }

    } else {

        for(FeatureWithRestrictions &feature: m_featuresWithRestrictions)
            if(feature.m_featureName.compare(feature_name) == 0)
            {
                feature_state = feature.m_state;
                m_disabledFeaturesWithRestrisctions.push_back(feature);
            }
    }

    if(feature_state != nullptr)
        feature_state->setEnabled(false);
    else
        std::cerr << "StateManager::disableFeature -> feature state is null!" << std::endl;

    if(slot_id == -1)
        notifyAll(feature_name, feature_name, FeatureState::DISABLED);
    else
        notify(feature_name, slot_id, feature_name, FeatureState::DISABLED);

    reactivateDefaultFeatures();
}

bool StateManager::isActivated(std::string feature_name)
{
    for(FeatureStateData &activated_feature: m_currentActivatedDefaultFeatures)
        if(activated_feature.m_name.compare(feature_name) == 0)
            return true;

    for(FeatureWithRestrictions &activated_feature: m_currentActivatedFeaturesWithRestrisctions)
        if(activated_feature.m_featureName.compare(feature_name) == 0)
            return true;

    return false;
}

bool StateManager::isEnabled(std::string feature_name)
{
    for(FeatureStateData &feature: m_disabledDefaultFeatures)
        if(feature.m_name.compare(feature_name) == 0)
            return false;

    for(FeatureWithRestrictions &feature: m_disabledFeaturesWithRestrisctions)
        if(feature.m_featureName.compare(feature_name) == 0)
            return false;

    return true;
}

bool StateManager::isAdded(std::string feature_name)
{
    for(FeatureStateData &feature: m_defaultFeatures)
        if(feature.m_name.compare(feature_name) == 0)
            return true;

    for(FeatureWithRestrictions &feature: m_featuresWithRestrictions)
        if(feature.m_featureName.compare(feature_name) == 0)
            return true;

    return false;
}

bool StateManager::isDefaultFeature(std::string feature_name)
{
    for(FeatureStateData &feature: m_defaultFeatures)
        if(feature.m_name.compare(feature_name) == 0)
            return true;

    return false;
}

bool StateManager::isFeatureWithRestrictions(std::string feature_name)
{
    for(FeatureWithRestrictions &feature: m_featuresWithRestrictions)
        if(feature.m_featureName.compare(feature_name) == 0)
            return true;

    return false;
}

void StateManager::reactivateDefaultFeatures()
{
    for(FeatureStateData &default_feature: m_defaultFeatures)
    {
        if(!isEnabled(default_feature.m_name))
            continue;

        bool flag_found_restriction = false;
        for(FeatureWithRestrictions &activated_feature: m_currentActivatedFeaturesWithRestrisctions)
        {
            for(std::string locked_feature: activated_feature.m_lockedFeatureNames)
                if(locked_feature.compare(default_feature.m_name) == 0)
                {
                    flag_found_restriction = true;
                    break;
                }

            if(flag_found_restriction)
                break;
        }

        if(flag_found_restriction)
            continue;

        bool flag_found_activated_feature = false;
        for(FeatureStateData &activated_feature: m_currentActivatedDefaultFeatures)
            if(activated_feature.m_name.compare(default_feature.m_name) == 0)
            {
                flag_found_activated_feature = true;
                break;
            }

        if(!flag_found_activated_feature)
        {
            FeatureStates::sPtr feature_state = default_feature.m_state;

            m_currentActivatedDefaultFeatures.push_back(default_feature);

            feature_state->setActivated(true);

            notifyAll(default_feature.m_name, default_feature.m_name, FeatureState::ACTIVATED);
        }
    }
}
