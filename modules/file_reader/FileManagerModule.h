#ifndef FILE_MANAGER_MODULE_H
#define FILE_MANAGER_MODULE_H

#include <memory>
#include "core/BaseModule.h"
#include "ModelAcquisitionSet.h"

class FileManagerModule: public BaseModule
{
public:
    FileManagerModule();

    void init(MainWindow *mainWindow);
    std::shared_ptr<ModelAcquisitionSet> getModelAcquisitionSet() const;
    void processDirectory(std::string directory_path);

private:
    std::shared_ptr<ModelAcquisitionSet> m_modelAcquisitionSet;
    MainWindow *m_mainWindow;
};

#endif // FILE_MANAGER_MODULE_H
