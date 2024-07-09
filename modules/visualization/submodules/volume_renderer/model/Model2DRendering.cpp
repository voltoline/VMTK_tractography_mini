#include "Model2DRendering.h"

Model2DRendering::Model2DRendering(float scale_factors[]):
    m_axialData(scale_factors, Model2DRenderingEvent::AXIAL_SLICE_CHANGED, Model2DRenderingEvent::AXIAL_TRANSFORMATION_CHANGED, this),
    m_coronalData(scale_factors, Model2DRenderingEvent::CORONAL_SLICE_CHANGED, Model2DRenderingEvent::CORONAL_TRANSFORMATION_CHANGED, this),
    m_sagittalData(scale_factors, Model2DRenderingEvent::SAGITTAL_SLICE_CHANGED, Model2DRenderingEvent::SAGITTAL_TRANSFORMATION_CHANGED, this)
{
    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];
}

void Model2DRendering::getScaleFactors(float scale_factors[4])
{
    scale_factors[0] = m_scaleFactors[0];
    scale_factors[1] = m_scaleFactors[1];
    scale_factors[2] = m_scaleFactors[2];
    scale_factors[3] = m_scaleFactors[3];
}

Model2DRendering::SliceData& Model2DRendering::getAxialData()
{
    return m_axialData;
}

Model2DRendering::SliceData& Model2DRendering::getCoronalData()
{
    return m_coronalData;
}

Model2DRendering::SliceData& Model2DRendering::getSagittalData()
{
    return m_sagittalData;
}

Model2DRendering::SliceData::SliceData(float scale_factors[], Model2DRenderingEvent plane_event, Model2DRenderingEvent transform_event, Model2DRendering *model_2d)
{
    m_transformEvent = transform_event;
    m_planeEvent = plane_event;
    m_model2D = model_2d;

    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    if(m_planeEvent == Model2DRenderingEvent::SAGITTAL_SLICE_CHANGED)
    {
        m_planePos.m_v1 = glm::vec3( 1.0f * m_scaleFactors[1],  1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v2 = glm::vec3(-1.0f * m_scaleFactors[1],  1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v3 = glm::vec3(-1.0f * m_scaleFactors[1], -1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v4 = glm::vec3( 1.0f * m_scaleFactors[1], -1.0f * m_scaleFactors[2], 0.0f);

    } else if(m_planeEvent == Model2DRenderingEvent::CORONAL_SLICE_CHANGED) {

        m_planePos.m_v1 = glm::vec3( 1.0f * m_scaleFactors[0],  1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v2 = glm::vec3(-1.0f * m_scaleFactors[0],  1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v3 = glm::vec3(-1.0f * m_scaleFactors[0], -1.0f * m_scaleFactors[2], 0.0f);
        m_planePos.m_v4 = glm::vec3( 1.0f * m_scaleFactors[0], -1.0f * m_scaleFactors[2], 0.0f);

    } else if(m_planeEvent == Model2DRenderingEvent::AXIAL_SLICE_CHANGED) {

        m_planePos.m_v1 = glm::vec3( 1.0f * m_scaleFactors[0],  1.0f * m_scaleFactors[1], 0.0f);
        m_planePos.m_v2 = glm::vec3(-1.0f * m_scaleFactors[0],  1.0f * m_scaleFactors[1], 0.0f);
        m_planePos.m_v3 = glm::vec3(-1.0f * m_scaleFactors[0], -1.0f * m_scaleFactors[1], 0.0f);
        m_planePos.m_v4 = glm::vec3( 1.0f * m_scaleFactors[0], -1.0f * m_scaleFactors[1], 0.0f);
    }
}

Model2DRenderingEvent Model2DRendering::SliceData::setPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    if(m_model2D->isNotifying())
    {
        std::cerr << "Model2DRendering::SliceData::setPlanePos -->> A notification is in progress!" << std::endl;
    }

    m_planePos.m_v1 = v1;
    m_planePos.m_v2 = v2;
    m_planePos.m_v3 = v3;
    m_planePos.m_v4 = v4;

    return m_planeEvent;
}

void Model2DRendering::SliceData::getPlanePos(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    v1 = m_planePos.m_v1;
    v2 = m_planePos.m_v2;
    v3 = m_planePos.m_v3;
    v4 = m_planePos.m_v4;
}

Model2DRenderingEvent Model2DRendering::SliceData::setPlaneTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    if(m_model2D->isNotifying())
    {
        std::cerr << "Model2DRendering::SliceData::setPlaneTexCoord -->> A notification is in progress!" << std::endl;
    }

    m_planeTex.m_v1 = v1;
    m_planeTex.m_v2 = v2;
    m_planeTex.m_v3 = v3;
    m_planeTex.m_v4 = v4;

    return m_planeEvent;
}

void Model2DRendering::SliceData::getPlaneTexCoord(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, glm::vec3 &v4)
{
    v1 = m_planeTex.m_v1;
    v2 = m_planeTex.m_v2;
    v3 = m_planeTex.m_v3;
    v4 = m_planeTex.m_v4;
}

Model2DRenderingEvent Model2DRendering::SliceData::setModelViewMatrix(const glm::mat4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
    return m_transformEvent;
}

Model2DRenderingEvent Model2DRendering::SliceData::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
    return m_transformEvent;
}

glm::mat4 Model2DRendering::SliceData::getModelViewMatrix() const
{
    return m_modelViewMatrix;
}

glm::mat4 Model2DRendering::SliceData::getProjectionMatrix() const
{
    return m_projectionMatrix;
}
