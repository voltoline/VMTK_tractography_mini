#ifndef QTOOL_TRANSFORMATION_H
#define QTOOL_TRANSFORMATION_H

#include <QWidget>

#include "../ToolTransformation.h"

class QButtonGroup;

namespace Ui {
class QToolTransformation;
}

class QToolTransformation : public QWidget, public ToolTransformation
{
    Q_OBJECT

public:
    explicit QToolTransformation(QWidget *parent = nullptr);
    ~QToolTransformation();

    void updateTransformationEnabled() override;

    void updateMouseMode() override;

private slots:
    void m_buttonGroupMouseTools_buttonClicked(int mode);

    void on_m_pushButtonResetMouseTools_clicked();

    void on_m_checkBoxActive3D_toggled(bool checked);

    void on_m_checkBoxActive2D_toggled(bool checked);

private:
    Ui::QToolTransformation *ui;

    QButtonGroup *m_buttonGroupMouseTools;
};

#endif // QTOOLTRANSFORMATION_H
