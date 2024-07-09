#include "Model3DTransformation.h"

#include "utilities/functions/vmtkGeneralFunctions.h"

Model3DTransformation::Model3DTransformation()
{
    m_mouseMode = MouseMode::ROTATION;

    m_isTransformationActivated = true;
    m_isTransformationEnabled = true;

    m_projLeft = m_projBottom = -1.3f;
    m_projRight = m_projTop = 1.3f;

    m_3DTransformationStateChangedHandlerSlotID = -1;
    m_stateManager = nullptr;

    m_preTransformationMatrix = glm::mat4(1.0f);
    m_postTransformationMatrix = glm::mat4(1.0f);

    resetTransformations();
}

Model3DTransformation::~Model3DTransformation()
{
    if(m_stateManager != nullptr)
        m_stateManager->unsubscribe("3D_TRANSFORMATION", m_3DTransformationStateChangedHandlerSlotID);
}

Model3DTransformation::MouseMode Model3DTransformation::getMouseMode() const
{
    return m_mouseMode;
}

void Model3DTransformation::setMouseMode(const MouseMode _3D_mouse_mode)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setMouseMode -->> A notification is in progress!" << std::endl;
    }

    m_mouseMode = _3D_mouse_mode;

    addEvent(Model3DTransformationEvent::MOUSE_MODE_CHANGED);
}

void Model3DTransformation::resetTransformations()
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::resetTransformations -->> A notification is in progress!" << std::endl;
    }

    mScalingMatrix = glm::mat4(1.0f);
    mTranslationMatrix = glm::mat4(1.0f);
    mRotationMatrix = glm::mat4(1.0f);

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

glm::mat4 Model3DTransformation::getScalingMatrix() const
{
    return mScalingMatrix;
}

glm::mat4 Model3DTransformation::getRotationMatrix() const
{
    return mRotationMatrix;
}

glm::mat4 Model3DTransformation::getTranslationMatrix() const
{
    return mTranslationMatrix;
}

glm::mat4 Model3DTransformation::getProjectionMatrix(bool additional_transf) const
{
    return vgf::makeNearProjectionMatrix(getModelViewMatrix(additional_transf), m_projLeft, m_projRight, m_projBottom, m_projTop);;
}

glm::mat4 Model3DTransformation::getMVPMatrix(bool additional_transf) const
{
    return getProjectionMatrix(additional_transf) * getModelViewMatrix(additional_transf);
}

glm::mat4 Model3DTransformation::getModelViewMatrix(bool additional_transf) const
{
    if(additional_transf)
        return m_postTransformationMatrix * mTranslationMatrix * mRotationMatrix * mScalingMatrix * m_preTransformationMatrix;
    else
        return mTranslationMatrix * mRotationMatrix * mScalingMatrix;
}

void Model3DTransformation::setScalingMatrix(glm::mat4 scaling)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setScalingMatrix -->> A notification is in progress!" << std::endl;
    }

    mScalingMatrix = scaling;

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void Model3DTransformation::setRotationMatrix(glm::mat4 rotation)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setRotationMatrix -->> A notification is in progress!" << std::endl;
    }

    mRotationMatrix = rotation;

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void Model3DTransformation::setTranslationMatrix(glm::mat4 translation)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setTranslationMatrix -->> A notification is in progress!" << std::endl;
    }

    mTranslationMatrix = translation;

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

bool Model3DTransformation::isTransformationActivated() const
{
    return m_isTransformationActivated;
}

void Model3DTransformation::setTransformationActivated(bool isTransformationEnabled)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setTransformationActivated -->> A notification is in progress!" << std::endl;
    }

    if(m_isTransformationEnabled)
    {
        if(m_isTransformationActivated != isTransformationEnabled)
        {
            m_isTransformationActivated = isTransformationEnabled;

            if(m_stateManager != nullptr)
            {
                if(isTransformationEnabled)
                    m_stateManager->activateFeature("3D_TRANSFORMATION", m_3DTransformationStateChangedHandlerSlotID);
                else
                    m_stateManager->deactivateFeature("3D_TRANSFORMATION", m_3DTransformationStateChangedHandlerSlotID);
            }

            addEvent(Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
        }
    }
}

void Model3DTransformation::setStateManager(StateManager *state_manager)
{
    if(m_stateManager == state_manager)
        return;

    if(m_stateManager != nullptr)
    {
        m_stateManager->unsubscribe("3D_TRANSFORMATION", m_3DTransformationStateChangedHandlerSlotID);
        m_stateManager->removeFeature("3D_TRANSFORMATION");
    }

    m_stateManager = state_manager;

    if(m_stateManager->addDefaultFeature("3D_TRANSFORMATION"))
    {
        m_3DTransformationStateChangedHandlerSlotID = m_stateManager->subscribe("3D_TRANSFORMATION",  CREATE_SLOT(this, &Model3DTransformation::handle3DTransformationStateChanged));
        m_stateManager->activateFeature("3D_TRANSFORMATION");

    } else {

        m_3DTransformationStateChangedHandlerSlotID = m_stateManager->subscribe("3D_TRANSFORMATION",  CREATE_SLOT(this, &Model3DTransformation::handle3DTransformationStateChanged));
        m_isTransformationActivated = m_stateManager->isActivated("3D_TRANSFORMATION");
    }
}

void Model3DTransformation::handle3DTransformationStateChanged(std::string , FeatureState state)
{
    switch(state)
    {
    case FeatureState::ACTIVATED:
    {
        if(m_isTransformationActivated == false)
        {
            m_isTransformationActivated = true;
            notify(Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::DEACTIVATED:
    {
        if(m_isTransformationActivated == true)
        {
            m_isTransformationActivated = false;
            notify(Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
        }

        break;
    }

    case FeatureState::ENABLED:
        m_isTransformationEnabled = true;
        break;

    case FeatureState::DISABLED:
        m_isTransformationEnabled = false;

        if(m_isTransformationActivated == true)
        {
            m_isTransformationActivated = false;
            notify(Model3DTransformationEvent::_3D_TRANSFORMATION_STATE_CHANGED);
        }

        break;
    }
}

bool Model3DTransformation::isEnabled() const
{
    return m_isTransformationEnabled;
}

glm::mat4 Model3DTransformation::getPreTransformationMatrix() const
{
    return m_preTransformationMatrix;
}

glm::mat4 Model3DTransformation::getPostTransformationMatrix() const
{
    return m_postTransformationMatrix;
}

void Model3DTransformation::setPreTransformationMatrix(const glm::mat4 &preTransformationMatrix)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setPreTransformationMatrix -->> A notification is in progress!" << std::endl;
    }

    m_preTransformationMatrix = preTransformationMatrix;

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}

void Model3DTransformation::setPostTransformationMatrix(const glm::mat4 &postTransformationMatrix)
{
    if(isNotifying())
    {
        std::cerr << "Model3DTransformation::setPostTransformationMatrix -->> A notification is in progress!" << std::endl;
    }

    m_postTransformationMatrix = postTransformationMatrix;

    addEvent(Model3DTransformationEvent::_3D_TRANSFORMATIONS_CHANGED);
}
