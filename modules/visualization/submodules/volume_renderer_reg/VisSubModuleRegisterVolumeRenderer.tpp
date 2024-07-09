#include "VisSubModuleRegisterVolumeRenderer.h"

#include "../volume_renderer/VisSubModuleSingleVolumeRenderer.h"
#include "../volume_renderer/event_handler/EventHandlerVolume.h"
#include "../volume_renderer/event_handler/EventHandler2DRendering.h"
#include "../volume_renderer/event_handler/EventHandler3DRendering.h"
#include <modules/visualization/AcquisitionModule.h>
#include <modules/visualization/submodules/volume_renderer_reg/ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.h>
#include <modules/visualization/submodules/volume_renderer_reg/ext_volume_renderer/RegisterSliceRenderer.h>
#include "ui/ToolVolumeBlending.h"
#include "ui/qt/UIFactoryRegisteredVolumeQt.h"

template<class vis_module_type>
VisSubModuleRegisterVolumeRenderer<vis_module_type>::VisSubModuleRegisterVolumeRenderer()
{
    m_raycastingVolumeRenderer3D = nullptr;
    m_axialSliceRenderer = nullptr;
    m_coronalSliceRenderer = nullptr;
    m_sagittalSliceRenderer = nullptr;

    m_eventHandlerVolume = nullptr;
    m_eventHandler2DRendering = nullptr;
    m_eventHandler3DRendering = nullptr;

    m_controllerVolumeBlendingTool = nullptr;
}

template<class vis_module_type>
VisSubModuleRegisterVolumeRenderer<vis_module_type>::~VisSubModuleRegisterVolumeRenderer()
{
    if(m_eventHandlerVolume != nullptr)
        delete m_eventHandlerVolume;

    if(m_eventHandler2DRendering != nullptr)
        delete m_eventHandler2DRendering;

    if(m_eventHandler3DRendering != nullptr)
        delete m_eventHandler3DRendering;


    if(m_controllerVolumeBlendingTool != nullptr)
    {

    }

    if(m_raycastingVolumeRenderer3D != nullptr)
        delete m_raycastingVolumeRenderer3D;

    if(m_axialSliceRenderer != nullptr)
        delete m_axialSliceRenderer;

    if(m_coronalSliceRenderer != nullptr)
        delete m_coronalSliceRenderer;

    if(m_sagittalSliceRenderer != nullptr)
        delete m_sagittalSliceRenderer;
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::init(VisualizationModule *vis_module)
{
    m_module = static_cast<vis_module_type*>(vis_module);

    createModel();
    createOpenGLResource();
    createEventHandlers();
    createControllers();
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::createModel()
{
    auto *acquisition = m_module->getReferenceModule()->getAcquisition();

    unsigned short volume_dim[3] = {acquisition->getDimensions(0), acquisition->getDimensions(1), acquisition->getDimensions(2)};
    double pixel_spacing[3] = {acquisition->getPixelSpacing(0), acquisition->getPixelSpacing(1), acquisition->getPixelSpacing(2)};

    ModelVolume *module_volume = new ModelVolume(volume_dim, pixel_spacing);
    setModelVolume(module_volume);

    float scale_factors[4];
    getModelVolume()->getScaleFactors(scale_factors);

    Model3DRendering *model_3d_rendering = new Model3DRendering(scale_factors, volume_dim);
    model_3d_rendering->setStateManager(m_module->getStateManager());
    setModel3DRendering(model_3d_rendering);

    Model2DRendering *model_2d_rendering = new Model2DRendering(scale_factors);
    setModel2DRendering(model_2d_rendering);

    VisSubModuleSingleVolumeRenderer<AcquisitionModule> *float_vis_submodule_volume_rendering = static_cast<VisSubModuleSingleVolumeRenderer<AcquisitionModule>*>(m_module->getFloatingModule()->getVisSubModule("VisSubModuleBaseVolumeRenderer"));
    m_modelFloatingVolume = float_vis_submodule_volume_rendering->getModelVolume();
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::createOpenGLResource()
{
    VisSubModuleSingleVolumeRenderer<AcquisitionModule> *ref_vis_submodule_volume_rendering = static_cast<VisSubModuleSingleVolumeRenderer<AcquisitionModule>*>(m_module->getReferenceModule()->getVisSubModule("VisSubModuleBaseVolumeRenderer"));
    VisSubModuleSingleVolumeRenderer<AcquisitionModule> *float_vis_submodule_volume_rendering = static_cast<VisSubModuleSingleVolumeRenderer<AcquisitionModule>*>(m_module->getFloatingModule()->getVisSubModule("VisSubModuleBaseVolumeRenderer"));

    m_referenceOpenGLVolumeRenderingResource = ref_vis_submodule_volume_rendering->getOpenGLVolumeRenderingResource();
    m_floatingOpenGLVolumeRenderingResource = float_vis_submodule_volume_rendering->getOpenGLVolumeRenderingResource();
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::createEventHandlers()
{
    if(m_eventHandlerVolume == nullptr)
    {
        m_eventHandlerVolume = new EventHandlerVolume();
        m_eventHandlerVolume->setModelVolume(getModelVolume());
        m_eventHandlerVolume->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
    }

    if(m_eventHandler2DRendering == nullptr)
    {
        if(m_module->getRendererAxial() || m_module->getRendererCoronal() || m_module->getRendererSagittal())
        {
            m_eventHandler2DRendering = new EventHandler2DRendering();
            m_eventHandler2DRendering->setModel2DRendering(getModel2DRendering());
            m_eventHandler2DRendering->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
        }
    }

    if(m_eventHandler3DRendering == nullptr)
    {
        if(m_module->getRenderer3D())
        {
            m_eventHandler3DRendering = new EventHandler3DRendering();
            m_eventHandler3DRendering->setModel3DRendering(getModel3DRendering());
            m_eventHandler3DRendering->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
        }
    }
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::createControllers()
{
    m_controllerVolumeBlendingTool = new ControllerVolumeBlendingTool();
    m_controllerVolumeBlendingTool->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());

    m_module->getMainWindow()->addUIFactory(UIFactoryRegisteredVolumeQt::uPtr(new UIFactoryRegisteredVolumeQt()));
    m_module->getMainWindow()->createTool(m_controllerVolumeBlendingTool->getToolClassName(), "Exploration", 0, 7, 2);

    ToolVolumeBlending *tool = dynamic_cast<ToolVolumeBlending*>(m_module->getMainWindow()->getTool(m_controllerVolumeBlendingTool->getToolClassName(), "Exploration"));
    if(tool != nullptr)
        tool->setController(m_module->getVisualizationTab(), m_controllerVolumeBlendingTool);
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::configure3DView()
{
    float ref_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);

    auto *ref_acquisition = m_module->getReferenceModule()->getAcquisition();
    auto *float_acquisition = m_module->getFloatingModule()->getAcquisition();

    unsigned short ref_volume_dim[4];
    ref_volume_dim[0] = ref_acquisition->getDimensions(0);
    ref_volume_dim[1] = ref_acquisition->getDimensions(1);
    ref_volume_dim[2] = ref_acquisition->getDimensions(2);
    ref_volume_dim[3] = 1.0;

    unsigned short float_volume_dim[4];
    float_volume_dim[0] = float_acquisition->getDimensions(0);
    float_volume_dim[1] = float_acquisition->getDimensions(1);
    float_volume_dim[2] = float_acquisition->getDimensions(2);
    float_volume_dim[3] = 1.0;

    m_raycastingVolumeRenderer3D = new RegisterRaycastingVolumeRenderer3D(ref_volume_dim, float_volume_dim, ref_scale_factor, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);

    vmtkAbstractRender *renderer_3D = m_module->getRenderer3D();
    renderer_3D->addSubRenderer(m_raycastingVolumeRenderer3D);

    m_eventHandlerVolume->setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);
    m_eventHandler3DRendering->setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);

    m_controllerVolumeBlendingTool->setRenderer3D(m_raycastingVolumeRenderer3D);
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::configureAxialView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceModule()->getAcquisition();
    auto *float_acquisition = m_module->getFloatingModule()->getAcquisition();

    float ref_volume_dim[4];
    ref_volume_dim[0] = ref_acquisition->getDimensions(0);
    ref_volume_dim[1] = ref_acquisition->getDimensions(1);
    ref_volume_dim[2] = ref_acquisition->getDimensions(2);
    ref_volume_dim[3] = 1.0;

    float float_volume_dim[4];
    float_volume_dim[0] = float_acquisition->getDimensions(0);
    float_volume_dim[1] = float_acquisition->getDimensions(1);
    float_volume_dim[2] = float_acquisition->getDimensions(2);
    float_volume_dim[3] = 1.0;

    m_axialSliceRenderer = new RegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererAxial(m_axialSliceRenderer);

    vmtkAbstractRender *renderer_axial = m_module->getRendererAxial();
    renderer_axial->addSubRenderer(m_axialSliceRenderer);

    m_eventHandler2DRendering->setAxialSliceRenderer(m_axialSliceRenderer);
    m_controllerVolumeBlendingTool->setRendererAxial(m_axialSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::configureCoronalView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceModule()->getAcquisition();
    auto *float_acquisition = m_module->getFloatingModule()->getAcquisition();

    float ref_volume_dim[4];
    ref_volume_dim[0] = ref_acquisition->getDimensions(0);
    ref_volume_dim[1] = ref_acquisition->getDimensions(1);
    ref_volume_dim[2] = ref_acquisition->getDimensions(2);
    ref_volume_dim[3] = 1.0;

    float float_volume_dim[4];
    float_volume_dim[0] = float_acquisition->getDimensions(0);
    float_volume_dim[1] = float_acquisition->getDimensions(1);
    float_volume_dim[2] = float_acquisition->getDimensions(2);
    float_volume_dim[3] = 1.0;

    m_coronalSliceRenderer = new RegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererCoronal(m_coronalSliceRenderer);

    vmtkAbstractRender *renderer_coronal = m_module->getRendererCoronal();
    renderer_coronal->addSubRenderer(m_coronalSliceRenderer);

    m_eventHandler2DRendering->setCoronalSliceRenderer(m_coronalSliceRenderer);
    m_controllerVolumeBlendingTool->setRendererCoronal(m_coronalSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::configureSagittalView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceModule()->getAcquisition();
    auto *float_acquisition = m_module->getFloatingModule()->getAcquisition();

    float ref_volume_dim[4];
    ref_volume_dim[0] = ref_acquisition->getDimensions(0);
    ref_volume_dim[1] = ref_acquisition->getDimensions(1);
    ref_volume_dim[2] = ref_acquisition->getDimensions(2);
    ref_volume_dim[3] = 1.0;

    float float_volume_dim[4];
    float_volume_dim[0] = float_acquisition->getDimensions(0);
    float_volume_dim[1] = float_acquisition->getDimensions(1);
    float_volume_dim[2] = float_acquisition->getDimensions(2);
    float_volume_dim[3] = 1.0;

    m_sagittalSliceRenderer = new RegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererSagittal(m_sagittalSliceRenderer);

    vmtkAbstractRender *renderer_sagittal = m_module->getRendererSagittal();
    renderer_sagittal->addSubRenderer(m_sagittalSliceRenderer);

    m_eventHandler2DRendering->setSagittalSliceRenderer(m_sagittalSliceRenderer);
    m_controllerVolumeBlendingTool->setRendererSagittal(m_sagittalSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::on3DRenderingSurfaceCreated()
{
    float left, right, bottom, top, back, front;
    getModel3DRendering()->getProxyGeometryDim(left, right, bottom, top, back, front);

    m_raycastingVolumeRenderer3D->setClippingValues(left, right, bottom, top, back, front);
    m_raycastingVolumeRenderer3D->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
    m_raycastingVolumeRenderer3D->setHighlightPlanesActivated(getModel3DRendering()->isHighlightedPlanesActivated());
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::onAxialRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getAxialData().getPlanePos(v1, v2, v3, v4);

    m_axialSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_axialSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::onCoronalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getCoronalData().getPlanePos(v1, v2, v3, v4);

    m_coronalSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_coronalSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}

template<class vis_module_type>
void VisSubModuleRegisterVolumeRenderer<vis_module_type>::onSagittalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getSagittalData().getPlanePos(v1, v2, v3, v4);

    m_sagittalSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_sagittalSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}
