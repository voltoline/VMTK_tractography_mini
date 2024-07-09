#include "vmtkAcquisitionAbstract.h"

short vmtkAcquisitionAbstract::m_id_counter = 0;

vmtkAcquisitionAbstract::vmtkAcquisitionAbstract(std::string acquisition_class_name):
    m_type(VMTK_NO_ACQUISITION),
    m_acquisition_path(""),
    m_modality_str(""),
    m_institution_name(""),
    m_manufactures_model(""),
    m_series_description(""),
    m_patient_id(""),
    m_patient_birth_date(""),
    m_study_description(""),
    m_AcquisitionClassName(acquisition_class_name),
    m_id(-1)
{
    m_id = m_id_counter;
    m_id_counter++;
}

void vmtkAcquisitionAbstract::printAttributes()
{
    std::cout << "************************* vmtkAcquisitionAbstract Attributes *************************" << std::endl;

    std::cout << "Acquisition path: " << m_acquisition_path << std::endl;

    if(m_type == VMTK_NO_ACQUISITION)
        std::cout << "Type: NO_ACQUISITION" << std::endl;

    if(m_type == VMTK_MRI_T1_T2_ACQUISITION)
        std::cout << "Type: MRI_T1_T2_ACQUISITION" << std::endl;

    if(m_type == VMTK_PET_ACQUISITION)
        std::cout << "Type: PET_ACQUISITION" << std::endl;

    std::cout << "Modality: " << m_modality_str << std::endl;

    std::cout << "**************************************************************************************" << std::endl;
}

std::string vmtkAcquisitionAbstract::getInstitutionName() const
{
    return m_institution_name;
}

void vmtkAcquisitionAbstract::setInstitutionName(const std::string &institution_name)
{
    m_institution_name = institution_name;
}

std::string vmtkAcquisitionAbstract::getManufacturesModel() const
{
    return m_manufactures_model;
}

void vmtkAcquisitionAbstract::setManufacturesModel(const std::string &manufactures_model)
{
    m_manufactures_model = manufactures_model;
}

std::string vmtkAcquisitionAbstract::getSeriesDescription() const
{
    return m_series_description;
}

void vmtkAcquisitionAbstract::setSeriesDescription(const std::string &series_description)
{
    m_series_description = series_description;
}

std::string vmtkAcquisitionAbstract::getPatientID() const
{
    return m_patient_id;
}

void vmtkAcquisitionAbstract::setPatientID(const std::string &patient_id)
{
    m_patient_id = patient_id;
}

std::string vmtkAcquisitionAbstract::getPatientBirthDate() const
{
    return m_patient_birth_date;
}

void vmtkAcquisitionAbstract::setPatientBirthDate(const std::string &patient_birth_date)
{
    m_patient_birth_date = patient_birth_date;
}

std::string vmtkAcquisitionAbstract::getStudyDescription() const
{
    return m_study_description;
}

void vmtkAcquisitionAbstract::setStudyDescription(const std::string &study_description)
{
    m_study_description = study_description;
}

std::string vmtkAcquisitionAbstract::getAcquisitionClassName() const
{
    return m_AcquisitionClassName;
}
