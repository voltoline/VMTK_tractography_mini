#ifndef QTAB_DTI_TRACTOGRAPHY_H
#define QTAB_DTI_TRACTOGRAPHY_H

#include <QDialog>
#include <QWidget>
#include <QItemSelection>

class QDTIroiUIHandler;
class QDTITractsUIHandler;
class ControllerDTITractographyToolTab;
class ControllerROIToolTab;

#include "../ToolTractography.h"

namespace Ui {
class QTabDTITractography;
}

class QTabDTITractography: public QWidget, public ToolTractography
{
    Q_OBJECT

public:
    explicit QTabDTITractography(QWidget *parent = 0);
    ~QTabDTITractography();

    void updateController(ControllerDTITractographyToolTab *controller);
    void updateController(ControllerROIToolTab *controller);

    void updateTractography();
    void updateTractographyEnabled();

private slots:
    void on_m_pushButtonGlobalTracts_clicked();
    void on_m_pushButtonAddTract_clicked();
    void on_m_pushButtonRemoveTract_clicked();

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_m_groupBoxROIPlaneFilter_toggled(bool arg1);

    void on_m_spinBoxROIPlaneAngle_valueChanged(int arg1);

private:
    bool eventFilter(QObject *object, QEvent *event);

    Ui::QTabDTITractography *ui;
    ControllerDTITractographyToolTab *mPtrController;

    QDTITractsUIHandler *m_DTITractsUIHandler;
    QDTIroiUIHandler *m_DTIroiUIHandler;
};

#endif // QTAB_DTI_TRACTOGRAPHY_H
