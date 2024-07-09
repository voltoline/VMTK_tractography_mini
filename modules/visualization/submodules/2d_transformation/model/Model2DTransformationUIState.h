#ifndef MODEL_2D_TRANSFORMATION_UI_STATE_H
#define MODEL_2D_TRANSFORMATION_UI_STATE_H

#include "modules/visualization/StateManager.h"
#include "model/ModelSubject.h"

enum class Model2DTransformationUIStateEvent
{
    NONE,
    TRANSFORMATION_2D_STATE_CHANGED
};

class Model2DTransformationUIState: public ModelSubject<Model2DTransformationUIState, Model2DTransformationUIStateEvent>
{
public:
    Model2DTransformationUIState();
    ~Model2DTransformationUIState();

    void setStateManager(StateManager *state_manager);

    bool isTransformationActivated() const;
    void setTransformationActivated(bool isTransformationActivated);

    bool isEnabled() const;

private:

    StateManager *m_stateManager;
    FeatureStates::sPtr m_states2DTransformation;
};

#endif // MODEL_2D_TRANSFORMATION_UI_STATE_H
