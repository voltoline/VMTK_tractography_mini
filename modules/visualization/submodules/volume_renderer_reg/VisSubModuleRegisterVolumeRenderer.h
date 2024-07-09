#ifndef VIS_SUB_MODULE_REGISTER_VOLUME_RENDERER_H
#define VIS_SUB_MODULE_REGISTER_VOLUME_RENDERER_H

#include "../volume_renderer/VisSubModuleBaseVolumeRenderer.h"
#include "../volume_renderer/OpenGLVolumeRenderingResource.h"
#include "controllers/ControllerVolumeBlendingTool.h"

class RegisterSliceRenderer;
class OpenGLVolumeRenderingResource;
class RegisterRaycastingVolumeRenderer3D;
class EventHandlerVolume;
class EventHandler2DRendering;
class EventHandler3DRendering;

template <class vis_module_type>
class VisSubModuleRegisterVolumeRenderer: public VisSubModuleBaseVolumeRenderer
{
public:

    VisSubModuleRegisterVolumeRenderer();
    ~VisSubModuleRegisterVolumeRenderer();

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

    ModelVolume *m_modelFloatingVolume;

    EventHandlerVolume *m_eventHandlerVolume;
    EventHandler2DRendering *m_eventHandler2DRendering;
    EventHandler3DRendering *m_eventHandler3DRendering;

    OpenGLVolumeRenderingResource::sPtr m_referenceOpenGLVolumeRenderingResource;
    OpenGLVolumeRenderingResource::sPtr m_floatingOpenGLVolumeRenderingResource;

    RegisterRaycastingVolumeRenderer3D *m_raycastingVolumeRenderer3D;
    RegisterSliceRenderer *m_axialSliceRenderer;
    RegisterSliceRenderer *m_coronalSliceRenderer;
    RegisterSliceRenderer *m_sagittalSliceRenderer;

    ControllerVolumeBlendingTool *m_controllerVolumeBlendingTool;
};

#include "VisSubModuleRegisterVolumeRenderer.tpp"

#endif // VIS_SUB_MODULE_REGISTER_VOLUME_RENDERER_H
