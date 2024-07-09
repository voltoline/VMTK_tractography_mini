#ifndef PROGRESS_DIALOG_H_
#define PROGRESS_DIALOG_H_

#include <stdexcept>
#include <functional>
#include <iostream>

/**
 * @brief Progress dialog interface for general propose use.
 *
 * All protected members are updated before onProgressUpdate() is called.
 */

class ProgressDialog
{

public:
    ProgressDialog();
    virtual ~ProgressDialog();

    virtual void start() = 0;
    virtual void finish() = 0;
    virtual void onProgressUpdate(bool update_label_text = false) = 0;

    int getCurrentPercentage() const;
    void setCurrentPercentage(int value);

    int getMinPercentage() const;
    void setMinPercentage(int minPercentage);

    int getMaxPercentage() const;
    void setMaxPercentage(int maxPercentage);

    std::string getLabelText() const;
    void setLabelText(const std::string &labelText);

    std::string getTitleText() const;
    void setTitleText(const std::string &titleText);

    void setTask(const std::function<void ()> &task);
    void executeTask();

    bool isTaskFinished() const;

    void resetState();

private:
    int mCurrentPercentage; /**< completion percentage (0-100) */
    int mMinPercentage;
    int mMaxPercentage;
    std::string mLabelText;
    std::string mTitleText;

    std::function<void()> m_task;
    bool m_taskFinished;
};

/**
 * @brief Exception to be thrown from ProgressDialog::onProgressUpdate() to interrupt the current process.
 */
class ProgressDialogAbortException : public std::exception {
public:
	const char* what() const throw();
};

#endif /* PROGRESS_DIALOG_H_ */
