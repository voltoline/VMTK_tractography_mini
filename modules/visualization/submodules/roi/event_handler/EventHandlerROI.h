#ifndef EVENT_HANDLER_ROI_H
#define EVENT_HANDLER_ROI_H

#include "model/ModelObserver.h"
#include "../ModelROI.h"

class RenderingSurfaceSet;
class ROIRenderer2D;

class EventHandlerROI: public ModelObserver<ModelROI, ModelROISetEvent>
{
public:
    EventHandlerROI();
    ~EventHandlerROI();

    void update(ModelROI *subject, ModelROISetEvent event_type);

    void setActivationFreeDrawROI(bool activation);

    void setModelROI(ModelROI *modelROI);
    void setMainWindow(MainWindow *mainWindow);
    void setStateManager(StateManager *stateManager);

private:

    ModelROI *m_modelROI;
    MainWindow *m_mainWindow;
    StateManager *m_stateManager;

    bool m_activeFreeDrawROI;
    bool m_movableCursorWasEnabled;

    bool m_isUIBlocked;
};

#endif // EVENT_HANDLER_SLICE_2D_TO_ROI_H
