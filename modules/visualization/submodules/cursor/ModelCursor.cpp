#include "ModelCursor.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "ui/vmtkControl.h"
#include "utilities/functions/vmtkGeneralFunctions.h"

ModelCursor::ModelCursor()
{
    m_cursorPosition3D[0] = 0.f;
    m_cursorPosition3D[1] = 0.f;
    m_cursorPosition3D[2] = 0.f;

    m_voxelOrder = VoxelOrder::LAS;

    m_phyScale[0] = 0.f;
    m_phyScale[1] = 0.f;
    m_phyScale[2] = 0.f;

    m_scaleFactors[0] = 0.f;
    m_scaleFactors[1] = 0.f;
    m_scaleFactors[2] = 0.f;
    m_scaleFactors[3] = 0.f;

    m_flagSync2DSlice = true;
    m_isActivated = true;

    m_model2DSlice = nullptr;

    m_stateManager = nullptr;

    m_movableCursorStateChangedHandlerSlotID = -1;
    m_lockedCursorStateChangedHandlerSlotID = -1;

    m_cursorMode = Mode::MOVE;

    m_isEnabled = true;
    mapDrawCursorType.insert(DrawCursorType::crossCursor, "Cross");
    mapDrawCursorType.insert(DrawCursorType::radiusCursor, "Radius");
    mapDrawCursorType.insert(DrawCursorType::lineCursor, "Line");
    mapDrawCursorType.insert(DrawCursorType::invisibleCursor, "Invisible");

    m_drawCursorType = DrawCursorType::crossCursor;

    m_OffScreenEnablePair.first = "";
    m_OffScreenEnablePair.second = false;
}

ModelCursor::~ModelCursor()
{
    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }

    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);
        m_stateManager->unsubscribe("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);
    }
}

void ModelCursor::update(Model2DSlice *, Model2DSliceEvent event_type)
{
    switch(event_type)
    {
    case Model2DSliceEvent::VOXEL_ORDER_CHANGED:
    {
        if(m_model2DSlice->isRadiological()) {
            setRadiologicalVoxelOrder();
        } else {
            setNeurologicalVoxelOrder();
        }
        // Begin Code -> modified by Iván (16:11 18/10/2018)
        // Atualiza a visualização do cursor 2D a partir da posição 3D (posição global no espaço 3D),
        // quando muda a visualização 2D (las ou ras).

        {
            m_sliceData.m_sagittal = m_model2DSlice->getSagittalSlice();

            if(m_voxelOrder == VoxelOrder::RAS)                //Ting (9/12/2019)
                m_cursorPosition3D[0]=(((m_scaleFactors[0] - m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - m_scaleFactors[0]); // R -> L

            else if(m_voxelOrder == VoxelOrder::LAS)
                // to fit to the radiological sagittal view (A<-P)(30/08/2015)
                m_cursorPosition3D[0]=(((m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - (m_scaleFactors[0])); // L

            if(isActivated())
                notifyAll(ModelCursorEvent::CURSOR_POSITION_CHANGED, this, ModelCursorEvent::CURSOR_POSITION_CHANGED);
        }

        break;
        // End Code
    }

    case Model2DSliceEvent::SLICING_CHANGED:
    {
        if(m_sliceData.m_axial != m_model2DSlice->getAxialSlice())
        {
            m_sliceData.m_axial = m_model2DSlice->getAxialSlice();
            m_cursorPosition3D[2]= (((m_model2DSlice->getAxialSlice()*m_phyScale[2])*2)-(m_scaleFactors[2]));  // S
        }

        if(m_sliceData.m_coronal != m_model2DSlice->getCoronalSlice())
        {
            m_sliceData.m_coronal = m_model2DSlice->getCoronalSlice();
            m_cursorPosition3D[1]= (((m_scaleFactors[1]-m_model2DSlice->getCoronalSlice()*m_phyScale[1])*2)-(m_scaleFactors[1]));  // A -> P
        }

        if(m_sliceData.m_sagittal != m_model2DSlice->getSagittalSlice())
        {
            m_sliceData.m_sagittal = m_model2DSlice->getSagittalSlice();

            if(m_voxelOrder == VoxelOrder::RAS)                //Ting (9/12/2019)
                m_cursorPosition3D[0]=(((m_scaleFactors[0] - m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - m_scaleFactors[0]); // R -> L

            else if(m_voxelOrder == VoxelOrder::LAS)
                // to fit to the radiological sagittal view (A<-P)(30/08/2015)
                m_cursorPosition3D[0]=(((m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - (m_scaleFactors[0])); // L
        }

        if(isActivated())
            notifyAll(ModelCursorEvent::CURSOR_POSITION_CHANGED, this, ModelCursorEvent::CURSOR_POSITION_CHANGED);

        break;
    }

    default:
        break;
    }
}

ModelCursorEvent ModelCursor::setCursorPositionFrom3DView(glm::ivec2 mousePos, glm::vec4 viewport, glm::ivec2 screen_size, float depthZ)
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorPositionFrom3DView -->> A notification is in progress!" << std::endl;
    }

    int height = screen_size.y;
    glm::ivec2 win = vgf::orientedMousePosition(mousePos, height);
    if(!vgf::validatePositionOnViewport(viewport, win.x, win.y)){
        return ModelCursorEvent::NONE;
    }

    glm::vec3 pos3D = glm::unProject( glm::vec3(win.x, win.y, depthZ),
                                      m_modelViewMatrix3D, m_projectionMatrix3D, viewport );

    vgf::vec3ToArray(pos3D,m_cursorPosition3D);

    update2DSlices();

    return ModelCursorEvent::CURSOR_POSITION_CHANGED;
}

ModelCursorEvent ModelCursor::setCursorPositionFromAxialView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2])
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorPositionFromAxialView -->> A notification is in progress!" << std::endl;
    }

    m_mousePosX = mouse_pos_x;
    m_mousePosY = mouse_pos_y;

    glm::vec3 vcoord = glm::unProject(glm::vec3(m_mousePosX, screen_size[1] - m_mousePosY, 1),
                                      m_modelViewMatrixAxial,
                                      m_projectionMatrixAxial,
                                      glm::make_vec4(viewport));

    if (m_voxelOrder == VoxelOrder::LAS)
    {
        m_cursorPosition3D[0] = vcoord[0];       // L
    }
    else if (m_voxelOrder == VoxelOrder::RAS)
    {
        m_cursorPosition3D[0] = vcoord[0]*(-1);  // R -> L
    }
    m_cursorPosition3D[1] = vcoord[1]*-1;    // A -> P
    m_cursorPosition3D[2]= (((m_model2DSlice->getAxialSlice()*m_phyScale[2])*2)-(m_scaleFactors[2]));  // S

    update2DSlices();

    return ModelCursorEvent::CURSOR_POSITION_CHANGED;
}

ModelCursorEvent ModelCursor::setCursorPositionFromCoronalView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2])
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorPositionFromCoronalView -->> A notification is in progress!" << std::endl;
    }

    m_mousePosX = mouse_pos_x;
    m_mousePosY = mouse_pos_y;

    glm::vec3 vcoord = glm::unProject(glm::vec3(m_mousePosX, screen_size[1] - m_mousePosY, 1),
                                      m_modelViewMatrixCoronal,
                                      m_projectionMatrixCoronal,
                                      glm::make_vec4(viewport));

    // Coronal is the master
     if (m_voxelOrder == VoxelOrder::LAS)
     {
         m_cursorPosition3D[0] = vcoord[0];         // L
     }
     else if (m_voxelOrder == VoxelOrder::RAS)
     {
         m_cursorPosition3D[0] = vcoord[0]*(-1);   // R -> L
     }
     m_cursorPosition3D[1]= (((m_scaleFactors[1]-m_model2DSlice->getCoronalSlice()*m_phyScale[1])*2)-(m_scaleFactors[1]));  // A -> P
     m_cursorPosition3D[2]= vcoord[1];    // S -> S

     update2DSlices();

    return ModelCursorEvent::CURSOR_POSITION_CHANGED;
}

ModelCursorEvent ModelCursor::setCursorPositionFromSagittalView(int mouse_pos_x, int mouse_pos_y, int viewport[4], int screen_size[2])
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorPositionFromSagittalView -->> A notification is in progress!" << std::endl;
    }

    m_mousePosX = mouse_pos_x;
    m_mousePosY = mouse_pos_y;

    glm::vec3 vcoord = glm::unProject(glm::vec3(m_mousePosX, screen_size[1] - m_mousePosY, 1),
                                      m_modelViewMatrixSagittal,
                                      m_projectionMatrixSagittal,
                                      glm::make_vec4(viewport));

    if(m_voxelOrder == VoxelOrder::RAS)
    {
        m_cursorPosition3D[0]=(((m_scaleFactors[0] - m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - m_scaleFactors[0]); // R -> L
        m_cursorPosition3D[1]= vcoord[0] *(-1) ;  // P -> A -> P->P (Ting, 23/1/2017)

    } else if(m_voxelOrder == VoxelOrder::LAS) {

        // to fit to the radiological sagittal view (A<-P)(30/08/2015)
        m_cursorPosition3D[0]=(((m_model2DSlice->getSagittalSlice() * m_phyScale[0]) * 2) - (m_scaleFactors[0])); // L
        m_cursorPosition3D[1]= vcoord[0];  // P -> P
    }
    m_cursorPosition3D[2]= vcoord[1];       // S -> S

    update2DSlices();

    return ModelCursorEvent::CURSOR_POSITION_CHANGED;
}

void ModelCursor::getCursorPosition3DAsSliceCoordinates(unsigned short vec[])
{
    float sliced = (m_cursorPosition3D[2]+(m_scaleFactors[2]))/(2.0*m_phyScale[2]);   // S
    unsigned short axial = (unsigned short) sliced;

    sliced = ((m_scaleFactors[1])-m_cursorPosition3D[1])/(2.0*m_phyScale[1]);  // P -> A
    unsigned short coronal = (unsigned short) sliced;

    if (m_voxelOrder == VoxelOrder::RAS)
        sliced = ((m_scaleFactors[0])-m_cursorPosition3D[0])/(2.0*m_phyScale[0]); //L -> R
    else
        sliced = ((m_cursorPosition3D[0]+(m_scaleFactors[0]))/(2.0*m_phyScale[0]));  //L

    unsigned short sagittal = (unsigned short) sliced;

    vec[0] = sagittal;
    vec[1] = coronal;
    vec[2] = axial;
}

void ModelCursor::update2DSlices()
{
    if (m_flagSync2DSlice)
    {
        float sliced = (m_cursorPosition3D[2]+(m_scaleFactors[2]))/(2.0*m_phyScale[2]);   // S
        Model2DSliceEvent event = m_model2DSlice->setAxialSlice((unsigned short)sliced);

        sliced = ((m_scaleFactors[1])-m_cursorPosition3D[1])/(2.0*m_phyScale[1]);  // P -> A
        Model2DSliceEvent aux = m_model2DSlice->setCoronalSlice((unsigned short)sliced);

        if(aux != Model2DSliceEvent::NONE)
            event = aux;

        if (m_voxelOrder == VoxelOrder::RAS)
            sliced = ((m_scaleFactors[0])-m_cursorPosition3D[0])/(2.0*m_phyScale[0]); //L -> R
        else
            sliced = ((m_cursorPosition3D[0]+(m_scaleFactors[0]))/(2.0*m_phyScale[0]));  //L
        aux = m_model2DSlice->setSagittalSlice((unsigned short)sliced);

        if(aux != Model2DSliceEvent::NONE)
            event = aux;

        m_model2DSlice->notify(event, this);
    }
}


QMap<DrawCursorType, QString> ModelCursor::getMapDrawCursorType() const
{
    return mapDrawCursorType;
}

DrawCursorType ModelCursor::getDrawCursorType() const
{
    return m_drawCursorType;
}

ModelCursorEvent ModelCursor::setDrawCursorType(DrawCursorType newDrawCursorType)
{
    if(isNotifying()){
//        std::cerr << "ModelCursor::setDrawCursorType -->> A notification is in progress!" << std::endl;
    }

    m_drawCursorType = newDrawCursorType;
    return  ModelCursorEvent::CURSOR_DRAW_TYPE_CHANGED;
}

ModelCursorEvent ModelCursor::setOffScreenEnable(QString name, bool enable){
    if(isNotifying()){
//        std::cerr << "ModelCursor::setOffScreenEnable -->> A notification is in progress!" << std::endl;
    }
    m_OffScreenEnablePair.first = name;
    m_OffScreenEnablePair.second = enable;
    return  ModelCursorEvent:: CURSOR_OFFSCREEN_CHANGED;
}

const QPair<QString, bool> &ModelCursor::getOffScreenEnablePair() const{
    return m_OffScreenEnablePair;
}

void ModelCursor::setProjectionMatrixSagittal(const glm::mat4 &projectionMatrixSagittal)
{
    m_projectionMatrixSagittal = projectionMatrixSagittal;
}

void ModelCursor::setModelViewMatrixSagittal(const glm::mat4 &modelViewMatrixSagittal)
{
    m_modelViewMatrixSagittal = modelViewMatrixSagittal;
}

void ModelCursor::setProjectionMatrixCoronal(const glm::mat4 &projectionMatrixCoronal)
{
    m_projectionMatrixCoronal = projectionMatrixCoronal;
}

void ModelCursor::setModelViewMatrixCoronal(const glm::mat4 &modelViewMatrixCoronal)
{
    m_modelViewMatrixCoronal = modelViewMatrixCoronal;
}

void ModelCursor::setProjectionMatrixAxial(const glm::mat4 &projectionMatrixAxial)
{
    m_projectionMatrixAxial = projectionMatrixAxial;
}

void ModelCursor::setModelViewMatrixAxial(const glm::mat4 &modelViewMatrixAxial)
{
    m_modelViewMatrixAxial = modelViewMatrixAxial;
}

void ModelCursor::setProjectionMatrix3D(const glm::mat4 &projectionMatrix3D)
{
    m_projectionMatrix3D = projectionMatrix3D;
}

void ModelCursor::setModelViewMatrix3D(const glm::mat4 &modelViewMatrix3D)
{
    m_modelViewMatrix3D = modelViewMatrix3D;
}

bool ModelCursor::isEnabled() const
{
    return m_isEnabled;
}

ModelCursor::Mode ModelCursor::getCursorMode() const
{
    return m_cursorMode;
}

ModelCursorEvent ModelCursor::setCursorMode(const Mode &cursorMode)
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorMode -->> A notification is in progress!" << std::endl;
    }

    if(m_cursorMode == cursorMode)
        return ModelCursorEvent::NONE;

    m_cursorMode = cursorMode;

    if(m_isActivated)
    {
        if(isMovable())
        {
            m_stateManager->deactivateFeature("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);
            m_stateManager->activateFeature("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);

        } else {

            m_stateManager->deactivateFeature("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);
            m_stateManager->activateFeature("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);
        }

    }

    return ModelCursorEvent::CURSOR_MODE_CHANGED;
}

ModelCursorEvent ModelCursor::setCursorPosition3D(glm::vec3 cursor_position)
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setCursorPosition3D -->> A notification is in progress!" << std::endl;
    }

    m_cursorPosition3D[0] = cursor_position[0];
    m_cursorPosition3D[1] = cursor_position[1];
    m_cursorPosition3D[2] = cursor_position[2];

    return ModelCursorEvent::CURSOR_POSITION_CHANGED;
}

void ModelCursor::setFlagSync2DSlice(bool flagSync2DSlice)
{
    m_flagSync2DSlice = flagSync2DSlice;
}
glm::vec3 ModelCursor::getCursorPosition3D()
{
    glm::vec3 cursor_pos;
    cursor_pos.x = m_cursorPosition3D[0];
    cursor_pos.y = m_cursorPosition3D[1];
    cursor_pos.z = m_cursorPosition3D[2];
    return cursor_pos;
}

glm::vec3 ModelCursor::getCursorPositionAxial()
{

    // Axial slice
    glm::vec3 cursor_pos;

    cursor_pos.x = m_cursorPosition3D[0];  // L
    if (m_voxelOrder == VoxelOrder::RAS) {
        cursor_pos.x *= -1;
    }
    cursor_pos.y = m_cursorPosition3D[1]*(-1);  //P -> A
    cursor_pos.z = m_cursorPosition3D[2];

    return cursor_pos;
}

glm::vec3 ModelCursor::getCursorPositionCoronal()
{
    // Coronal slice
    glm::vec3 cursor_pos;

    cursor_pos.x = m_cursorPosition3D[0];  // L
    if (m_voxelOrder == VoxelOrder::RAS)
    {
        cursor_pos.x *= (-1);   // L -> R
    }
    cursor_pos.y = m_cursorPosition3D[2];  // S -> S
    cursor_pos.z = m_cursorPosition3D[1];

    return cursor_pos;
}

glm::vec3 ModelCursor::getCursorPositionSagittal()
{
    glm::vec3 cursor_pos;
    // Sagittal slice
    // to fit to the radiological sagittal view (A<-P)(30/08/2015)
    cursor_pos.x = m_cursorPosition3D[1]; //P
    if (m_voxelOrder == VoxelOrder::RAS)
    {
        cursor_pos.x = m_cursorPosition3D[1]*(-1); //P -> A
    }

    cursor_pos.y = m_cursorPosition3D[2];      // S
    cursor_pos.z = m_cursorPosition3D[0];

    return cursor_pos;
}

void ModelCursor::setRadiologicalVoxelOrder()
{
    m_voxelOrder = VoxelOrder::LAS;
}

void ModelCursor::setNeurologicalVoxelOrder()
{
    m_voxelOrder = VoxelOrder::RAS;
}

void ModelCursor::setModel2DSlice(Model2DSlice *model2DSlice)
{
    if(m_model2DSlice == model2DSlice)
        return;

    if(m_model2DSlice != nullptr)
    {
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
        m_model2DSlice->unsubscribe(this, Model2DSliceEvent::SLICING_CHANGED);
    }

    m_model2DSlice = model2DSlice;
    m_model2DSlice->subscribe(this, Model2DSliceEvent::VOXEL_ORDER_CHANGED);
    m_model2DSlice->subscribe(this, Model2DSliceEvent::SLICING_CHANGED);

    m_sliceData.m_axial = m_model2DSlice->getAxialSlice();
    m_sliceData.m_coronal = m_model2DSlice->getCoronalSlice();
    m_sliceData.m_sagittal = m_model2DSlice->getSagittalSlice();
}

void ModelCursor::setPhyScale(const float phy_scale[3])
{
    m_phyScale[0] = phy_scale[0];
    m_phyScale[1] = phy_scale[1];
    m_phyScale[2] = phy_scale[2];
}

void ModelCursor::setScaleFactors(const float scale_factors[3])
{
    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];
}

bool ModelCursor::isActivated() const
{
    return m_isActivated;
}

ModelCursorEvent ModelCursor::setActivated(bool state)
{
    if(isNotifying())
    {
        std::cerr << "ModelCursor::setActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isActivated == state)
        return ModelCursorEvent::NONE;

    m_isActivated = state;

    if(m_stateManager != nullptr)
    {
        if(m_isActivated)
        {
            if(isMovable())
                m_stateManager->activateFeature("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);
            else
                m_stateManager->activateFeature("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);

        } else {

            if(isMovable())
                m_stateManager->deactivateFeature("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);
            else
                m_stateManager->deactivateFeature("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);
        }

    }

    return ModelCursorEvent::CURSOR_STATE_CHANGED;
}

bool ModelCursor::isLocked() const
{
    return (m_cursorMode == ModelCursor::Mode::LOCK);
}

bool ModelCursor::isMovable() const
{
    return (m_cursorMode == ModelCursor::Mode::MOVE);
}

void ModelCursor::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("MOVABLE_CURSOR", m_movableCursorStateChangedHandlerSlotID);
        m_stateManager->removeFeature("MOVABLE_CURSOR");

        m_stateManager->unsubscribe("LOCKED_CURSOR", m_lockedCursorStateChangedHandlerSlotID);
        m_stateManager->removeFeature("LOCKED_CURSOR");
    }

    m_stateManager = state_manager;

    if(!m_stateManager->isAdded("MOVABLE_CURSOR"))
    {
        m_stateManager->addDefaultFeature("MOVABLE_CURSOR");
        m_movableCursorStateChangedHandlerSlotID = m_stateManager->subscribe("MOVABLE_CURSOR", CREATE_SLOT(this, &ModelCursor::handleCursorStateChanged));
        m_stateManager->activateFeature("MOVABLE_CURSOR");

    } else {

        m_movableCursorStateChangedHandlerSlotID = m_stateManager->subscribe("MOVABLE_CURSOR", CREATE_SLOT(this, &ModelCursor::handleCursorStateChanged));
        m_isActivated = m_stateManager->isActivated("MOVABLE_CURSOR");
    }

    if(!m_stateManager->isAdded("LOCKED_CURSOR"))
    {
        m_stateManager->addStateWithRestrictions("LOCKED_CURSOR", std::vector<std::string>{});
        m_lockedCursorStateChangedHandlerSlotID = m_stateManager->subscribe("LOCKED_CURSOR", CREATE_SLOT(this, &ModelCursor::handleCursorStateChanged));

    } else {

        m_lockedCursorStateChangedHandlerSlotID = m_stateManager->subscribe("LOCKED_CURSOR", CREATE_SLOT(this, &ModelCursor::handleCursorStateChanged));
        m_isActivated = m_stateManager->isActivated("LOCKED_CURSOR");
    }

}

void ModelCursor::handleCursorStateChanged(std::string feature_name, FeatureState state)
{
    bool state_changed = false;
    bool mode_changed = false;

    switch(state)
    {
    case FeatureState::ACTIVATED:
    {
        if(m_isActivated == false)
        {
            m_isActivated = true;
            state_changed = true;
        }

        break;
    }

    case FeatureState::DEACTIVATED:
    {
        if(m_isActivated == true)
        {
            m_isActivated = false;
            state_changed = true;

            if(feature_name.compare("LOCKED_CURSOR") == 0) {
                m_isActivated = true;
                m_cursorMode = Mode::MOVE;
                mode_changed = true;

            }
        }

        break;
    }

    case FeatureState::ENABLED:
    {
        if(m_isEnabled == false)
        {
            m_isEnabled = true;
            state_changed = true;
        }

        break;
    }

    case FeatureState::DISABLED:
    {
        if(m_isEnabled == true)
        {
            m_isEnabled = false;
            state_changed = true;
        }

        break;
    }

    }

    if(feature_name.compare("MOVABLE_CURSOR") == 0)
    {
        if(state == FeatureState::ACTIVATED && m_cursorMode != Mode::MOVE)
        {
            m_cursorMode = Mode::MOVE;
            mode_changed = true;
        }

    } else if(feature_name.compare("LOCKED_CURSOR") == 0) {

        if(state == FeatureState::ACTIVATED && m_cursorMode != Mode::LOCK)
        {
            m_cursorMode = Mode::LOCK;
            mode_changed = true;
        }
    }

    if(state_changed)
        notifyAll(ModelCursorEvent::CURSOR_STATE_CHANGED, this, ModelCursorEvent::CURSOR_STATE_CHANGED);

    if(mode_changed)
        notifyAll(ModelCursorEvent::CURSOR_MODE_CHANGED, this, ModelCursorEvent::CURSOR_MODE_CHANGED);
}
