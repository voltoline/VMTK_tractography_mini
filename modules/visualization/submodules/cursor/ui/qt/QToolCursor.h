#ifndef QTOOLCURSOR_H
#define QTOOLCURSOR_H

#include <QWidget>

#include "../ToolCursor.h"
#include <QDebug>

namespace Ui {
class QToolCursor;
}

class QToolCursor : public QWidget, public ToolCursor
{
    Q_OBJECT

public:
    explicit QToolCursor(QWidget *parent = nullptr);
    ~QToolCursor();

    void updateComponentsEnabled();
    void updateCursor();

    void updateDrawType();
private slots:
    /**
     * @brief
     *
     * @param state
     */
    void on_m_groupBoxCursor_toggled(bool state);

    void on_m_comboBoxSelectDraw_currentIndexChanged(int index);

private:
    Ui::QToolCursor *ui;
};

#endif // QTOOLCURSOR_H
