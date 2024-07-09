#ifndef BASE_RAYCASTING_VOLUME_RENDERER_3D_H
#define BASE_RAYCASTING_VOLUME_RENDERER_3D_H

#include <map>
#include <QMap>
#include "BaseRaycastingShader.h"
#include "RaycastingResultShader.h"

#include <renderer/SubRendererBase.h>

class BaseRaycastingVolumeRenderer3D: public SubRendererBase
{
public:
    BaseRaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[]);
    ~BaseRaycastingVolumeRenderer3D();

    void initialize() override;

    void render() override;

    void resize(int width, int height) override;

    BaseRaycastingShader *getRaycastingShader() const;

    void setEnableRayCastingBack(bool enableRayCastingBack);

    void setClippingPlane(const glm::vec4 &clippingPlane);

    void setMPRActivated(bool enableMPR);

    void getZDepthFrontFromRayCasting(int wX, int wY, glm::vec4 viewport, float &depthZFront);

    bool isInitialized() const;

    void setMVPMatrix(const glm::mat4 &mvp);

    void setBProgressiveRefinement(bool bProgressiveRefinement);

    void setNoiseThreshold(float getNoiseThreshold);

    void setStateThreshold(bool stateThreshold);

    void setFixedLowSteps();

    void unsetFixedLowSteps();

    /**
   * @brief sets the clipping planes.
   * @param[in] fXLeft leftmost x-clipping plane
   * @param[in] fXRight rightmost x-clipping plane
   * @param[in] fYTop topmost y-clipping plane
   * @param[in] fYBottom bottommost y-clipping plane
   * @param[in] fZBack backmost z-clipping plane
   * @param[in] fZFront frontmost z-clipping plane
   */
    void setClippingValues(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_);

    void setFBOSizeFactor(float FBOSizeFactor);

    virtual void changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader);

    virtual void restoreRaycastingShader();

    virtual void addRayCastingShaderToOffScreen(QString name, BaseRaycastingShader *rcs, bool enable = false);

    BaseRaycastingShader *getRaycastingBackShader() const;

    void highlightAxialSlice(unsigned int slice);

    void highlightSagittalSlice(unsigned int slice);

    void highlightCoronalSlice(unsigned int slice);

    void setHighlightPlanesActivated(bool state);

    bool getEnableRayCastingBack() const;

    void executeOffScreens(RaycastingPreRenderShader *raycastingPreRenderShader);

    void resizeOffScreens(int width, int height);

    BaseRaycastingShader *getRCSOffScreen(QString name);

    void setMVPMatrixOffScreen(const glm::mat4 &mvp);

    void setNoiseThresholdOffScreen(float noiseThreshold);

    void setStateThresholdOffScreen(bool stateThreshold);

    void setMPRActivatedOffScreen(bool enableMPR);

    void setFixedLowStepsOffScreen();

    void unsetFixedLowStepsOffScreen();

    void setClippingPlaneOffScreen(const glm::vec4 &clipping_plane);

    void setFBOSizeFactorOffScreen(float FBOSizeFactor);

    bool getZDepthFrontFromRayCastingOffScreen(QString name, int wX, int wY, glm::vec4 viewport, float &depthZFront);
    void highlightAxialSliceOffScreen(float slice_coord_tex[3]);
    void highlightCoronalSliceOffScreen(float slice_coord_tex[3]);
    void highlightSagittalSliceOffScreen(float slice_coord_tex[]);
    void setHighlightPlanesActivatedOffScren(bool state);
    bool getEnableOffScreen(QString name);
    void setEnableOffScreen(QString name, bool enable);

    bool offScreenRCSMap() const;


    bool existOffScreenRCSMap(QString osName) const;

protected:
    BaseRaycastingShader *getPreviousRaycastingShader() const;

    int getWidth() const;

    int getHeight() const;

    void setScaleFactors(float scale_factors[]);

    unsigned short m_volumeDimension[3];

    float m_scaleFactors[4];                  /**< scale factor for compensating distortions on the texture volume */

private:
    virtual BaseRaycastingShader* createRaycastingShader() = 0;

    virtual BaseRaycastingShader* createBackRaycastingShader() = 0;

    float m_FBOSizeFactor;                   /**< scale factor for FBO */

    bool m_enableRayCastingBack;

    bool m_enableOffScreen;


    BaseRaycastingShader *m_previousRaycastingShader;

    RaycastingPreRenderShader *m_raycastingPreRenderShader;

    BaseRaycastingShader *m_raycastingShader;

    BaseRaycastingShader *m_raycastingBackShader;

    /**
     * @brief m_offScreenRCSMap: Mapa de BaseRayCasting frontal para offscreen
     */
    QMap<QString, BaseRaycastingShader *>m_offScreenRCSMap;

    /**
     * @brief m_enableOffScreenMap: Mapa para habilitar ou desabilitar um elemento do mapa 'm_offScreenRCSMap'.
     */
    QMap<QString, bool> m_enableOffScreenMap;

    RaycastingResultShader *m_raycastingResultShader;

    float m_highlightedPlanes[3];

    int m_width;

    int m_height;

    bool m_bProgressiveRefinement;            /**< whether progressive refinement is active? */

    bool m_isInitialized;
};

#endif // BASE_RAYCASTING_VOLUME_RENDERER_3D_H
