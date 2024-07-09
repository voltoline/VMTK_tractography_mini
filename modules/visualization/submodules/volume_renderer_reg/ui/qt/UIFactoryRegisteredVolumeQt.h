#ifndef UI_FACTORY_REGISTERED_VOLUMES_QT_H
#define UI_FACTORY_REGISTERED_VOLUMES_QT_H

#include "ui/qt/UIFactoryQt.h"

class UIFactoryRegisteredVolumeQt: public UIFactoryQt
{
public:
    typedef std::shared_ptr<UIFactoryRegisteredVolumeQt> sPtr;
    typedef std::unique_ptr<UIFactoryRegisteredVolumeQt> uPtr;

    UIFactoryRegisteredVolumeQt();
    ~UIFactoryRegisteredVolumeQt() { }

    ToolBase *createTool(std::string tool_class_name) override;
};

#endif // UI_FACTORY_REGISTERED_VOLUMES_QT_H
