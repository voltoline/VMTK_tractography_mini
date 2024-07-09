#ifndef QDTI_TRACTS_UI_HANDLER_H
#define QDTI_TRACTS_UI_HANDLER_H

#include "ui_QTabDTITractography.h"

#include <QCheckBox>

class ControllerDTITractographyToolTab;

class QDTITractsUIHandler: public QObject
{
    Q_OBJECT

public:
    QDTITractsUIHandler(QWidget *parent, Ui::QTabDTITractography *_ui);
    ~QDTITractsUIHandler() { }

    void updateController(ControllerDTITractographyToolTab *controller);
    void updateComponents();
    void addTract();
    void removeTract();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private slots:
    void on_m_spinBoxMinFiberLength_valueChanged(int value);
    void on_m_spinBoxMaxFiberLength_valueChanged(int value);
    void on_m_spinBoxStoppingCriterionAngle_valueChanged(int value);
    void on_m_doubleSpinBoxStoppingCriterionFA_valueChanged(double value);
    void on_m_doubleSpinBoxSeedCreationFA_valueChanged(double value);
    void on_m_doubleSpinBoxWpunct_valueChanged(double value);

    void handleTableEditionRadioButtonToggled(bool checked);
    void handleTableShowCheckBoxToggled(bool checked);
    void handleTableTractNameItemChanged(QTableWidgetItem *item);

    void on_m_radioButtonROITract_toggled(bool checked);

    void on_checkBoxAutomaticROIFiberTracking_toggled(bool checked);

    void on_m_pushButtonRoiTracts_clicked();

public slots:
    void on_m_spinBoxROIPlaneAngle_valueChanged(int value);

    void on_m_groupBoxROIPlaneFilter_toggled(bool value);

private:
    void updateSpinBoxesInfo();
    void updateTableInfo();
    void verifySelectedEditableRow();


    QWidget *m_parent;

    QButtonGroup *m_radioButtonGroupTractMethod;

    QButtonGroup *m_radioButtonGroupTable;
    QList<QCheckBox*> m_tableCheckBoxList;
    QList<QRadioButton*> m_tableRadioButtonList;

    Ui::QTabDTITractography *ui;
    ControllerDTITractographyToolTab *mPtrController;
};

#endif // QDTI_TRACTS_UI_HANDLER_H
