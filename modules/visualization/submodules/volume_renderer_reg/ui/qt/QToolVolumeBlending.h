#ifndef QTOOLVOLUMEBLENDING_H
#define QTOOLVOLUMEBLENDING_H

#include <QWidget>
#include "../ToolVolumeBlending.h"

namespace Ui {
class QToolVolumeBlending;
}

class QToolVolumeBlending : public QWidget, public ToolVolumeBlending
{
    Q_OBJECT

public:
    explicit QToolVolumeBlending(QWidget *parent = nullptr);
    ~QToolVolumeBlending();

    void updateVolumeBlending() override;
    void updateVolumeBlendingEnabled() override;

    void enableComponents(bool state);

private slots:
    void on_m_horizontalSliderBlender_valueChanged(int value);

private:
    Ui::QToolVolumeBlending *ui;
};

#endif // QTOOLVOLUMEBLENDING_H
