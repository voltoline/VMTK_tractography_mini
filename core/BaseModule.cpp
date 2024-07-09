#include "BaseModule.h"

BaseModule::BaseModule(std::string module_class_name):
    m_moduleClassName(module_class_name)
{

}

std::string BaseModule::getModuleClassName() const
{
    return m_moduleClassName;
}
