#ifndef VIS_SUB_MODULE_TEXT_RENDERER_H
#define VIS_SUB_MODULE_TEXT_RENDERER_H

#include "modules/visualization/VisualizationModule.h"

class TextRenderer2DView;
class Model2DSlice;

template <class vis_module_type>
class VisSubModuleTextRenderer: public VisualizationSubModule
{
public:
    VisSubModuleTextRenderer(std::string vis_sub_module_name = "VisSubModuleTextRenderer");
    ~VisSubModuleTextRenderer() { }

    void init(VisualizationModule *vis_module);

private:
    TextRenderer2DView* create2DViewTextRender();

    vis_module_type *m_module;

    Model2DSlice *m_modelSlice2D;

    TextRenderer2DView *m_rendererAxial;
    TextRenderer2DView *m_rendererCoronal;
    TextRenderer2DView *m_rendererSagittal;
};

#include "VisSubModuleTextRenderer.tpp"

#endif // VIS_SUB_MODULE_TEXT_RENDERER_H
