#include "EventHandler3DRendering.h"

#include "../3d_renderer/BaseRaycastingVolumeRenderer3D.h"

EventHandler3DRendering::EventHandler3DRendering()
{
    m_model3DRendering = nullptr;
    m_raycastingVolumeRenderer3D = nullptr;
    m_renderingSurfaceSet = nullptr;
}

EventHandler3DRendering::~EventHandler3DRendering()
{
    unsubscribeEvents();
}

void EventHandler3DRendering::update(Model3DRendering *, Model3DRenderingEvent event_type)
{
    switch(event_type)
    {

    case Model3DRenderingEvent::PROXY_GEOMETRY_CHANGED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        float left, right, bottom, top, back, front;
        m_model3DRendering->getProxyGeometryDim(left, right, bottom, top, back, front);

        m_raycastingVolumeRenderer3D->setClippingValues(left, right, bottom, top, back, front);

        m_renderingSurfaceSet->update3DRendering();

        break;
    }

    case Model3DRenderingEvent::TRANSFORMATIONS_CHANGED:
    {
        glm::mat4 model_view_matrix = m_model3DRendering->getModelViewMatrix();
        glm::mat4 projection_matrix = m_model3DRendering->getProjectionMatrix();

        glm::mat4 mvp = projection_matrix * model_view_matrix;

        m_raycastingVolumeRenderer3D->setMVPMatrix(mvp);

        m_renderingSurfaceSet->update3DRendering();
        break;
    }

    case Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_raycastingVolumeRenderer3D->highlightAxialSlice(m_model3DRendering->getHighlightedPlaneAxial());
        m_raycastingVolumeRenderer3D->highlightCoronalSlice(m_model3DRendering->getHighlightedPlaneCoronal());
        m_raycastingVolumeRenderer3D->highlightSagittalSlice(m_model3DRendering->getHighlightedPlaneSagittal());

        m_renderingSurfaceSet->update3DRendering();

        break;
    }

    case Model3DRenderingEvent::HIGHLIGHTED_PLANE_STATE_CHAGNED:
    {
        m_renderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

        m_raycastingVolumeRenderer3D->setHighlightPlanesActivated(m_model3DRendering->isHighlightedPlanesActivated());
        m_raycastingVolumeRenderer3D->highlightAxialSlice(m_model3DRendering->getHighlightedPlaneAxial());
        m_raycastingVolumeRenderer3D->highlightCoronalSlice(m_model3DRendering->getHighlightedPlaneCoronal());
        m_raycastingVolumeRenderer3D->highlightSagittalSlice(m_model3DRendering->getHighlightedPlaneSagittal());

        m_renderingSurfaceSet->update3DRendering();

        break;
    }

    default:
        break;

    }
}

void EventHandler3DRendering::setModel3DRendering(Model3DRendering *model3DRendering)
{
    unsubscribeEvents();

    m_model3DRendering = model3DRendering;

    m_model3DRendering->subscribe(this, Model3DRenderingEvent::PROXY_GEOMETRY_CHANGED);
    m_model3DRendering->subscribe(this, Model3DRenderingEvent::TRANSFORMATIONS_CHANGED);
    m_model3DRendering->subscribe(this, Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED);
    m_model3DRendering->subscribe(this, Model3DRenderingEvent::HIGHLIGHTED_PLANE_STATE_CHAGNED);
}

void EventHandler3DRendering::setRaycastingVolumeRenderer3D(BaseRaycastingVolumeRenderer3D *raycastingVolumeRenderer3D)
{
    m_raycastingVolumeRenderer3D = raycastingVolumeRenderer3D;
}

void EventHandler3DRendering::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandler3DRendering::unsubscribeEvents()
{
    if(m_model3DRendering != nullptr)
    {
        m_model3DRendering->unsubscribe(this, Model3DRenderingEvent::PROXY_GEOMETRY_CHANGED);
        m_model3DRendering->unsubscribe(this, Model3DRenderingEvent::TRANSFORMATIONS_CHANGED);
        m_model3DRendering->unsubscribe(this, Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED);
    }
}
