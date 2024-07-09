#ifndef QCHECKEDTABWIDGET_H
#define QCHECKEDTABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTabBar>
#include <iostream>

class QCheckedTabWidget : public  QTabWidget
{
    Q_OBJECT

public:
    explicit QCheckedTabWidget(QWidget *parent = 0);
    ~QCheckedTabWidget();
      void addTab(QWidget *widget, const QString &label, bool checkBox);
      std::vector<unsigned short> listIDTabsWithCheck();
      int countTabsWithCheck();
      void uncheckAllTab();

signals:
      void tabCheckBoxToggled(unsigned short tab_index, bool checked);

private slots:
      void handleTabCheckBoxToggled(bool checked);

private:
     QList<QCheckBox*> checklist;
     QList<bool> statusCheckList;

     std::vector<unsigned short> m_listOrderedCheckedTab;
};

#endif // CHECKEDTABWIDGET_H
