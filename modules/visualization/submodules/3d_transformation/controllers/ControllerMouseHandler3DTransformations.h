#ifndef CONTROLLER_MOUSE_HANDLER_3D_TRANSFORMATIONS_H
#define CONTROLLER_MOUSE_HANDLER_3D_TRANSFORMATIONS_H

#include "ui/RenderingSurface.h"
#include <glm/glm.hpp>
#include "../models/Model3DTransformation.h"

class MouseEvent;
class RenderingSurfaceSet;

class ControllerMouseHandler3DTransformations: public ControllerRenderingSurface
{
public:
    enum class MouseMode
    {
        ROTATION = -2,
        SCALING = -3,
        TRANSLATION = -4
    };

    ControllerMouseHandler3DTransformations();

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseMoveEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);
    void mouseReleaseEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface = nullptr);

    void setModel3DTransformation(Model3DTransformation *model3DTransformation);
    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setMouseMode(const MouseMode &mouseMode);

private:

    void handleMouseMovement();
    void createRotationMatrix();
    void createScalingMatrix();
    void createTranslationMatrix();

    MouseMode m_mouseMode;

    glm::vec2 m_mousePressPosition; /**< mouse position vector */
    glm::vec2 m_mouseCurrentPosition;

    Model3DTransformation *m_model3DTransformation;
    RenderingSurfaceSet *m_renderingSurfaceSet;
};

#endif // CONTROLLER_MOUSE_HANDLER_3D_TRANSFORMATIONS_H
