#include "QToolCursor.h"
#include "ui_QToolCursor.h"

QToolCursor::QToolCursor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToolCursor)
{
    ui->setupUi(this);
}

QToolCursor::~QToolCursor()
{
    delete ui;
}

void QToolCursor::updateComponentsEnabled()
{
    bool state;

    if(getController() == nullptr)
        state = false;
    else
        state = getController()->isCursorEnabled();

    /** Cursor Enable **/
    ui->m_groupBoxCursor->setEnabled(state);
}

void QToolCursor::updateCursor()
{
    if(getController() == nullptr)
        return;

    bool state = getController()->isCursorActivated();

    ui->m_groupBoxCursor->blockSignals(true);
    ui->m_groupBoxCursor->setChecked(state);
    ui->m_groupBoxCursor->blockSignals(false);

    state = getController()->isCursorLocked();

    ui->m_comboBoxSelectDraw->blockSignals(true);
    ui->m_comboBoxSelectDraw->clear();
    for(DrawCursorType enuId:getController()->getMapDrawCursorType().keys()){
        ui->m_comboBoxSelectDraw->addItem(getController()->getMapDrawCursorType().value(enuId), QVariant(enuId) );
    }
    ui->m_comboBoxSelectDraw->blockSignals(false);
}

void QToolCursor::on_m_groupBoxCursor_toggled(bool state)
{
    getController()->setCursorActivated(state);
}


void QToolCursor::on_m_comboBoxSelectDraw_currentIndexChanged(int index)
{
    DrawCursorType drawTypeSelected = static_cast<DrawCursorType>(ui->m_comboBoxSelectDraw->itemData(index).toInt());
    getController()->setDrawCursorType(drawTypeSelected);
}

void QToolCursor::updateDrawType(){
    if(getController() == nullptr)
        return;
    int index = ui->m_comboBoxSelectDraw->findData( getController()->getDrawCursorType() );
    ui->m_comboBoxSelectDraw->setCurrentIndex(index);
}
