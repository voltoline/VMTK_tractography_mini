#include "QTabDTITractography.h"

#include "ui_QTabDTITractography.h"
#include "QDTITractsUIHandler.h"
#include "QDTIroiUIHandler.h"
#include <QMessageBox>
#include <qevent.h>

#include <modules/visualization/submodules/dti/controllers/ControllerDTITractographyToolTab.h>

QTabDTITractography::QTabDTITractography(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTabDTITractography),
    mPtrController(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("Tract Creation");

    m_DTITractsUIHandler = new QDTITractsUIHandler(this, ui);
    m_DTIroiUIHandler = new QDTIroiUIHandler(this, ui);

    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);

    // to disable enter key
    ui->m_spinBoxMinFiberLength->installEventFilter(this);
    ui->m_spinBoxMaxFiberLength->installEventFilter(this);
    ui->m_spinBoxStoppingCriterionAngle->installEventFilter(this);
    ui->m_doubleSpinBoxStoppingCriterionFA->installEventFilter(this);
    ui->m_doubleSpinBoxSeedCreationFA->installEventFilter(this);
    ui->m_doubleSpinBoxWpunct->installEventFilter(this);
    ui->m_spinBoxROIPlaneAngle->installEventFilter(this);


    connect(ui->m_tableWidgetTracts->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this, SLOT(selectionChanged(const QItemSelection&, const QItemSelection&)));
}

QTabDTITractography::~QTabDTITractography()
{
    delete ui;
}

void QTabDTITractography::updateController(ControllerDTITractographyToolTab *controller)
{
    mPtrController = controller;
    ToolTractography::updateController(controller);
    m_DTITractsUIHandler->updateController(controller);
}

void QTabDTITractography::updateController(ControllerROIToolTab *controller)
{
    ToolTractography::updateController(controller);
    m_DTIroiUIHandler->updateController(controller, mPtrController);
}

void QTabDTITractography::updateTractography()
{
    m_DTITractsUIHandler->updateComponents();
    m_DTIroiUIHandler->updateComponents();
}

void QTabDTITractography::updateTractographyEnabled()
{
    bool state;

    if(mPtrController == nullptr)
        state = false;
    else
        state = mPtrController->isTractographyEnabled();

    this->setEnabled(state);
}

void QTabDTITractography::on_m_pushButtonGlobalTracts_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Global Tractography", "This task is memory intensive and might lock your interface. Do you want to continue?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        mPtrController->computeGlobalTractography();
    }
}

void QTabDTITractography::on_m_pushButtonAddTract_clicked()
{
    m_DTITractsUIHandler->addTract();
}

void QTabDTITractography::on_m_pushButtonRemoveTract_clicked()
{
    m_DTITractsUIHandler->removeTract();
}

void QTabDTITractography::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    m_DTITractsUIHandler->selectionChanged(selected, deselected);
}

void QTabDTITractography::on_m_groupBoxROIPlaneFilter_toggled(bool arg1)
{
    m_DTITractsUIHandler->on_m_groupBoxROIPlaneFilter_toggled(arg1);
}

void QTabDTITractography::on_m_spinBoxROIPlaneAngle_valueChanged(int arg1)
{
    m_DTITractsUIHandler->on_m_spinBoxROIPlaneAngle_valueChanged(arg1);
}

bool QTabDTITractography::eventFilter(QObject */*object*/, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            std::cout << "eventFilter -->> Enter not work here! " << std::endl;
            return true;
        } else
            return false;
    }
    return false;
}
