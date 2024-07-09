#ifndef CURSOR_H
#define CURSOR_H
#include "modules/visualization/submodules/cursor/VisSubModuleCursor.h"
#include "cursor_global.h"


template<class vis_module_type, class renderer_3D_type, class renderer_2D_type>
class CURSORSHARED_EXPORT cursor : public VisSubModuleCursor<vis_module_type, renderer_3D_type, renderer_2D_type>
{
//    Q_OBJECT
public:
    cursor(std::string vis_sub_module_name = "VisSubModuleCursor")
        : VisSubModuleCursor<vis_module_type, renderer_3D_type, renderer_2D_type>(vis_sub_module_name)
    {
    }

};

#endif // CURSOR_H
