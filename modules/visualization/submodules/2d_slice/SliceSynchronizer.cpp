#include "SliceSynchronizer.h"

#include <iostream>

SliceSynchronizer::SliceSynchronizer():
    mFlagSynchronizationState(false),
    m_isSyncAxialAndBackFrontEnabled(false),
    m_isSyncCoronalAndBottomTopEnabled(false),
    m_isSyncSagittalAndRightLeftEnabled(false)
{
    m_isEnabled = true;
    m_stateManager = nullptr;

    m_syncStateChangedHandlerSlotID = -1;
}

void SliceSynchronizer::update(Model2DSlice *, Model2DSliceEvent event)
{
    if(event == Model2DSliceEvent::SLICING_CHANGED)
    {
        sync3DClipping();

    } else if(event == Model2DSliceEvent::VOXEL_ORDER_CHANGED) {

        mModel3DClipping->resetClipping();
        mModel3DClipping->notify();
    }
}

SliceSynchronizerEvent SliceSynchronizer::setSynchronizationActivated(bool state)
{
    if(mFlagSynchronizationState == state)
        return SliceSynchronizerEvent::NONE;

    mFlagSynchronizationState = state;

    if(mFlagSynchronizationState)
    {
        if(!m_stateManager->isActivated("SLICE_SYNCRONIZER"))
            m_stateManager->activateFeature("SLICE_SYNCRONIZER", m_syncStateChangedHandlerSlotID);

        m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);
        m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);

    } else {

        if(m_stateManager->isActivated("SLICE_SYNCRONIZER"))
            m_stateManager->deactivateFeature("SLICE_SYNCRONIZER", m_syncStateChangedHandlerSlotID);

        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);

        m_isSyncAxialAndBackFrontEnabled = false;
        m_isSyncCoronalAndBottomTopEnabled = false;
        m_isSyncSagittalAndRightLeftEnabled = false;
    }

    return SliceSynchronizerEvent::SYNC_STATE_CHANGED;
}

void SliceSynchronizer::sync3DClipping()
{
    syncBackFrontClipping();
    syncBottomTopClipping();
    syncRightLeftClipping();
}

//Should it be TopBottomClipping()? (Ting, 5/12/2019)
void SliceSynchronizer::syncBackFrontClipping()
{
    if((mFlagSynchronizationState && m_isSyncAxialAndBackFrontEnabled))
    {
        mSliceData.mAxial = m_model2DSlice->getAxialSlice(Model2DSlice::VoxelOrder::LPS);

        if(m_model2DSlice->isRadiological())
        {
            unsigned short new_back_clipping = mSliceData.mAxial;
            unsigned short old_back_clipping = mModel3DClipping->getBackClipping();

            if(new_back_clipping != old_back_clipping)
            {
                mModel3DClipping->setBackClipping(new_back_clipping);
                mModel3DClipping->notify();

            }
        } else {


            unsigned short new_front_clipping = mSliceData.mAxial;
            unsigned short old_front_clipping = mModel3DClipping->getFrontClipping();

            if(new_front_clipping != old_front_clipping)
            {
                mModel3DClipping->setFrontClipping(new_front_clipping);
                mModel3DClipping->notify();
            }
        }
    }
}

// Should it be BackFrontClipping()? Ting, 05/12/2019
void SliceSynchronizer::syncBottomTopClipping()
{
    if((mFlagSynchronizationState && m_isSyncCoronalAndBottomTopEnabled))
    {
        mSliceData.mCoronal = m_model2DSlice->getCoronalSlice();

        if(m_model2DSlice->isRadiological())
        {
            unsigned short new_top_clipping = mSliceData.mCoronal;
            unsigned short old_top_clipping = mModel3DClipping->getTopClipping();

           if(new_top_clipping != old_top_clipping)
            {
                mModel3DClipping->setTopClipping(new_top_clipping);
                mModel3DClipping->notify();
            }

        } else {

            unsigned short new_top_clipping = mSliceData.mCoronal;
            unsigned short old_top_clipping = mModel3DClipping->getTopClipping();

            if(new_top_clipping != old_top_clipping)
            {
                mModel3DClipping->setTopClipping(new_top_clipping);
                mModel3DClipping->notify();
            }
        }
    }
}

void SliceSynchronizer::syncRightLeftClipping()
{
    if((mFlagSynchronizationState && m_isSyncSagittalAndRightLeftEnabled))
    {

        mSliceData.mSagittal = m_model2DSlice->getSagittalSlice();

        if(m_model2DSlice->isRadiological())
        {
            unsigned short new_right_clipping = mSliceData.mSagittal;  //changed by Ting, 05/12/2019
            unsigned short old_right_clipping = mModel3DClipping->getLeftClipping();

            if(new_right_clipping != old_right_clipping)
            {
                mModel3DClipping->setLeftClipping(new_right_clipping);
                mModel3DClipping->notify();
            }

        } else {

            unsigned short new_left_clipping = mSliceData.mSagittal;
            unsigned short old_left_clipping = mModel3DClipping->getRightClipping();

            if(new_left_clipping != old_left_clipping)
            {
                mModel3DClipping->setRightClipping(new_left_clipping);
                mModel3DClipping->notify();
            }
        }

    }
}

bool SliceSynchronizer::isSynchronizationActivated() const
{
    return mFlagSynchronizationState;
}

void SliceSynchronizer::setModel3DClipping(Model3DClipping *model3DClipping)
{
    mModel3DClipping = model3DClipping;
}

void SliceSynchronizer::setModel2DSlice(Model2DSlice *model2DSlice)
{
    m_model2DSlice = model2DSlice;
}

SliceSynchronizerEvent SliceSynchronizer::enableSyncOnlyAxialAndBackFront(bool sync_3d_clipping)
{
    if(!m_isSyncAxialAndBackFrontEnabled)
    {
        m_isSyncAxialAndBackFrontEnabled = true;
        m_isSyncCoronalAndBottomTopEnabled = false;
        m_isSyncSagittalAndRightLeftEnabled = false;

        mModel3DClipping->resetClipping();
        mModel3DClipping->notify();

        if(sync_3d_clipping)
            sync3DClipping();

        return SliceSynchronizerEvent::SYNC_MODE_CHANGED;
    }

    return SliceSynchronizerEvent::NONE;
}

SliceSynchronizerEvent SliceSynchronizer::enableSyncOnlyCoronalAndBottomTop(bool sync_3d_clipping)
{
    if(!m_isSyncCoronalAndBottomTopEnabled)
    {
        m_isSyncAxialAndBackFrontEnabled = false;
        m_isSyncCoronalAndBottomTopEnabled = true;
        m_isSyncSagittalAndRightLeftEnabled = false;

        mModel3DClipping->resetClipping();
        mModel3DClipping->notify();

        if(sync_3d_clipping)
            sync3DClipping();

        return SliceSynchronizerEvent::SYNC_MODE_CHANGED;
    }

    return SliceSynchronizerEvent::NONE;
}

SliceSynchronizerEvent SliceSynchronizer::enableSyncOnlySagittalAndRightLeft(bool sync_3d_clipping)
{
    if(!m_isSyncSagittalAndRightLeftEnabled)
    {
        m_isSyncAxialAndBackFrontEnabled = false;
        m_isSyncCoronalAndBottomTopEnabled = false;
        m_isSyncSagittalAndRightLeftEnabled = true;

        mModel3DClipping->resetClipping();
        mModel3DClipping->notify();

        if(sync_3d_clipping)
            sync3DClipping();

        return SliceSynchronizerEvent::SYNC_MODE_CHANGED;
    }

    return SliceSynchronizerEvent::NONE;
}

bool SliceSynchronizer::isSyncSagittalAndRightLeftEnabled() const
{
    return m_isSyncSagittalAndRightLeftEnabled;
}

bool SliceSynchronizer::isSyncCoronalAndBottomTopEnabled() const
{
    return m_isSyncCoronalAndBottomTopEnabled;
}

bool SliceSynchronizer::isSyncAxialAndBackFrontEnabled() const
{
    return m_isSyncAxialAndBackFrontEnabled;
}

void SliceSynchronizer::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("SLICE_SYNCRONIZER", m_syncStateChangedHandlerSlotID);
        m_stateManager->removeFeature("SLICE_SYNCRONIZER");
    }

    m_stateManager = state_manager;

    if(m_stateManager->addStateWithRestrictions("SLICE_SYNCRONIZER", std::vector<std::string>{""}))
    {
        m_syncStateChangedHandlerSlotID = m_stateManager->subscribe("SLICE_SYNCRONIZER",  CREATE_SLOT(this, &SliceSynchronizer::handleSyncStateChanged));

    }
}

void SliceSynchronizer::handleSyncStateChanged(std::string , FeatureState state)
{
    switch(state)
    {
    case FeatureState::ACTIVATED:
    {
        if(mFlagSynchronizationState == false)
        {
            setSynchronizationActivated(true);
            notify(SliceSynchronizerEvent::SYNC_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::DEACTIVATED:
    {
        if(mFlagSynchronizationState == true)
        {
            setSynchronizationActivated(false);
            notify(SliceSynchronizerEvent::SYNC_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::ENABLED:
    {
        if(m_isEnabled == false)
        {
            m_isEnabled = true;
            notify(SliceSynchronizerEvent::SYNC_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::DISABLED:
    {
        if(m_isEnabled == true)
        {
            m_isEnabled = false;
            notify(SliceSynchronizerEvent::SYNC_STATE_CHANGED);
        }

        break;
    }

    }
}

SliceSynchronizerEvent SliceSynchronizer::stopSyncAllSlices()
{
    m_isSyncAxialAndBackFrontEnabled = false;
    m_isSyncCoronalAndBottomTopEnabled = false;
    m_isSyncSagittalAndRightLeftEnabled = false;

    return SliceSynchronizerEvent::SYNC_MODE_CHANGED;
}

bool SliceSynchronizer::isEnabled() const
{
    return m_isEnabled;
}
