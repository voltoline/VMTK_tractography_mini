#ifndef QTOOL_DIFFUSION_H
#define QTOOL_DIFFUSION_H

#include <QWidget>

#include "../ToolDiffusion.h"

class QButtonGroup;

namespace Ui {
class QToolDiffusion;
}

class QToolDiffusion : public QWidget, public ToolDiffusion
{
    Q_OBJECT

public:
    explicit QToolDiffusion(QWidget *parent = nullptr);
    ~QToolDiffusion();

    void updateDiffusion();
    void updateDiffusionEnabled();

    QButtonGroup& getButtonGroup() const;

private slots:
    void on_m_radioButtonDWI_toggled(bool checked);
    void on_m_spinBoxVolumeNumber_valueChanged(int i);

private:
    Ui::QToolDiffusion *ui;

    QButtonGroup *m_buttonGroup;
};

#endif // QTOOL_DIFFUSION_H
