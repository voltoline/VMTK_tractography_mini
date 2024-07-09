#include "Model3DRendering.h"

Model3DRendering::Model3DRendering(float scale_factors[], unsigned short volume_dim[])
{
    m_proxyClipXLeft = -1.0f;
    m_proxyClipXRight = 1.0f;
    m_proxyClipYTop = 1.0f;
    m_proxyClipYBottom = -1.0f;
    m_proxyClipZBack = 1.0f;
    m_proxyClipZFront = -1.0f;

    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    m_volumeDimension[0] = volume_dim[0];
    m_volumeDimension[1] = volume_dim[1];
    m_volumeDimension[2] = volume_dim[2];

    m_stateManager = nullptr;
    m_statesHighlighted3DPlane = nullptr;
}

void Model3DRendering::getScaleFactors(float scale_factors[4])
{
    scale_factors[0] = m_scaleFactors[0];
    scale_factors[1] = m_scaleFactors[1];
    scale_factors[2] = m_scaleFactors[2];
    scale_factors[3] = m_scaleFactors[3];
}

Model3DRenderingEvent Model3DRendering::setModelViewMatrix(const glm::mat4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;

    return Model3DRenderingEvent::TRANSFORMATIONS_CHANGED;
}

Model3DRenderingEvent Model3DRendering::setProjectionMatrix(const glm::mat4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;

    return Model3DRenderingEvent::TRANSFORMATIONS_CHANGED;
}

glm::mat4 Model3DRendering::getModelViewMatrix() const
{
    return m_modelViewMatrix;
}

glm::mat4 Model3DRendering::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

unsigned int Model3DRendering::getHighlightedPlaneAxial() const
{
    return m_highlightedPlaneAxial;
}

Model3DRenderingEvent Model3DRendering::setHighlightedPlaneAxial(unsigned int highlightedPlaneAxial)
{
    m_highlightedPlaneAxial = highlightedPlaneAxial;

    return Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED;
}

unsigned int Model3DRendering::getHighlightedPlaneCoronal() const
{
    return m_highlightedPlaneCoronal;
}

Model3DRenderingEvent Model3DRendering::setHighlightedPlaneCoronal(unsigned int highlightedPlaneCoronal)
{
    m_highlightedPlaneCoronal = highlightedPlaneCoronal;

    return Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED;
}

unsigned int Model3DRendering::getHighlightedPlaneSagittal() const
{
    return m_highlightedPlaneSagittal;
}

Model3DRenderingEvent Model3DRendering::setHighlightedPlaneSagittal(unsigned int highlightedPlaneSagittal)
{
    m_highlightedPlaneSagittal = highlightedPlaneSagittal;

    return Model3DRenderingEvent::HIGHLIGHTED_PLANE_CHAGNED;
}

void Model3DRendering::setHighlightedPlanesActivated(bool state)
{
    if(m_statesHighlighted3DPlane != nullptr)
        m_statesHighlighted3DPlane->setActivated(state);
}

bool Model3DRendering::isHighlightedPlanesActivated() const
{
    if(m_statesHighlighted3DPlane != nullptr)
        return m_statesHighlighted3DPlane->isActivated();
    else
        return false;
}

bool Model3DRendering::isHighlightedPlanesEnabled() const
{
    if(m_statesHighlighted3DPlane != nullptr)
        return m_statesHighlighted3DPlane->isEnabled();
    else
        return false;
}

void Model3DRendering::setStateManager(StateManager *stateManager)
{
    m_stateManager = stateManager;

    if(m_stateManager != nullptr)
    {
        if(m_stateManager->addStateWithRestrictions("HIGHLIGHTED_3D_PLANE", {"CR", "3D_CLIPPING", "MULTIPLANAR", "2DSLICE_TO_3DCLIPPING_SYNC"}))
        {
            m_statesHighlighted3DPlane = m_stateManager->getStatesOf("HIGHLIGHTED_3D_PLANE");
            m_statesHighlighted3DPlane->setStateChangedCallback([this]()
            {
                this->notify(Model3DRenderingEvent::HIGHLIGHTED_PLANE_STATE_CHAGNED);
            });
        }
    }
}

Model3DRenderingEvent Model3DRendering::setProxyGeometryDimFromClippingValue(unsigned short fXLeft_, unsigned short fXRight_, unsigned short fYBottom_, unsigned short fYTop_, unsigned short fZBack_, unsigned short fZFront_)
{
    float fXLeft   = (fXLeft_ / float(m_volumeDimension[0])) - (1 / float(m_volumeDimension[0] * 2));
    float fXRight  = (fXRight_ / float(m_volumeDimension[0])) - (1 / float(m_volumeDimension[0] * 2));
    float fYBottom = (fYBottom_ / float(m_volumeDimension[1])) - (1 / float(m_volumeDimension[1] * 2));
    float fYTop    = (fYTop_ / float(m_volumeDimension[1])) - (1 / float(m_volumeDimension[1] * 2));
    float fZBack   = (fZFront_ / float(m_volumeDimension[2])) - (1 / float(m_volumeDimension[2] * 2));
    float fZFront  = (fZBack_ / float(m_volumeDimension[2])) - (1 / float(m_volumeDimension[2] * 2));

    fXLeft = 2*fXLeft-1;
    fXRight= 2*fXRight-1;
    fYBottom= 2*fYBottom-1;
    fYTop= 2*fYTop-1;
    fZBack= 2*fZBack-1;
    fZFront= 2*fZFront-1;

    if(fXLeft==fXRight)
        fXLeft-=0.01f;

    if(fYTop==fYBottom)
        fYBottom-=0.01f;

    if(fZBack==fZFront)
        fZBack-=0.01f;

    m_proxyClipXLeft = fXLeft;
    m_proxyClipXRight = fXRight;
    m_proxyClipYTop = fYTop;
    m_proxyClipYBottom = fYBottom;
    m_proxyClipZBack = fZBack;
    m_proxyClipZFront = fZFront;

    return Model3DRenderingEvent::PROXY_GEOMETRY_CHANGED;
}

Model3DRenderingEvent Model3DRendering::setProxyGeometryDim(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_)
{
    m_proxyClipXLeft = fXLeft_;
    m_proxyClipXRight = fXRight_;
    m_proxyClipYTop = fYTop_;
    m_proxyClipYBottom = fYBottom_;
    m_proxyClipZBack = fZBack_;
    m_proxyClipZFront = fZFront_;

    return Model3DRenderingEvent::PROXY_GEOMETRY_CHANGED;
}

void Model3DRendering::getProxyGeometryDim(float &fXLeft_, float &fXRight_, float &fYBottom_, float &fYTop_, float &fZBack_, float &fZFront_)
{
    fXLeft_ = m_proxyClipXLeft;
    fXRight_= m_proxyClipXRight;
    fYBottom_= m_proxyClipYBottom;
    fYTop_ = m_proxyClipYTop;
    fZBack_ = m_proxyClipZBack;
    fZFront_ = m_proxyClipZFront;
}
