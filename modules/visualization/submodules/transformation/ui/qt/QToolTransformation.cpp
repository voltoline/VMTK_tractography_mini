#include "QToolTransformation.h"
#include "ui_QToolTransformation.h"

#include <QButtonGroup>

QToolTransformation::QToolTransformation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToolTransformation)
{
    ui->setupUi(this);

    m_buttonGroupMouseTools = new QButtonGroup;
    m_buttonGroupMouseTools->addButton(ui->m_radioButtonMouseRotation);
    m_buttonGroupMouseTools->addButton(ui->m_radioButtonMouseScale);
    m_buttonGroupMouseTools->addButton(ui->m_radioButtonMouseMove);

    connect(m_buttonGroupMouseTools, SIGNAL(buttonClicked(int)), this,SLOT(m_buttonGroupMouseTools_buttonClicked(int)));
}

QToolTransformation::~QToolTransformation()
{
    delete ui;
}

void QToolTransformation::updateTransformationEnabled()
{
    if(getController() == nullptr)
    {
        ui->m_checkBoxActive3D->setEnabled(false);

        ui->m_checkBoxActive3D->blockSignals(true);
        ui->m_checkBoxActive3D->setChecked(false);
        ui->m_checkBoxActive3D->blockSignals(false);

        ui->m_checkBoxActive2D->setEnabled(false);

        ui->m_checkBoxActive2D->blockSignals(true);
        ui->m_checkBoxActive2D->setChecked(false);
        ui->m_checkBoxActive2D->blockSignals(false);

        ui->m_radioButtonMouseMove->setEnabled(false);
        ui->m_radioButtonMouseRotation->setEnabled(false);
        ui->m_radioButtonMouseScale->setEnabled(false);
        ui->m_pushButtonResetMouseTools->setEnabled(false);

    } else {
        ui->m_checkBoxActive3D->setEnabled(getController()->is3DTransformationEnabled());

        ui->m_checkBoxActive3D->blockSignals(true);
        ui->m_checkBoxActive3D->setChecked(getController()->is3DTransformationActivated());
        ui->m_checkBoxActive3D->blockSignals(false);

        ui->m_checkBoxActive2D->setEnabled(getController()->is2DTransformationEnabled());

        ui->m_checkBoxActive2D->blockSignals(true);
        ui->m_checkBoxActive2D->setChecked(getController()->is2DTransformationActivated());
        ui->m_checkBoxActive2D->blockSignals(false);

        bool state = getController()->is3DTransformationActivated() || getController()->is2DTransformationActivated();

        if(getController()->is3DTransformationEnabled() == false && getController()->is2DTransformationEnabled() == false)
            state = false;

        ui->m_radioButtonMouseMove->setEnabled(state);
        ui->m_radioButtonMouseRotation->setEnabled(state);
        ui->m_radioButtonMouseScale->setEnabled(state);
        ui->m_pushButtonResetMouseTools->setEnabled(state);
    }
}

void QToolTransformation::updateMouseMode()
{
    if(getController() == nullptr)
        return;

    short int mouse_mode = getController()->getMouseMode();

    m_buttonGroupMouseTools->blockSignals(true);

    switch(mouse_mode)
    {
    case -2:
    {
        ui->m_radioButtonMouseRotation->blockSignals(true);
        ui->m_radioButtonMouseRotation->setChecked(true);
        ui->m_radioButtonMouseRotation->blockSignals(false);
        break;
    }

    case -3:
    {
        ui->m_radioButtonMouseScale->blockSignals(true);
        ui->m_radioButtonMouseScale->setChecked(true);
        ui->m_radioButtonMouseScale->blockSignals(false);
        break;
    }

    case -4:
    {
        ui->m_radioButtonMouseMove->blockSignals(true);
        ui->m_radioButtonMouseMove->setChecked(true);
        ui->m_radioButtonMouseMove->blockSignals(false);
        break;
    }

    }

    m_buttonGroupMouseTools->blockSignals(false);
}

void QToolTransformation::m_buttonGroupMouseTools_buttonClicked(int mode)
{
    getController()->setMouseMode(mode);
}

void QToolTransformation::on_m_pushButtonResetMouseTools_clicked()
{
    getController()->resetTransformations();
}

void QToolTransformation::on_m_checkBoxActive3D_toggled(bool checked)
{
    getController()->set3DTransformationActivated(checked);

    bool state = checked || getController()->is2DTransformationActivated();

    ui->m_radioButtonMouseMove->setEnabled(state);
    ui->m_radioButtonMouseRotation->setEnabled(state);
    ui->m_radioButtonMouseScale->setEnabled(state);
    ui->m_pushButtonResetMouseTools->setEnabled(state);
}

void QToolTransformation::on_m_checkBoxActive2D_toggled(bool checked)
{
    getController()->set2DTransformationActivated(checked);

    bool state = checked || getController()->is3DTransformationActivated();

    ui->m_radioButtonMouseMove->setEnabled(state);
    ui->m_radioButtonMouseRotation->setEnabled(state);
    ui->m_radioButtonMouseScale->setEnabled(state);
    ui->m_pushButtonResetMouseTools->setEnabled(state);
}
