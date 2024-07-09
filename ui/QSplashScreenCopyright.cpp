#include "ui/QSplashScreenCopyright.h"
#include <QTimer>
#include <iostream>

QSplashScreenCopyright::QSplashScreenCopyright(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSplashScreenCopyright)
{
    ui->setupUi(this);
    t = new QTimer(this);
    counterTime = 0;
    m_milisecondsAutoClose = 5000; //Defaults to 5000 milliseconds for the auto-close.
    connect(t, SIGNAL(timeout()), this, SLOT(counterTimeClose()));
    t->start(1000);
}

QSplashScreenCopyright::~QSplashScreenCopyright()
{
    delete ui;
}

void QSplashScreenCopyright::on_m_start_app_clicked()
{
    close();
    disconnect(t, SIGNAL(timeout()), this, SLOT(counterTimeClose()));
    delete t;
    close();
}

void QSplashScreenCopyright::counterTimeClose()
{
     counterTime += t->interval();
     if(counterTime == m_milisecondsAutoClose)
     {
         t->stop();
         disconnect(t, SIGNAL(timeout()), this, SLOT(counterTimeClose()));
         delete t;
         close();
     }
}
