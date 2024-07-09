#ifndef MODEL_3D_CLIPPING_H
#define MODEL_3D_CLIPPING_H

#include "model/ModelSubject.h"
//#include "model/StateManager.h"
#include "modules/visualization/StateManager.h"

enum class Model3DClippingEvent
{
    CLIPPING_CHANGED,
    CLIPPING_STATE_CHANGED,
    ORIENTATION_CHANGED,
};

class Model3DClipping: public ModelSubject<Model3DClipping, Model3DClippingEvent>
{
public:

    enum class Orientation
    {
        LPS,
        RAS,
        LAS,
        CURRENT
    };

    Model3DClipping();
    ~Model3DClipping();

    void handle3DClippingStateChanged(std::string , FeatureState state);

    void setLeftRightInterval(unsigned short min, unsigned short max);
    void setBottomTopInterval(unsigned short min, unsigned short max);
    void setBackFrontInterval(unsigned short min, unsigned short max);

    void getLeftRightInterval(unsigned short &min, unsigned short &max);
    void getBottomTopInterval(unsigned short &min, unsigned short &max);
    void getBackFrontInterval(unsigned short &min, unsigned short &max);

    void setLeftClipping(unsigned short value);     // X axis
    void setRightClipping(unsigned short value);    // X axis
    void setBottomClipping(unsigned short value);   // Y axis
    void setTopClipping(unsigned short value);      // Y axis
    void setBackClipping(unsigned short value);     // Z axis
    void setFrontClipping(unsigned short value);    // Z axis

    unsigned short getLeftClipping(Orientation orient = Orientation::CURRENT) const;
    unsigned short getRightClipping(Orientation orient = Orientation::CURRENT) const;
    unsigned short getBottomClipping(Orientation orient = Orientation::CURRENT) const;
    unsigned short getTopClipping(Orientation orient = Orientation::CURRENT) const;
    unsigned short getBackClipping(Orientation orient = Orientation::CURRENT) const;
    unsigned short getFrontClipping(Orientation orient = Orientation::CURRENT) const;

    void resetClipping();

    void setStateManager(StateManager *state_manager);

    bool isActivated() const;
    void setActivated(bool isActivated);

    void setOrientation(const Orientation &orientation);

    Orientation getOrientation() const;

    bool isEnabled() const;

private:
    unsigned short mLeft;
    unsigned short mRight;
    unsigned short mBottom;
    unsigned short mTop;
    unsigned short mBack;
    unsigned short mFront;

    unsigned short mLeftRightMin;
    unsigned short mLeftRightMax;
    unsigned short mBottomTopMin;
    unsigned short mBottomTopMax;
    unsigned short mBackFrontMin;
    unsigned short mBackFrontMax;

    StateManager *m_stateManager;

    Orientation m_orientation;

    bool m_isActivated;
    bool m_isEnabled;
    int m_3DClippingStateChangedHandlerSlotID;
};

#endif // MODEL_3D_CLIPPING_H
