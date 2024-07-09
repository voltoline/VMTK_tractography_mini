#include "FileManagerModule.h"

#include <fstream>
#include <modules/visualization/DiffusionRegisterVisualizationModule.h>

#include <modules/file_reader/dicom/vmtkDiffAcquisition.h>

#include <core/ProgressDialog.h>

FileManagerModule::FileManagerModule():
    BaseModule("FileManagerModule")
{
    m_modelAcquisitionSet = std::make_shared<ModelAcquisitionSet>();

    m_mainWindow = nullptr;
}

void FileManagerModule::init(MainWindow *mainWindow)
{

    ModelAcquisitionSetEvent event;
    ProgressDialog *progress_bar = mainWindow->createProgressDialog();

    progress_bar->setMinPercentage(0);
    progress_bar->setMaxPercentage(0);
    progress_bar->setTask([&event, this](){

        event = m_modelAcquisitionSet->loadNiftiFile("./volume/rvr_Reg_-_DTI_high_iso20_SENSE_20150509181941_602.nii");
        event = m_modelAcquisitionSet->loadNiftiFile("./volume/rvr_VBM_6min_SENSE_20150509181941_501.nii");

    });
    progress_bar->setLabelText("Loading NIfTI file...");
    progress_bar->start();
    progress_bar->finish();
    delete progress_bar;

    std::ifstream in("./volume/rvr_reg.reg");
    std::string aux;
    glm::mat4 m_registrationMatrix;
    glm::mat4 m_registrationMatrixInv;

    in >> aux;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            in >> m_registrationMatrix[i][j];

    in >> aux;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            in >> m_registrationMatrixInv[i][j];

    in.close();

    std::shared_ptr<DiffusionRegisterVisualizationModel> module = std::make_shared<DiffusionRegisterVisualizationModel>(static_cast<vmtkDiffAcquisition*>((*m_modelAcquisitionSet)[0]), static_cast<vmtkAcquisition*>((*m_modelAcquisitionSet)[1]), m_registrationMatrix, m_registrationMatrixInv);
    mainWindow->getModuleManager().addModule(module);
}

std::shared_ptr<ModelAcquisitionSet> FileManagerModule::getModelAcquisitionSet() const
{
    return m_modelAcquisitionSet;
}
