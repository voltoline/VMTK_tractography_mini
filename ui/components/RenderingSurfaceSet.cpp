#include "RenderingSurfaceSet.h"

RenderingSurfaceSet::RenderingSurfaceSet()
{
    mFocus = Focus::NONE;

    mRenderingSurface3D = new NullRenderingSurface();
    mRenderingSurfaceAxial = new NullRenderingSurface();
    mRenderingSurfaceCoronal = new NullRenderingSurface();
    mRenderingSurfaceSagittal = new NullRenderingSurface();
}

void RenderingSurfaceSet::update3DRendering()
{
    mRenderingSurface3D->updateRendering();
}

void RenderingSurfaceSet::update2DRendering()
{
    mRenderingSurfaceAxial->updateRendering();
    mRenderingSurfaceCoronal->updateRendering();
    mRenderingSurfaceSagittal->updateRendering();
}

void RenderingSurfaceSet::updateAxialRendering()
{
    mRenderingSurfaceAxial->updateRendering();
}

void RenderingSurfaceSet::updateCoronalRendering()
{
    mRenderingSurfaceCoronal->updateRendering();
}

void RenderingSurfaceSet::updateSagittalRendering()
{
    mRenderingSurfaceSagittal->updateRendering();
}

void RenderingSurfaceSet::removeFocus()
{
    if(mFocus == Focus::NONE)
        return;

    mFocus = Focus::NONE;

    mRenderingSurface3D->removeFocusBorder();
    mRenderingSurfaceAxial->removeFocusBorder();
    mRenderingSurfaceCoronal->removeFocusBorder();
    mRenderingSurfaceSagittal->removeFocusBorder();
}

void RenderingSurfaceSet::setFocus3D()
{
    if(mFocus == Focus::_3D)
        return;

    mFocus = Focus::_3D;

    mRenderingSurface3D->createFocusBorder();
    mRenderingSurfaceAxial->removeFocusBorder();
    mRenderingSurfaceCoronal->removeFocusBorder();
    mRenderingSurfaceSagittal->removeFocusBorder();
}

void RenderingSurfaceSet::setFocusAxial()
{
    if(mFocus == Focus::AXIAL)
        return;

    mFocus = Focus::AXIAL;

    mRenderingSurface3D->removeFocusBorder();
    mRenderingSurfaceAxial->createFocusBorder();
    mRenderingSurfaceCoronal->removeFocusBorder();
    mRenderingSurfaceSagittal->removeFocusBorder();
}

void RenderingSurfaceSet::setFocusCoronal()
{
    if(mFocus == Focus::CORONAL)
        return;

    mFocus = Focus::CORONAL;

    mRenderingSurface3D->removeFocusBorder();
    mRenderingSurfaceAxial->removeFocusBorder();
    mRenderingSurfaceCoronal->createFocusBorder();
    mRenderingSurfaceSagittal->removeFocusBorder();
}

void RenderingSurfaceSet::setFocusSagittal()
{
    if(mFocus == Focus::SAGITTAL)
        return;

    mFocus = Focus::SAGITTAL;

    mRenderingSurface3D->removeFocusBorder();
    mRenderingSurfaceAxial->removeFocusBorder();
    mRenderingSurfaceCoronal->removeFocusBorder();
    mRenderingSurfaceSagittal->createFocusBorder();
}

void RenderingSurfaceSet::saveImageToFile()
{
    switch(mFocus)
    {

    case Focus::NONE:
    {
        std::cout << "Click on a window to save the image." << std::endl;
        break;
    }

    case Focus::_3D:
    {
        mRenderingSurface3D->saveImage();
        break;
    }

    case Focus::AXIAL:
    {
        mRenderingSurfaceAxial->saveImage();
        break;
    }

    case Focus::CORONAL:
    {
        mRenderingSurfaceCoronal->saveImage();
        break;
    }

    case Focus::SAGITTAL:
    {
        mRenderingSurfaceSagittal->saveImage();
        break;
    }
    }

}

RenderingSurface *RenderingSurfaceSet::getRenderingSurface3D() const
{
    return mRenderingSurface3D;
}

void RenderingSurfaceSet::setRenderingSurface3D(RenderingSurface *renderingSurface3D)
{
    if(mRenderingSurface3D != nullptr)
    {
        std::vector<ControllerRenderingSurface *> controllers = mRenderingSurface3D->getControllers();
        for(ControllerRenderingSurface *controller: controllers)
            renderingSurface3D->addController(controller);
    }

    mRenderingSurface3D = renderingSurface3D;
}

RenderingSurface *RenderingSurfaceSet::getRenderingSurfaceAxial() const
{
    return mRenderingSurfaceAxial;
}

void RenderingSurfaceSet::setRenderingSurfaceAxial(RenderingSurface *renderingSurfaceAxial)
{
    if(mRenderingSurfaceAxial != nullptr)
    {
        std::vector<ControllerRenderingSurface *> controllers = mRenderingSurfaceAxial->getControllers();
        for(ControllerRenderingSurface *controller: controllers)
            renderingSurfaceAxial->addController(controller);
    }

    mRenderingSurfaceAxial = renderingSurfaceAxial;
}

RenderingSurface *RenderingSurfaceSet::getRenderingSurfaceCoronal() const
{
    return mRenderingSurfaceCoronal;
}

void RenderingSurfaceSet::setRenderingSurfaceCoronal(RenderingSurface *renderingSurfaceCoronal)
{
    if(mRenderingSurfaceCoronal != nullptr)
    {
        std::vector<ControllerRenderingSurface *> controllers = mRenderingSurfaceCoronal->getControllers();
        for(ControllerRenderingSurface *controller: controllers)
            renderingSurfaceCoronal->addController(controller);
    }

    mRenderingSurfaceCoronal = renderingSurfaceCoronal;
}

RenderingSurface *RenderingSurfaceSet::getRenderingSurfaceSagittal() const
{
    return mRenderingSurfaceSagittal;
}

void RenderingSurfaceSet::setRenderingSurfaceSagittal(RenderingSurface *renderingSurfaceSagittal)
{
    if(mRenderingSurfaceSagittal != nullptr)
    {
        std::vector<ControllerRenderingSurface *> controllers = mRenderingSurfaceSagittal->getControllers();
        for(ControllerRenderingSurface *controller: controllers)
            renderingSurfaceSagittal->addController(controller);
    }

    mRenderingSurfaceSagittal = renderingSurfaceSagittal;
}

RenderingSurfaceSet::Focus RenderingSurfaceSet::getFocus()
{
    return mFocus;
}
