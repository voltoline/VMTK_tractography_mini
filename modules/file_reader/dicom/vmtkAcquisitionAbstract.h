#ifndef VMTK_ACQUISITION_ABSTRACT_H
#define VMTK_ACQUISITION_ABSTRACT_H

#include <iostream>

enum vmtkAcquisitionType
{
    VMTK_NO_ACQUISITION,
    VMTK_MRI_T1_T2_ACQUISITION,
    VMTK_PET_ACQUISITION,
    VMTK_CT_ACQUISITION,
    VMTK_DIFF_ACQUISITION
};


class vmtkAcquisitionParameters
{
public:
    vmtkAcquisitionParameters() {}
    vmtkAcquisitionParameters(const std::string &path) { m_acquisition_path = path; }

    std::string getAcquisition_path() const { return m_acquisition_path; }
    void setAcquisition_path(const std::string &value) { m_acquisition_path = value; }

    vmtkAcquisitionType getType() const { return m_type; }
    void setType(const vmtkAcquisitionType &value) { m_type = value; }

private:
    vmtkAcquisitionType m_type;
    std::string m_acquisition_path;
};


class vmtkAcquisitionAbstract
{
public:
    vmtkAcquisitionAbstract(std::string acquisition_class_name);
    virtual ~vmtkAcquisitionAbstract() {}

    void setType(vmtkAcquisitionType type) { m_type = type; }
    unsigned short getType() const { return m_type; }

    std::string getacquisitionPath() const { return m_acquisition_path; }
    void setAcquisitionPath(const std::string &value) { m_acquisition_path = value; }

    short getId() const { return m_id; }
    void setId(short id) { m_id = id; }

    const std::string getModalityStr() const { return m_modality_str; }
    void setModalityStr(const std::string &modality_str) { m_modality_str = modality_str; }

    void printAttributes();

    std::string getInstitutionName() const;
    void setInstitutionName(const std::string &institution_name);

    std::string getManufacturesModel() const;
    void setManufacturesModel(const std::string &manufactures_model);

    std::string getSeriesDescription() const;
    void setSeriesDescription(const std::string &series_description);

    std::string getPatientID() const;
    void setPatientID(const std::string &patient_id);

    std::string getPatientBirthDate() const;
    void setPatientBirthDate(const std::string &patient_birth_date);

    std::string getStudyDescription() const;
    void setStudyDescription(const std::string &study_description);

    std::string getAcquisitionClassName() const;

private:
    vmtkAcquisitionType m_type;

    std::string m_acquisition_path;
    std::string m_modality_str;
    std::string m_institution_name;
    std::string m_manufactures_model;
    std::string m_series_description;
    std::string m_patient_id;
    std::string m_patient_birth_date;
    std::string m_study_description;

    std::string m_AcquisitionClassName;

    short m_id;
    static short m_id_counter;
};

#endif // VMTK_ACQUISITION_ABSTRACT_H
