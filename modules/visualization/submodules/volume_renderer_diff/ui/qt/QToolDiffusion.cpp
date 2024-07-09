#include "QToolDiffusion.h"
#include "ui_QToolDiffusion.h"

#include <QButtonGroup>

QToolDiffusion::QToolDiffusion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToolDiffusion)
{
    ui->setupUi(this);

    m_buttonGroup = new QButtonGroup;
    m_buttonGroup->addButton(ui->m_radioButtonDWI);

    ui->m_spinBoxVolumeNumber->blockSignals(true);
    ui->m_spinBoxVolumeNumber->setMinimum(1);
    ui->m_spinBoxVolumeNumber->blockSignals(false);
}

QToolDiffusion::~QToolDiffusion()
{
    delete ui;
}

void QToolDiffusion::updateDiffusion()
{
    if(getController() == nullptr)
        return;

    if(getController()->isDWIRenderingActivated())
    {
        ui->m_radioButtonDWI->blockSignals(true);
        ui->m_radioButtonDWI->setChecked(true);
        ui->m_radioButtonDWI->blockSignals(false);

        ui->m_spinBoxVolumeNumber->setEnabled(true);

    } else {

        ui->m_spinBoxVolumeNumber->setEnabled(false);
    }

    ui->m_spinBoxVolumeNumber->blockSignals(true);
    ui->m_spinBoxVolumeNumber->setValue(getController()->getDWIVolumeNumber());
    ui->m_spinBoxVolumeNumber->setMaximum(getController()->getDWIVolumeNumberMax());
    ui->m_spinBoxVolumeNumber->blockSignals(false);
}

void QToolDiffusion::updateDiffusionEnabled()
{
    bool state;

    if(getController() == nullptr)
        state = false;
    else
        state = getController()->isDiffusionEnabled();

    ui->m_radioButtonDWI->setEnabled(state);
    ui->m_spinBoxVolumeNumber->setEnabled(state);
}

void QToolDiffusion::on_m_radioButtonDWI_toggled(bool checked)
{
    getController()->setDWIRenderingActivated(checked);

    ui->m_spinBoxVolumeNumber->setEnabled(checked);
}

void QToolDiffusion::on_m_spinBoxVolumeNumber_valueChanged(int value)
{
    getController()->setDWIVolumeNumber(value);
}

QButtonGroup& QToolDiffusion::getButtonGroup() const
{
    return *m_buttonGroup;
}
