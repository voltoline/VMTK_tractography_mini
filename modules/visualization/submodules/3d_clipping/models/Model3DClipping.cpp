#include "Model3DClipping.h"

Model3DClipping::Model3DClipping()
{
    mLeftRightMin = 0;
    mLeftRightMax = 0;
    mBottomTopMin = 0;
    mBottomTopMax = 0;
    mBackFrontMin = 0;
    mBackFrontMax = 0;

    m_stateManager = nullptr;

    m_isActivated = true;
    m_isEnabled = true;

    m_orientation = Orientation::LAS;

    resetClipping();
}

Model3DClipping::~Model3DClipping()
{

}

void Model3DClipping::setLeftRightInterval(unsigned short min, unsigned short max)
{
    mLeftRightMin = min;
    mLeftRightMax = max;
}

void Model3DClipping::setBottomTopInterval(unsigned short min, unsigned short max)
{
    mBottomTopMin = min;
    mBottomTopMax = max;
}

void Model3DClipping::setBackFrontInterval(unsigned short min, unsigned short max)
{
    mBackFrontMin = min;
    mBackFrontMax = max;
}

void Model3DClipping::getLeftRightInterval(unsigned short &min, unsigned short &max)
{
    min = mLeftRightMin;
    max = mLeftRightMax;
}

void Model3DClipping::getBottomTopInterval(unsigned short &min, unsigned short &max)
{
    min = mBottomTopMin;
    max = mBottomTopMax;
}

void Model3DClipping::getBackFrontInterval(unsigned short &min, unsigned short &max)
{
    min = mBackFrontMin;
    max = mBackFrontMax;
}

void Model3DClipping::resetClipping()
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::resetClipping -->> A notification is in progress!" << std::endl;
    }

    if(m_orientation == Orientation::LAS || m_orientation == Orientation::LPS)
    {
        mLeft = mLeftRightMin;
        mRight = mLeftRightMax;

    } else if(m_orientation == Orientation::RAS) {

        mLeft = mLeftRightMax;
        mRight = mLeftRightMin;
    }

    if(m_orientation == Orientation::LAS || m_orientation == Orientation::RAS)
    {
        mBottom = mBottomTopMax;
        mTop = mBottomTopMin ;

    } else if(m_orientation == Orientation::LPS) {

        mBottom = mBottomTopMin;
        mTop = mBottomTopMax;
    }

    mBack = mBackFrontMin;
    mFront = mBackFrontMax;

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setLeftClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setLeftClipping -->> A notification is in progress!" << std::endl;
    }

    if(mLeftRightMin > value)
        value = mLeftRightMin;

    else if(mLeftRightMax < value)
        value = mLeftRightMax;

    if(mLeft == value)
        return;

    mLeft = value;

    if(m_orientation == Orientation::RAS)
    {
        if(mLeft < mRight)
        {
            setRightClipping(mLeft);
        }

    } else {

        if(mLeft > mRight)
        {
            setRightClipping(mLeft);
        }
    }

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setRightClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setRightClipping -->> A notification is in progress!" << std::endl;
    }

    if(mLeftRightMin > value)
        value = mLeftRightMin;

    else if(mLeftRightMax < value)
        value = mLeftRightMax;

    if(mRight == value)
        return;

    mRight = value;

    if(m_orientation == Orientation::RAS)
    {
        if(mLeft < mRight)
        {
            setLeftClipping(mRight);
        }

    } else {

        if(mLeft > mRight)
        {
            setLeftClipping(mRight);
        }
    }

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setBottomClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setBottomClipping -->> A notification is in progress!" << std::endl;
    }

    if(mBottomTopMin > value)
        value = mBottomTopMin;

    else if(mBottomTopMax < value)
        value = mBottomTopMax;

    if(mBottom == value)
        return;

    mBottom = value;

    if(m_orientation == Orientation::LAS || m_orientation == Orientation::RAS)
    {
        if(mTop > mBottom)
        {
            setTopClipping(mBottom);
        }

    } else {

        if(mTop < mBottom)
        {
            setTopClipping(mBottom);
        }
    }
    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setTopClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setTopClipping -->> A notification is in progress!" << std::endl;
    }

    if(mBottomTopMin > value)
        value = mBottomTopMin;

    else if(mBottomTopMax < value)
        value = mBottomTopMax;

    if(mTop == value)
        return;

    mTop = value;

    if(m_orientation == Orientation::LAS || m_orientation == Orientation::RAS)
    {
        if(mTop > mBottom)
        {
            setBottomClipping(mTop);
        }

    } else {

        if(mTop < mBottom)
        {
            setBottomClipping(mTop);
        }
    }

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setBackClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setBackClipping -->> A notification is in progress!" << std::endl;
    }

    if(mBackFrontMin > value)
        value = mBackFrontMin;

    else if(mBackFrontMax < value)
        value = mBackFrontMax;

    if(mBack == value)
        return;

    mBack = value;

    if(mBack > mFront)
    {
        setFrontClipping(mBack);
    }

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

void Model3DClipping::setFrontClipping(unsigned short value)
{
    if(isNotifying())
    {
        std::cerr << "Model3DClipping::setFrontClipping -->> A notification is in progress!" << std::endl;
    }

    if(mBackFrontMin > value)
        value = mBackFrontMin;

    else if(mBackFrontMax < value)
        value = mBackFrontMax;

    if(mFront == value)
        return;

    mFront = value;

    if(mBack > mFront)
    {
        setBackClipping(mFront);
    }

    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

unsigned short Model3DClipping::getLeftClipping(Orientation orient) const
{
    if(orient == Orientation::LPS || orient == Orientation::LAS)
    {
        if(m_orientation == Orientation::RAS)
            return mLeftRightMax - mLeft + 1;
        else
            return mLeft;
    }

    if(orient == Orientation::RAS)
    {
        if(m_orientation == Orientation::LPS || m_orientation == Orientation::LAS)
            return mLeftRightMax - mLeft + 1;
    }

    return mLeft;
}

unsigned short Model3DClipping::getRightClipping(Orientation orient) const
{
    if(orient == Orientation::LPS || orient == Orientation::LAS)
    {
        if(m_orientation == Orientation::RAS)
            return mLeftRightMax - mRight + 1;
        else
            return mRight;
    }

    if(orient == Orientation::RAS)
    {
        if(m_orientation == Orientation::LPS || m_orientation == Orientation::LAS)
            return mLeftRightMax - mRight + 1;
    }

    return mRight;
}

unsigned short Model3DClipping::getBottomClipping(Orientation orient) const
{
    if(orient == Orientation::LPS)
    {
        if(m_orientation == Orientation::LAS || m_orientation == Orientation::RAS)
            return mBottomTopMax - mBottom + 1;
        else
            return mBottom;
    }

    if(orient == Orientation::LAS || orient == Orientation::RAS)
    {
        if(m_orientation == Orientation::LPS)
            return mBottomTopMax - mBottom + 1;
    }

    return mBottom;
}

unsigned short Model3DClipping::getTopClipping(Orientation orient) const
{
    if(orient == Orientation::LPS)
    {
        if(m_orientation == Orientation::LAS || m_orientation == Orientation::RAS)
            return mBottomTopMax - mTop + 1;
        else
            return mTop;
    }

    if(orient == Orientation::LAS || orient == Orientation::RAS)
    {
        if(m_orientation == Orientation::LPS)
            return mBottomTopMax - mTop + 1;
    }

    return mTop;
}

unsigned short Model3DClipping::getBackClipping(Orientation /*orient*/) const
{
    return mBack;
}

unsigned short Model3DClipping::getFrontClipping(Orientation /*orient*/) const
{
    return mFront;
}

void Model3DClipping::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("3D_CLIPPING", m_3DClippingStateChangedHandlerSlotID);
        m_stateManager->removeFeature("3D_CLIPPING");
    }

    m_stateManager = state_manager;

    if(m_stateManager->addStateWithRestrictions("3D_CLIPPING", std::vector<std::string>{"2DSLICE_TO_3DCLIPPING_SYNC", "2D_SLICE", "MULTIPLANAR"}))
    {
        m_3DClippingStateChangedHandlerSlotID = m_stateManager->subscribe("3D_CLIPPING",  CREATE_SLOT(this, &Model3DClipping::handle3DClippingStateChanged));

    } else {

        m_3DClippingStateChangedHandlerSlotID = m_stateManager->subscribe("3D_CLIPPING",  CREATE_SLOT(this, &Model3DClipping::handle3DClippingStateChanged));
        m_isActivated = m_stateManager->isActivated("3D_CLIPPING");
    }
}

bool Model3DClipping::isActivated() const
{
    return m_isActivated;
}

void Model3DClipping::setActivated(bool isActivated)
{
    if(m_isEnabled)
    {
        if(m_isActivated != isActivated)
        {
            m_isActivated = isActivated;

            if(m_stateManager != nullptr)
            {
                if(m_isActivated)
                    m_stateManager->activateFeature("3D_CLIPPING", m_3DClippingStateChangedHandlerSlotID);
                else
                    m_stateManager->deactivateFeature("3D_CLIPPING", m_3DClippingStateChangedHandlerSlotID);
            }

            addEvent(Model3DClippingEvent::CLIPPING_STATE_CHANGED);
        }
    }
}

void Model3DClipping::setOrientation(const Orientation &orientation)
{
    if(orientation == m_orientation)
        return;

    if(m_orientation == Orientation::LAS || m_orientation == Orientation::LPS)
        if(orientation == Orientation::RAS)
        {
            mLeft = mLeftRightMax - mLeft + 1;
            mRight = mLeftRightMax - mRight + 1;
        }

    if(m_orientation == Orientation::RAS)
    {
        mLeft = mLeftRightMax - mLeft + 1;
        mRight = mLeftRightMax - mRight + 1;
    }

    m_orientation = orientation;

    addEvent(Model3DClippingEvent::ORIENTATION_CHANGED);
    addEvent(Model3DClippingEvent::CLIPPING_CHANGED);
}

Model3DClipping::Orientation Model3DClipping::getOrientation() const
{
    return m_orientation;
}

bool Model3DClipping::isEnabled() const
{
    return m_isEnabled;
}

void Model3DClipping::handle3DClippingStateChanged(std::string , FeatureState state)
{
    switch(state)
    {
    case FeatureState::ACTIVATED:
    {
        if(m_isActivated == false)
        {
            m_isActivated = true;
            notify(Model3DClippingEvent::CLIPPING_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::DEACTIVATED:
    {
        if(m_isActivated == true)
        {
            m_isActivated = false;
            notify(Model3DClippingEvent::CLIPPING_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::ENABLED:
        m_isEnabled = true;
        break;

    case FeatureState::DISABLED:
        m_isEnabled = false;

        if(m_isActivated == true)
        {
            m_isActivated = false;
            notify(Model3DClippingEvent::CLIPPING_STATE_CHANGED);
        }

        break;
    }
}
