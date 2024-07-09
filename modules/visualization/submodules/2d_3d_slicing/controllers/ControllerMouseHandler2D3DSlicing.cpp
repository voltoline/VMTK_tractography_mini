#include "ControllerMouseHandler2D3DSlicing.h"

#include "../../3d_clipping/models/Model3DClipping.h"
#include "../../2d_slice/model/Model2DSlice.h"

#include <ui/components/RenderingSurfaceSet.h>

ControllerMouseHandler2D3DSlicing::ControllerMouseHandler2D3DSlicing(View view)
{
    m_view = view;
    m_isSyncActivated = false;

    m_renderingSurfaceSet = nullptr;
    m_model2DSlice = nullptr;
    m_model3dClipping = nullptr;
}

void ControllerMouseHandler2D3DSlicing::mousePressEvent(MouseEvent &/*mouse_event*/, RenderingSurface */*rendering_surface*/)
{
    if(m_model2DSlice != nullptr || m_model3dClipping != nullptr)
    {
        if(isAxial())
        {
            m_model3dClipping->resetClipping();
            m_model3dClipping->setBackClipping(m_model2DSlice->getAxialSlice());

        } else if(isCoronal()) {

            m_model3dClipping->resetClipping();
            m_model3dClipping->setTopClipping(m_model2DSlice->getCoronalSlice());

        } else if(isSagittal()) {

            m_model3dClipping->resetClipping();

            if(m_model2DSlice->isRadiological())
                m_model3dClipping->setLeftClipping(m_model2DSlice->getSagittalSlice());
            else
                m_model3dClipping->setRightClipping(m_model2DSlice->getSagittalSlice());
        }

        m_model3dClipping->notify();
    }
}

void ControllerMouseHandler2D3DSlicing::setModel2DSlice(Model2DSlice *model2DSlice)
{
    m_model2DSlice = model2DSlice;
}

void ControllerMouseHandler2D3DSlicing::setModel3dClipping(Model3DClipping *model3dClipping)
{
    m_model3dClipping = model3dClipping;
}

bool ControllerMouseHandler2D3DSlicing::isAxial()
{
    return m_view == ControllerMouseHandler2D3DSlicing::View::AXIAL;
}

bool ControllerMouseHandler2D3DSlicing::isCoronal()
{
    return m_view == ControllerMouseHandler2D3DSlicing::View::CORONAL;
}

bool ControllerMouseHandler2D3DSlicing::isSagittal()
{
    return m_view == ControllerMouseHandler2D3DSlicing::View::SAGITTAL;
}

void ControllerMouseHandler2D3DSlicing::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
{
    m_renderingSurfaceSet = renderingSurfaceSet;
}

void ControllerMouseHandler2D3DSlicing::setSyncActivated(bool state)
{
    if(m_isSyncActivated == state)
        return;

    m_isSyncActivated = state;

    if(state)
        updateRenderingSurfaceController(this);
    else
        updateRenderingSurfaceController(nullptr);
}

void ControllerMouseHandler2D3DSlicing::updateRenderingSurfaceController(ControllerRenderingSurface *controller)
{
    if(m_renderingSurfaceSet == nullptr)
        return;

    if(controller == nullptr)
    {
        {
            if(isAxial())
            {
                if(m_renderingSurfaceSet->getRenderingSurfaceAxial())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceAxial()->removeController(this);
                }

            } else if(isCoronal()) {
                if(m_renderingSurfaceSet->getRenderingSurfaceCoronal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceCoronal()->removeController(this);
                }

            } else {
                if(m_renderingSurfaceSet->getRenderingSurfaceSagittal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceSagittal()->removeController(this);
                }
            }
        }

    } else {

        {
            if(isAxial())
            {
                if(m_renderingSurfaceSet->getRenderingSurfaceAxial())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceAxial()->addController(this);
                }

            } else if(isCoronal()) {
                if(m_renderingSurfaceSet->getRenderingSurfaceCoronal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceCoronal()->addController(this);
                }

            } else {
                if(m_renderingSurfaceSet->getRenderingSurfaceSagittal())
                {
                    m_renderingSurfaceSet->getRenderingSurfaceSagittal()->addController(this);
                }
            }
        }
    }

}
