#ifndef CONTROLLER_MOUSE_HANDLER_2D_TRANSFORMATIONS_H
#define CONTROLLER_MOUSE_HANDLER_2D_TRANSFORMATIONS_H

#include <glm/glm.hpp>
#include <functional>
#include "ui/RenderingSurface.h"
#include "../model/Model2DTransformation.h"

class RenderingSurfaceSet;
class Model2DRendering;

class ControllerMouseHandler2DTransformations: public ControllerRenderingSurface
{
public:

    enum class View
    {
        AXIAL,
        CORONAL,
        SAGITTAL
    };

    enum class MouseMode
    {
        NONE,
        ROTATION,
        SCALING,
        TRANSLATION
    };

    ControllerMouseHandler2DTransformations(View view);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);
    void setModel2DTransformation(Model2DTransformation *model2DTransformation);

    void setMouseMode(const MouseMode &mouseMode);

private:
    bool isAxial();
    bool isCoronal();
    bool isSagittal();

    void handleMouseMovement();
    void createRotationMatrix();
    void createScalingMatrix();
    void createTranslationMatrix();

    View m_view;
    MouseMode m_mouseMode;

    glm::vec2 m_mousePressPosition;
    glm::vec2 m_mouseCurrentPosition;

    Model2DTransformation *m_model2DTransformation;
    RenderingSurfaceSet *m_renderingSurfaceSet;

    std::function<void()> m_setFocus;
    std::function<RenderingSurface*()> m_getRenderingSurface;
    std::function<Model2DTransformation::TransformationData&()> m_getTransformationData;
};

#endif // CONTROLLER_MOUSE_HANDLER_2D_TRANSFORMATIONS_H
