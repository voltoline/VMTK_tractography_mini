#include "VisSubModuleRegisterDiffusionVolumeRenderer.h"

#include "../volume_renderer/event_handler/EventHandlerVolume.h"
#include "ext_volume_renderer_reg/DiffusionRegisterSliceRenderer.h"
#include "ext_volume_renderer_reg/RegisteredDiffusionRaycastingVolumeRenderer3D.h"
#include "../volume_renderer/event_handler/EventHandler2DRendering.h"
#include "../volume_renderer/event_handler/EventHandler3DRendering.h"
#include <modules/visualization/submodules/volume_renderer_diff/controllers/ControllerDiffusionTool.h>
#include <modules/visualization/submodules/volume_renderer_diff/event_handlers/EventHandlerDiffusion.h>
#include <modules/visualization/submodules/volume_renderer_diff/models/ModelDiffusionUI.h>
#include <modules/visualization/submodules/volume_renderer_diff/ui/ToolDiffusion.h>
#include <modules/visualization/submodules/volume_renderer_diff/ui/qt/UIFactoryDiffusionRendererQt.h>
#include <modules/visualization/submodules/volume_renderer_reg/ui/ToolVolumeBlending.h>
#include <modules/visualization/submodules/volume_renderer_reg/ui/qt/UIFactoryRegisteredVolumeQt.h>

template<class vis_module_type>
VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::VisSubModuleRegisterDiffusionVolumeRenderer()
{
    m_raycastingVolumeRenderer3D = nullptr;
    m_axialSliceRenderer = nullptr;
    m_coronalSliceRenderer = nullptr;
    m_sagittalSliceRenderer = nullptr;

    m_modelDiffusionUI = nullptr;

    m_eventHandlerVolume = nullptr;
    m_eventHandler2DRendering = nullptr;
    m_eventHandler3DRendering = nullptr;
    m_eventHandlerDiffusion = nullptr;

    m_controllerRegisterToolTab = nullptr;
}

template<class vis_module_type>
VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::~VisSubModuleRegisterDiffusionVolumeRenderer()
{
    if(m_eventHandlerVolume != nullptr)
        delete m_eventHandlerVolume;

    if(m_eventHandler2DRendering != nullptr)
        delete m_eventHandler2DRendering;

    if(m_eventHandler3DRendering != nullptr)
        delete m_eventHandler3DRendering;

    if(m_eventHandlerDiffusion != nullptr)
        delete m_eventHandlerDiffusion;


    if(m_controllerRegisterToolTab != nullptr)
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

    if(m_modelDiffusionUI != nullptr)
        delete m_modelDiffusionUI;
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::init(VisualizationModule *vis_module)
{
    m_module = static_cast<vis_module_type*>(vis_module);

    createModel();
    createOpenGLResource();
    createEventHandlers();
    createControllers();
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::createModel()
{
    {

        auto *float_acquisition = m_module->getFloatingAcquisition();
        unsigned short volume_dim[3] = {float_acquisition->getDimensions(0), float_acquisition->getDimensions(1), float_acquisition->getDimensions(2)};
        double pixel_spacing[3] = {float_acquisition->getPixelSpacing(0), float_acquisition->getPixelSpacing(1), float_acquisition->getPixelSpacing(2)};

        m_modelFloatingVolume = new ModelVolume(volume_dim, pixel_spacing);

        //Construct grayscale presentation function from the normalized values.
        m_modelFloatingVolume->setGrayscaleVolumeData (reinterpret_cast<unsigned short*>(float_acquisition->getImageBuffer()), float_acquisition->getBitsAllocated(), float_acquisition->getUmax());

        m_modelFloatingVolume->setThresholdInterval(0, float_acquisition->getUmax());

        m_floatingOpenGLVolumeRenderingResource = std::make_shared<OpenGLVolumeRenderingResource>();

        //Estimate the default range form the ones recomended by the vendors (DICOM) (added by Ting, 25/11/2019)
        m_modelFloatingVolume->estimateDefaultRanges(float_acquisition->getRawUmin(), float_acquisition->getUmax(), float_acquisition->getVOILUTWindowCenter(), float_acquisition->getVOILUTWindowWidth());

        std::cout << "VisSubModuleRegisterDiffusionVolumeRenderer: getMax1D2DTextyureSize() = " << m_floatingOpenGLVolumeRenderingResource->getMax1D2DTextureSize() << std::endl;
        m_modelFloatingVolume->createDefaultTransferFunctionBuffer((unsigned short) (m_floatingOpenGLVolumeRenderingResource->getMax1D2DTextureSize()/4), float_acquisition->getUmax());

        m_modelFloatingVolume->getTransferFunction()->load("./volume/vmb_tf_buffer.tfb");
    }

    auto *acquisition = m_module->getReferenceAcquisition();

    m_modelDiffusionUI = new ModelDiffusionUI(acquisition);

    unsigned short volume_dim[3] = {acquisition->getDimensions(0), acquisition->getDimensions(1), acquisition->getDimensions(2)};
    double pixel_spacing[3] = {acquisition->getPixelSpacing(0), acquisition->getPixelSpacing(1), acquisition->getPixelSpacing(2)};

    ModelVolume *module_volume = new ModelVolume(volume_dim, pixel_spacing);
    setModelVolume(module_volume);

    float scale_factors[4];
    getModelVolume()->getScaleFactors(scale_factors);

    Model3DRendering *model_3d_rendering = new Model3DRendering(scale_factors, volume_dim);
    model_3d_rendering->setStateManager(m_module->getStateManager());

    //commented out to disable "slicing mode"  (Ting, /12/2022)
//    m_module->getStateManager()->enableFeature("HIGHLIGHTED_3D_PLANE");
//    m_module->getStateManager()->activateFeature("HIGHLIGHTED_3D_PLANE");

    setModel3DRendering(model_3d_rendering);

    Model2DRendering *model_2d_rendering = new Model2DRendering(scale_factors);
    setModel2DRendering(model_2d_rendering);

    {
        //Construct grayscale presentation function from the normalized values.
        getModelVolume()->setGrayscaleVolumeData (reinterpret_cast<unsigned short*>(acquisition->getImageBuffer(0)), acquisition->getBitsAllocated(), acquisition->getUmax());

        getModelVolume()->setThresholdInterval(0, acquisition->getUmax());

        m_referenceOpenGLVolumeRenderingResource = std::make_shared<OpenGLVolumeRenderingResource>();

        //Estimate the default range form the ones recomended by the vendors (DICOM)
        getModelVolume()->estimateDefaultRanges(acquisition->getRawUmin(), acquisition->getUmax(), acquisition->getVOILUTWindowCenter(),acquisition->getVOILUTWindowWidth());

        getModelVolume()->createDefaultTransferFunctionBuffer((unsigned short) (m_referenceOpenGLVolumeRenderingResource->getMax1D2DTextureSize()/4), acquisition->getUmax());

        getModelVolume()->getTransferFunction()->load("./volume/dti_tf_buffer.tfb");
    }
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::createOpenGLResource()
{
    {

        auto *acquisition = m_module->getReferenceAcquisition();
        unsigned short volume_dim[3] = {acquisition->getDimensions(0), acquisition->getDimensions(1), acquisition->getDimensions(2)};

        // verify whether m_openGLVolumeRenderingResource exists
        if (! m_referenceOpenGLVolumeRenderingResource) //added by Ting (22/11/2019)
            m_referenceOpenGLVolumeRenderingResource = std::make_shared<OpenGLVolumeRenderingResource>();

        m_referenceOpenGLVolumeRenderingResource->setVolumeDimension(volume_dim);

        ///Ting: replace equalized volume by the normalized pixel values to ensure linear transformation (18/11/2019)
        m_referenceOpenGLVolumeRenderingResource->setGrayscaleVolumeData(getModelVolume()->getMappedGrayscaleVolume());

        m_referenceOpenGLVolumeRenderingResource->setTransferFunctionData(getModelVolume()->getTransferFunction());
        m_referenceOpenGLVolumeRenderingResource->setSampleInterpolation(OpenGLVolumeRenderingResource::SampleInterpolation::LINEAR);

        getModelVolume()->setIsVolumeLoadedAsTexture(true);
    }

    {

        auto *acquisition = m_module->getFloatingAcquisition();
        unsigned short volume_dim[3] = {acquisition->getDimensions(0), acquisition->getDimensions(1), acquisition->getDimensions(2)};

        // verify whether m_openGLVolumeRenderingResource exists
        if (! m_floatingOpenGLVolumeRenderingResource) //added by Ting (22/11/2019)
            m_floatingOpenGLVolumeRenderingResource = std::make_shared<OpenGLVolumeRenderingResource>();

        m_floatingOpenGLVolumeRenderingResource->setVolumeDimension(volume_dim);

        ///Ting: replace equalized volume by the normalized pixel values to ensure linear transformation (18/11/2019)
        m_floatingOpenGLVolumeRenderingResource->setGrayscaleVolumeData(m_modelFloatingVolume->getMappedGrayscaleVolume());

        m_floatingOpenGLVolumeRenderingResource->setTransferFunctionData(m_modelFloatingVolume->getTransferFunction());
        m_floatingOpenGLVolumeRenderingResource->setSampleInterpolation(OpenGLVolumeRenderingResource::SampleInterpolation::LINEAR);

        m_modelFloatingVolume->setIsVolumeLoadedAsTexture(true);
    }
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::createEventHandlers()
{
    if(m_eventHandlerVolume == nullptr)
    {
        m_eventHandlerVolume = new EventHandlerDiffusionVolume<RegisteredDiffusionRaycastingVolumeRenderer3D, DiffusionRegisterSliceRenderer>();
        m_eventHandlerVolume->setOpenGLVolumeRenderingResource(m_referenceOpenGLVolumeRenderingResource);
        m_eventHandlerVolume->setModelVolume(getModelVolume());
        m_eventHandlerVolume->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
    }

    if(m_eventHandler2DRendering == nullptr)
    {
        m_eventHandler2DRendering = new EventHandler2DRendering();
        m_eventHandler2DRendering->setModel2DRendering(getModel2DRendering());
        m_eventHandler2DRendering->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
    }

    if(m_eventHandler3DRendering == nullptr)
    {
        m_eventHandler3DRendering = new EventHandler3DRendering();
        m_eventHandler3DRendering->setModel3DRendering(getModel3DRendering());
        m_eventHandler3DRendering->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
    }

    if(m_eventHandlerDiffusion == nullptr)
    {
        m_eventHandlerDiffusion = new EventHandlerDiffusion();
        m_eventHandlerDiffusion->setModelVolume(getModelVolume());
        m_eventHandlerDiffusion->setModelDiffusion(m_modelDiffusionUI);
        m_eventHandlerDiffusion->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());
    }
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::createControllers()
{
    m_controllerRegisterToolTab = new ControllerVolumeBlendingTool();
    m_controllerRegisterToolTab->setRenderingSurfaceSet(m_module->getRenderingSurfaceSet());

    ControllerDiffusionTool *controller_diffusion_tool = new ControllerDiffusionTool();
    controller_diffusion_tool->setModelDiffusionUI(m_modelDiffusionUI);

    m_module->getMainWindow()->addUIFactory(UIFactoryRegisteredVolumeQt::uPtr(new UIFactoryRegisteredVolumeQt()));
    m_module->getMainWindow()->createTool(m_controllerRegisterToolTab->getToolClassName(), "Exploration", 0, 7, 2);

    ToolVolumeBlending *tool_blending = dynamic_cast<ToolVolumeBlending*>(m_module->getMainWindow()->getTool(m_controllerRegisterToolTab->getToolClassName(), "Exploration"));
    if(tool_blending != nullptr)
        tool_blending->setController(m_module->getVisualizationTab(), m_controllerRegisterToolTab);

    m_module->getMainWindow()->addUIFactory(UIFactoryDiffusionRendererQt::uPtr(new UIFactoryDiffusionRendererQt()));
    m_module->getMainWindow()->createMenuToolBaseUniqueAddingDialog("Diffusion", "DWI", controller_diffusion_tool->getToolClassName(), "UIFactoryDiffusionRendererQt");

    ToolDiffusion *tool_diff = static_cast<ToolDiffusion*>(m_module->getMainWindow()->getToolUniqueWindowFloat(controller_diffusion_tool->getToolClassName()));
    if(tool_diff != nullptr)
        tool_diff->setController(m_module->getVisualizationTab(), controller_diffusion_tool);
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::configure3DView()
{
    float ref_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);

    auto *ref_acquisition = m_module->getReferenceAcquisition();
    auto *float_acquisition = m_module->getFloatingAcquisition();

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

    m_raycastingVolumeRenderer3D = new RegisteredDiffusionRaycastingVolumeRenderer3D(ref_volume_dim, float_volume_dim, ref_scale_factor, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);

    vmtkAbstractRender *renderer_3D = m_module->getRenderer3D();
    renderer_3D->addSubRenderer(m_raycastingVolumeRenderer3D);

    m_eventHandlerVolume->setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);
    m_eventHandler3DRendering->setRaycastingVolumeRenderer3D(m_raycastingVolumeRenderer3D);
    m_controllerRegisterToolTab->setRenderer3D(m_raycastingVolumeRenderer3D);
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::configureAxialView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceAcquisition();
    auto *float_acquisition = m_module->getFloatingAcquisition();

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

    m_axialSliceRenderer = new DiffusionRegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererAxial(m_axialSliceRenderer);

    vmtkAbstractRender *renderer_axial = m_module->getRendererAxial();
    renderer_axial->addSubRenderer(m_axialSliceRenderer);

    m_eventHandlerVolume->setSliceRendererAxial(m_axialSliceRenderer);
    m_eventHandler2DRendering->setAxialSliceRenderer(m_axialSliceRenderer);
    m_controllerRegisterToolTab->setRendererAxial(m_axialSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::configureCoronalView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceAcquisition();
    auto *float_acquisition = m_module->getFloatingAcquisition();

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

    m_coronalSliceRenderer = new DiffusionRegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererCoronal(m_coronalSliceRenderer);

    vmtkAbstractRender *renderer_coronal = m_module->getRendererCoronal();
    renderer_coronal->addSubRenderer(m_coronalSliceRenderer);

    m_eventHandlerVolume->setSliceRendererCoronal(m_coronalSliceRenderer);
    m_eventHandler2DRendering->setCoronalSliceRenderer(m_coronalSliceRenderer);
    m_controllerRegisterToolTab->setRendererCoronal(m_coronalSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::configureSagittalView()
{
    float ref_scale_factor[4], float_scale_factor[4];
    getModelVolume()->getScaleFactors(ref_scale_factor);
    m_modelFloatingVolume->getScaleFactors(float_scale_factor);

    auto *ref_acquisition = m_module->getReferenceAcquisition();
    auto *float_acquisition = m_module->getFloatingAcquisition();

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

    m_sagittalSliceRenderer = new DiffusionRegisterSliceRenderer(ref_scale_factor, ref_scale_factor, ref_volume_dim, float_volume_dim, m_referenceOpenGLVolumeRenderingResource, m_floatingOpenGLVolumeRenderingResource);

    setSliceRendererSagittal(m_sagittalSliceRenderer);

    vmtkAbstractRender *renderer_sagittal = m_module->getRendererSagittal();
    renderer_sagittal->addSubRenderer(m_sagittalSliceRenderer);

    m_eventHandlerVolume->setSliceRendererSagittal(m_sagittalSliceRenderer);
    m_eventHandler2DRendering->setSagittalSliceRenderer(m_sagittalSliceRenderer);
    m_controllerRegisterToolTab->setRendererSagittal(m_sagittalSliceRenderer);
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::on3DRenderingSurfaceCreated()
{
    float left, right, bottom, top, back, front;
    getModel3DRendering()->getProxyGeometryDim(left, right, bottom, top, back, front);

    m_raycastingVolumeRenderer3D->setClippingValues(left, right, bottom, top, back, front);
    m_raycastingVolumeRenderer3D->setStateThreshold(getModelVolume()->getThresholdState());
    m_raycastingVolumeRenderer3D->setNoiseThreshold(getModelVolume()->getMappedThreshold());
    m_raycastingVolumeRenderer3D->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
    m_raycastingVolumeRenderer3D->setHighlightPlanesActivated(getModel3DRendering()->isHighlightedPlanesActivated());
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::onAxialRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getAxialData().getPlanePos(v1, v2, v3, v4);

    m_axialSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_axialSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::onCoronalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getCoronalData().getPlanePos(v1, v2, v3, v4);

    m_coronalSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_coronalSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}

template<class vis_module_type>
void VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::onSagittalRenderingSurfaceCreated()
{
    glm::vec3 v1, v2, v3, v4;
    getModel2DRendering()->getSagittalData().getPlanePos(v1, v2, v3, v4);

    m_sagittalSliceRenderer->setPlanePos(v1, v2, v3, v4);
    m_sagittalSliceRenderer->setRegistrationMatrixInv(m_module->getInverseRegistrationMatrix());
}

template<class vis_module_type>
ModelDiffusionUI *VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::getModelDiffusionUI() const
{
    return m_modelDiffusionUI;
}

template<class vis_module_type>
RegisteredDiffusionRaycastingVolumeRenderer3D *VisSubModuleRegisterDiffusionVolumeRenderer<vis_module_type>::getRaycastingVolumeRenderer3D() const
{
    return m_raycastingVolumeRenderer3D;
}
