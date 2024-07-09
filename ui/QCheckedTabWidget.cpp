#include "QCheckedTabWidget.h"

#include <QMessageBox>

QCheckedTabWidget::QCheckedTabWidget(QWidget *parent) : QTabWidget(parent)
{

}

QCheckedTabWidget::~QCheckedTabWidget()
{

}

void QCheckedTabWidget::addTab(QWidget *widget, const QString &label, bool checkBox)
{
    QTabWidget::addTab(widget, label);

    QCheckBox *cb = nullptr;

    if(checkBox)
    {
        QTabBar *tb = tabBar();
        cb = new QCheckBox();

        cb->setObjectName("m_check_" + widget->objectName());
        tb->setTabButton(indexOf(widget), QTabBar::LeftSide, (QWidget*)cb);

        connect(cb, SIGNAL(toggled(bool)), this, SLOT(handleTabCheckBoxToggled(bool)));
    }

    statusCheckList.insert(indexOf(widget), checkBox);
    checklist.insert(indexOf(widget), cb);
}

int QCheckedTabWidget::countTabsWithCheck()
{
    return m_listOrderedCheckedTab.size();
}

void QCheckedTabWidget::uncheckAllTab()
{
    for(QCheckBox *cb: checklist)
    {
        if(cb != nullptr)
            if(cb->isChecked())
                cb->setChecked(false);
    }
}

void QCheckedTabWidget::handleTabCheckBoxToggled(bool checked)
{
    QCheckBox *sender = qobject_cast<QCheckBox*>(QObject::sender());

    if(m_listOrderedCheckedTab.size() == 2 && checked)
    {
        QMessageBox::information(this, QString::fromStdString("Alert"), QString::fromStdString("You can select up to 2 views!"));

        sender->blockSignals(true);
        sender->setChecked(false);
        sender->blockSignals(false);

        return;
    }

    for(unsigned short i = 0; i < checklist.size(); i++)
        if(checklist[i] == sender)
        {
            if(checked)
            {
                m_listOrderedCheckedTab.push_back(i);

            } else {

                for(unsigned short j = 0; j < m_listOrderedCheckedTab.size(); j++)
                    if(m_listOrderedCheckedTab[j] == i)
                        m_listOrderedCheckedTab.erase(m_listOrderedCheckedTab.begin() + j);
            }

            emit tabCheckBoxToggled(i, checked);
            return;
        }
}

std::vector<unsigned short> QCheckedTabWidget::listIDTabsWithCheck()
{
    return m_listOrderedCheckedTab;
}
