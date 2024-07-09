#ifndef SLICE_SYNCHRONIZER_H
#define SLICE_SYNCHRONIZER_H

#include "model/ModelSubject.h"
#include "modules/visualization/StateManager.h"
#ifdef usinglib_slice2d
#include "internal_libraries/slice2d/include/Model2DSlice.h"
#else
#include "model/Model2DSlice.h"
#endif
#include "modules/visualization/submodules/3d_clipping/models/Model3DClipping.h"

enum class SliceSynchronizerEvent
{
    NONE,
    SYNC_STATE_CHANGED,
    SYNC_MODE_CHANGED
};

class SliceSynchronizer:
        public ModelSubject<SliceSynchronizer, SliceSynchronizerEvent>,
        public ModelObserver<Model2DSlice, Model2DSliceEvent>
{
public:
    SliceSynchronizer();

    void update(Model2DSlice *subject, Model2DSliceEvent event);

    void setModel3DClipping(Model3DClipping *model3DClipping);
    void setModel2DSlice(Model2DSlice *model2DSlice);

    bool isSyncAxialAndBackFrontEnabled() const;
    bool isSyncCoronalAndBottomTopEnabled() const;
    bool isSyncSagittalAndRightLeftEnabled() const;

    SliceSynchronizerEvent setSynchronizationActivated(bool state);
    bool isSynchronizationActivated() const;

    bool isEnabled() const;

    void setStateManager(StateManager *state_manager);
    void handleSyncStateChanged(std::string feature_name, FeatureState state);

    SliceSynchronizerEvent stopSyncAllSlices();

    SliceSynchronizerEvent enableSyncOnlyAxialAndBackFront(bool sync_3d_clipping = true);
    SliceSynchronizerEvent enableSyncOnlyCoronalAndBottomTop(bool sync_3d_clipping = true);
    SliceSynchronizerEvent enableSyncOnlySagittalAndRightLeft(bool sync_3d_clipping = true);

private:
    void sync3DClipping();

    typedef struct
    {
        unsigned short mAxial;
        unsigned short mCoronal;
        unsigned short mSagittal;

    } SliceData;

    typedef struct
    {
        unsigned short mLeft;
        unsigned short mRight;
        unsigned short mBottom;
        unsigned short mTop;
        unsigned short mBack;
        unsigned short mFront;

    } ClippingData;

    void syncBackFrontClipping();
    void syncBottomTopClipping();
    void syncRightLeftClipping();

    void syncSagittalSlice();
    void syncCoronalSlice();
    void syncAxialSlice();

    Model3DClipping *mModel3DClipping;
    Model2DSlice *m_model2DSlice;

    SliceData mSliceData;

    StateManager *m_stateManager;

    bool mFlagSynchronizationState;
    bool m_isEnabled;
    int m_syncStateChangedHandlerSlotID;

    bool m_isSyncAxialAndBackFrontEnabled;
    bool m_isSyncCoronalAndBottomTopEnabled;
    bool m_isSyncSagittalAndRightLeftEnabled;
};

#endif // SLICE_SYNCHRONIZER_H
