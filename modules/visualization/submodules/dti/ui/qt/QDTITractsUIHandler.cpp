#include "QDTITractsUIHandler.h"

#include <QButtonGroup>
#include <QMessageBox>

#include <modules/visualization/submodules/dti/controllers/ControllerDTITractographyToolTab.h>

QDTITractsUIHandler::QDTITractsUIHandler(QWidget *parent, Ui::QTabDTITractography *_ui)
{
    m_parent = parent;

    mPtrController = nullptr;

    ui = _ui;

    ui->m_tableWidgetTracts->setColumnCount(9);
    ui->m_tableWidgetTracts->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->m_pushButtonRemoveTract->setEnabled(false);
    ui->m_pushButtonRemoveTract->setAutoDefault(false);
    ui->m_pushButtonRemoveTract->setDefault(false);

    ui->m_pushButtonAddTract->setAutoDefault(false);
    ui->m_pushButtonGlobalTracts->setAutoDefault(false);
    ui->m_pushButtonRoiTracts->setAutoDefault(false);

    ui->m_pushButtonAddTract->setDefault(false);
    ui->m_pushButtonGlobalTracts->setDefault(false);
    ui->m_pushButtonRoiTracts->setDefault(false);

    ui->m_groupBoxGlobalTractography->setDisabled(true);

    m_radioButtonGroupTable = new QButtonGroup;

    m_radioButtonGroupTractMethod = new QButtonGroup;
    m_radioButtonGroupTractMethod->addButton(ui->m_radioButtonGlobalTract);
    m_radioButtonGroupTractMethod->addButton(ui->m_radioButtonROITract);

    QStringList horizontal_header_tracts = {"Edit", "Show", "Tract Name",
                                            "Fiber Length\nMin", "Fiber Length\nMax",
                                            "Stopping Criteria\nAngle >", "Stopping Criteria\nFA <",
                                            "Seed Creation\nFA >", "Wpunct"};

    ui->m_tableWidgetTracts->setHorizontalHeaderLabels(horizontal_header_tracts);
    ui->m_tableWidgetTracts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    connect(ui->m_spinBoxMinFiberLength, SIGNAL(valueChanged(int)), this, SLOT(on_m_spinBoxMinFiberLength_valueChanged(int)));
    connect(ui->m_spinBoxMaxFiberLength, SIGNAL(valueChanged(int)), this, SLOT(on_m_spinBoxMaxFiberLength_valueChanged(int)));
    connect(ui->m_spinBoxStoppingCriterionAngle, SIGNAL(valueChanged(int)), this, SLOT(on_m_spinBoxStoppingCriterionAngle_valueChanged(int)));
    connect(ui->m_doubleSpinBoxStoppingCriterionFA, SIGNAL(valueChanged(double)), this, SLOT(on_m_doubleSpinBoxStoppingCriterionFA_valueChanged(double)));
    connect(ui->m_doubleSpinBoxSeedCreationFA, SIGNAL(valueChanged(double)), this, SLOT(on_m_doubleSpinBoxSeedCreationFA_valueChanged(double)));
    connect(ui->m_doubleSpinBoxWpunct, SIGNAL(valueChanged(double)), this, SLOT(on_m_doubleSpinBoxWpunct_valueChanged(double)));

    connect(ui->m_tableWidgetTracts, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(handleTableTractNameItemChanged(QTableWidgetItem *)));

    connect(ui->m_radioButtonROITract, SIGNAL(toggled(bool)), this, SLOT(on_m_radioButtonROITract_toggled(bool)));

    connect(ui->checkBoxAutomaticRender, SIGNAL(toggled(bool)), this, SLOT(on_checkBoxAutomaticROIFiberTracking_toggled(bool)));

    connect(ui->m_pushButtonRoiTracts, SIGNAL(clicked()), this, SLOT(on_m_pushButtonRoiTracts_clicked()));
}

void QDTITractsUIHandler::updateController(ControllerDTITractographyToolTab *controller)
{
    mPtrController = controller;
}

void QDTITractsUIHandler::updateComponents()
{
    if(mPtrController != nullptr && mPtrController->isTractographyEnabled())
    {
        updateSpinBoxesInfo();
        updateTableInfo();

        if(mPtrController->isAutomaticROIFiberTrackingEnabled())
        {
            ui->m_pushButtonRoiTracts->setEnabled(false);
            ui->checkBoxAutomaticRender->setChecked(true);

        } else {

            if(ui->m_radioButtonROITract->isChecked())
            {
                ui->m_pushButtonRoiTracts->setEnabled(true);

                if(!mPtrController->isFiberBundleUpdated(mPtrController->getCurrentFiberBundleEditinIndex()))
                {
                    ui->m_pushButtonRoiTracts->setText("Update");
                    ui->m_pushButtonRoiTracts->setStyleSheet("background-color: red");

                } else {

                    ui->m_pushButtonRoiTracts->setText("Update");
                    ui->m_pushButtonRoiTracts->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #565656, stop: 0.1 #525252, stop: 0.5 #4e4e4e, stop: 0.9 #4a4a4a, stop: 1 #464646);");
                }
            }
        }
    }
}

void QDTITractsUIHandler::on_m_pushButtonRoiTracts_clicked()
{
    std::cout << "Generate Tracts based on ROI's" << std::endl;

    mPtrController->computeTractographyFromROIs();
}

void QDTITractsUIHandler::on_checkBoxAutomaticROIFiberTracking_toggled(bool checked)
{
    mPtrController->setAutomaticROIFiberTrackingEnabled(checked);
    ui->m_pushButtonRoiTracts->setEnabled(!checked);
}

void QDTITractsUIHandler::addTract()
{
    bool is_visible = false;
    std::string tract_default_name = std::string("new tract ") + std::to_string(ui->m_tableWidgetTracts->rowCount());

    ui->m_tableWidgetTracts->blockSignals(true);

    ui->m_tableWidgetTracts->insertRow(0);

    QWidget *radio_button_widget = new QWidget();
    QRadioButton *radio_button = new QRadioButton();
    m_radioButtonGroupTable->addButton(radio_button);
    m_tableRadioButtonList.push_front(radio_button);
    radio_button->setChecked(true);

    QHBoxLayout *layout_radio_button = new QHBoxLayout(radio_button_widget);
    layout_radio_button->addWidget(radio_button);
    layout_radio_button->setAlignment(Qt::AlignCenter);
    layout_radio_button->setContentsMargins(0,0,0,0);
    ui->m_tableWidgetTracts->setCellWidget(0, 0, radio_button_widget);

    QWidget *check_box_widget = new QWidget();
    QCheckBox *check_box = new QCheckBox();
    m_tableCheckBoxList.push_front(check_box);
    check_box->setChecked(is_visible);

    QHBoxLayout *layout_check_box = new QHBoxLayout(check_box_widget);
    layout_check_box->addWidget(check_box);
    layout_check_box->setAlignment(Qt::AlignCenter);
    layout_check_box->setContentsMargins(0,0,0,0);
    ui->m_tableWidgetTracts->setCellWidget(0, 1, check_box_widget);

    QTableWidgetItem *new_cell_tract_name = new QTableWidgetItem();
    new_cell_tract_name->setText(tract_default_name.c_str());
    ui->m_tableWidgetTracts->setItem(0, 2, new_cell_tract_name);

    ui->m_tableWidgetTracts->blockSignals(false);

    connect(radio_button, SIGNAL(toggled(bool)), this, SLOT(handleTableEditionRadioButtonToggled(bool)));
    connect(check_box, SIGNAL(toggled(bool)), this, SLOT(handleTableShowCheckBoxToggled(bool)));

    mPtrController->addFiberBundle(is_visible, tract_default_name, true);

    verifySelectedEditableRow();
}

void QDTITractsUIHandler::removeTract()
{
    QModelIndexList selected_rows = ui->m_tableWidgetTracts->selectionModel()->selectedRows();

    if(selected_rows.size() > 0)
    {
        int row_index = selected_rows[0].row();

        m_radioButtonGroupTable->removeButton(m_tableRadioButtonList[row_index]);

        m_tableRadioButtonList.removeAt(row_index);
        m_tableCheckBoxList.removeAt(row_index);

        ui->m_tableWidgetTracts->removeRow(row_index);

        mPtrController->deleteFiberBundle(row_index);
    }

    verifySelectedEditableRow();
}

void QDTITractsUIHandler::selectionChanged(const QItemSelection &selected, const QItemSelection &/*deselected*/)
{
    if(selected.indexes().size() > 0)
        ui->m_pushButtonRemoveTract->setEnabled(true);
    else
        ui->m_pushButtonRemoveTract->setEnabled(false);
}

void QDTITractsUIHandler::handleTableEditionRadioButtonToggled(bool checked)
{
    if(checked)
    {
        QRadioButton *sender = qobject_cast<QRadioButton*>(QObject::sender());

        for(unsigned short i = 0; i < m_tableRadioButtonList.size(); i++)
            if(m_tableRadioButtonList[i] == sender)
            {
                mPtrController->setCurrentFiberBundleEditinIndex(i);
                break;
            }
    }
}

void QDTITractsUIHandler::handleTableShowCheckBoxToggled(bool checked)
{
    QCheckBox *sender = qobject_cast<QCheckBox*>(QObject::sender());

    for(unsigned short i = 0; i < m_tableCheckBoxList.size(); i++)
        if(m_tableCheckBoxList[i] == sender)
        {
            if(mPtrController->isFiberBundleReconstructed(i))
            {
                mPtrController->setFiberBundleVisible(i, checked);

            } else {

                sender->blockSignals(true);
                sender->setChecked(false);
                sender->blockSignals(false);

                QMessageBox::information(nullptr, QString::fromStdString("Tractography"), QString::fromStdString("Reconstruct a fiber bundle before viewing!"));
            }
            break;
        }
}

void QDTITractsUIHandler::handleTableTractNameItemChanged(QTableWidgetItem *item)
{
    if(item->column() == 2)
        mPtrController->setFiberBundleName(item->row(), item->text().toStdString());
}

void QDTITractsUIHandler::on_m_radioButtonROITract_toggled(bool checked)
{
    if(checked)
    {
        ui->m_pushButtonGlobalTracts->setEnabled(false);

        ui->checkBoxAutomaticRender->setEnabled(true);
        if(ui->checkBoxAutomaticRender->isChecked())
            ui->m_pushButtonRoiTracts->setEnabled(false);
        else
            ui->m_pushButtonRoiTracts->setEnabled(true);

        mPtrController->enableROIBasedFiberTracking(true);

    } else {

        ui->m_pushButtonGlobalTracts->setEnabled(true);

        ui->m_pushButtonRoiTracts->setEnabled(false);
        ui->checkBoxAutomaticRender->setEnabled(false);

        mPtrController->enableROIBasedFiberTracking(false);
    }
}

void QDTITractsUIHandler::updateSpinBoxesInfo()
{
    unsigned int fiber_length_min;
    unsigned int fiber_length_max;
    unsigned short stopping_criterion_angle;
    float stopping_criterion_fa;
    float seed_creation_fa;
    float wpunct;
    int roi_angle_filter;
    bool roi_plane_filter_activated;

    mPtrController->getTrackingParameters(fiber_length_min, fiber_length_max,
                                          stopping_criterion_angle, stopping_criterion_fa,
                                          seed_creation_fa, wpunct);

    mPtrController->getROIPlaneFilterParameters(roi_plane_filter_activated, roi_angle_filter);

    ui->m_spinBoxMinFiberLength->blockSignals(true);
    ui->m_spinBoxMinFiberLength->setValue(fiber_length_min);
    ui->m_spinBoxMinFiberLength->blockSignals(false);

    ui->m_spinBoxMaxFiberLength->blockSignals(true);
    ui->m_spinBoxMaxFiberLength->setValue(fiber_length_max);
    ui->m_spinBoxMaxFiberLength->blockSignals(false);

    ui->m_spinBoxStoppingCriterionAngle->blockSignals(true);
    ui->m_spinBoxStoppingCriterionAngle->setValue(stopping_criterion_angle);
    ui->m_spinBoxStoppingCriterionAngle->blockSignals(false);

    ui->m_doubleSpinBoxStoppingCriterionFA->blockSignals(true);
    ui->m_doubleSpinBoxStoppingCriterionFA->setValue(stopping_criterion_fa);
    ui->m_doubleSpinBoxStoppingCriterionFA->blockSignals(false);

    ui->m_doubleSpinBoxSeedCreationFA->blockSignals(true);
    ui->m_doubleSpinBoxSeedCreationFA->setValue(seed_creation_fa);
    ui->m_doubleSpinBoxSeedCreationFA->blockSignals(false);

    ui->m_doubleSpinBoxWpunct->blockSignals(true);
    ui->m_doubleSpinBoxWpunct->setValue(wpunct);
    ui->m_doubleSpinBoxWpunct->blockSignals(false);

    ui->m_spinBoxROIPlaneAngle->blockSignals(true);
    ui->m_spinBoxROIPlaneAngle->setValue(roi_angle_filter);
    ui->m_spinBoxROIPlaneAngle->blockSignals(false);

    ui->m_groupBoxROIPlaneFilter->blockSignals(true);
    ui->m_groupBoxROIPlaneFilter->setChecked(roi_plane_filter_activated);
    ui->m_groupBoxROIPlaneFilter->blockSignals(false);
}

void QDTITractsUIHandler::updateTableInfo()
{
    ui->m_tableWidgetTracts->setRowCount(0);

    delete m_radioButtonGroupTable;
    m_radioButtonGroupTable = new QButtonGroup();

    m_tableCheckBoxList.clear();
    m_tableRadioButtonList.clear();

    ui->m_tableWidgetTracts->blockSignals(true);

    unsigned short num_fiber_bundle = mPtrController->getFiberBundlesCount();
    for(unsigned short i = 0; i < num_fiber_bundle; i++)
    {
        ui->m_tableWidgetTracts->insertRow(i);

        short current_editioin_index = mPtrController->getCurrentFiberBundleEditinIndex();
        bool is_visible = mPtrController->isFiberBundleVisible(i);
        std::string fiber_name = mPtrController->getFiberBundleName(i);
        unsigned int fiber_length_min;
        unsigned int fiber_length_max;
        unsigned short stopping_criterion_angle;
        float stopping_criterion_fa;
        float seed_creation_fa;
        float wpunct;

        mPtrController->getFiberBundleTrackingParameters(i, fiber_length_min, fiber_length_max,
                                                         stopping_criterion_angle, stopping_criterion_fa,
                                                         seed_creation_fa, wpunct);

        QRadioButton *radio_button = new QRadioButton();
        m_tableRadioButtonList.push_back(radio_button);
        m_radioButtonGroupTable->addButton(radio_button);
        if(current_editioin_index == i)
            radio_button->setChecked(true);
        else
            radio_button->setChecked(false);

        QWidget *radio_button_widget = new QWidget();
        QHBoxLayout *layout_radio_button = new QHBoxLayout(radio_button_widget);
        layout_radio_button->addWidget(radio_button);
        layout_radio_button->setAlignment(Qt::AlignCenter);
        layout_radio_button->setContentsMargins(0,0,0,0);
        ui->m_tableWidgetTracts->setCellWidget(i, 0, radio_button_widget);

        QCheckBox *check_box = new QCheckBox();
        check_box->setChecked(is_visible);
        m_tableCheckBoxList.push_back(check_box);

        QWidget *check_box_widget = new QWidget();
        QHBoxLayout *layout_check_box = new QHBoxLayout(check_box_widget);
        layout_check_box->addWidget(check_box);
        layout_check_box->setAlignment(Qt::AlignCenter);
        layout_check_box->setContentsMargins(0,0,0,0);
        ui->m_tableWidgetTracts->setCellWidget(i, 1, check_box_widget);

        QTableWidgetItem *new_cell_tract_name = new QTableWidgetItem();
        new_cell_tract_name->setText(fiber_name.c_str());
        ui->m_tableWidgetTracts->setItem(i, 2, new_cell_tract_name);

        QTableWidgetItem *new_cell_length_min = new QTableWidgetItem();
        new_cell_length_min->setText(QString::number(fiber_length_min));
        new_cell_length_min->setFlags(new_cell_length_min->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 3, new_cell_length_min);

        QTableWidgetItem *new_cell_length_max = new QTableWidgetItem();
        new_cell_length_max->setText(QString::number(fiber_length_max));
        new_cell_length_max->setFlags(new_cell_length_max->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 4, new_cell_length_max);

        QTableWidgetItem *new_cell_stopping_angle = new QTableWidgetItem();
        new_cell_stopping_angle->setText(QString::number(stopping_criterion_angle));
        new_cell_stopping_angle->setFlags(new_cell_stopping_angle->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 5, new_cell_stopping_angle);

        QTableWidgetItem *new_cell_stopping_fa = new QTableWidgetItem();
        new_cell_stopping_fa->setText(QString::number(stopping_criterion_fa));
        new_cell_stopping_fa->setFlags(new_cell_stopping_fa->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 6, new_cell_stopping_fa);

        QTableWidgetItem *new_cell_seed_fa = new QTableWidgetItem();
        new_cell_seed_fa->setText(QString::number(seed_creation_fa));
        new_cell_seed_fa->setFlags(new_cell_seed_fa->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 7, new_cell_seed_fa);

        QTableWidgetItem *new_cell_wpunct = new QTableWidgetItem();
        new_cell_wpunct->setText(QString::number(wpunct));
        new_cell_wpunct->setFlags(new_cell_wpunct->flags() & ~Qt::ItemIsEditable);
        ui->m_tableWidgetTracts->setItem(i, 8, new_cell_wpunct);
    }

    ui->m_tableWidgetTracts->blockSignals(false);

    for(QRadioButton *radio_button: m_tableRadioButtonList)
        connect(radio_button, SIGNAL(toggled(bool)), this, SLOT(handleTableEditionRadioButtonToggled(bool)));

    for(QCheckBox *check_box: m_tableCheckBoxList)
        connect(check_box, SIGNAL(toggled(bool)), this, SLOT(handleTableShowCheckBoxToggled(bool)));

    verifySelectedEditableRow();
}

void QDTITractsUIHandler::verifySelectedEditableRow()
{
    QAbstractButton *checked_button = m_radioButtonGroupTable->checkedButton();
    if(checked_button)
    {
//        ui->m_groupBoxGlobalTractography->setEnabled(true);  # DESCOMENTAR QUANDO QUISER ATIVAR DE NOVO

        ui->m_groupBoxROITractography->setEnabled(true);

        if(ui->m_radioButtonGlobalTract->isChecked())
        {
            ui->m_pushButtonGlobalTracts->setEnabled(true);

            ui->m_pushButtonRoiTracts->setEnabled(false);
            ui->checkBoxAutomaticRender->setEnabled(false);

        } else {

            ui->m_pushButtonGlobalTracts->setEnabled(false);

            ui->checkBoxAutomaticRender->setEnabled(true);
            if(ui->checkBoxAutomaticRender->isChecked())
                ui->m_pushButtonRoiTracts->setEnabled(false);
            else
                ui->m_pushButtonRoiTracts->setEnabled(true);
        }

        if(ui->checkBoxROI->isChecked())
        {
            ui->m_groupBoxAddROI->setEnabled(true);
            ui->pushRemoveROI->setEnabled(false);
        }

    } else {

        ui->m_groupBoxAddROI->setEnabled(false);
        ui->pushRemoveROI->setEnabled(false);

        ui->m_groupBoxGlobalTractography->setEnabled(false);
        ui->m_groupBoxROITractography->setEnabled(false);
    }
}

void QDTITractsUIHandler::on_m_spinBoxMinFiberLength_valueChanged(int value)
{
    mPtrController->setFiberLengthMin(value);
}

void QDTITractsUIHandler::on_m_spinBoxMaxFiberLength_valueChanged(int value)
{
    mPtrController->setFiberLengthMax(value);
}

void QDTITractsUIHandler::on_m_spinBoxStoppingCriterionAngle_valueChanged(int value)
{
    mPtrController->setStoppingCriteriaAngle(value);
}

void QDTITractsUIHandler::on_m_doubleSpinBoxStoppingCriterionFA_valueChanged(double value)
{
    mPtrController->setStoppingCriteriaFA(value);
}

void QDTITractsUIHandler::on_m_doubleSpinBoxSeedCreationFA_valueChanged(double value)
{
    mPtrController->setSeedCreationParameters(value);
}

void QDTITractsUIHandler::on_m_doubleSpinBoxWpunct_valueChanged(double value)
{
    mPtrController->setWpunctParameter(value);
}

void QDTITractsUIHandler::on_m_spinBoxROIPlaneAngle_valueChanged(int value)
{
    mPtrController->setROIPlaneAngle(value);
}

void QDTITractsUIHandler::on_m_groupBoxROIPlaneFilter_toggled(bool value)
{
    mPtrController->setActivatedROIPlaneFilter(value);
}
