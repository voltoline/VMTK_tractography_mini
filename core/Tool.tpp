#include "Tool.h"

template<typename super_class_t, int type_index, typename controller_t>
ToolFunctions<super_class_t, type_index, controller_t>::ToolFunctions()
{
    m_superClass = static_cast<super_class_t*>(this);
}

template<typename super_class_t, int type_index, typename controller_t>
void ToolFunctions<super_class_t, type_index, controller_t>::setController(VisualizationTab *vis_tab, controller_t *controller/*, bool call_update_components*/)
{
    std::get<type_index>(m_superClass->getControllers(vis_tab)) = controller;

    if(vis_tab == m_superClass->getCurrentVisualizationTab())
    {
        controller_t *fixed_controller = std::get<type_index>(m_superClass->getFixedControllers());
        if(fixed_controller != nullptr)
            return;

        controller->setTool(m_superClass);
        updateController(controller);
        m_superClass->updateComponents();
    }
}

template<typename super_class_t, int type_index, typename controller_t>
void ToolFunctions<super_class_t, type_index, controller_t>::removeController(VisualizationTab *vis_tab, controller_t *controller)
{
    controller_t *previous_controller = std::get<type_index>(m_superClass->getControllers(vis_tab));

    if(previous_controller == controller)
    {
        std::get<type_index>(m_superClass->getControllers(vis_tab)) = nullptr;

        if(vis_tab == m_superClass->getCurrentVisualizationTab())
        {
            controller_t *fixed_controller = std::get<type_index>(m_superClass->getFixedControllers());
            if(fixed_controller != nullptr)
                return;

            controller->setTool(nullptr);
            updateController(static_cast<controller_t*>(nullptr));
            m_superClass->updateComponents();
        }
    }
}

template<typename super_class_t, int type_index, typename controller_t>
void ToolFunctions<super_class_t, type_index, controller_t>::setFixedController(/*VisualizationTab *vis_tab, */controller_t *controller/*, bool call_update_components*/)
{
    controller_t *previous_controller = std::get<type_index>(m_superClass->getFixedControllers());
    std::get<type_index>(m_superClass->getFixedControllers()) = controller;

    if(previous_controller != nullptr)
        previous_controller->setIsFixed(false);

    if(controller != nullptr)
        controller->setIsFixed(true);
    else
        controller = std::get<type_index>(m_superClass->getControllers(m_superClass->getCurrentVisualizationTab()));

    if(controller != nullptr)
        controller->setTool(m_superClass);

    updateController(controller);
    m_superClass->updateComponents();
}

template<typename super_class_t, int type_index, typename controller_t>
void ToolFunctions<super_class_t, type_index, controller_t>::removeFixedController(controller_t *controller)
{
    controller_t *previous_controller = std::get<type_index>(m_superClass->getFixedControllers());

    if(previous_controller == controller)
    {
        previous_controller->setTool(nullptr);
        previous_controller->setIsFixed(false);

        std::get<type_index>(m_superClass->getFixedControllers()) = nullptr;

        controller = std::get<type_index>(m_superClass->getControllers(m_superClass->getCurrentVisualizationTab()));

        if(controller != nullptr)
            controller->setTool(m_superClass);

        updateController(controller);
        m_superClass->updateComponents();
    }
}

//=================================================================================================================================================================

template<typename ...controller_types>
Tool<controller_types...>::Tool(std::string tool_class_name):
    ToolBase(tool_class_name)
{
    m_fixedControllers = std::make_tuple( (static_cast<controller_types*>(nullptr)) ...);
    m_currentVisualizationTab = nullptr;
}

template<typename ...controller_types>
void Tool<controller_types...>::currentVisualizationTabChanged(VisualizationTab *vis_tab)
{
    auto it = m_visualizationTabMap.find(m_currentVisualizationTab);
    if (it != m_visualizationTabMap.end())
    {
        std::tuple<controller_types* ...> &tuple = m_visualizationTabMap[m_currentVisualizationTab];
        tool_aux::for_each_in_tuple(tuple, DisableToolControllerFunctor(*this));
    }

    m_currentVisualizationTab = vis_tab;

    it = m_visualizationTabMap.find(vis_tab);
    if (it == m_visualizationTabMap.end())
    {
        std::tuple<controller_types* ...> tuple = std::make_tuple((static_cast<controller_types*>(nullptr)) ...);
        tool_aux::for_each_in_tuple(tuple, UpdateCurrentToolControllerFunctor(*this));

    } else {

        std::tuple<controller_types* ...> &tuple = m_visualizationTabMap[vis_tab];
        tool_aux::for_each_in_tuple(tuple, UpdateCurrentToolControllerFunctor(*this));
    }

    updateComponents();
}

template<typename ...controller_types>
std::tuple<controller_types*...> &Tool<controller_types...>::getControllers(VisualizationTab *vis_tab)
{
    auto it = m_visualizationTabMap.find(vis_tab);
    if (it == m_visualizationTabMap.end())
        m_visualizationTabMap[vis_tab] = std::make_tuple((static_cast<controller_types*>(nullptr)) ...);

    return m_visualizationTabMap[vis_tab];
}

template<typename ...controller_types>
std::tuple<controller_types*...> &Tool<controller_types...>::getFixedControllers()
{
    return m_fixedControllers;
}

template<typename ...controller_types>
VisualizationTab *Tool<controller_types...>::getCurrentVisualizationTab()
{
    return m_currentVisualizationTab;
}
