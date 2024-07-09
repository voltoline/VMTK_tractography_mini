#include "Model2DTransformation.h"

#include "utilities/functions/vmtkGeneralFunctions.h"

Model2DTransformation::Model2DTransformation():
    m_axialData(Model2DTransformationEvent::AXIAL_TRANSFORMATIONS_CHANGED, this),
    m_coronalData(Model2DTransformationEvent::CORONAL_TRANSFORMATIONS_CHANGED, this),
    m_sagittalData(Model2DTransformationEvent::SAGITTAL_TRANSFORMATIONS_CHANGED, this)
{
    m_mouseMode = MouseMode::ROTATION;

    m_axialData.resetTransformations();
    m_coronalData.resetTransformations();
    m_sagittalData.resetTransformations();
}

Model2DTransformation::~Model2DTransformation()
{

}

Model2DTransformation::MouseMode Model2DTransformation::getMouseMode() const
{
    return m_mouseMode;
}

Model2DTransformationEvent Model2DTransformation::setMouseMode(const MouseMode _2D_mouse_mode)
{
    if(isNotifying())
    {
        std::cerr << "Model2DTransformation::setMouseMode -->> A notification is in progress!" << std::endl;
    }

    if(m_mouseMode == _2D_mouse_mode)
        return Model2DTransformationEvent::NONE;

    m_mouseMode = _2D_mouse_mode;

    return Model2DTransformationEvent::MOUSE_MODE_CHANGED;
}

Model2DTransformation::TransformationData& Model2DTransformation::getAxialData()
{
    return m_axialData;
}

Model2DTransformation::TransformationData& Model2DTransformation::getCoronalData()
{
    return m_coronalData;
}

Model2DTransformation::TransformationData& Model2DTransformation::getSagittalData()
{
    return m_sagittalData;
}

Model2DTransformation::TransformationData::TransformationData(Model2DTransformationEvent event, Model2DTransformation *model_2d)
{
    m_event = event;
    m_model2d = model_2d;
}

glm::mat4 Model2DTransformation::TransformationData::getRotationMatrix() const
{
    return m_rotationMatrix;
}

Model2DTransformationEvent Model2DTransformation::TransformationData::setRotationMatrix(const glm::mat4 &rotationMatrix)
{
    if(m_model2d->isNotifying())
    {
        std::cerr << "Model2DTransformation::TransformationData::setRotationMatrix -->> A notification is in progress!" << std::endl;
    }
    m_rotationMatrix = rotationMatrix;
    updateProjection();
    return m_event;
}

glm::mat4 Model2DTransformation::TransformationData::getScalingMatrix() const
{
    return m_scalingMatrix;
}

Model2DTransformationEvent Model2DTransformation::TransformationData::setScalingMatrix(const glm::mat4 &scalingMatrix)
{
    if(m_model2d->isNotifying())
    {
        std::cerr << "Model2DTransformation::TransformationData::setScalingMatrix -->> A notification is in progress!" << std::endl;
    }
    m_scalingMatrix = scalingMatrix;
    updateProjection();

    return m_event;
}

glm::mat4 Model2DTransformation::TransformationData::getTranslationMatrix() const
{
    return m_translationMatrix;
}

Model2DTransformationEvent Model2DTransformation::TransformationData::setTranslationMatrix(const glm::mat4 &translationMatrix)
{
    if(m_model2d->isNotifying())
    {
        std::cerr << "Model2DTransformation::TransformationData::setTranslationMatrix -->> A notification is in progress!" << std::endl;
    }
    m_translationMatrix = translationMatrix;
    updateProjection();

    return m_event;
}

glm::mat4 Model2DTransformation::TransformationData::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

Model2DTransformationEvent Model2DTransformation::TransformationData::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    if(m_model2d->isNotifying())
    {
        std::cerr << "Model2DTransformation::TransformationData::setProjectionMatrix -->> A notification is in progress!" << std::endl;
    }
    m_projectionMatrix = projectionMatrix;

    return m_event;
}

glm::mat4 Model2DTransformation::TransformationData::getMVPMatrix() const
{
    return m_projectionMatrix * m_translationMatrix * m_rotationMatrix * m_scalingMatrix;
}

glm::mat4 Model2DTransformation::TransformationData::getModelViewMatrix() const
{
    return m_translationMatrix * m_rotationMatrix * m_scalingMatrix;
}

Model2DTransformationEvent Model2DTransformation::TransformationData::resetTransformations()
{
    m_rotationMatrix = glm::mat4(1.0f);
    m_scalingMatrix = glm::mat4(1.0f);
    m_translationMatrix = glm::mat4(1.0f);
    m_projectionMatrix = vgf::makeNearProjectionMatrix(glm::mat4(1.0f), -1.0f, 1.0f, -1.0f, 1.0f);

    return m_event;
}

void Model2DTransformation::TransformationData::updateProjection()
{
    m_projectionMatrix = vgf::makeNearProjectionMatrix(m_translationMatrix*m_rotationMatrix*m_scalingMatrix, -1.0f, 1.0f, -1.0f, 1.0f);
}
