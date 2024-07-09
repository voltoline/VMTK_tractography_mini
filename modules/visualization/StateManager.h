#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <memory>
#include "model/EventNotifier.h"

class StateManager;

class FeatureStates
{
public:
    typedef std::shared_ptr<FeatureStates> sPtr;

    FeatureStates(std::string feature_name, bool is_default, StateManager *state_manager);

    bool isActivated() const;
    void setActivated(bool isActivated);

    bool isEnabled() const;
    void setEnabled(bool isEnabled);

    bool isDefault() const;

    void setStateChangedCallback(std::function<void()> callback_state_changed);
    void setStateChangedCallback(std::function<void(bool, bool)> callback_state_changed);

    std::string getFeatureName() const;

    void setAutoDeactivate(bool autoDeactivate);

private:
    std::string m_featureName;

    bool m_autoDeactivate;

    bool m_isDefault;

    bool m_isActivated;
    bool m_isEnabled;

    StateManager *m_stateManager;

    std::function<void()> m_callbackStateChanged01;
    std::function<void(bool, bool)> m_callbackStateChanged02;
};

enum class FeatureState
{
    ACTIVATED,
    DEACTIVATED,
    ENABLED,
    DISABLED
};

class StateManager: public EventNotifier<std::string, std::string, FeatureState>
{
public:

    StateManager();

    int subscribe(std::string feature_name, std::function<void(std::string, FeatureState)> slot);

    bool addDefaultFeature(std::string feature_name);
    bool addStateWithRestrictions(std::string feature_name, std::vector<std::string> locked_feature_names);
    void removeFeature(std::string feature_name);

    FeatureStates::sPtr getStatesOf(std::string feature_name);

    void activateFeature(std::string feature_name, int slot_id = -1);
    void deactivateFeature(std::string feature_name, int slot_id = -1, bool reactivate_default_features = true);

    void enableFeature(std::string feature_name, int slot_id = -1, bool reactivate_default_features = true);
    void disableFeature(std::string feature_name, int slot_id = -1);

    bool isActivated(std::string feature_name);
    bool isEnabled(std::string feature_name);
    bool isAdded(std::string feature_name);
    bool isDefaultFeature(std::string feature_name);
    bool isFeatureWithRestrictions(std::string feature_name);

private:
    void reactivateDefaultFeatures();
    std::vector<std::string> findActivatedConflictingFeatures(std::string feature_name);

    typedef struct feature_with_restrictions
    {
        FeatureStates::sPtr m_state;
        std::string m_featureName;
        std::vector<std::string> m_lockedFeatureNames;

    } FeatureWithRestrictions;

    typedef struct feature_state_data
    {
        FeatureStates::sPtr m_state;
        std::string m_name;

    } FeatureStateData;

    std::vector<FeatureStateData> m_defaultFeatures;
    std::vector<FeatureWithRestrictions> m_featuresWithRestrictions;

    std::vector<FeatureStateData> m_currentActivatedDefaultFeatures;
    std::vector<FeatureWithRestrictions> m_currentActivatedFeaturesWithRestrisctions;

    std::vector<FeatureStateData> m_disabledDefaultFeatures;
    std::vector<FeatureWithRestrictions> m_disabledFeaturesWithRestrisctions;
};

#endif // STATE_MANAGER_H
