#ifndef UIFACTORY_QT_H
#define UIFACTORY_QT_H

#include "core/UIFactoryBase.h"

class QWidget;

class UIFactoryQt: public UIFactoryBase
{
public:
    typedef std::shared_ptr<UIFactoryQt> sPtr;
    typedef std::unique_ptr<UIFactoryQt> uPtr;

    UIFactoryQt(std::string factory_class_name);
    ~UIFactoryQt() { }

    void setParentWidget(QWidget *parent);

protected:
    QWidget* getParentWidget();

private:
    QWidget *m_parent;
};

#endif // UIFACTORYQT_H
