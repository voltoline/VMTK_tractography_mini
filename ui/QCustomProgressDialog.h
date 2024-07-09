#ifndef QCUSTOM_PROGRESS_DIALOG_H_
#define QCUSTOM_PROGRESS_DIALOG_H_

#include "core/ProgressDialog.h"

#include <QObject>
#include <QThread>

class QProgressDialog;
class QWidget;

class QCustomProgressDialog: public QObject, public ProgressDialog {
public:
    QCustomProgressDialog(QWidget *parent);
    ~QCustomProgressDialog();

    virtual void onProgressUpdate(bool update_label_text = false);
    virtual void start();
    virtual void finish();

private:
    void createProgressDialog();

    QProgressDialog *m_progressDialog;
    QWidget *m_parent;
};

class QCustomProgressDialogWorker : public QThread
{
public:
    QCustomProgressDialogWorker(QCustomProgressDialog *progress_bar): m_progressBar(progress_bar) { }

private:
    void run() override
    {
        if(m_progressBar != nullptr)
            m_progressBar->executeTask();
    }

    QCustomProgressDialog *m_progressBar;
};

#endif // QCUSTOM_PROGRESS_DIALOG_H_
