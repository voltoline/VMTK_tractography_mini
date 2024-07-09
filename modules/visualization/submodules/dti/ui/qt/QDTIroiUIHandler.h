#ifndef QDTIROIUIHANDLER_H
#define QDTIROIUIHANDLER_H

#include <QWidget>

#include <modules/visualization/submodules/roi/ModelROI.h>

class ControllerROIToolTab;
class ControllerDTITractographyToolTab;
class QTableWidgetItem;
class QCheckBox;
class QItemSelection;

namespace Ui {
class QTabDTITractography;
}

class QDTIroiUIHandler: public QObject
{
    Q_OBJECT

public:
    QDTIroiUIHandler(QWidget *parent, Ui::QTabDTITractography *_ui);
    ~QDTIroiUIHandler() { }

    void updateController(ControllerROIToolTab *controller, ControllerDTITractographyToolTab *tractController);
    void updateComponents();

    void loadROI(QWidget *parent);
    void saveROI(QWidget *parent);
    void toggleActivationROI(bool checked);
    void removeROI();
    void addROI(ModelROI::roi_type type, std::string type_roi);

private slots:

    void handleTableCheckBoxToggled(bool checked);
    void handleTableItemChanged(QTableWidgetItem *item);
    void handleTableItemSelectionChanged(const QItemSelection &current_selection, const QItemSelection &previous_selection);
    void handleROITableRowPressed(const QModelIndex&);

    void on_pushButtonAddSeedROI_clicked();
    void on_pushButtonAddAndROI_clicked();
    void on_pushButtonAddNotROI_clicked();

    void on_pushLoadROI_clicked();
    void on_pushSaveROI_clicked();
    void on_pushRemoveROI_clicked();
    void on_checkBoxROI_toggled(bool checked);
    void on_m_radioButtonROITract_toggled(bool checked);

private:

    void updateROITable();

    QWidget *m_parent;

    QList<QCheckBox*> m_tableCheckBoxList;

    std::vector<int> list_id;

    Ui::QTabDTITractography *ui;
    ControllerROIToolTab *mPtrController;
    ControllerDTITractographyToolTab *mTractPtrController;
};

#endif // QDTIROIUIHANDLER_H
