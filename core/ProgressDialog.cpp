#include "ProgressDialog.h"

ProgressDialog::ProgressDialog()
{
    mCurrentPercentage = 0;
    mMinPercentage = 0;
    mMaxPercentage = 100;
    mLabelText = "Processing...";
    mTitleText = "VMTK";

    m_taskFinished = false;
    m_task = [](){ };
}

ProgressDialog::~ProgressDialog()
{

}

int ProgressDialog::getCurrentPercentage() const
{
    return mCurrentPercentage;
}

void ProgressDialog::setCurrentPercentage(int value)
{
    mCurrentPercentage = value;
}

int ProgressDialog::getMinPercentage() const
{
    return mMinPercentage;
}

void ProgressDialog::setMinPercentage(int minPercentage)
{
    mMinPercentage = minPercentage;
}

int ProgressDialog::getMaxPercentage() const
{
    return mMaxPercentage;
}

void ProgressDialog::setMaxPercentage(int maxPercentage)
{
    mMaxPercentage = maxPercentage;
}

std::string ProgressDialog::getLabelText() const
{
    return mLabelText;
}

void ProgressDialog::setLabelText(const std::string &labelText)
{
    mLabelText = labelText;
}

std::string ProgressDialog::getTitleText() const
{
    return mTitleText;
}

void ProgressDialog::setTitleText(const std::string &titleText)
{
    mTitleText = titleText;
}

void ProgressDialog::setTask(const std::function<void ()> &task)
{
    m_task = task;
}

void ProgressDialog::executeTask()
{
    m_taskFinished = false;
    m_task();
    m_taskFinished = true;
}

bool ProgressDialog::isTaskFinished() const
{
    return m_taskFinished;
}

void ProgressDialog::resetState()
{
    m_taskFinished = false;
}

const char* ProgressDialogAbortException::what() const throw() {
    return "ProgressDialogAbortException";
}
