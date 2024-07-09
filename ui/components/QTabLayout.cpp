#include "ui/components/QTabLayout.h"

QTabLayout::QTabLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTabLayout)
{
    ui->setupUi(this);
}

QTabLayout::~QTabLayout()
{
    delete ui;
}
