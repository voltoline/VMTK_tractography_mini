#ifndef QDIALOGBUTTON_H
#define QDIALOGBUTTON_H

#include <QWidget>

class QHBoxLayout;

namespace Ui {
class QDialogButton;
}

class QDialogButton : public QWidget
{
    Q_OBJECT

public:
    explicit QDialogButton(QWidget *parent = nullptr);
    ~QDialogButton();

    void setButtonText(std::string name);
    void setCustomWidget(QWidget *widget);

private slots:
    void on_m_pushButton_clicked();

private:
    Ui::QDialogButton *ui;

    QDialog *m_dialog;
    QHBoxLayout *m_dialogLayout;
};

#endif // QDIALOGBUTTON_H
