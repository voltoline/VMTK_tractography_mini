#include "ModelAcquisitionSet.h"
#include "nifti//NiftiFileLoader3DAcquisition.h"
#include "nifti/NiftiFileLoaderDiffusionAcquisition.h"

ModelAcquisitionSet::ModelAcquisitionSet()
{

}

ModelAcquisitionSetEvent ModelAcquisitionSet::loadNiftiFile(std::string file_path)
{
    NiftiFileLoader3DAcquisition nifti_loader;
    vmtkAcquisitionAbstract *acq = nifti_loader.loadImage(file_path);

    if(acq == nullptr)
    {
        NiftiFileLoaderDiffusionAcquisition nifti_loader;
        acq = nifti_loader.loadImage(file_path);
    }

    if(acq == nullptr)
        return ModelAcquisitionSetEvent::FILE_NOT_OPENED;

    m_acquisitionVector.push_back(acq);

    return ModelAcquisitionSetEvent::ACQUISITION_ADDED;
}

vmtkAcquisitionAbstract* ModelAcquisitionSet::getNewAcquisition()
{
    return m_acquisitionVector[m_acquisitionVector.size() - 1];
}

ModelAcquisitionSetEvent ModelAcquisitionSet::addAcquisition(vmtkAcquisitionAbstract *acquisition)
{
    if(acquisition != nullptr)
    {
        m_acquisitionVector.push_back(acquisition);
        return ModelAcquisitionSetEvent::ACQUISITION_ADDED;
    }

    return ModelAcquisitionSetEvent::NONE;
}

ModelAcquisitionSetEvent ModelAcquisitionSet::removeAcquisition(vmtkAcquisitionAbstract *acquisition)
{
    for(unsigned short i = 0; i < m_acquisitionVector.size(); i++)
        if(m_acquisitionVector[i]->getId() == acquisition->getId())
            m_acquisitionVector.erase(m_acquisitionVector.begin() + i);

    return ModelAcquisitionSetEvent::ACQUISITION_REMOVED;
}

vmtkAcquisitionAbstract *ModelAcquisitionSet::operator[](unsigned int i)
{
    if(i < m_acquisitionVector.size())
        return m_acquisitionVector[i];

    return nullptr;
}
