#ifndef QTOOLSLICEORIENTATION_H
#define QTOOLSLICEORIENTATION_H

#include <QWidget>
#include "../ToolSliceOrientation.h"

class QButtonGroup;

namespace Ui {
class QToolSliceOrientation;
}

class QToolSliceOrientation : public QWidget, public ToolSliceOrientation
{
    Q_OBJECT

public:
    explicit QToolSliceOrientation(QWidget *parent = nullptr);
    ~QToolSliceOrientation();

    void updateComponentsEnabled() override;

    void updateSliceOrientation() override;

private slots:

    void on_m_comboBoxOrientation_currentIndexChanged(int index);

private:
    Ui::QToolSliceOrientation *ui;
};

#endif // QTOOLSLICEORIENTATION_H
