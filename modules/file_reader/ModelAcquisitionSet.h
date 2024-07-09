#ifndef MODEL_ACQUISITION_SET_H
#define MODEL_ACQUISITION_SET_H

#include <memory>
#include <vector>
#include "model/ModelSubject.h"
#include "modules/file_reader/dicom/vmtkAcquisitionAbstract.h"

/**
 * @brief Events to be triggered at the end of file importing.
 *
 */
enum class ModelAcquisitionSetEvent
{
    NONE,
    FILE_NOT_OPENED,  /**< not opened */
    ACQUISITION_ADDED, /**< new scan is added */
    ACQUISITION_REMOVED /**< scan is removed */
};

/** @defgroup Volume_Import Data Importing module
 *
 * This is the group of methods to import different modalities of medical scans.
 *
 * Two file formats are supported:
 * <a href="https://www.nema.org/Standards/Pages/Digital-Imaging-and-Communications-in-Medicine.aspx" target=_complement>dicom</a>
 * and <a href="https://nifti.nimh.nih.gov/" target=_complement>nifti</a>. 3D scans, including DWI, are supported.
 *
 */

/**
 * @ingroup Volume_Import
 *
 * @brief The ModelAcquisitionSet class
 */
class ModelAcquisitionSet: public ModelSubject<ModelAcquisitionSet, ModelAcquisitionSetEvent>
{
public:

    /**
     * @brief Constructor of an object of the class ModelAcquisitionSet.
     *
     */
    ModelAcquisitionSet();

    /**
     * @brief Load a file of Nifti format.
     *
     * @param file_path file.
     * @return ModelAcquisitionSetEvent trigerred event.
     */
    ModelAcquisitionSetEvent loadNiftiFile(std::string file_path);

    /**
     * @brief Get newly loaded data (file or folder).
     *
     * @return vmtkAcquisitionAbstract object corresponding to the newly loaded data.
     */
    vmtkAcquisitionAbstract* getNewAcquisition();

    ModelAcquisitionSetEvent addAcquisition(vmtkAcquisitionAbstract *acquisition);

    /**
     * @brief Remove a loaded scan.
     *
     * @param acquisition identifier od the loaded scan.
     * @return ModelAcquisitionSetEvent triggered event.
     */
    ModelAcquisitionSetEvent removeAcquisition(vmtkAcquisitionAbstract *acquisition);

    /**
     * @brief Get a loaded scan.
     *
     * @param i  index of the loaded scan.
     * @return vmtkAcquisitionAbstract *operator object corresponding to the loded scan.
     */
    vmtkAcquisitionAbstract* operator[](unsigned int i);

private:
    /**
     * @brief
     *
     * @param path
     * @return ModelAcquisitionSetEvent
     */
    std::vector<vmtkAcquisitionAbstract*> m_acquisitionVector; /**< array of loaded volumes. */
};

#endif // MODEL_ACQUISITION_SET_H
