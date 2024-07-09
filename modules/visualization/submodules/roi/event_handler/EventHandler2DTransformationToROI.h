#ifndef EVENT_HANDLER_2D_TRANSFORMATION_TO_ROI_H
#define EVENT_HANDLER_2D_TRANSFORMATION_TO_ROI_H

#include <modules/visualization/submodules/2d_transformation/model/Model2DTransformation.h>

class ROIRenderer2D;

class EventHandler2DTransformationToROI: public ModelObserver<Model2DTransformation, Model2DTransformationEvent>
{
public:
    EventHandler2DTransformationToROI();

    void update(Model2DTransformation *subject, Model2DTransformationEvent event_type);

    void setModel2DTransformation(Model2DTransformation *model2DTransformation);
    void setRendererROIAxial(ROIRenderer2D *rendererROIAxial);
    void setRendererROICoronal(ROIRenderer2D *rendererROICoronal);
    void setRendererROISagittal(ROIRenderer2D *rendererROISagittal);

private:
    Model2DTransformation *m_model2DTransformation;

    ROIRenderer2D *m_rendererROIAxial;
    ROIRenderer2D *m_rendererROICoronal;
    ROIRenderer2D *m_rendererROISagittal;
};

#endif // EVENT_HANDLER_2D_TRANSFORMATION_TO_ROI_H
