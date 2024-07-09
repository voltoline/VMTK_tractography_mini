#include "QDialogButton.h"
#include "ui_QDialogButton.h"

#include <QDialog>

QDialogButton::QDialogButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDialogButton)
{
    ui->setupUi(this);
    m_dialog = new QDialog(this);

    m_dialogLayout = new QHBoxLayout(m_dialog);
    m_dialogLayout->setContentsMargins(0, 0, 0, 0);

    m_dialog->setLayout(m_dialogLayout);
}

QDialogButton::~QDialogButton()
{
    delete ui;
}

void QDialogButton::setButtonText(std::string name)
{
    ui->m_pushButton->setText(name.c_str());
}

void QDialogButton::setCustomWidget(QWidget *widget)
{
    if(widget != nullptr)
        m_dialogLayout->addWidget(widget);
}

void QDialogButton::on_m_pushButton_clicked()
{
    m_dialog->show();
}
