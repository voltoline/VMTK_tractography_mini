#ifndef TOOLTAB_H
#define TOOLTAB_H

#include "core/BaseToolTab.h"

#include <map>
#include <tuple>
#include <utility>

namespace tooltab_aux
{
    // auxiliar function based on the code of
    //https://stackoverflow.com/questions/16387354/template-tuple-calling-a-function-on-each-element/37100197

    template<int... Is>
    struct seq { };

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

    template<int... Is>
    struct gen_seq<0, Is...> : seq<Is...> { };

    template<typename T, typename F, int... Is>
    void for_each(T&& t, F f, seq<Is...>)
    {
        (void)std::initializer_list<int> { (f(std::get<Is>(t)), void(), 0)... };
    }

    template<typename... Ts, typename F>
    void for_each_in_tuple(std::tuple<Ts...> const& t, F f)
    {
        for_each(t, f, gen_seq<sizeof...(Ts)>());
    }

    //https://stackoverflow.com/questions/26169198/how-to-get-the-index-of-a-type-in-a-variadic-type-pack
    template <typename T, typename... Ts>
    struct type_index;

    template <typename T, typename... Ts>
    struct type_index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

    template <typename T, typename U, typename... Ts>
    struct type_index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + type_index<T, Ts...>::value> {};
}

//based on https://stackoverflow.com/questions/47431565/variadic-template-with-recursive-inheritance-and-using-declaration
template <typename super_class_t, int type_index, typename controller_t, typename... controller_types>
class ToolTabFunctions: public ToolTabFunctions<super_class_t, type_index, controller_t>, public ToolTabFunctions<super_class_t, (type_index + 1), controller_types...>
{
public:
    using ToolTabFunctions<super_class_t, type_index, controller_t>::setController;
    using ToolTabFunctions<super_class_t, (type_index + 1), controller_types...>::setController;

    using ToolTabFunctions<super_class_t, type_index, controller_t>::setFixedController;
    using ToolTabFunctions<super_class_t, (type_index + 1), controller_types...>::setFixedController;

    using ToolTabFunctions<super_class_t, type_index, controller_t>::removeFixedController;
    using ToolTabFunctions<super_class_t, (type_index + 1), controller_types...>::removeFixedController;

protected:
    using ToolTabFunctions<super_class_t, type_index, controller_t>::updateController;
    using ToolTabFunctions<super_class_t, (type_index + 1), controller_types...>::updateController;
};


template <typename super_class_t, int type_index, typename controller_t>
class ToolTabFunctions<super_class_t, type_index, controller_t>
{
public:
    ToolTabFunctions();

    virtual ~ToolTabFunctions() { }

    void setController(VisualizationTab *vis_tab, controller_t *controller/*, bool call_update_components = false*/);
    void setFixedController(/*VisualizationTab *vis_tab, */controller_t *controller/*, bool call_update_components = false*/);
    void removeFixedController(controller_t *controller);

protected:
    virtual void updateController(controller_t *controller) = 0;

private:
    super_class_t *m_superClass;
};


template <typename ...controller_types>
class ToolTab: public vmtk::BaseToolTab, public ToolTabFunctions<ToolTab<controller_types...>, 0, controller_types...>
{
    template <typename, int , typename, typename... > friend class ToolTabFunctions;

public:
    using ToolTabFunctions<ToolTab<controller_types...>, 0, controller_types...>::setController;
    using ToolTabFunctions<ToolTab<controller_types...>, 0, controller_types...>::setFixedController;
    using ToolTabFunctions<ToolTab<controller_types...>, 0, controller_types...>::removeFixedController;

    ToolTab(std::string tool_tab_class_name, std::string tool_tab_title);
    virtual ~ToolTab() { }

    void currentVisualizationTabChanged(VisualizationTab *vis_tab);

private:

    struct UpdateCurrentToolTabControllerFunctor
    {
        ToolTab<controller_types...> &t;

        UpdateCurrentToolTabControllerFunctor(ToolTab<controller_types...> &t_): t(t_) { }

        template<typename T>
        void operator () (T *v)
        {
            const std::size_t type_index = tooltab_aux::type_index<T, controller_types...>::value;
            T *fixed_controller = std::get<type_index>(t.getFixedControllers());

            if(fixed_controller == nullptr)
            {
                if(v != nullptr)
                    t.updateController(v);
                else
                    t.updateController(static_cast<T*>(v));

            } else
                t.updateController(fixed_controller);
        }
    };

    using ToolTabFunctions<ToolTab<controller_types...>, 0, controller_types...>::updateController;

    std::tuple<controller_types* ...>& getControllers(VisualizationTab *vis_tab);
    std::tuple<controller_types* ...>& getFixedControllers();
    VisualizationTab* getCurrentVisualizationTab();

    typedef std::map<VisualizationTab*, std::tuple<controller_types* ...> > VisualizationTabMap;
    VisualizationTabMap m_visualizationTabMap;
    std::tuple<controller_types* ...> m_fixedControllers;
    VisualizationTab *m_currentVisualizationTab;
};

#include "ToolTab.tpp"

#endif // TOOLTAB_H
