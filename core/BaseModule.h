#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <memory>
#include <iostream>

class MainWindow;

/**
 * @brief
 *
 * @class BaseModule BaseModule.h "BaseModule.h"
 */
class BaseModule
{
public:
    typedef std::shared_ptr<BaseModule> sPtr;

    /**
     * @brief
     *
     * @fn BaseModule
     * @param module_class_name
     */
    BaseModule(std::string module_class_name);

    /**
     * @brief
     *
     * @fn ~BaseModule
     */
    virtual ~BaseModule() { }

    /**
     * @brief
     *
     * @fn init
     * @param mainWindow
     */
    virtual void init(MainWindow *main_window) = 0;

    /**
     * @brief
     *
     * @fn getModuleClassName
     * @return std::string
     */
    std::string getModuleClassName() const;

private:
    std::string m_moduleClassName;  /**< TODO: describe */
};

#endif // BASE_MODULE_H
