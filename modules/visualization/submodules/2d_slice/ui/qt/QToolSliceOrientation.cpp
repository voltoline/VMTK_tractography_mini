#include "QToolSliceOrientation.h"
#include "ui_QToolSliceOrientation.h"

#include <QButtonGroup>

QToolSliceOrientation::QToolSliceOrientation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToolSliceOrientation)
{
    ui->setupUi(this);
}

QToolSliceOrientation::~QToolSliceOrientation()
{
    delete ui;
}

void QToolSliceOrientation::updateComponentsEnabled()
{
    bool state;

    if(getController() == nullptr)
        state = false;
    else
        state = getController()->isOrientationEnabled();

    /** Orientation buttons (Voxel-order) **/
    ui->m_comboBoxOrientation->setEnabled(state);
}

void QToolSliceOrientation::updateSliceOrientation()
{
    if(getController() == nullptr)
        return;

    ui->m_comboBoxOrientation->blockSignals(true);

    if(getController()->isNeurological())
        ui->m_comboBoxOrientation->setCurrentIndex(1);

    else if(getController()->isRadiological())
        ui->m_comboBoxOrientation->setCurrentIndex(0);

    ui->m_comboBoxOrientation->blockSignals(false);
}

void QToolSliceOrientation::on_m_comboBoxOrientation_currentIndexChanged(int index)
{
    if(index == 0)
        getController()->setRadiologicalMode();

    else if(index == 1)
        getController()->setNeurologicalMode();
}
