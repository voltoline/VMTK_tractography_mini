#include "DiffusionRegisterVisualizationModule.h"

#include "renderer/vmtkRender3D.h"
#include "renderer/vmtkRender2D.h"
#include "modules/visualization/submodules/volume_renderer_diff_reg/VisSubModuleRegisterDiffusionVolumeRenderer.h"
#include <modules/visualization/submodules/transformation/VisSubModuleTransformation.h>
#include "modules/visualization/submodules/cursor/VisSubModuleCursor.h"
#include "modules/visualization/submodules/dti/VisSubModuleDTI.h"
#include "modules/visualization/submodules/roi/VisSubModuleROI.h"
#include "modules/visualization/submodules/text_renderer/VisSubModuleTextRenderer.h"
#include "modules/visualization/submodules/2d_3d_slicing/VisSubModule2D3DSlicing.h"

#define COORD_REG_DIFF_VIS_MODULE_TYPE REG_DIFF_VIS_MODULE_TYPE, DIFF_ACQ_MODULE_TYPE, ACQ_MODULE_TYPE
#define COORD_REG_DIFF_VIS_MODULE_TYPE_2 DiffusionRegisterVisualizationModel, DiffusionAcquisitionModule, AcquisitionModule

DiffusionRegisterVisualizationModel::DiffusionRegisterVisualizationModel(vmtkDiffAcquisition *reference_acquisition, vmtkAcquisition *floating_acquisition, glm::mat4 registration_matrix, glm::mat4 registration_matrix_inv):
    VisualizationModule("DiffusionRegisterVisualizationModel")
{
    m_referenceAcquisition = reference_acquisition;
    m_floatingAcquisition = floating_acquisition;

    m_modelRigidRegister = nullptr;
    m_registerID = 0;

    m_registrationMatrix = registration_matrix;
    m_registrationMatrixInv = registration_matrix_inv;

}

vmtkDiffAcquisition *DiffusionRegisterVisualizationModel::getAcquisition()
{
    return m_referenceAcquisition;
}

void DiffusionRegisterVisualizationModel::loadSubModules()
{
    createVisualizationTab("Reg", false);

    addVisSubModule(new VisSubModuleRegisterDiffusionVolumeRenderer<DiffusionRegisterVisualizationModel>());

    addVisSubModule(new VisSubModuleTransformation());

    addVisSubModule(new VisSubModule2D3DSlicing<DiffusionRegisterVisualizationModel>());

    addVisSubModule(new VisSubModuleCursor<DiffusionRegisterVisualizationModel>());

    addVisSubModule(new VisSubModuleDTI<DiffusionRegisterVisualizationModel>());

    addVisSubModule(new VisSubModuleROI<DiffusionRegisterVisualizationModel>());

    addVisSubModule(new VisSubModuleTextRenderer<DiffusionRegisterVisualizationModel>());
}

glm::mat4 DiffusionRegisterVisualizationModel::getRegistrationMatrix()
{
    return m_registrationMatrix;
}

glm::mat4 DiffusionRegisterVisualizationModel::getInverseRegistrationMatrix()
{
    return m_registrationMatrixInv;
}

vmtkAbstractRender *DiffusionRegisterVisualizationModel::createRenderer3D()
{
    return new vmtkRender3D();
}

vmtkAbstractRender *DiffusionRegisterVisualizationModel::createRenderer2D()
{
    return new vmtkRender2D();
}

vmtkAcquisition *DiffusionRegisterVisualizationModel::getFloatingAcquisition() const
{
    return m_floatingAcquisition;
}

vmtkDiffAcquisition *DiffusionRegisterVisualizationModel::getReferenceAcquisition() const
{
    return m_referenceAcquisition;
}
