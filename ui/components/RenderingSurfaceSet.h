#ifndef RENDERING_SURFACE_SET_H
#define RENDERING_SURFACE_SET_H

#include "ui/RenderingSurface.h"

class RenderingSurfaceSet
{
public:

    enum class Focus
    {
        NONE = 0,
        _3D,
        AXIAL,
        CORONAL,
        SAGITTAL
    };

    Focus mFocus;

    RenderingSurfaceSet();

    void update3DRendering();
    void update2DRendering();
    void updateAxialRendering();
    void updateCoronalRendering();
    void updateSagittalRendering();

    void removeFocus();
    void setFocus3D();
    void setFocusAxial();
    void setFocusCoronal();
    void setFocusSagittal();

    void saveImageToFile();

    RenderingSurface *getRenderingSurface3D() const;
    void setRenderingSurface3D(RenderingSurface *renderingSurface3D);

    RenderingSurface *getRenderingSurfaceAxial() const;
    void setRenderingSurfaceAxial(RenderingSurface *renderingSurfaceAxial);

    RenderingSurface *getRenderingSurfaceCoronal() const;
    void setRenderingSurfaceCoronal(RenderingSurface *renderingSurfaceCoronal);

    RenderingSurface *getRenderingSurfaceSagittal() const;
    void setRenderingSurfaceSagittal(RenderingSurface *renderingSurfaceSagittal);

    Focus getFocus();

private:

    RenderingSurface *mRenderingSurface3D;
    RenderingSurface *mRenderingSurfaceAxial;
    RenderingSurface *mRenderingSurfaceCoronal;
    RenderingSurface *mRenderingSurfaceSagittal;
};

#endif // RENDERING_SURFACE_SET_H
