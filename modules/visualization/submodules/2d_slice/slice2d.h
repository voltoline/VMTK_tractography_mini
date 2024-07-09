#ifndef SLICE2D_H
#define SLICE2D_H
#include "modules/visualization/submodules/2d_slice/VisSubModule2DSlice.h"
#include "slice2d_global.h"


template<class vis_module_type, class renderer_3D_type, class renderer_2D_type>
class SLICE2DSHARED_EXPORT slice2d : public VisSubModule2DSlice<vis_module_type, renderer_3D_type, renderer_2D_type>
{
//    Q_OBJECT
public:
    slice2d(std::string vis_sub_module_name = "VisSubModule2DSlice")
        : VisSubModule2DSlice<vis_module_type, renderer_3D_type, renderer_2D_type>(vis_sub_module_name)
    {
    }

};

#endif // SLICE2D_H
