#include "QTool2D3DSlicing.h"
#include "ui_QTool2D3DSlicing.h"

QTool2D3DSlicing::QTool2D3DSlicing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTool2D3DSlicing)
{
    ui->setupUi(this);

    ui->m_comboBox3DMode->addItem("Deactivated");
    ui->m_comboBox3DMode->addItem("3D Slicing");
    ui->m_comboBox3DMode->addItem("2D Stacking");
    ui->m_comboBox3DMode->addItem("3D Clipping");
}

QTool2D3DSlicing::~QTool2D3DSlicing()
{
    delete ui;
}

void QTool2D3DSlicing::updateComponentsEnabled()
{
        bool state = false;
        ui->m_comboBox3DMode->setEnabled(state);

        /** Clipping slider and spinnerBox**/
        ui->m_sliderClipXLeft->setEnabled(state);
        ui->m_sliderClipXRight->setEnabled(state);
        ui->m_sliderClipYBottom->setEnabled(state);
        ui->m_sliderClipYTop->setEnabled(state);
        ui->m_sliderClipZBack->setEnabled(state);
        ui->m_sliderClipZFront->setEnabled(state);
        ui->m_spinBoxXLeftClipping->setEnabled(state);
        ui->m_spinBoxXRightClipping->setEnabled(state);
        ui->m_spinBoxYBottomClipping->setEnabled(state);
        ui->m_spinBoxYTopClipping->setEnabled(state);
        ui->m_spinBoxZBackClipping->setEnabled(state);
        ui->m_spinBoxZFrontClipping->setEnabled(state);

        ui->m_pushButtonResetEdge->setEnabled(state);
        ui->m_pushButtonResetMiddle->setEnabled(state);

    if(getController() != nullptr) {

        ui->m_comboBox3DMode->setEnabled(true);

        bool state_3d_clipping = getController()->is3DClippingActivated();
        bool state_slice_3d = getController()->is3DSliceActivated();
        bool state_2d_slice = getController()->is2DSliceActivated();


        ui->m_comboBox3DMode->blockSignals(true);
        if(state_slice_3d)
            ui->m_comboBox3DMode->setCurrentIndex(1);
        else if(state_2d_slice)
            ui->m_comboBox3DMode->setCurrentIndex(2);
        else if(state_3d_clipping)
            ui->m_comboBox3DMode->setCurrentIndex(3);
        else {
            ui->m_comboBox3DMode->setCurrentIndex(0);
            ui->m_comboBox3DMode->blockSignals(false);
            return;
        }
        ui->m_comboBox3DMode->blockSignals(false);

        ui->m_pushButtonResetEdge->setEnabled(true);
        ui->m_pushButtonResetMiddle->setEnabled(true);

        if(state_3d_clipping)
        {
            /** Clipping slider and spinnerBox**/
            ui->m_sliderClipXLeft->setEnabled(state_3d_clipping);
            ui->m_sliderClipXRight->setEnabled(state_3d_clipping);
            ui->m_sliderClipYBottom->setEnabled(state_3d_clipping);
            ui->m_sliderClipYTop->setEnabled(state_3d_clipping);
            ui->m_sliderClipZBack->setEnabled(state_3d_clipping);
            ui->m_sliderClipZFront->setEnabled(state_3d_clipping);
            ui->m_spinBoxXLeftClipping->setEnabled(state_3d_clipping);
            ui->m_spinBoxXRightClipping->setEnabled(state_3d_clipping);
            ui->m_spinBoxYBottomClipping->setEnabled(state_3d_clipping);
            ui->m_spinBoxYTopClipping->setEnabled(state_3d_clipping);
            ui->m_spinBoxZBackClipping->setEnabled(state_3d_clipping);
            ui->m_spinBoxZFrontClipping->setEnabled(state_3d_clipping);

        } else {

            bool state = state_slice_3d || state_2d_slice;

            ui->m_sliderClipYTop->setEnabled(state);
            ui->m_sliderClipZBack->setEnabled(state);
            ui->m_spinBoxYTopClipping->setEnabled(state);
            ui->m_spinBoxZBackClipping->setEnabled(state);

            ui->m_spinBoxXLeftClipping->setEnabled(state);
            ui->m_sliderClipXLeft->setEnabled(state);

            ui->m_sliderClipXRight->setEnabled(state);
            ui->m_spinBoxXRightClipping->setEnabled(state);

            ui->m_sliderClipYBottom->setEnabled(false);
            ui->m_sliderClipZFront->setEnabled(false);
            ui->m_spinBoxYBottomClipping->setEnabled(false);
            ui->m_spinBoxZFrontClipping->setEnabled(false);

            if(getController()->isRadiological())
            {
                ui->m_sliderClipXRight->setEnabled(false);
                ui->m_spinBoxXRightClipping->setEnabled(false);

            } else {

                ui->m_spinBoxXLeftClipping->setEnabled(false);
                ui->m_sliderClipXLeft->setEnabled(false);
            }
        }
    }
}

void QTool2D3DSlicing::updateIntervals()
{
    if(getController() == nullptr)
        return;

    int left_right_min, left_right_max, bottom_top_min, bottom_top_max, back_front_min, back_front_max;
    getController()->getClippingIntervals(left_right_min, left_right_max, bottom_top_min, bottom_top_max, back_front_min, back_front_max);
    int x = left_right_max, y = bottom_top_max, z = back_front_max;

    ui->m_sliderClipXLeft->blockSignals(true);
    ui->m_sliderClipXLeft->setMaximum(x);
    ui->m_sliderClipXLeft->setMinimum(1);
    ui->m_sliderClipXLeft->setPageStep(x/10);
    ui->m_sliderClipXLeft->blockSignals(false);

    ui->m_sliderClipXRight->blockSignals(true);
    ui->m_sliderClipXRight->setMaximum(x);
    ui->m_sliderClipXRight->setMinimum(1);
    ui->m_sliderClipXRight->setPageStep(x/10);
    ui->m_sliderClipXRight->blockSignals(false);

    ui->m_sliderClipYBottom->blockSignals(true);
    ui->m_sliderClipYBottom->setMaximum(y);
    ui->m_sliderClipYBottom->setMinimum(1);
    ui->m_sliderClipYBottom->setPageStep(y/10);
    ui->m_sliderClipYBottom->blockSignals(false);

    ui->m_sliderClipYTop->blockSignals(true);
    ui->m_sliderClipYTop->setMaximum(y);
    ui->m_sliderClipYTop->setMinimum(1);
    ui->m_sliderClipYTop->setPageStep(y/10);
    ui->m_sliderClipYTop->blockSignals(false);

    ui->m_sliderClipZBack->blockSignals(true);
    ui->m_sliderClipZBack->setMaximum(z);
    ui->m_sliderClipZBack->setMinimum(1);
    ui->m_sliderClipZBack->setPageStep(z/10);
    ui->m_sliderClipZBack->blockSignals(false);

    ui->m_sliderClipZFront->blockSignals(true);
    ui->m_sliderClipZFront->setMaximum(z);
    ui->m_sliderClipZFront->setMinimum(1);
    ui->m_sliderClipZFront->setPageStep(z/10);
    ui->m_sliderClipZFront->blockSignals(false);



    ui->m_spinBoxXLeftClipping->blockSignals(true);
    ui->m_spinBoxXLeftClipping->setMaximum(x);
    ui->m_spinBoxXLeftClipping->setMinimum(1);
    ui->m_spinBoxXLeftClipping->blockSignals(false);

    ui->m_spinBoxXRightClipping->blockSignals(true);
    ui->m_spinBoxXRightClipping->setMaximum(x);
    ui->m_spinBoxXRightClipping->setMinimum(1);
    ui->m_spinBoxXRightClipping->blockSignals(false);

    ui->m_spinBoxYBottomClipping->blockSignals(true);
    ui->m_spinBoxYBottomClipping->setMaximum(y);
    ui->m_spinBoxYBottomClipping->setMinimum(1);
    ui->m_spinBoxYBottomClipping->blockSignals(false);

    ui->m_spinBoxYTopClipping->blockSignals(true);
    ui->m_spinBoxYTopClipping->setMaximum(y);
    ui->m_spinBoxYTopClipping->setMinimum(1);
    ui->m_spinBoxYTopClipping->blockSignals(false);

    ui->m_spinBoxZBackClipping->blockSignals(true);
    ui->m_spinBoxZBackClipping->setMaximum(z);
    ui->m_spinBoxZBackClipping->setMinimum(1);
    ui->m_spinBoxZBackClipping->blockSignals(false);

    ui->m_spinBoxZFrontClipping->blockSignals(true);
    ui->m_spinBoxZFrontClipping->setMaximum(z);
    ui->m_spinBoxZFrontClipping->setMinimum(1);
    ui->m_spinBoxZFrontClipping->blockSignals(false);
}

void QTool2D3DSlicing::updateClipping()
{
    if(getController() == nullptr)
        return;

    int clipLeft, clipRight, clipBottom, clipTop, clipBack, clipFront;
    getController()->getClippingData(clipLeft, clipRight, clipBottom, clipTop, clipBack, clipFront);

    ui->m_sliderClipXLeft->blockSignals(true);
    ui->m_sliderClipXLeft->setValue(clipLeft);
    ui->m_sliderClipXLeft->blockSignals(false);

    ui->m_sliderClipXRight->blockSignals(true);
    ui->m_sliderClipXRight->setValue(clipRight);
    ui->m_sliderClipXRight->blockSignals(false);

    ui->m_sliderClipYBottom->blockSignals(true);
    ui->m_sliderClipYBottom->setValue(clipBottom);
    ui->m_sliderClipYBottom->blockSignals(false);

    ui->m_sliderClipYTop->blockSignals(true);
    ui->m_sliderClipYTop->setValue(clipTop);
    ui->m_sliderClipYTop->blockSignals(false);

    ui->m_sliderClipZBack->blockSignals(true);
    ui->m_sliderClipZBack->setValue(clipBack);
    ui->m_sliderClipZBack->blockSignals(false);

    ui->m_sliderClipZFront->blockSignals(true);
    ui->m_sliderClipZFront->setValue(clipFront);
    ui->m_sliderClipZFront->blockSignals(false);



    ui->m_spinBoxXLeftClipping->blockSignals(true);
    ui->m_spinBoxXLeftClipping->setValue(clipLeft);
    ui->m_spinBoxXLeftClipping->blockSignals(false);

    ui->m_spinBoxXRightClipping->blockSignals(true);
    ui->m_spinBoxXRightClipping->setValue(clipRight);
    ui->m_spinBoxXRightClipping->blockSignals(false);

    ui->m_spinBoxYBottomClipping->blockSignals(true);
    ui->m_spinBoxYBottomClipping->setValue(clipBottom);
    ui->m_spinBoxYBottomClipping->blockSignals(false);

    ui->m_spinBoxYTopClipping->blockSignals(true);
    ui->m_spinBoxYTopClipping->setValue(clipTop);
    ui->m_spinBoxYTopClipping->blockSignals(false);

    ui->m_spinBoxZBackClipping->blockSignals(true);
    ui->m_spinBoxZBackClipping->setValue(clipBack);
    ui->m_spinBoxZBackClipping->blockSignals(false);

    ui->m_spinBoxZFrontClipping->blockSignals(true);
    ui->m_spinBoxZFrontClipping->setValue(clipFront);
    ui->m_spinBoxZFrontClipping->blockSignals(false);
}

void QTool2D3DSlicing::on_m_pushButtonResetEdge_clicked()
{
    getController()->resetToEdge();
}

void QTool2D3DSlicing::on_m_pushButtonResetMiddle_clicked()
{
    getController()->resetToMiddle();
}

void QTool2D3DSlicing::on_m_sliderClipXRight_valueChanged(int value)
{
    getController()->setRightClipping(value);
    ui->m_spinBoxXRightClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_sliderClipXLeft_valueChanged(int value)
{
    getController()->setLeftClipping(value);
    ui->m_spinBoxXLeftClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_sliderClipYBottom_valueChanged(int value)
{
    getController()->setBottomClipping(value);
    ui->m_spinBoxYBottomClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_sliderClipYTop_valueChanged(int value)
{
    getController()->setTopClipping(value);
    ui->m_spinBoxYTopClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_sliderClipZBack_valueChanged(int value)
{
    getController()->setBackClipping(value);
    ui->m_spinBoxZBackClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_sliderClipZFront_valueChanged(int value)
{
    getController()->setFrontClipping(value);
    ui->m_spinBoxZFrontClipping->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxXRightClipping_valueChanged(int value)
{
    ui->m_sliderClipXRight->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxXLeftClipping_valueChanged(int value)
{
    ui->m_sliderClipXLeft->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxYBottomClipping_valueChanged(int value)
{
    ui->m_sliderClipYBottom->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxYTopClipping_valueChanged(int value)
{
    ui->m_sliderClipYTop->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxZBackClipping_valueChanged(int value)
{
    ui->m_sliderClipZBack->setValue(value);
}

void QTool2D3DSlicing::on_m_spinBoxZFrontClipping_valueChanged(int value)
{
    ui->m_sliderClipZFront->setValue(value);
}

void QTool2D3DSlicing::on_m_comboBox3DMode_currentIndexChanged(int index)
{
    std::cout << "combo " << index << std::endl;

    if(getController())
    {
        switch(index)
        {
        case 0:
            getController()->set3DClippingActivated(false);
            getController()->set3DSliceActivated(false);
            getController()->set2DSliceActivated(false);
            break;

        case 1:
            getController()->set3DClippingActivated(false);
            getController()->set3DSliceActivated(true);
            break;

        case 2:
            getController()->set3DClippingActivated(false);
            getController()->set3DSliceActivated(false);
            break;

        case 3:
        {
            getController()->set3DClippingActivated(true);
            getController()->set3DSliceActivated(false);
            break;
        }

        default:
            std::cerr << "QTool2D3DSlicing::on_m_comboBox3DMode_currentIndexChanged --> index not handle." << std::endl;
        }

    }
}
