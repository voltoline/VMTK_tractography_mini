#ifndef QSPLASHSCREENCOPYRIGHT_H
#define QSPLASHSCREENCOPYRIGHT_H

#include "ui/ui_QSplashScreenCopyright.h"
#include <QWidget>

namespace Ui {
class QSplashScreenCopyright;
}

class QSplashScreenCopyright : public QWidget
{
    Q_OBJECT

public:
    explicit QSplashScreenCopyright(QWidget *parent = 0);
    ~QSplashScreenCopyright();
    void setSecondsAutoClose(int seconds){this->m_milisecondsAutoClose=seconds*1000;}

private slots:

    void on_m_start_app_clicked();
    void counterTimeClose();

private:
    Ui::QSplashScreenCopyright *ui;
    QTimer *t;
    int counterTime;
    int m_milisecondsAutoClose;
};

#endif // QSPLASHSCREENCOPYRIGHT_H
