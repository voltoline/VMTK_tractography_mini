#include "EventHandlerROI.h"

EventHandlerROI::EventHandlerROI()
{
    m_activeFreeDrawROI = false;

    m_movableCursorWasEnabled = false;

    m_isUIBlocked = false;

    m_modelROI = nullptr;
    m_mainWindow = nullptr;
    m_stateManager = nullptr;
}

EventHandlerROI::~EventHandlerROI()
{

}

void EventHandlerROI::update(ModelROI */*subject*/, ModelROISetEvent event_type)
{
    switch(event_type)
    {
    case ModelROISetEvent::LAST_ROI_REMOVED:
    case ModelROISetEvent::ROI_REMOVED:
    case ModelROISetEvent::INDEX_ERROR:
    case ModelROISetEvent::ROI_ENDED:
    {
        std::cout << "EventHandlerROI::update:Ended ROI Event Capture" << std::endl;

        if(m_activeFreeDrawROI)
        {
            if(m_isUIBlocked)
            {
                m_isUIBlocked = false;

                if(m_mainWindow != nullptr)
                {
                    m_mainWindow->blockToolTabs(false);
                    m_mainWindow->blockVisualizationTabs(false);
                }

                if(m_stateManager->isActivated("LOCKED_CURSOR"))
                {
                    {
                        m_stateManager->deactivateFeature("LOCKED_CURSOR");
                    }
                }
            }
        }

        break;
    }

    case ModelROISetEvent::ROI_EDITION:
    {
        if(m_activeFreeDrawROI)
        {
            if(m_mainWindow != nullptr)
            {
                m_mainWindow->blockToolTabs(true);
                m_mainWindow->blockVisualizationTabs(true);

                m_isUIBlocked = true;
            }

            if(m_stateManager->isActivated("MOVABLE_CURSOR"))
            {
                m_movableCursorWasEnabled = true;
                m_stateManager->activateFeature("LOCKED_CURSOR");

            } else {
                m_movableCursorWasEnabled = false;
            }
        }
        break;
    }

    default:
        break;
    }
}

void EventHandlerROI::setActivationFreeDrawROI(bool activation)
{
    m_activeFreeDrawROI = activation;
}

void EventHandlerROI::setModelROI(ModelROI *modelROI)
{
    m_modelROI = modelROI;

    m_modelROI->subscribe(this, ModelROISetEvent::ROI_EDITION);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_ENDED);
    m_modelROI->subscribe(this, ModelROISetEvent::INDEX_ERROR);
    m_modelROI->subscribe(this, ModelROISetEvent::ROI_REMOVED);
    m_modelROI->subscribe(this, ModelROISetEvent::LAST_ROI_REMOVED);
}

void EventHandlerROI::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}

void EventHandlerROI::setStateManager(StateManager *stateManager)
{
    m_stateManager = stateManager;
}
