#include "TextRenderer2DView.h"
#include "VisSubModuleTextRenderer.h"

#include "TextRenderer3DView.h"
#include "event_handlers/EventHandlerSlice2DToText.h"
#include <modules/visualization/submodules/2d_3d_slicing/VisSubModule2D3DSlicing.h>

template<class vis_module_type>
VisSubModuleTextRenderer<vis_module_type>::VisSubModuleTextRenderer(std::string vis_sub_module_name):
    VisualizationSubModule(vis_sub_module_name)
{

}

template<class vis_module_type>
void VisSubModuleTextRenderer<vis_module_type>::init(VisualizationModule *vis_module)
{
    m_module = static_cast<vis_module_type*>(vis_module);
    auto *acquisition = m_module->getAcquisition();

    m_modelSlice2D = static_cast<VisSubModule2D3DSlicing<vis_module_type>*>(vis_module->getVisSubModule("VisSubModule2D3DSlicing"))->getModel2DSlice();

    if(vis_module->getRenderer3D() != nullptr)
    {
        TextRenderer3DView *text_renderer_3d_view = new TextRenderer3DView();
        vis_module->getRenderer3D()->addSubRenderer(text_renderer_3d_view);
    }

    if(vis_module->getRendererAxial() != nullptr)
    {
        unsigned short min, max, slice;
        m_modelSlice2D->getAxialInterval(min, max);
        slice = m_modelSlice2D->getAxialSlice();

        m_rendererAxial = create2DViewTextRender();
        m_rendererAxial->setAxis(TextRenderer2DView::TAxis::VMTK_Z_AXIS);
        m_rendererAxial->setMaxSlice(max);
        m_rendererAxial->setPixelSpacing(acquisition->getPixelSpacing(2));
        m_rendererAxial->setCurrentSlice(slice);

        if(m_modelSlice2D->isRadiological())
            m_rendererAxial->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_LAS);
        else
            m_rendererAxial->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_RAS);

        vis_module->getRendererAxial()->addSubRenderer(m_rendererAxial);
    }

    if(vis_module->getRendererCoronal() != nullptr)
    {
        unsigned short min, max, slice;
        m_modelSlice2D->getCoronalInterval(min, max);
        slice = m_modelSlice2D->getCoronalSlice();

        m_rendererCoronal = create2DViewTextRender();
        m_rendererCoronal->setAxis(TextRenderer2DView::TAxis::VMTK_Y_AXIS);
        m_rendererCoronal->setMaxSlice(max);
        m_rendererCoronal->setPixelSpacing(acquisition->getPixelSpacing(1));
        m_rendererCoronal->setCurrentSlice(slice);

        if(m_modelSlice2D->isRadiological())
            m_rendererCoronal->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_LAS);
        else
            m_rendererCoronal->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_RAS);

        vis_module->getRendererCoronal()->addSubRenderer(m_rendererCoronal);
    }

    if(vis_module->getRendererSagittal() != nullptr)
    {
        unsigned short min, max, slice;
        m_modelSlice2D->getSagittalInterval(min, max);
        slice = m_modelSlice2D->getSagittalSlice();

        m_rendererSagittal = create2DViewTextRender();
        m_rendererSagittal->setAxis(TextRenderer2DView::TAxis::VMTK_X_AXIS);
        m_rendererSagittal->setMaxSlice(max);
        m_rendererSagittal->setPixelSpacing(acquisition->getPixelSpacing(0));
        m_rendererSagittal->setCurrentSlice(slice);

        if(m_modelSlice2D->isRadiological())
            m_rendererSagittal->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_LAS);
        else
            m_rendererSagittal->setVoxelOrder(TextRenderer2DView::TVoxelOrder::VMTK_RAS);

        vis_module->getRendererSagittal()->addSubRenderer(m_rendererSagittal);
    }

    EventHandlerSlice2DToText *event_handler_slice = new EventHandlerSlice2DToText();
    event_handler_slice->setModel2DSlice(m_modelSlice2D);
    event_handler_slice->setRendererAxial(m_rendererAxial);
    event_handler_slice->setRendererCoronal(m_rendererCoronal);
    event_handler_slice->setRendererSagittal(m_rendererSagittal);
}

template<class vis_module_type>
TextRenderer2DView *VisSubModuleTextRenderer<vis_module_type>::create2DViewTextRender()
{
    auto *acquisition = m_module->getAcquisition();

    TextRenderer2DView *text_renderer_2d_view = new TextRenderer2DView();
    text_renderer_2d_view->setInstitutionName(acquisition->getInstitutionName());
    text_renderer_2d_view->setManufacturesModel(acquisition->getManufacturesModel());
    text_renderer_2d_view->setSerieDescription(acquisition->getSeriesDescription());
    text_renderer_2d_view->setPatientID(acquisition->getPatientID());
    text_renderer_2d_view->setPatientBirthDate(acquisition->getPatientBirthDate());
    text_renderer_2d_view->setStudyDescription(acquisition->getStudyDescription());

    return text_renderer_2d_view;
}
