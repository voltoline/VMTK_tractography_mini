#ifndef CONTROLLER_MOUSE_HANDLER_2D_3D_SLICING_H
#define CONTROLLER_MOUSE_HANDLER_2D_3D_SLICING_H

#include "ui/RenderingSurface.h"

class Model3DClipping;
class Model2DSlice;
class RenderingSurfaceSet;

class ControllerMouseHandler2D3DSlicing: public ControllerRenderingSurface
{
public:

    enum class View
    {
        AXIAL,
        CORONAL,
        SAGITTAL
    };

    ControllerMouseHandler2D3DSlicing(View view);

    void mousePressEvent(MouseEvent &mouse_event, RenderingSurface *renderingSurface) override;

    void setModel2DSlice(Model2DSlice *model2DSlice);

    void setModel3dClipping(Model3DClipping *model3dClipping);

    void setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet);

    void setSyncActivated(bool state);

private:

    bool isAxial();
    bool isCoronal();
    bool isSagittal();

    void updateRenderingSurfaceController(ControllerRenderingSurface *controller);

    RenderingSurfaceSet *m_renderingSurfaceSet;

    View m_view;

    Model2DSlice *m_model2DSlice;
    Model3DClipping *m_model3dClipping;

    bool m_isSyncActivated;
};

#endif // CONTROLLER_MOUSE_HANDLER_2D_3D_SLICING_H
