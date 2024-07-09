#include "EventHandler2DSlice.h"

#include "../CommonObjects2DSlice.h"

EventHandler2DSlice::EventHandler2DSlice(CommonObjects2DSlice *common_objects)
{
    m_model2DSlice = common_objects->getModel2DSlice();
    m_model3DClipping = common_objects->getModel3DClipping();
    m_model3DRendering = common_objects->getModel3DRendering();
    m_model2DRendering = common_objects->getModel2DRendering();
    m_renderingSurfaceSet = common_objects->getRenderingSurfaceSet();

    m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);

    m_sliceData.m_axial = m_model2DSlice->getAxialSlice();
    m_sliceData.m_coronal = m_model2DSlice->getCoronalSlice();
    m_sliceData.m_sagittal = m_model2DSlice->getSagittalSlice();
}

EventHandler2DSlice::~EventHandler2DSlice()
{
    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }
}

void EventHandler2DSlice::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    switch(event_type)
    {

    case Model2DSliceEvent::SLICING_CHANGED:
    {
        unsigned short axial = m_model2DSlice->getAxialSlice();
        unsigned short coronal = m_model2DSlice->getCoronalSlice();
        unsigned short sagittal = m_model2DSlice->getSagittalSlice();


        if(m_sliceData.m_axial != axial)
        {
            m_sliceData.m_axial = axial;

            glm::vec3 v1, v2, v3, v4;
            m_model2DSlice->getAxialTexCoord(v1, v2, v3, v4);

            Model2DRenderingEvent event = m_model2DRendering->getAxialData().setPlaneTexCoord(v1, v2, v3, v4);
            m_model2DRendering->notify(event);

            Model3DRenderingEvent event_3d = m_model3DRendering->setHighlightedPlaneAxial(m_model2DSlice->getAxialSlice(Model2DSlice::VoxelOrder::LPS));
            if(m_model3DRendering->isHighlightedPlanesActivated())
                m_model3DRendering->notify(event_3d);
        }

        if(m_sliceData.m_coronal != coronal) {

            m_sliceData.m_coronal = coronal;

            glm::vec3 v1, v2, v3, v4;
            m_model2DSlice->getCoronalTexCoord(v1, v2, v3, v4);

            Model2DRenderingEvent event = m_model2DRendering->getCoronalData().setPlaneTexCoord(v1, v2, v3, v4);
            m_model2DRendering->notify(event);

            Model3DRenderingEvent event_3d = m_model3DRendering->setHighlightedPlaneCoronal(m_model2DSlice->getCoronalSlice(Model2DSlice::VoxelOrder::LPS));
            if(m_model3DRendering->isHighlightedPlanesActivated())
                m_model3DRendering->notify(event_3d);
        }

        if(m_sliceData.m_sagittal != sagittal) {

            m_sliceData.m_sagittal = sagittal;

            glm::vec3 v1, v2, v3, v4;
            m_model2DSlice->getSagittalTexCoord(v1, v2, v3, v4);

            Model2DRenderingEvent event = m_model2DRendering->getSagittalData().setPlaneTexCoord(v1, v2, v3, v4);
            m_model2DRendering->notify(event);

            Model3DRenderingEvent event_3d = m_model3DRendering->setHighlightedPlaneSagittal(m_model2DSlice->getSagittalSlice(Model2DSlice::VoxelOrder::LPS));
            if(m_model3DRendering->isHighlightedPlanesActivated())
                m_model3DRendering->notify(event_3d);
        }

        break;
    }

    case Model2DSliceEvent::VOXEL_ORDER_CHANGED:
    {
        if(m_model3DClipping != nullptr)
        {
            if(m_model2DSlice->isRadiological())
            {
                m_model3DClipping->setOrientation(Model3DClipping::Orientation::LAS);
                m_model3DClipping->notify();

            } else {

                m_model3DClipping->setOrientation(Model3DClipping::Orientation::RAS);
                m_model3DClipping->notify();
            }
        }

        glm::vec3 v1, v2, v3, v4;
        m_model2DSlice->getAxialTexCoord(v1, v2, v3, v4);

        Model2DRenderingEvent event = m_model2DRendering->getAxialData().setPlaneTexCoord(v1, v2, v3, v4);
        m_model2DRendering->notify(event);

        m_model2DSlice->getCoronalTexCoord(v1, v2, v3, v4);

        event = m_model2DRendering->getCoronalData().setPlaneTexCoord(v1, v2, v3, v4);
        m_model2DRendering->notify(event);

        m_model2DSlice->getSagittalTexCoord(v1, v2, v3, v4);

        event = m_model2DRendering->getSagittalData().setPlaneTexCoord(v1, v2, v3, v4);
        m_model2DRendering->notify(event);

        break;
    }

    default:
        break;
    }
}
