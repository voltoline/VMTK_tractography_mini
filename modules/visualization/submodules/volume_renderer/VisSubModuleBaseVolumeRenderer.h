#ifndef VIS_SUB_MODULE_BASE_VOLUME_RENDERER_H
#define VIS_SUB_MODULE_BASE_VOLUME_RENDERER_H

#include "3d_renderer/BaseRaycastingVolumeRenderer3D.h"
#include "modules/visualization/VisualizationModule.h"
#include "model/ModelVolume.h"
#include "model/Model2DRendering.h"
#include "model/Model3DRendering.h"
#include "model/ModelTransferFunction.h"
#include "2d_renderer/BaseSliceRenderer.h"

class VisSubModuleBaseVolumeRenderer: public VisualizationSubModule
{
public:
    VisSubModuleBaseVolumeRenderer(std::string vis_sub_module_name = "VisSubModuleBaseVolumeRenderer");
    ~VisSubModuleBaseVolumeRenderer() { }

    ModelVolume *getModelVolume();
    Model2DRendering *getModel2DRendering() const;
    Model3DRendering *getModel3DRendering() const;

    BaseRaycastingVolumeRenderer3D *getRaycastingVolumeRenderer3D() const;
    BaseSliceRenderer *getSliceRendererAxial() const;
    BaseSliceRenderer *getSliceRendererCoronal() const;
    BaseSliceRenderer *getSliceRendererSagittal() const;
    ModelTransferFunction *getModelTransferFunction() const;

protected:
    void setModelVolume(ModelVolume *model_volume);
    void setModel2DRendering(Model2DRendering *model2DRendering);
    void setModel3DRendering(Model3DRendering *model3DRendering);
    void setModelTransferFunction(ModelTransferFunction *modelTransferFunction);

    void setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *baseRaycastingVolumeRenderer3D);
    void setSliceRendererAxial(BaseSliceRenderer *baseSliceRendererAxial);
    void setSliceRendererCoronal(BaseSliceRenderer *baseSliceRendererCoronal);
    void setSliceRendererSagittal(BaseSliceRenderer *baseSliceRendererSagittal);

private:
    ModelVolume *m_modelVolume;
    Model2DRendering *m_model2DRendering;
    Model3DRendering *m_model3DRendering;
    ModelTransferFunction *m_modelTransferFunction;

    BaseRaycastingVolumeRenderer3D *m_baseRaycastingVolumeRenderer3D;
    BaseSliceRenderer *m_baseSliceRendererAxial;
    BaseSliceRenderer *m_baseSliceRendererCoronal;
    BaseSliceRenderer *m_baseSliceRendererSagittal;
};

#endif // VIS_SUB_MODULE_BASE_VOLUME_RENDERER_H
