#include "MainWindow.h"

#include "core/BaseModule.h"

MainWindow::MainWindow():
    m_currentVisualizationTab(nullptr)
{
    m_moduleManager = new ModuleManager(this);
    m_toolsMap = new std::map<std::string, std::vector<ToolBase*> >();
    m_toolTabFreeSpace = new std::map<std::string, UIFreeSpace>();
    m_moduleWithTabVector = new std::vector<BaseModule::sPtr>();
}

MainWindow::~MainWindow()
{
    // delete first
    delete m_moduleManager;
    delete m_moduleWithTabVector;

    // delete after
    delete m_toolsMap;
    delete m_toolTabFreeSpace;
}

void MainWindow::init()
{
    m_moduleManager->init();
}

VisualizationTab *MainWindow::createVisualizationTab(std::string vis_tab_title, BaseModule *module, std::string vis_tab_class_name, bool enable_check_box)
{    
    VisualizationTab *vis_tab = addVisualizationTab(vis_tab_class_name, vis_tab_title, enable_check_box);

    if(vis_tab != nullptr)
    {
        for(unsigned short i = 0; i < m_moduleManager->getSize(); i++)
        {
            auto aux_module = m_moduleManager->getModule(i);
            if(module == aux_module.get())
                m_moduleWithTabVector->push_back(aux_module);
        }

        m_visualizationTabs.push_back(vis_tab);
        short current_tab_index = m_visualizationTabs.size() - 1;

        if(m_visualizationTabs.size() == 1)
            currentVisualizationTabChanged(0);
        else
            setCurrentVisualizationTab(current_tab_index);
    }

    return vis_tab;
}

void MainWindow::currentVisualizationTabChanged(short index)
{
    short num_vis_tab = m_visualizationTabs.size();

    if(num_vis_tab > 0)
    {
        for(short i = 0; i < num_vis_tab; i++)
        {
            if(i == index)
                m_visualizationTabs[i]->setStateCurrentTab(true);
            else
                m_visualizationTabs[i]->setStateCurrentTab(false);
        }

        if(index < num_vis_tab)
        {
            m_currentVisualizationTab = m_visualizationTabs[index];
            for(vmtk::BaseToolTab *tool_tab: m_toolTabs)
                tool_tab->currentVisualizationTabChanged(m_currentVisualizationTab);

            for(auto &tool_data: *m_toolsMap)
                for(auto *tool: tool_data.second)
                    tool->currentVisualizationTabChanged(m_currentVisualizationTab);
        }

        notifyCurrentVisualizationTabChanged(index);
    }
}

void MainWindow::visualizationTabClosed(int index)
{
    std::cout << "vis closed index " << index << std::endl;

    if(index < static_cast<int>(m_moduleWithTabVector->size()))
    {
        m_moduleManager->removeModule((*m_moduleWithTabVector)[index]);
        m_moduleWithTabVector->erase(m_moduleWithTabVector->begin() + index);
    }

    if(index < static_cast<int>(m_visualizationTabs.size()))
    {
        m_visualizationTabs.erase(m_visualizationTabs.begin() + index);
    }
}

void MainWindow::currentToolTabChanged(short index, std::string class_name)
{
    notifyCurrentToolTabChanged(index, class_name);
}

ModuleManager& MainWindow::getModuleManager()
{
    return *m_moduleManager;
}

std::shared_ptr<BaseModule> MainWindow::getModuleWithVisualizationTab(unsigned short vis_index)
{
    return (*m_moduleWithTabVector)[vis_index];
}

unsigned short MainWindow::getNumVisualizationTab()
{
    return m_moduleWithTabVector->size();
}

void MainWindow::handleCloseEvent()
{
    for(auto vis_tab: m_visualizationTabs)
        vis_tab->handleCloseEvent();
}

void MainWindow::notifyTabCheckBoxTooggled(unsigned short tab_index, bool checked)
{
    m_signalTabCheckBoxToggled.emitAllSignals(tab_index, checked);
}

void MainWindow::notifyCurrentVisualizationTabChanged(unsigned short tab_index)
{
    m_signalCurrentVisualizationTabChanged.emitAllSignals(tab_index);
}

void MainWindow::notifyCurrentToolTabChanged(short tab_index, std::string tab_class_name)
{
    m_signalCurrentToolTabChanged.emitAllSignals(tab_index, tab_class_name);
}

std::vector<UIFactoryBase::uPtr> &MainWindow::getUIFactories()
{
    return m_uiFactories;
}

int MainWindow::addTabCheckBoxToggledHandler(std::function<void (unsigned short, bool)> slot)
{
    return m_signalTabCheckBoxToggled.connect(slot);
}

int MainWindow::addCurrentVisualizationTabChangedHandler(std::function<void (unsigned short)> slot)
{
    return m_signalCurrentVisualizationTabChanged.connect(slot);
}

int MainWindow::addCurrentToolTabChangedHandler(std::function< void (short, std::string) > slot)
{
    return m_signalCurrentToolTabChanged.connect(slot);
}

short MainWindow::getVisualizationTabIndexOf(VisualizationTab *vis_tab)
{
    for(unsigned short i = 0; i < m_visualizationTabs.size(); i++)
        if(m_visualizationTabs[i] == vis_tab)
            return i;

    return -1;
}

vmtk::BaseToolTab *MainWindow::getToolTab(std::string tool_tab_class_name)
{
    for(vmtk::BaseToolTab *tool_tab: m_toolTabs)
        if(tool_tab->getToolTabClassName().compare(tool_tab_class_name) == 0)
            return tool_tab;

    vmtk::BaseToolTab *new_tool_tab = addToolTab(tool_tab_class_name, "");

    if(new_tool_tab != nullptr)
    {
        new_tool_tab->currentVisualizationTabChanged(m_currentVisualizationTab);
        m_toolTabs.push_back(new_tool_tab);

    } else
        std::cerr << "ToolTabManager::getToolTab -->> " << tool_tab_class_name << " tool tab not found!" << std::endl;

    return new_tool_tab;
}

bool MainWindow::createToolAsDialogWindow(std::string tool_name, std::string tool_tab_title, std::string button_name)
{
    auto it = m_toolsMap->find(tool_tab_title);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_tab_title];

        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
    } else {
        (*m_toolsMap)[tool_tab_title] = std::vector<ToolBase*>();
        (*m_toolTabFreeSpace)[tool_tab_title] = UIFreeSpace();
    }

    unsigned short row, col;
    if((*m_toolTabFreeSpace)[tool_tab_title].findNextFreeSpace(row, col))
    {
        ToolBase *new_tool = addToolAsDialogWindow(tool_name, tool_tab_title, button_name, row, col, 1, 1);

        if(new_tool != nullptr)
        {
            new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);
            (*m_toolsMap)[tool_tab_title].push_back(new_tool);

        } else
            return false;

        (*m_toolTabFreeSpace)[tool_tab_title].setAllocated(row, col);
        return true;
    }

    std::cerr << "ToolTabManager::createToolAsDialogWindow -->> No UI space available!" << std::endl;
    return false;
}

bool MainWindow::createToolAsDialogWindow(std::string tool_name, std::string tool_tab_title, std::string button_name, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span)
{
    auto it = m_toolsMap->find(tool_tab_title);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_tab_title];

        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
    } else {
        (*m_toolsMap)[tool_tab_title] = std::vector<ToolBase*>();
        (*m_toolTabFreeSpace)[tool_tab_title] = UIFreeSpace();
    }

    if(!(*m_toolTabFreeSpace)[tool_tab_title].isAvalible(row, col, row_span, col_span))
    {
        std::cerr << "MainWindow::createToolAsDialogWindow -->> UI space (" << row << "," << col << ","  << row_span << "," << col_span << ") not available!" << std::endl;

        (*m_toolTabFreeSpace)[tool_tab_title].findNextFreeSpace(row, col);
        row_span = 1;
        col_span = 1;

        std::cerr << "\tUsing next available space (" << row << "," << col << ", 1, 1) !" << std::endl;
    }

    ToolBase *new_tool = addToolAsDialogWindow(tool_name, tool_tab_title, button_name, row, col, row_span, col_span);

    if(new_tool != nullptr)
    {
        new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);
        (*m_toolsMap)[tool_tab_title].push_back(new_tool);

    } else
        return false;

    (*m_toolTabFreeSpace)[tool_tab_title].setAllocated(row, col, row_span, col_span);

    return true;
}

bool MainWindow::createTool(std::string tool_name, std::string tool_tab_title)
{
    auto it = m_toolsMap->find(tool_tab_title);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_tab_title];

        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
    } else {
        (*m_toolsMap)[tool_tab_title] = std::vector<ToolBase*>();
        (*m_toolTabFreeSpace)[tool_tab_title] = UIFreeSpace();
    }

    unsigned short row, col;
    if((*m_toolTabFreeSpace)[tool_tab_title].findNextFreeSpace(row, col))
    {
        ToolBase *new_tool = addTool(tool_name, tool_tab_title, row, col, 1, 1);

        if(new_tool != nullptr)
        {
            new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);
            (*m_toolsMap)[tool_tab_title].push_back(new_tool);

        } else
            return false;

        (*m_toolTabFreeSpace)[tool_tab_title].setAllocated(row, col);

        return true;
    }

    std::cerr << "ToolTabManager::createTool -->> No UI space available!" << std::endl;
    return false;
}

bool MainWindow::createTool(std::string tool_name, std::string tool_tab_title, unsigned short row, unsigned short col, unsigned short row_span, unsigned short col_span)
{
    auto it = m_toolsMap->find(tool_tab_title);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_tab_title];

        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
    } else {
        (*m_toolsMap)[tool_tab_title] = std::vector<ToolBase*>();
        (*m_toolTabFreeSpace)[tool_tab_title] = UIFreeSpace();
    }

    if(!(*m_toolTabFreeSpace)[tool_tab_title].isAvalible(row, col, row_span, col_span))
    {
        std::cerr << "MainWindow::createTool -->> UI space (" << row << ", " << col << ", "  << row_span << ", " << col_span << ") not available!" << std::endl;

        (*m_toolTabFreeSpace)[tool_tab_title].findNextFreeSpace(row, col);

        std::cerr << "\tUsing next available space (" << row << ", " << col << ")!" << std::endl;
    }

    ToolBase *new_tool = addTool(tool_name, tool_tab_title, row, col, row_span, col_span);

    if(new_tool != nullptr)
    {
        new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);
        (*m_toolsMap)[tool_tab_title].push_back(new_tool);

    } else
        return false;

    (*m_toolTabFreeSpace)[tool_tab_title].setAllocated(row, col, row_span, col_span);

    return true;
}

ToolBase *MainWindow::getTool(std::string tool_name, std::string tool_tab_title)
{
    auto it = m_toolsMap->find(tool_tab_title);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_tab_title];

        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return tool;
    }

    std::cerr << "MainWindow::getTool -->> Not found " << tool_name << " in " << tool_tab_title << " tooltab!" << std::endl;

    return nullptr;
}

bool MainWindow::createMenuToolBaseUniqueAddingDialog(std::string mainMenu, std::string subMenuName, std::string tool_name, std::string ui_name)
{
    auto it = m_toolsMap->find(tool_name);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_name];
        for(auto *tool: tool_vec){
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
        }
    } else {
        (*m_toolsMap)[tool_name] = std::vector<ToolBase*>();
    }

    ToolBase *new_tool = addMenuToolbaseUniqueAddingDialog(mainMenu, subMenuName, tool_name, ui_name);

    if(new_tool != nullptr)
    {

        //Código IVAN
        new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);

        (*m_toolsMap)[tool_name].push_back(new_tool);

    } else{
        return false;
    }
    return true;
}


bool MainWindow::createMenuToolBaseUnique(std::string mainMenu, std::string tool_name, std::string ui_name)
{
    auto it = m_toolsMap->find(tool_name);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_name];
        for(auto *tool: tool_vec){
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
        }
    } else {
        (*m_toolsMap)[tool_name] = std::vector<ToolBase*>();
    }

    ToolBase *new_tool = addMenuToolbaseUnique(mainMenu, tool_name, ui_name );

    if(new_tool != nullptr)
    {

        //Código IVAN
        new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);

        (*m_toolsMap)[tool_name].push_back(new_tool);

    } else{
        return false;
    }
    return true;
}

bool MainWindow::createMenuToolBaseforTools(std::string mainMenuName, std::string groupMenuToolName, std::string tool_name, std::string ui_name)
{

    auto it = m_toolsMap->find(tool_name);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_name];
        for(auto *tool: tool_vec){
            if(tool->getToolClassName().compare(tool_name) == 0)
                return true;
        }
    } else {
        (*m_toolsMap)[tool_name] = std::vector<ToolBase*>();
    }

    ToolBase *new_tool = addMenuToolBaseforTools(mainMenuName, groupMenuToolName, tool_name, ui_name );
    if(new_tool != nullptr)
    {
        new_tool->currentVisualizationTabChanged(m_currentVisualizationTab);
        (*m_toolsMap)[tool_name].push_back(new_tool);
    } else{
        return false;
    }
    return true;

}

ToolBase * MainWindow::getToolUniqueWindowFloat(std::string tool_name){
    auto it = m_toolsMap->find(tool_name);
    if (it != m_toolsMap->end())
    {
        std::vector<ToolBase*> tool_vec = (*m_toolsMap)[tool_name];
        for(auto *tool: tool_vec)
            if(tool->getToolClassName().compare(tool_name) == 0)
                return tool;
    }
    std::cerr << "MainWindow::getTool -->> Not found " << tool_name << std::endl;
    return nullptr;
}

void MainWindow::addUIFactory(UIFactoryBase::uPtr ui_factory)
{
    for(auto &f: m_uiFactories){
        if(f->getUIFactoryClassName().compare(ui_factory->getUIFactoryClassName()) == 0){
            break;
        }
    }

    m_uiFactories.push_back(std::move(ui_factory));
}
