#ifndef UI_FACTORY_TRANSFORMATION_QT_H
#define UI_FACTORY_TRANSFORMATION_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactoryTransformationQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactoryTransformationQt> sPtr;
    typedef std::unique_ptr<UIFactoryTransformationQt> uPtr;

    UIFactoryTransformationQt();
    ~UIFactoryTransformationQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_TRANSFORMATION_QT_H
