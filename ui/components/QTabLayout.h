#ifndef QTABLAYOUT_H
#define QTABLAYOUT_H

#include <QWidget>
#include "ui/ui_QTabLayout.h"

namespace Ui {
class QTabLayout;
}

class QTabLayout : public QWidget
{
    Q_OBJECT

public:
    explicit QTabLayout(QWidget *parent = 0);
    ~QTabLayout();

private:
    Ui::QTabLayout *ui;
};

#endif // QTABLAYOUT_H
