#include "QToolVolumeBlending.h"
#include "ui_QToolVolumeBlending.h"

QToolVolumeBlending::QToolVolumeBlending(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToolVolumeBlending)
{
    ui->setupUi(this);

    connect(ui->m_horizontalSliderBlender, SIGNAL(valueChanged(int)),ui->m_spinBoxBlender, SLOT(setValue(int)));
    connect(ui->m_spinBoxBlender, SIGNAL(valueChanged(int)),ui->m_horizontalSliderBlender, SLOT(setValue(int)));
}

void QToolVolumeBlending::updateVolumeBlendingEnabled()
{
    if(getController() != nullptr /*&& mPtrSecondPartController->isUIEnabled()*/)
        enableComponents(true);

    else
        enableComponents(false);
}

void QToolVolumeBlending::enableComponents(bool state)
{
    ui->m_horizontalSliderBlender->setEnabled(state);
    ui->m_spinBoxBlender->setEnabled(state);
}

void QToolVolumeBlending::on_m_horizontalSliderBlender_valueChanged(int value)
{
    getController()->setBlendingFactor(value);
}

QToolVolumeBlending::~QToolVolumeBlending()
{
    delete ui;
}

void QToolVolumeBlending::updateVolumeBlending()
{
    ui->m_horizontalSliderBlender->blockSignals(true);
    ui->m_horizontalSliderBlender->setValue(getController()->getBlendingFactor());
    ui->m_horizontalSliderBlender->blockSignals(false);

    ui->m_spinBoxBlender->blockSignals(true);
    ui->m_spinBoxBlender->setValue(getController()->getBlendingFactor());
    ui->m_spinBoxBlender->blockSignals(false);
}
