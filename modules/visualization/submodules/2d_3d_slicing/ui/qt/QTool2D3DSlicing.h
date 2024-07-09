#ifndef QTOOL2D3DSLICING_H
#define QTOOL2D3DSLICING_H

#include <QWidget>

#include "../Tool2D3DSlicing.h"

namespace Ui {
class QTool2D3DSlicing;
}

class QTool2D3DSlicing : public QWidget, public Tool2D3DSlicing
{
    Q_OBJECT

public:
    explicit QTool2D3DSlicing(QWidget *parent = nullptr);
    ~QTool2D3DSlicing();

    void updateComponentsEnabled() override;
    void updateIntervals() override;
    void updateClipping() override;

private slots:
    void on_m_sliderClipXRight_valueChanged(int value);
    void on_m_sliderClipXLeft_valueChanged(int value);
    void on_m_sliderClipYBottom_valueChanged(int value);
    void on_m_sliderClipYTop_valueChanged(int value);
    void on_m_sliderClipZBack_valueChanged(int value);
    void on_m_sliderClipZFront_valueChanged(int value);

    void on_m_spinBoxXLeftClipping_valueChanged(int value);
    void on_m_spinBoxXRightClipping_valueChanged(int value);
    void on_m_spinBoxYBottomClipping_valueChanged(int value);
    void on_m_spinBoxYTopClipping_valueChanged(int value);
    void on_m_spinBoxZBackClipping_valueChanged(int value);
    void on_m_spinBoxZFrontClipping_valueChanged(int value);

    /**
     * @brief
     *
     */
    void on_m_pushButtonResetEdge_clicked();

    void on_m_pushButtonResetMiddle_clicked();

    void on_m_comboBox3DMode_currentIndexChanged(int index);

private:
    Ui::QTool2D3DSlicing *ui;
};

#endif // QTOOL2D3DSLICING_H
