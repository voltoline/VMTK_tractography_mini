#include "EventHandler3DClipping.h"

#include "modules/visualization/submodules/volume_renderer/model/Model3DRendering.h"

EventHandler3DClipping::EventHandler3DClipping()
{
    m_model3DClipping = nullptr;
    m_model3DRendering = nullptr;
}

void EventHandler3DClipping::update(Model3DClipping *, Model3DClippingEvent event_type)
{
    switch(event_type)
    {

    case Model3DClippingEvent::CLIPPING_CHANGED:
    {
        unsigned short left = m_model3DClipping->getLeftClipping(Model3DClipping::Orientation::LPS);
        unsigned short right = m_model3DClipping->getRightClipping(Model3DClipping::Orientation::LPS);
        unsigned short bottom = m_model3DClipping->getBottomClipping(Model3DClipping::Orientation::LPS);
        unsigned short top = m_model3DClipping->getTopClipping(Model3DClipping::Orientation::LPS);
        unsigned short back = m_model3DClipping->getBackClipping(Model3DClipping::Orientation::LPS);
        unsigned short front = m_model3DClipping->getFrontClipping(Model3DClipping::Orientation::LPS);

        Model3DRenderingEvent event = m_model3DRendering->setProxyGeometryDimFromClippingValue(left, right, bottom, top, back, front);
        m_model3DRendering->notify(event);

        break;
    }

    default:
        break;

    }
}

void EventHandler3DClipping::setModel3DClipping(Model3DClipping *model3DClipping)
{
    m_model3DClipping = model3DClipping;
    m_model3DClipping->subscribe(this, Model3DClippingEvent::CLIPPING_CHANGED);
    m_model3DClipping->subscribe(this, Model3DClippingEvent::CLIPPING_STATE_CHANGED);
}

void EventHandler3DClipping::setModel3DRendering(Model3DRendering *model_3d_rendering)
{
    m_model3DRendering = model_3d_rendering;
}
