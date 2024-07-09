#include "UIFactoryQt.h"

UIFactoryQt::UIFactoryQt(std::string factory_class_name):
    UIFactoryBase(factory_class_name)
{
    m_parent = nullptr;
}

void UIFactoryQt::setParentWidget(QWidget *parent)
{
    m_parent = parent;
}

QWidget* UIFactoryQt::getParentWidget()
{
    return m_parent;
}
