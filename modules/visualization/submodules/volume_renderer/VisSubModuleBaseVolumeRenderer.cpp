#include "VisSubModuleBaseVolumeRenderer.h"

VisSubModuleBaseVolumeRenderer::VisSubModuleBaseVolumeRenderer(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{
    m_modelVolume = nullptr;
    m_model2DRendering = nullptr;
    m_model3DRendering = nullptr;

    m_baseRaycastingVolumeRenderer3D = nullptr;
    m_baseSliceRendererAxial = nullptr;
    m_baseSliceRendererCoronal = nullptr;
    m_baseSliceRendererSagittal = nullptr;
}

ModelVolume *VisSubModuleBaseVolumeRenderer::getModelVolume()
{
    if(m_modelVolume == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getModelVolume() -->> model not set!" << std::endl;

    return m_modelVolume;
}

void VisSubModuleBaseVolumeRenderer::setModelVolume(ModelVolume *model_volume)
{
    m_modelVolume = model_volume;
}


void VisSubModuleBaseVolumeRenderer::setModel2DRendering(Model2DRendering *model2DRendering)
{
    m_model2DRendering = model2DRendering;
}

Model2DRendering *VisSubModuleBaseVolumeRenderer::getModel2DRendering() const
{
    if(m_model2DRendering == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getModel2DRendering() -->> model not set!" << std::endl;

    return m_model2DRendering;
}

void VisSubModuleBaseVolumeRenderer::setModel3DRendering(Model3DRendering *model3DRendering)
{
    m_model3DRendering = model3DRendering;
}

void VisSubModuleBaseVolumeRenderer::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *baseRaycastingVolumeRenderer3D)
{
    m_baseRaycastingVolumeRenderer3D = baseRaycastingVolumeRenderer3D;
}

BaseSliceRenderer *VisSubModuleBaseVolumeRenderer::getSliceRendererSagittal() const
{
    if(m_baseSliceRendererSagittal == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getSliceRendererSagittal() -->> renderer not set!" << std::endl;

    return m_baseSliceRendererSagittal;
}

void VisSubModuleBaseVolumeRenderer::setSliceRendererSagittal(BaseSliceRenderer *baseSliceRendererSagittal)
{
    m_baseSliceRendererSagittal = baseSliceRendererSagittal;
}

ModelTransferFunction *VisSubModuleBaseVolumeRenderer::getModelTransferFunction() const
{
    if(m_modelTransferFunction == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getModelTransferFunction() -->> model not set!" << std::endl;

    return m_modelTransferFunction;
}

void VisSubModuleBaseVolumeRenderer::setModelTransferFunction(ModelTransferFunction *modelTransferFunction)
{
    m_modelTransferFunction = modelTransferFunction;
}

BaseSliceRenderer *VisSubModuleBaseVolumeRenderer::getSliceRendererCoronal() const
{
    if(m_baseSliceRendererCoronal == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getSliceRendererCoronal() -->> renderer not set!" << std::endl;
    
    return m_baseSliceRendererCoronal;
}

void VisSubModuleBaseVolumeRenderer::setSliceRendererCoronal(BaseSliceRenderer *baseSliceRendererCoronal)
{
    m_baseSliceRendererCoronal = baseSliceRendererCoronal;
}

BaseSliceRenderer *VisSubModuleBaseVolumeRenderer::getSliceRendererAxial() const
{
    if(m_baseSliceRendererAxial == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getSliceRendererAxial() -->> renderer not set!" << std::endl;

    return m_baseSliceRendererAxial;
}

void VisSubModuleBaseVolumeRenderer::setSliceRendererAxial(BaseSliceRenderer *baseSliceRendererAxial)
{
    m_baseSliceRendererAxial = baseSliceRendererAxial;
}

BaseRaycastingVolumeRenderer3D *VisSubModuleBaseVolumeRenderer::getRaycastingVolumeRenderer3D() const
{
    if(m_baseRaycastingVolumeRenderer3D == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getRaycastingVolumeRenderer3D() -->> renderer not set!" << std::endl;

    return m_baseRaycastingVolumeRenderer3D;
}

Model3DRendering *VisSubModuleBaseVolumeRenderer::getModel3DRendering() const
{
    if(m_model3DRendering == nullptr)
        std::cout << "VisSubModuleBaseVolumeRenderer::getModel3DRendering() -->> model not set!" << std::endl;

    return m_model3DRendering;
}
