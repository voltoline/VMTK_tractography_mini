#ifndef MODEL_3D_RENDERING_H
#define MODEL_3D_RENDERING_H

#include "model/ModelSubject.h"
#include <glm/glm.hpp>
#include <modules/visualization/StateManager.h>

enum class Model3DRenderingEvent
{
    NONE,
    PROXY_GEOMETRY_CHANGED,
    TRANSFORMATIONS_CHANGED,
    HIGHLIGHTED_PLANE_CHAGNED,
    HIGHLIGHTED_PLANE_STATE_CHAGNED,
};

class Model3DRendering: public ModelSubject<Model3DRendering, Model3DRenderingEvent>
{
public:
    Model3DRendering(float scale_factors[4], unsigned short volume_dim[3]);

    Model3DRenderingEvent setProxyGeometryDimFromClippingValue(unsigned short fXLeft_, unsigned short fXRight_,
                                                          unsigned short fYBottom_, unsigned short fYTop_,
                                                          unsigned short fZBack_, unsigned short fZFront_);

    Model3DRenderingEvent setProxyGeometryDim(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_);
    void getProxyGeometryDim(float &fXLeft_, float &fXRight_, float &fYBottom_, float &fYTop_, float &fZBack_, float &fZFront_);
    void getScaleFactors(float scale_factors[4]);

    Model3DRenderingEvent setModelViewMatrix(const glm::mat4 &modelViewMatrix);
    Model3DRenderingEvent setProjectionMatrix(const glm::mat4 &projectionMatrix);

    glm::mat4 getModelViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    unsigned int getHighlightedPlaneAxial() const;
    Model3DRenderingEvent setHighlightedPlaneAxial(unsigned int highlightedPlaneAxial);

    unsigned int getHighlightedPlaneCoronal() const;
    Model3DRenderingEvent setHighlightedPlaneCoronal(unsigned int highlightedPlaneCoronal);

    unsigned int getHighlightedPlaneSagittal() const;
    Model3DRenderingEvent setHighlightedPlaneSagittal(unsigned int highlightedPlaneSagittal);

    void setHighlightedPlanesActivated(bool state);
    bool isHighlightedPlanesActivated() const;

    bool isHighlightedPlanesEnabled() const;

    void setStateManager(StateManager *stateManager);

private:
    float m_proxyClipXLeft;                       /**< x left clip plane */
    float m_proxyClipXRight;                      /**< x right clip plane */
    float m_proxyClipYTop;                        /**< y top clip plane */
    float m_proxyClipYBottom;                     /**< y bottom clip plane */
    float m_proxyClipZBack;                       /**< z back clip plane */
    float m_proxyClipZFront;                      /**< z front clip plane */

    unsigned short m_volumeDimension[3];

    float m_scaleFactors[4];

    glm::mat4 m_modelViewMatrix;
    glm::mat4 m_projectionMatrix;

    unsigned int m_highlightedPlaneAxial;
    unsigned int m_highlightedPlaneCoronal;
    unsigned int m_highlightedPlaneSagittal;

    StateManager *m_stateManager;
    FeatureStates::sPtr m_statesHighlighted3DPlane;
};

#endif // MODEL3DRENDERING_H
