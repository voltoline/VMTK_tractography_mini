#include "QDTIroiUIHandler.h"

#include "ui_QTabDTITractography.h"

#include <modules/visualization/submodules/roi/controllers/ControllerROIToolTab.h>
#include <modules/visualization/submodules/dti/controllers/ControllerDTITractographyToolTab.h>

#include <QFileDialog>
#include <QStringListModel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QtWidgets>
#include <QCheckBox>

QDTIroiUIHandler::QDTIroiUIHandler(QWidget *parent, Ui::QTabDTITractography *_ui)
{
    m_parent = parent;

    mPtrController = nullptr;
    mTractPtrController = nullptr;

    ui = _ui;

    ui->m_tableWidgetROI->setColumnCount(3);
    ui->m_tableWidgetROI->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList horizontal_header_rois = {"Active", "ROI Name", "Type"};
    ui->m_tableWidgetROI->setHorizontalHeaderLabels(horizontal_header_rois);
    ui->m_tableWidgetROI->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->pushLoadROI->setEnabled(false );
    ui->pushRemoveROI->setEnabled(false);
    ui->pushRemoveROI->setAutoDefault(false);
    ui->pushRemoveROI->setDefault(false);
    ui->m_pushButtonRoiTracts->setEnabled(false);
    ui->pushSaveROI->setEnabled(false);

    ui->m_groupBoxAddROI->setEnabled(false);

    connect(ui->m_tableWidgetROI, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(handleTableItemChanged(QTableWidgetItem *)));
    connect(ui->m_tableWidgetROI->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(handleTableItemSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(ui->m_tableWidgetROI, SIGNAL(pressed(const QModelIndex&)), this,  SLOT(handleROITableRowPressed(const QModelIndex&)));

    connect(ui->m_pushButtonAddSeedROI, SIGNAL(clicked()), this, SLOT(on_pushButtonAddSeedROI_clicked()));
    connect(ui->m_pushButtonAddAndROI, SIGNAL(clicked()), this, SLOT(on_pushButtonAddAndROI_clicked()));
    connect(ui->m_pushButtonAddNotROI, SIGNAL(clicked()), this, SLOT(on_pushButtonAddNotROI_clicked()));

    connect(ui->pushLoadROI, SIGNAL(clicked()), this, SLOT(on_pushLoadROI_clicked()));
    connect(ui->pushSaveROI, SIGNAL(clicked()), this, SLOT(on_pushSaveROI_clicked()));
    connect(ui->pushRemoveROI, SIGNAL(clicked()), this, SLOT(on_pushRemoveROI_clicked()));
    connect(ui->checkBoxROI, SIGNAL(toggled(bool)), this, SLOT(on_checkBoxROI_toggled(bool)));
    connect(ui->m_radioButtonROITract, SIGNAL(toggled(bool)), this, SLOT(on_m_radioButtonROITract_toggled(bool)));
}

void QDTIroiUIHandler::updateController(ControllerROIToolTab *controller, ControllerDTITractographyToolTab *tractController)
{
    mPtrController = controller;
    mTractPtrController = tractController;
}

void QDTIroiUIHandler::updateComponents()
{
    //Activa/Deactivate ROI checkbox accordingly to model State
    ui->checkBoxROI->blockSignals(true);
    if(mPtrController != nullptr)
    {
        bool FreeDrawState = mPtrController->getFreeDrawControllerState();
        ui->checkBoxROI->setChecked(FreeDrawState);
        if(FreeDrawState && (mTractPtrController != nullptr) && (mTractPtrController->getCurrentFiberBundleEditinIndex() > -1))
        {
            ui->m_groupBoxAddROI->setEnabled(true);
            ui->pushLoadROI->setEnabled(true);
            ui->pushRemoveROI->setEnabled(false);

            if(ui->m_radioButtonROITract->isChecked())
            {
                ui->m_pushButtonAddSeedROI->setEnabled(true);
                ui->checkBoxAutomaticRender->setEnabled(true);
                ui->m_pushButtonRoiTracts->setEnabled(true);

            } else {

                ui->m_pushButtonAddSeedROI->setEnabled(false);
                ui->checkBoxAutomaticRender->setEnabled(false);
                ui->m_pushButtonRoiTracts->setEnabled(false);
            }

            ui->m_pushButtonAddNotROI->setEnabled(true);
            ui->m_pushButtonAddAndROI->setEnabled(true);

        } else {

            ui->m_groupBoxAddROI->setEnabled(false);
            ui->pushLoadROI->setEnabled(false);
            ui->pushRemoveROI->setEnabled(false);

            ui->m_pushButtonAddSeedROI->setEnabled(false);
            ui->m_pushButtonAddNotROI->setEnabled(false);
            ui->m_pushButtonAddAndROI->setEnabled(false);

            ui->checkBoxAutomaticRender->setEnabled(false);
            ui->m_pushButtonRoiTracts->setEnabled(false);
        }

        updateROITable();

        if(list_id.size() > 0)
            ui->pushSaveROI->setEnabled(true);
        else
            ui->pushSaveROI->setEnabled(false);

    }

    ui->checkBoxROI->blockSignals(false);

    if(mTractPtrController != nullptr && mTractPtrController->getCurrentFiberBundleEditinIndex() == -1)
        ui->checkBoxROI->setEnabled(false);
    else
        ui->checkBoxROI->setEnabled(true);
}

void QDTIroiUIHandler::updateROITable()
{
        //Clear Control List
        list_id.clear();

        ui->m_tableWidgetROI->blockSignals(true);

        ui->m_tableWidgetROI->setRowCount(0);

        if(mTractPtrController->getCurrentFiberBundleEditinIndex() == -1)
        {
            ui->m_tableWidgetROI->blockSignals(false);
            return;
        }

        //Get Model ROI Information
        if(mPtrController != nullptr)
        {
            std::vector<std::string>        names =           mPtrController->getROInameInformation();
            std::vector<int>                ids =             mPtrController->getROIidInformation();
            std::vector<ModelROI::roi_type> types =           mPtrController->getROITypeInformation();
            std::vector<int>                tract_ids =       mPtrController->getROItractInformation();
            std::vector<bool>               activation_info = mPtrController->getROIActivityInformation();

            //Update UI Table
            for(unsigned int i = 0; i < ids.size(); i++)
            {
                if(mTractPtrController->getFiberBundleID(mTractPtrController->getCurrentFiberBundleEditinIndex()) == tract_ids[i])
                {
                    std::string type_roi;
                    if(types[i] == ModelROI::roi_type::SEED)
                    {
                        type_roi = std::string("Seed");
                    }
                    else if (types[i] == ModelROI::roi_type::AND_FILTER)
                    {
                        type_roi = std::string("AND");
                    }
                    else
                    {
                        type_roi = std::string("NOT");
                    }
                    //Add entry on table
                    ui->m_tableWidgetROI->blockSignals(true);

                    ui->m_tableWidgetROI->insertRow(0);

                    QWidget *check_box_widget = new QWidget();
                    QCheckBox *check_box = new QCheckBox();
                    m_tableCheckBoxList.push_front(check_box);

                    check_box->setChecked(activation_info[i]);
                    //Only enable change in variable if Tract is Visible
                    check_box->setEnabled(mTractPtrController->isFiberBundleVisible(mTractPtrController->getCurrentFiberBundleEditinIndex()));

                    QHBoxLayout *layout_check_box = new QHBoxLayout(check_box_widget);
                    layout_check_box->addWidget(check_box);
                    layout_check_box->setAlignment(Qt::AlignCenter);
                    layout_check_box->setContentsMargins(0,0,0,0);
                    ui->m_tableWidgetROI->setCellWidget(0, 0, check_box_widget);

                    QTableWidgetItem *new_cell_roi_name = new QTableWidgetItem();
                    new_cell_roi_name->setText(names[i].c_str());
                    ui->m_tableWidgetROI->setItem(0, 1, new_cell_roi_name);

                    QTableWidgetItem *new_cell_roi_type = new QTableWidgetItem();
                    new_cell_roi_type->setText(type_roi.c_str());
                    ui->m_tableWidgetROI->setItem(0, 2, new_cell_roi_type);

                    ui->m_tableWidgetROI->blockSignals(false);

                    connect(check_box, SIGNAL(toggled(bool)), this, SLOT(handleTableCheckBoxToggled(bool)));

                    list_id.insert(list_id.begin(), ids[i]);

                }
            }
        }
        ui->m_tableWidgetROI->blockSignals(false);
}

void QDTIroiUIHandler::handleTableItemChanged(QTableWidgetItem *item)
{
    if(item->column() == 1)
    {
        int index_id = list_id[item->row()];
        mPtrController->setNameROI((unsigned int)index_id,item->text().toStdString());

    }
}

void QDTIroiUIHandler::handleTableItemSelectionChanged(const QItemSelection &current_selection, const QItemSelection &/*previous_selection*/)
{
    if(current_selection.indexes().size() <= 0)
    {
        ui->pushRemoveROI->setEnabled(false);
        return;
    }

    if((unsigned long)ui->m_tableWidgetROI->selectionModel()->selectedRows()[0].row() < list_id.size()) //casting (Ting:5/11/2022)
        ui->pushRemoveROI->setEnabled(true);
}

void QDTIroiUIHandler::handleROITableRowPressed(const QModelIndex &pressed)
{
    if((unsigned long)pressed.row() < list_id.size())                                       //casting (Ting: 5/11/2022)
        mPtrController->setCurrentActiveROI_id(list_id[pressed.row()]);
}

void QDTIroiUIHandler::on_pushButtonAddSeedROI_clicked()
{
    addROI(ModelROI::roi_type::SEED, std::string("Seed"));
}

void QDTIroiUIHandler::on_pushButtonAddAndROI_clicked()
{
    addROI(ModelROI::roi_type::AND_FILTER, std::string("AND"));
}

void QDTIroiUIHandler::on_pushButtonAddNotROI_clicked()
{
    addROI(ModelROI::roi_type::NOT_FILTER, std::string("NOT"));
}

void QDTIroiUIHandler::on_pushLoadROI_clicked()
{
    loadROI(m_parent);
}

void QDTIroiUIHandler::on_pushSaveROI_clicked()
{
    saveROI(m_parent);
}

void QDTIroiUIHandler::on_pushRemoveROI_clicked()
{
    removeROI();
}

void QDTIroiUIHandler::on_checkBoxROI_toggled(bool checked)
{
    toggleActivationROI(checked);
}

void QDTIroiUIHandler::on_m_radioButtonROITract_toggled(bool checked)
{
    if(ui->checkBoxROI->isChecked())
    {
        ui->m_pushButtonAddSeedROI->setEnabled(checked);
    }
}

void QDTIroiUIHandler::saveROI(QWidget *parent)
{
    if(mTractPtrController->getCurrentFiberBundleEditinIndex() == -1)
        return;

    if(mPtrController != nullptr) {

        QFileDialog *saveROIDialog = new QFileDialog(parent);
        saveROIDialog->setFileMode(QFileDialog::AnyFile);
        saveROIDialog->setAcceptMode(QFileDialog::AcceptSave);
#if __APPLE__
        saveROIDialog->setWindowModality( Qt::WindowModal ); // precisa colocar pelo que não é compativel com as janelas nativas do MAC OS
#endif
        QString filename = saveROIDialog->getSaveFileName(parent, "Save ROI(s)", "", "ROI File (*.roi)");

        if(filename != "")
        {
            int id_current_tract = mTractPtrController->getFiberBundleID(mTractPtrController->getCurrentFiberBundleEditinIndex());
            std::string name_current_tract = mTractPtrController->getFiberBundleName(mTractPtrController->getCurrentFiberBundleEditinIndex());
            bool saved = mPtrController->saveROI(id_current_tract, name_current_tract, filename.toStdString());

            if(saved == false)
            {
                QMessageBox::critical(parent, tr("Error"), tr("Failed to save file"));
                return; //Aborted
            }
        }
     }
}

void QDTIroiUIHandler::loadROI(QWidget * parent)
{
    if(mTractPtrController->getCurrentFiberBundleEditinIndex() == -1)
        return;

    //Open Results
    QFileDialog *qfd = new QFileDialog(parent);

    QString file_path = qfd->getOpenFileName(parent, tr("Open File"), QDir::currentPath(), tr("ROI Files (*.roi)"));
    if(file_path.isNull() || file_path == "")
    {
        return;
    }

    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream in(&file);
    std::vector<std::string> file_text;

    while(!in.atEnd()) {
         file_text.push_back(in.readLine().toStdString());
    }

    file.close();

    //Pass File as string to model interpret

    mPtrController->loadROI(file_text);

    updateComponents();

}

void QDTIroiUIHandler::removeROI()
{

    int id = mPtrController->getCurrentActiveROI_id();

    unsigned long index;                    //compatible type (Ting - 5/11/2022)
    for(index = 0; index < list_id.size(); index++)
    {
        if(list_id[index] == id)
        {
            list_id.erase(list_id.begin() + index);
            break;
        }
    }

    QModelIndexList selected_rows = ui->m_tableWidgetROI->selectionModel()->selectedRows();

    if(selected_rows.size() > 0)
    {
        int row_index = selected_rows[0].row();

        m_tableCheckBoxList.removeAt(row_index);

        ui->m_tableWidgetROI->removeRow(row_index);
    }

    ui->m_tableWidgetROI->selectionModel()->clearSelection();

    mPtrController->removeROI(id, false);

    ui->pushRemoveROI->setEnabled(false);
}

void QDTIroiUIHandler::addROI(ModelROI::roi_type type, std::string type_roi)
{
    if(mTractPtrController->getCurrentFiberBundleEditinIndex() == -1)
        return;

    //TODO Find a linker to Tract to add inofrmation on ROI
     short test = mTractPtrController->getCurrentFiberBundleEditinIndex();
     std::string tract_name = mTractPtrController->getFiberBundleName(test);
     short tract_id = mTractPtrController->getFiberBundleID(mTractPtrController->getCurrentFiberBundleEditinIndex());

    if (ui->checkBoxROI->isChecked())
    {
        int id = mPtrController->createNewROI(type, tract_name, tract_id);

        std::string roi_default_name = std::string("ROI ") + std::to_string(id + 1);

        mPtrController->setNameROI((unsigned int) id, roi_default_name);

        //Add ID to ROI list
        list_id.insert(list_id.begin(), id);

        ui->m_tableWidgetROI->blockSignals(true);

        ui->m_tableWidgetROI->insertRow(0);

        QWidget *check_box_widget = new QWidget();
        QCheckBox *check_box = new QCheckBox();
        m_tableCheckBoxList.push_front(check_box);
        check_box->setChecked(true);

        QHBoxLayout *layout_check_box = new QHBoxLayout(check_box_widget);
        layout_check_box->addWidget(check_box);
        layout_check_box->setAlignment(Qt::AlignCenter);
        layout_check_box->setContentsMargins(0,0,0,0);
        ui->m_tableWidgetROI->setCellWidget(0, 0, check_box_widget);

        QTableWidgetItem *new_cell_roi_name = new QTableWidgetItem();
        new_cell_roi_name->setText(roi_default_name.c_str());
        ui->m_tableWidgetROI->setItem(0, 1, new_cell_roi_name);

        QTableWidgetItem *new_cell_roi_type = new QTableWidgetItem();
        new_cell_roi_type->setText(type_roi.c_str());
        ui->m_tableWidgetROI->setItem(0, 2, new_cell_roi_type);

        ui->m_tableWidgetROI->blockSignals(false);

        connect(check_box, SIGNAL(toggled(bool)), this, SLOT(handleTableCheckBoxToggled(bool)));
    }

}

void QDTIroiUIHandler::toggleActivationROI(bool checked)
{
   if(checked == true)
   {
        ui->pushLoadROI->setEnabled(true);

        if(ui->m_tableWidgetTracts->rowCount() > 0)
        {
            ui->m_groupBoxAddROI->setEnabled(true);
            ui->pushRemoveROI->setEnabled(false);
        }

        if(ui->m_radioButtonROITract->isChecked())
        {
            ui->m_pushButtonRoiTracts->setEnabled(true);
            ui->m_pushButtonAddSeedROI->setEnabled(true);
            ui->checkBoxAutomaticRender->setEnabled(true);

        } else {
            ui->m_pushButtonRoiTracts->setEnabled(false);
            ui->m_pushButtonAddSeedROI->setEnabled(false);
            ui->checkBoxAutomaticRender->setEnabled(false);
        }

        ui->m_pushButtonAddNotROI->setEnabled(true);
        ui->m_pushButtonAddAndROI->setEnabled(true);

   } else {

       ui->pushLoadROI->setEnabled(false);
       ui->m_groupBoxAddROI->setEnabled(false);
       ui->pushRemoveROI->setEnabled(false);

       ui->checkBoxAutomaticRender->setEnabled(false);
       ui->m_pushButtonRoiTracts->setEnabled(false);

       ui->m_pushButtonAddSeedROI->setEnabled(false);
       ui->m_pushButtonAddNotROI->setEnabled(false);
       ui->m_pushButtonAddAndROI->setEnabled(false);
   }

   if(mPtrController != nullptr)
   {
        mPtrController->setFreeDrawROI(checked);
   }
}

void QDTIroiUIHandler::handleTableCheckBoxToggled(bool checked)
{
    QCheckBox *sender = qobject_cast<QCheckBox*>(QObject::sender());

    for(unsigned short i = 0; i < m_tableCheckBoxList.size(); i++)
        if(m_tableCheckBoxList[i] == sender)
        {
            //Set activation accordingly
            mPtrController->setROIactivity(list_id[i], checked);
            break;
        }

    mPtrController->setChangeROI(true);
}
