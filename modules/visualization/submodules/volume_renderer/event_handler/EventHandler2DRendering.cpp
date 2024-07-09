#include "EventHandler2DRendering.h"

#include "../2d_renderer/BaseSliceRenderer.h"

EventHandler2DRendering::EventHandler2DRendering()
{
    m_axialSliceRenderer = nullptr;
    m_coronalSliceRenderer = nullptr;
    m_sagittalSliceRenderer = nullptr;

    m_model2DRendering = nullptr;
    m_renderingSurfaceSet = nullptr;
}

EventHandler2DRendering::~EventHandler2DRendering()
{
    unsubscribeEvents();
}

void EventHandler2DRendering::update(Model2DRendering *, Model2DRenderingEvent event_type)
{
    switch(event_type)
    {
    case Model2DRenderingEvent::SAGITTAL_SLICE_CHANGED:
    {
        glm::vec3 v1, v2, v3, v4;
        m_model2DRendering->getSagittalData().getPlaneTexCoord(v1, v2, v3, v4);

        if(m_sagittalSliceRenderer)
            m_sagittalSliceRenderer->setPlaneTex(v1, v2, v3, v4);

        m_renderingSurfaceSet->updateSagittalRendering();

        break;
    }

    case Model2DRenderingEvent::CORONAL_SLICE_CHANGED:
    {
        glm::vec3 v1, v2, v3, v4;
        m_model2DRendering->getCoronalData().getPlaneTexCoord(v1, v2, v3, v4);

        if(m_coronalSliceRenderer)
            m_coronalSliceRenderer->setPlaneTex(v1, v2, v3, v4);

        m_renderingSurfaceSet->updateCoronalRendering();

        break;
    }

    case Model2DRenderingEvent::AXIAL_SLICE_CHANGED:
    {
        glm::vec3 v1, v2, v3, v4;
        m_model2DRendering->getAxialData().getPlaneTexCoord(v1, v2, v3, v4);

        if(m_axialSliceRenderer)
            m_axialSliceRenderer->setPlaneTex(v1, v2, v3, v4);

        m_renderingSurfaceSet->updateAxialRendering();

        break;
    }

    case Model2DRenderingEvent::SLICE_PLANE_CHANGED:
    {
        glm::vec3 v1, v2, v3, v4;

        m_model2DRendering->getAxialData().getPlanePos(v1, v2, v3, v4);

        if(m_axialSliceRenderer)
            m_axialSliceRenderer->setPlanePos(v1, v2, v3, v4);

        m_model2DRendering->getCoronalData().getPlanePos(v1, v2, v3, v4);

        if(m_coronalSliceRenderer)
            m_coronalSliceRenderer->setPlanePos(v1, v2, v3, v4);

        m_model2DRendering->getSagittalData().getPlanePos(v1, v2, v3, v4);

        if(m_sagittalSliceRenderer)
            m_sagittalSliceRenderer->setPlanePos(v1, v2, v3, v4);

        m_renderingSurfaceSet->update2DRendering();

        break;
    }

    case Model2DRenderingEvent::SAGITTAL_TRANSFORMATION_CHANGED:
    {
        glm::mat4 mvp = m_model2DRendering->getSagittalData().getProjectionMatrix() * m_model2DRendering->getSagittalData().getModelViewMatrix();

        if(m_sagittalSliceRenderer)
            m_sagittalSliceRenderer->setMVPMatrix(mvp);

        m_renderingSurfaceSet->getRenderingSurfaceSagittal()->updateRendering();
        break;
    }

    case Model2DRenderingEvent::AXIAL_TRANSFORMATION_CHANGED:
    {
        glm::mat4 mvp = m_model2DRendering->getAxialData().getProjectionMatrix() * m_model2DRendering->getAxialData().getModelViewMatrix();

        if(m_axialSliceRenderer)
            m_axialSliceRenderer->setMVPMatrix(mvp);

        m_renderingSurfaceSet->getRenderingSurfaceAxial()->updateRendering();

        break;
    }

    case Model2DRenderingEvent::CORONAL_TRANSFORMATION_CHANGED:
    {
        glm::mat4 mvp = m_model2DRendering->getCoronalData().getProjectionMatrix() * m_model2DRendering->getCoronalData().getModelViewMatrix();

        if(m_coronalSliceRenderer)
            m_coronalSliceRenderer->setMVPMatrix(mvp);

        m_renderingSurfaceSet->getRenderingSurfaceCoronal()->updateRendering();

        break;
    }

    default:
        break;

    }
}

void EventHandler2DRendering::setSagittalSliceRenderer(BaseSliceRenderer *slice_renderer)
{
    m_sagittalSliceRenderer = slice_renderer;
}

void EventHandler2DRendering::setCoronalSliceRenderer(BaseSliceRenderer *coronalSliceRenderer)
{
    m_coronalSliceRenderer = coronalSliceRenderer;
}

void EventHandler2DRendering::setAxialSliceRenderer(BaseSliceRenderer *axialSliceRenderer)
{
    m_axialSliceRenderer = axialSliceRenderer;
}

void EventHandler2DRendering::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void EventHandler2DRendering::unsubscribeEvents()
{
    if(m_model2DRendering != nullptr)
    {
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::SAGITTAL_SLICE_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::CORONAL_SLICE_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::AXIAL_SLICE_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::SLICE_PLANE_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::SAGITTAL_TRANSFORMATION_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::CORONAL_TRANSFORMATION_CHANGED);
        m_model2DRendering->unsubscribe(this, Model2DRenderingEvent::AXIAL_TRANSFORMATION_CHANGED);
    }
}

void EventHandler2DRendering::setModel2DRendering(Model2DRendering *model2DRendering)
{
    unsubscribeEvents();

    m_model2DRendering = model2DRendering;

    m_model2DRendering->subscribe(this, Model2DRenderingEvent::SAGITTAL_SLICE_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::CORONAL_SLICE_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::AXIAL_SLICE_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::SLICE_PLANE_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::SAGITTAL_TRANSFORMATION_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::CORONAL_TRANSFORMATION_CHANGED);
    m_model2DRendering->subscribe(this, Model2DRenderingEvent::AXIAL_TRANSFORMATION_CHANGED);
}
