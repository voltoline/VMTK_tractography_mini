#include "ModelDiffusionUI.h"

ModelDiffusionUI::ModelDiffusionUI(vmtkDiffAcquisition *acq)
{
    m_acq = acq;
    m_numDWIVolumes = m_acq->getDimensions(3);
    m_currentDWIIndex = 0;
    m_isDWIRenderingActivated = true;
}

void ModelDiffusionUI::setCurrentDWIIndex(unsigned short index)
{
    if(isNotifying())
        std::cerr << "ModelDTI::setCurrentDWIIndex -->> A notification is in progress!" << std::endl;

    if(index >= m_numDWIVolumes || index == m_currentDWIIndex)
        return;

    m_currentDWIIndex = index;

    addEvent(ModelDiffusionUIEvent::DWI_INDEX_CHANGED);
}

unsigned short ModelDiffusionUI::getCurrentDWIIndex()
{
    return m_currentDWIIndex;
}

unsigned short ModelDiffusionUI::getNumberOfDwiVolumes()
{
    return m_numDWIVolumes;
}

void ModelDiffusionUI::setDWIRenderingActivated(bool state)
{
    if(isNotifying())
        std::cerr << "ModelDTI::setDWIRenderingActivated -->> A notification is in progress!" << std::endl;

    if(state == m_isDWIRenderingActivated)
        return;

    m_isDWIRenderingActivated = state;

    addEvent(ModelDiffusionUIEvent::DWI_RENDERING_STATE_CHANGED);
}

bool ModelDiffusionUI::isDWIRenderingActivated()
{
    return m_isDWIRenderingActivated;
}

vmtkDiffAcquisition *ModelDiffusionUI::getAcquisition() const
{
    return m_acq;
}
