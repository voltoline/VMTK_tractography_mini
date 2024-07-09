#include "FreeDrawCursor.h"

FreeDrawCursor::FreeDrawCursor()
{

    m_lines_closed_ref = new FreeLineClosed();
    m_enableFreeDrawCursor = false;
    m_enableMovement = true;

    m_currentActiveFreeLine = 0;
}

void FreeDrawCursor::setShaderProgram(vmtkManagementShader *shaderProgram){
    m_lines_closed_ref->setShaderProgram(shaderProgram);
}

void FreeDrawCursor::setEnableFreeDrawCursor(bool enable)
{
    m_enableFreeDrawCursor = enable;
}


glm::mat4 FreeDrawCursor::getProjectionMatrix()
{
    return m_projectionMatrix;
}

void FreeDrawCursor::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

glm::vec3 FreeDrawCursor::getPos() const
{
    return m_pos;
}

void FreeDrawCursor::setPos(const glm::vec3 &pos)
{
    m_pos = pos;
}

void FreeDrawCursor::setCurrentActiveFreeDrawCursor(int id)
{
    if (m_FreeDrawCursor.size() > 0)
    {
        unsigned int index_id = 0;
        while(m_FreeDrawCursor.at(index_id).id != id)
        {
            index_id++;
            unsigned int size_free = m_FreeDrawCursor.size();
            if (index_id >= size_free)
            {
                std::cerr << "Couldn't find Free Draw Cursor Render Object with id: " << id << " for activation" << std::endl;
                return;
            }
        }
        m_currentActiveFreeLine = index_id;
    }
    else
    {
        std::cerr << "Attempt to Set Inexistent Free Draw Cursor" << std::endl;
    }
}

int FreeDrawCursor::getCurrentActiveDFreeDrawCursor()
{
    return m_currentActiveFreeLine;
}

unsigned int FreeDrawCursor::getSliceCurrentActiveFreeDrawCursor()
{
    return m_FreeDrawCursor.at(m_currentActiveFreeLine).slice;
}

void FreeDrawCursor::setSliceCurrentActiveFreeDrawCursor(int id, unsigned int slice)
{
    if (m_FreeDrawCursor.size() > 0)
    {
        int index_id = 0;
        while(m_FreeDrawCursor.at(index_id).id != id)
        {
            index_id++;
            int size_free = m_FreeDrawCursor.size();
            if (index_id >= size_free)
            {
                std::cerr << "Couldn't find Free Draw Cursor Render Object with id: " << id << " for Slice modification" << std::endl;
                return;
            }
        }
        std::cout << "Set Slice for Active Free Draw with id:" << id << " for slice: " << slice << std::endl;
        m_FreeDrawCursor.at(index_id).slice = slice;
    }
    else
    {
        std::cerr << "Attempt to change Slice on Inexistent Free Draw Cursor" << std::endl;
    }

}

void FreeDrawCursor::addFreeDrawCursor(int id, int tract_id, unsigned int slice, glm::vec4 color)
{
    FreeDrawCursor_attr newFreeDrawCursor;
    newFreeDrawCursor.lines = new FreeLineClosed();
    newFreeDrawCursor.num_lines = 0;
    newFreeDrawCursor.line_points.reserve(2);
    newFreeDrawCursor.id = id;
    newFreeDrawCursor.tract_id = tract_id;
    newFreeDrawCursor.slice = slice;

    newFreeDrawCursor.lines->setShaderProgram(m_lines_closed_ref->getShaderProgram());
    newFreeDrawCursor.color = color;

    m_FreeDrawCursor.push_back(newFreeDrawCursor);

    int index = m_FreeDrawCursor.size() - 1; //get reference to last one
    m_FreeDrawCursor.at(index).lines->initializePrimitive();
}

void FreeDrawCursor::removeFreeDrawCursor(int id)
{
    unsigned int index_id = 0;
    if (m_FreeDrawCursor.size() > 0)
    {
        while(m_FreeDrawCursor.at(index_id).id != id)
        {
            index_id++;
            if (index_id >= m_FreeDrawCursor.size())
            {
                std::cerr << "Couldn't find Free Draw Cursor Render Object with id: " << id << "for removal" << std::endl;
                return;
            }
        }
    }

    if(index_id >= m_FreeDrawCursor.size())
    {
        std::cerr << "Attempt to remove inexistent Free Draw Cursor" << std::endl;
        return;
    }
    m_FreeDrawCursor.erase(m_FreeDrawCursor.begin() + index_id);
    if(index_id == m_currentActiveFreeLine)
        m_currentActiveFreeLine = m_FreeDrawCursor.size() - 1;
}

void FreeDrawCursor::setColorFreeDrawCursor(int id, glm::vec4 color)
{
    if(m_FreeDrawCursor.size() > 0)
    {
        unsigned int index_id = 0;
        while(m_FreeDrawCursor.at(index_id).id != id)
        {
            index_id++;
            if (index_id >= m_FreeDrawCursor.size())
            {
                std::cerr << "Couldn't find Free Draw Cursor Render Object with id: " << id << "for Color Change" << std::endl;
                return;
            }
        }
        m_FreeDrawCursor.at(index_id).lines->setColorForAll(color);
    }
    else
    {
        std::cerr << "Attemp to change color of inexistent Free Draw Cursor" << std::endl;
    }

}

void FreeDrawCursor::initFreeDrawCursor()
{

}

void FreeDrawCursor::addPoint(glm::vec3 pos)
{
    if(m_FreeDrawCursor.at(m_currentActiveFreeLine).line_points.size() != 0)
        m_FreeDrawCursor.at(m_currentActiveFreeLine).lines->removeLastFreeLineClosed();
    m_FreeDrawCursor.at(m_currentActiveFreeLine).line_points.push_back(pos);
    m_FreeDrawCursor.at(m_currentActiveFreeLine).lines->addFreeLineClosed(m_FreeDrawCursor.at(m_currentActiveFreeLine).line_points, m_FreeDrawCursor.at(m_currentActiveFreeLine).color);
    m_FreeDrawCursor.at(m_currentActiveFreeLine).lines->updatingBuffersGeometry();
}

void FreeDrawCursor::drawFreeDrawCursor(unsigned int current_slice, int tract_id)
{
    if(!m_enableFreeDrawCursor)
    {
        return;
    }

    for(unsigned int i = 0; i < m_FreeDrawCursor.size(); i++)
    {
        if(m_FreeDrawCursor.at(i).slice == current_slice && m_FreeDrawCursor.at(i).tract_id == tract_id)
        {
            if(m_FreeDrawCursor.at(i).lines->bindingRender())
            {
                m_FreeDrawCursor.at(i).lines->setIDTextureActivated(0);
                m_FreeDrawCursor.at(i).lines->setGlobalTransformations(m_projectionMatrix, m_modelViewMatrix);
                m_FreeDrawCursor.at(i).lines->geometryRender();
                m_FreeDrawCursor.at(i).lines->releasingRender();
            }
        }

    }
}

void FreeDrawCursor::setModelViewMatrix(glm::mat4 modelViewMatrix) {
    m_modelViewMatrix = modelViewMatrix;
}

glm::mat4 FreeDrawCursor::getModelViewMatrix() {
    return m_modelViewMatrix;
}
