#ifndef CLIPPING_EVENT_HANDLER_H
#define CLIPPING_EVENT_HANDLER_H

#include "model/ModelObserver.h"
#include "../models/Model3DClipping.h"

class Model3DRendering;

class EventHandler3DClipping: public ModelObserver<Model3DClipping, Model3DClippingEvent>
{
public:
    EventHandler3DClipping();
    ~EventHandler3DClipping() { }

    void update(Model3DClipping *subject, Model3DClippingEvent event_type);

    void setModel3DClipping(Model3DClipping *model3DClipping);
    void setModel3DRendering(Model3DRendering *model_3d_rendering);

private:
    Model3DClipping *m_model3DClipping;
    Model3DRendering *m_model3DRendering;
};

#endif // CLIPPING_EVENT_HANDLER_H
