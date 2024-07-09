#ifndef MODEL_2D_SLICE_UI_STATE_H
#define MODEL_2D_SLICE_UI_STATE_H

#include "model/ModelSubject.h"
#include "modules/visualization/StateManager.h"

class StateManager;

enum class Model2DSliceUIStateEvent
{
    NONE,
    SLICE_STATE_CHANGED,
};

class Model2DSliceUIState: public ModelSubject<Model2DSliceUIState, Model2DSliceUIStateEvent>
{
public:
    Model2DSliceUIState();
    ~Model2DSliceUIState();

    bool isActivated() const;
    bool isEnabled() const;

    void setActivated(bool isActivated);

    void setStateManager(StateManager *stateManager);

private:

    StateManager *m_stateManager;
    FeatureStates::sPtr m_states2DSlice;
};

#endif // MODEL_2D_SLICE_UI_STATE_H
