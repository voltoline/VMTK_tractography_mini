#include "ModelTransferFunction.h"

ModelTransferFunction::ModelTransferFunction(){
    m_deltaX = 0;
    m_deltaY = 0;

    m_filePathTFLoaded = "";
    m_filePathTFSaved = "";

    m_equalizated = false;
}

ModelTransferFunctionEvent ModelTransferFunction::setWindowingDelta(int deltaX, int deltaY)
{
    m_deltaX = deltaX;
    m_deltaY = deltaY;

    return ModelTransferFunctionEvent::WINDOWING_CHANGED;
}

void ModelTransferFunction::getWindowingDelta(int &deltaX, int &deltaY)
{
    deltaX = m_deltaX;
    deltaY = m_deltaY;
}

void ModelTransferFunction::setAcquisitionMinMax(int min, int max)
{
    m_acqUmin = min;
    m_acqUmax = max;
}

void ModelTransferFunction::getAcquisitionMinMax(int &min, int &max)
{
    min = m_acqUmin;
    max = m_acqUmax;
}

void ModelTransferFunction::setWindowingSteps(int stepLevel, int stepWidth)
{
    m_stepLevel = stepLevel;
    m_stepWidth = stepWidth;
}

void ModelTransferFunction::getWindowingSteps(int &stepLevel, int &stepWidth)
{
    stepLevel = m_stepLevel;
    stepWidth = m_stepWidth;
}

void ModelTransferFunction::initiateWindowingFunction(int tfMin, int tfMax)
{
    m_win_center = tfMin+tfMax/2;
    m_win_width = tfMax - tfMin;
}

void ModelTransferFunction::updateWindowingFunction(int win_center, int win_width)
{
    m_win_center = win_center;
    m_win_width = win_width;
}

void ModelTransferFunction::getWindowingFunction(int &win_center, int &win_width)
{
    win_center =  m_win_center;
    win_width = m_win_width;
}

void ModelTransferFunction::estimateStepSize(int width, int height)
{
    //Ting: at least 5 intensity values per pixel (23/01/2022)
   if (width > height) {
       m_stepLevel = m_stepWidth = (m_acqUmax-m_acqUmin)/height;
   } else {
       m_stepLevel = m_stepWidth = (m_acqUmax-m_acqUmin)/width;
   }
   if (m_stepLevel < 5) m_stepLevel = m_stepWidth = 5;
}


void ModelTransferFunction::setFilePathTFSaved(const std::string &newFilePathTFSaved){
    m_filePathTFSaved = newFilePathTFSaved;
}

bool ModelTransferFunction::equalizated() const{
    return m_equalizated;
}

ModelTransferFunctionEvent ModelTransferFunction::setEqualizated(bool newEqualizated){
    m_equalizated = newEqualizated;
    return ModelTransferFunctionEvent::EQUALIZE_CHANGED;
}

void ModelTransferFunction::setFilePathTFLoaded(const std::string &newFilePathTFLoaded){
    m_filePathTFLoaded = newFilePathTFLoaded;
}

const std::string &ModelTransferFunction::filePathTFSaved() const{
    return m_filePathTFSaved;
}

ModelTransferFunctionEvent ModelTransferFunction::saveFileTF(std::string filePath){
    m_filePathTFSaved = filePath;
    return ModelTransferFunctionEvent::TF_FILE_SAVED;
}

const std::string &ModelTransferFunction::filePathTFLoaded() const{
    return m_filePathTFLoaded;
}

ModelTransferFunctionEvent ModelTransferFunction::loadFileTF(std::string filePath){
    m_filePathTFLoaded = filePath;
    return ModelTransferFunctionEvent::TF_FILE_LOADED;
}

