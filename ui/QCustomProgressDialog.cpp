#include "QCustomProgressDialog.h"

#include <QCoreApplication>
#include <QProgressDialog>
#include <QApplication>
#ifdef enableOldQGL
#include <QDesktopWidget>
#endif

QCustomProgressDialog::QCustomProgressDialog(QWidget *parent):
    QObject(parent)
{
    m_progressDialog = nullptr;
    m_parent = parent;
}

QCustomProgressDialog::~QCustomProgressDialog()
{
    finish();
}

void QCustomProgressDialog::start()
{
    if(m_progressDialog != nullptr)
        delete m_progressDialog;

    resetState();
    createProgressDialog();

    QCustomProgressDialogWorker worker(this);
    worker.start();

    while(!isTaskFinished())
    {
        QCoreApplication::processEvents();
        QObject::thread()->msleep(50);
    }

    worker.quit();
    worker.wait();
}

void QCustomProgressDialog::finish()
{
    if(m_progressDialog != nullptr)
    {
        delete m_progressDialog;
        m_progressDialog = nullptr;
    }
}

void QCustomProgressDialog::createProgressDialog()
{
    m_progressDialog = new QProgressDialog(getLabelText().c_str(),
                                           QString(),
                                           /* min */ getMinPercentage(),
                                           /* max */ getMaxPercentage(),
                                           /* parent */ m_parent,
                                           /* flags */ Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    m_progressDialog->setWindowTitle(getTitleText().c_str());
    m_progressDialog->setWindowModality(Qt::WindowModal);
    m_progressDialog->show();

    QPoint pos = m_parent->mapToGlobal(m_parent->pos());
    m_progressDialog->move((pos.x() + m_parent->width() - m_progressDialog->width()) / 2, (pos.y() + m_parent->height() - m_progressDialog->height()) / 2);
}

void QCustomProgressDialog::onProgressUpdate(bool update_label_text)
{
    if(m_progressDialog == nullptr)
        return;

    if(update_label_text)
        m_progressDialog->setLabelText(getLabelText().c_str());

    m_progressDialog->setValue(getCurrentPercentage());

    if (m_progressDialog->wasCanceled()) {
        throw ProgressDialogAbortException();
    }
}
