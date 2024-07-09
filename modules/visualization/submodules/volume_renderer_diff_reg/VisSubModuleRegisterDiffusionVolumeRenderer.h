#ifndef VIS_SUB_MODULE_REGISTER_DIFFUSION_VOLUME_RENDERER_H
#define VIS_SUB_MODULE_REGISTER_DIFFUSION_VOLUME_RENDERER_H

#include "../volume_renderer/VisSubModuleBaseVolumeRenderer.h"
#include "../volume_renderer_diff/ext_volume_renderer/EventHandlerDiffusionVolume.h"
#include <modules/visualization/submodules/volume_renderer_reg/controllers/ControllerVolumeBlendingTool.h>

class DiffusionRegisterSliceRenderer;
class RegisteredDiffusionRaycastingVolumeRenderer3D;
class ModelDiffusionUI;
class EventHandlerDiffusion;
class EventHandler2DRendering;
class EventHandler3DRendering;

template <class vis_module_type>
class VisSubModuleRegisterDiffusionVolumeRenderer: public VisSubModuleBaseVolumeRenderer
{
public:

    VisSubModuleRegisterDiffusionVolumeRenderer();
    ~VisSubModuleRegisterDiffusionVolumeRenderer();

    RegisteredDiffusionRaycastingVolumeRenderer3D *getRaycastingVolumeRenderer3D() const;

    ModelDiffusionUI *getModelDiffusionUI() const;

private:
    void init(VisualizationModule *vis_module) override;

    void createModel();
    void createOpenGLResource();
    void createEventHandlers();
    void createControllers();

    void configure3DView() override;
    void configureAxialView() override;
    void configureCoronalView() override;
    void configureSagittalView() override;

    void on3DRenderingSurfaceCreated() override;
    void onAxialRenderingSurfaceCreated() override;
    void onCoronalRenderingSurfaceCreated() override;
    void onSagittalRenderingSurfaceCreated() override;

    vis_module_type *m_module;

    ModelDiffusionUI *m_modelDiffusionUI;

    ModelVolume *m_modelFloatingVolume;
    EventHandlerDiffusionVolume<RegisteredDiffusionRaycastingVolumeRenderer3D, DiffusionRegisterSliceRenderer> *m_eventHandlerVolume;
    EventHandler2DRendering *m_eventHandler2DRendering;
    EventHandler3DRendering *m_eventHandler3DRendering;
    EventHandlerDiffusion *m_eventHandlerDiffusion;

    OpenGLVolumeRenderingResource::sPtr m_referenceOpenGLVolumeRenderingResource;
    OpenGLVolumeRenderingResource::sPtr m_floatingOpenGLVolumeRenderingResource;

    RegisteredDiffusionRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;
    DiffusionRegisterSliceRenderer *m_axialSliceRenderer;
    DiffusionRegisterSliceRenderer *m_coronalSliceRenderer;
    DiffusionRegisterSliceRenderer *m_sagittalSliceRenderer;

    ControllerVolumeBlendingTool *m_controllerRegisterToolTab;
};

#include "VisSubModuleRegisterDiffusionVolumeRenderer.tpp"

#endif // VIS_SUB_MODULE_REGISTER_DIFFUSION_VOLUME_RENDERER_H
