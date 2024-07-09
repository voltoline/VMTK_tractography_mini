#ifndef DIFFUSION_REGISTER_VISUALIZATION_MODULE_H
#define DIFFUSION_REGISTER_VISUALIZATION_MODULE_H

#include "VisualizationModule.h"

#include <glm/mat4x4.hpp>

#define REG_DIFF_VIS_MODULE_TYPE DiffusionRegisterVisualizationModel, vmtkAbstractRender, vmtkAbstractRender
#define REG_DIFF_VIS_MODULE_RENDERER_TYPE vmtkAbstractRender, vmtkAbstractRender

class ModelRigidRegister;
class vmtkDiffAcquisition;
class vmtkAcquisition;
class vmtkAbstractRender;

class DiffusionRegisterVisualizationModel: public VisualizationModule
{
public:
    DiffusionRegisterVisualizationModel(vmtkDiffAcquisition *reference_acquisition, vmtkAcquisition *floating_acquisition, glm::mat4 registration_matrix, glm::mat4 registration_matrix_inv);

    vmtkDiffAcquisition* getAcquisition();

    glm::mat4 getRegistrationMatrix();
    glm::mat4 getInverseRegistrationMatrix();

    void loadSubModules() override;

    vmtkDiffAcquisition *getReferenceAcquisition() const;
    vmtkAcquisition *getFloatingAcquisition() const;

protected:
    vmtkAbstractRender* createRenderer3D() override;
    vmtkAbstractRender* createRenderer2D() override;

private:

    vmtkDiffAcquisition *m_referenceAcquisition;
    vmtkAcquisition *m_floatingAcquisition;

    ModelRigidRegister *m_modelRigidRegister;
    unsigned short m_registerID;

    glm::mat4 m_registrationMatrix;
    glm::mat4 m_registrationMatrixInv;
};

#endif // DIFFUSION_REGISTER_VISUALIZATION_MODULE_H
