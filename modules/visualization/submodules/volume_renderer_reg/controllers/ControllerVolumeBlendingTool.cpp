#include "ControllerVolumeBlendingTool.h"

#include "ui/components/RenderingSurfaceSet.h"

#include "../ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.h"
#include "../ext_volume_renderer/RegisterSliceRenderer.h"

ControllerVolumeBlendingTool::ControllerVolumeBlendingTool():
    ControllerTool("ToolVolumeBlending")
{
    m_blendingFactor = 50;
}

int ControllerVolumeBlendingTool::getBlendingFactor()
{
    return m_blendingFactor;
}

void ControllerVolumeBlendingTool::setBlendingFactor(int value)
{
    m_blendingFactor = value;

    if(m_renderer3D != nullptr)
        m_renderer3D->setBlendingFactor(value);

    if(m_rendererAxial != nullptr)
        m_rendererAxial->setBlendingFactor(value);

    if(m_rendererCoronal != nullptr)
        m_rendererCoronal->setBlendingFactor(value);

    if(m_rendererSagittal != nullptr)
        m_rendererSagittal->setBlendingFactor(value);

    m_renderingSurfaceSet->update3DRendering();
    m_renderingSurfaceSet->update2DRendering();
}

void ControllerVolumeBlendingTool::setRenderer3D(RegisterRaycastingVolumeRenderer3D *renderer)
{
    m_renderer3D = renderer;
}

void ControllerVolumeBlendingTool::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void ControllerVolumeBlendingTool::setRendererAxial(RegisterSliceRenderer *rendererAxial)
{
    m_rendererAxial = rendererAxial;
}

void ControllerVolumeBlendingTool::setRendererCoronal(RegisterSliceRenderer *rendererCoronal)
{
    m_rendererCoronal = rendererCoronal;
}

void ControllerVolumeBlendingTool::setRendererSagittal(RegisterSliceRenderer *rendererSagittal)
{
    m_rendererSagittal = rendererSagittal;
}
