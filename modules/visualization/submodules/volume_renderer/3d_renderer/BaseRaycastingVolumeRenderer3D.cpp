#include "BaseRaycastingVolumeRenderer3D.h"

#include "RaycastingPreRenderShader.h"

BaseRaycastingVolumeRenderer3D::BaseRaycastingVolumeRenderer3D(unsigned short volume_dim[], float scale_factors[])
{
    m_volumeDimension[0] = volume_dim[0];
    m_volumeDimension[1] = volume_dim[1];
    m_volumeDimension[2] = volume_dim[2];

    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    m_raycastingPreRenderShader = nullptr;
    m_previousRaycastingShader = nullptr;
    m_raycastingShader = nullptr;
    m_raycastingBackShader = nullptr;
    m_raycastingResultShader = nullptr;

    m_enableRayCastingBack = false;

    m_FBOSizeFactor = 1.0f;

    m_bProgressiveRefinement = false;

    m_isInitialized = false;

    m_width = -1;
    m_height = -1;

    m_highlightedPlanes[0] = 0.5f;
    m_highlightedPlanes[1] = 0.5f;
    m_highlightedPlanes[2] = 0.5f;
}

BaseRaycastingVolumeRenderer3D::~BaseRaycastingVolumeRenderer3D()
{
    if(m_raycastingPreRenderShader)
        delete m_raycastingPreRenderShader;

    if(m_raycastingResultShader)
        delete m_raycastingResultShader;

    if(m_raycastingShader)
        delete m_raycastingShader;

    if(m_raycastingBackShader)
        delete m_raycastingBackShader;
}

void BaseRaycastingVolumeRenderer3D::initialize()
{
    if(m_isInitialized)
        return;

    m_raycastingPreRenderShader = new RaycastingPreRenderShader(m_scaleFactors);
    m_raycastingPreRenderShader->initialize();

    m_raycastingResultShader = new RaycastingResultShader();
    m_raycastingResultShader->initialize();

    m_raycastingShader = createRaycastingShader();

    if(m_raycastingShader)
        m_raycastingShader->initialize();
    else
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::initialize --> RaycastingShader not created!");

    m_isInitialized = true;
}

void BaseRaycastingVolumeRenderer3D::render()
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::render -->> Object not initialized!");

    //    preRender
    m_raycastingPreRenderShader->execute();

    // ofscreen
    executeOffScreens(m_raycastingPreRenderShader);

    //    raycasting
    m_raycastingShader->execute(m_raycastingPreRenderShader);

    if(m_enableRayCastingBack && m_raycastingBackShader != nullptr)
        m_raycastingBackShader->execute(m_raycastingPreRenderShader);

    m_raycastingResultShader->execute(m_raycastingShader->getRayTracedFBO().getTexture());
}

void BaseRaycastingVolumeRenderer3D::resize(int width, int height)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::resize -->> Object not initialized!");

    m_width = width;
    m_height = height;

    m_raycastingPreRenderShader->resize(width, height);

    resizeOffScreens(width, height);

    m_raycastingShader->resize(width, height);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->resize(width, height);
}

BaseRaycastingShader *BaseRaycastingVolumeRenderer3D::getRaycastingShader() const
{
    return m_raycastingShader;
}

BaseRaycastingShader *BaseRaycastingVolumeRenderer3D::getRaycastingBackShader() const
{
    return m_raycastingBackShader;
}


void BaseRaycastingVolumeRenderer3D::setEnableRayCastingBack(bool enableRayCastingBack)
{
    m_enableRayCastingBack = enableRayCastingBack;

    if(enableRayCastingBack)
    {
        if(m_raycastingBackShader == nullptr)
        {
            m_raycastingBackShader = createBackRaycastingShader();

            if(m_raycastingBackShader)
            {
                m_raycastingBackShader->initialize();

                if(getWidth() > 0 && getHeight() > 0)
                    m_raycastingBackShader->resize(getWidth(), getHeight());
            }
        }

    } else {

        if(m_raycastingBackShader != nullptr)
        {
            delete m_raycastingBackShader;
            m_raycastingBackShader = nullptr;
        }
    }
}

bool BaseRaycastingVolumeRenderer3D::isInitialized() const
{
    return m_isInitialized;
}

void BaseRaycastingVolumeRenderer3D::setMVPMatrix(const glm::mat4 &mvp)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setModelViewMatrix -->> Object not initialized!");

    m_raycastingPreRenderShader->setMVP(mvp);

    setMVPMatrixOffScreen(mvp);

    m_raycastingShader->setMVP(mvp);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setMVP(mvp);
}

BaseRaycastingShader *BaseRaycastingVolumeRenderer3D::getPreviousRaycastingShader() const
{
    return m_previousRaycastingShader;
}

void BaseRaycastingVolumeRenderer3D::setBProgressiveRefinement(bool bProgressiveRefinement)
{
    m_bProgressiveRefinement = bProgressiveRefinement;
}

void BaseRaycastingVolumeRenderer3D::setNoiseThreshold(float noiseThreshold)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setNoiseThreshold -->> Object not initialized!");

    setNoiseThresholdOffScreen(noiseThreshold);

    m_raycastingShader->setNoiseThreshold(noiseThreshold);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setNoiseThreshold(noiseThreshold);
}

void BaseRaycastingVolumeRenderer3D::setStateThreshold(bool stateThreshold)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setStateThreshold -->> Object not initialized!");

    setStateThresholdOffScreen(stateThreshold);

    m_raycastingShader->setStateThreshold(stateThreshold);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setStateThreshold(stateThreshold);
}

void BaseRaycastingVolumeRenderer3D::setMPRActivated(bool enableMPR)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setMPRActivated -->> Object not initialized!");

    setMPRActivatedOffScreen( enableMPR );

    m_raycastingShader->setEnableMPR(enableMPR);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setEnableMPR(enableMPR);
}

void BaseRaycastingVolumeRenderer3D::setFixedLowSteps()
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setFixedLowSteps -->> Object not initialized!");

    setFixedLowStepsOffScreen();

    m_raycastingShader->setBFixedLowSteps(true);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setBFixedLowSteps(true);

    if (m_bProgressiveRefinement){
        setFBOSizeFactor(m_FBOSizeFactor / 4);
        setFBOSizeFactorOffScreen( m_FBOSizeFactor / 4 );
    }
}

void BaseRaycastingVolumeRenderer3D::unsetFixedLowSteps()
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::unsetFixedLowSteps -->> Object not initialized!");

    unsetFixedLowStepsOffScreen();

    m_raycastingShader->setBFixedLowSteps(false);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setBFixedLowSteps(false);

    if (m_bProgressiveRefinement){
        setFBOSizeFactor(m_FBOSizeFactor * 4);
        setFBOSizeFactorOffScreen( m_FBOSizeFactor * 4 );
    }
}

void BaseRaycastingVolumeRenderer3D::setClippingPlane(const glm::vec4 &clipping_plane)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setClippingPlane -->> Object not initialized!");

    setClippingPlaneOffScreen(clipping_plane);

    m_raycastingShader->setClippingPlane(clipping_plane);
}

void BaseRaycastingVolumeRenderer3D::setClippingValues(float fXLeft_, float fXRight_, float fYBottom_, float fYTop_, float fZBack_, float fZFront_)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setClippingValues -->> Object not initialized!");

    m_raycastingPreRenderShader->setClippingValues(fXLeft_, fXRight_, fYBottom_, fYTop_, fZBack_, fZFront_);
}

void BaseRaycastingVolumeRenderer3D::setFBOSizeFactor(float FBOSizeFactor)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setFBOSizeFactor -->> Object not initialized!");

    m_FBOSizeFactor = FBOSizeFactor;

    m_raycastingPreRenderShader->setFBOSizeFactor(m_FBOSizeFactor);

    setFBOSizeFactorOffScreen( m_FBOSizeFactor );

    m_raycastingShader->setFBOSizeFactor(m_FBOSizeFactor);

    if(m_enableRayCastingBack && m_raycastingBackShader!= nullptr)
        m_raycastingBackShader->setFBOSizeFactor(m_FBOSizeFactor);

    resize(m_width, m_height);
}

void BaseRaycastingVolumeRenderer3D::getZDepthFrontFromRayCasting(int wX, int wY, glm::vec4 viewport, float &depthZFront)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::getZDepthFrontFromRayCasting -->> Object not initialized!");

    m_raycastingShader->getZDepthFrontFromRayCasting(wX, wY, viewport, depthZFront);
}

void BaseRaycastingVolumeRenderer3D::changeRaycastingShader(BaseRaycastingShader *new_raycasting_shader)
{
    new_raycasting_shader->copyData(m_raycastingShader);
    m_previousRaycastingShader = m_raycastingShader;
    m_raycastingShader = new_raycasting_shader;
}

void BaseRaycastingVolumeRenderer3D::addRayCastingShaderToOffScreen(QString name, BaseRaycastingShader *rcs, bool enable){
    rcs->copyData(m_raycastingShader);
    m_offScreenRCSMap.insert(name,rcs);
    m_enableOffScreenMap.insert(name, enable);
}

void BaseRaycastingVolumeRenderer3D::restoreRaycastingShader()
{
    m_previousRaycastingShader->copyData(m_raycastingShader);
    m_raycastingShader = m_previousRaycastingShader;
    m_previousRaycastingShader = nullptr;
}

int BaseRaycastingVolumeRenderer3D::getWidth() const
{
    return m_width;
}

int BaseRaycastingVolumeRenderer3D::getHeight() const
{
    return m_height;
}

void BaseRaycastingVolumeRenderer3D::setScaleFactors(float scale_factors[]){
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setScaleFactors -->> Object not initialized!");

    m_scaleFactors[0] = scale_factors[0];
    m_scaleFactors[1] = scale_factors[1];
    m_scaleFactors[2] = scale_factors[2];
    m_scaleFactors[3] = scale_factors[3];

    m_raycastingPreRenderShader->setScaleFactors(scale_factors);
}

bool BaseRaycastingVolumeRenderer3D::existOffScreenRCSMap(QString osName) const{
    return m_offScreenRCSMap.contains(osName);
}

void BaseRaycastingVolumeRenderer3D::setEnableOffScreen(QString name, bool enable){
    m_enableOffScreenMap.insert(name,enable);
}

bool BaseRaycastingVolumeRenderer3D::getEnableOffScreen(QString name){
    return m_enableOffScreenMap.value(name);
}

bool BaseRaycastingVolumeRenderer3D::getEnableRayCastingBack() const{
    return m_enableRayCastingBack;
}

void BaseRaycastingVolumeRenderer3D::highlightAxialSlice(unsigned int slice)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::highlightAxialSlice -->> Object not initialized!");

    m_highlightedPlanes[2] = (slice / float(m_volumeDimension[2])) - (1 / float(m_volumeDimension[2] * 2));
    m_raycastingShader->highlightPlanes(m_highlightedPlanes);
    highlightAxialSliceOffScreen(m_highlightedPlanes);
}

void BaseRaycastingVolumeRenderer3D::highlightCoronalSlice(unsigned int slice)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::highlightCoronalSlice -->> Object not initialized!");

    m_highlightedPlanes[1] = (slice / float(m_volumeDimension[1])) - (1 / float(m_volumeDimension[1] * 2));
    m_raycastingShader->highlightPlanes(m_highlightedPlanes);
    highlightAxialSliceOffScreen(m_highlightedPlanes);
}

void BaseRaycastingVolumeRenderer3D::highlightSagittalSlice(unsigned int slice)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::highlightSagittalSlice -->> Object not initialized!");

    m_highlightedPlanes[0] = (slice / float(m_volumeDimension[0])) - (1 / float(m_volumeDimension[0] * 2));
    m_raycastingShader->highlightPlanes(m_highlightedPlanes);
}

void BaseRaycastingVolumeRenderer3D::setHighlightPlanesActivated(bool state)
{
    if(!m_isInitialized)
        throw std::runtime_error("BaseRaycastingVolumeRenderer3D::setHighlightPlanesActivated -->> Object not initialized!");

    m_raycastingShader->highlightPlanesActivated(state);
}

void BaseRaycastingVolumeRenderer3D::executeOffScreens(RaycastingPreRenderShader *raycastingPreRenderShader){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){
                     brcs->execute(raycastingPreRenderShader);
                 }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::resizeOffScreens(int width, int height){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->resize(width, height); }
            }
        }
    }
}

BaseRaycastingShader *BaseRaycastingVolumeRenderer3D::getRCSOffScreen(QString name){
    return m_offScreenRCSMap.value(name, nullptr);
}

void BaseRaycastingVolumeRenderer3D::setMVPMatrixOffScreen(const glm::mat4 &mvp){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setMVP(mvp); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setNoiseThresholdOffScreen(float noiseThreshold){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setNoiseThreshold(noiseThreshold); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setStateThresholdOffScreen(bool stateThreshold){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setStateThreshold(stateThreshold); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setMPRActivatedOffScreen(bool enableMPR){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setEnableMPR(enableMPR); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setFixedLowStepsOffScreen(){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setBFixedLowSteps(true); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::unsetFixedLowStepsOffScreen(){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setBFixedLowSteps(false); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setClippingPlaneOffScreen(const glm::vec4 &clipping_plane){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setClippingPlane(clipping_plane); }
            }
        }
    }
}


void BaseRaycastingVolumeRenderer3D::setFBOSizeFactorOffScreen(float FBOSizeFactor){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->setFBOSizeFactor(FBOSizeFactor); }
            }
        }
    }
}

bool BaseRaycastingVolumeRenderer3D::getZDepthFrontFromRayCastingOffScreen(QString name, int wX, int wY, glm::vec4 viewport, float &depthZFront){
    if( !m_enableOffScreenMap.value(name) ){ return false; }
    BaseRaycastingShader *osBRC = m_offScreenRCSMap.value(name, nullptr);
    if( osBRC == nullptr){ return false; }
    osBRC->getZDepthFrontFromRayCasting(wX, wY, viewport, depthZFront);
    return true;
}

void BaseRaycastingVolumeRenderer3D::highlightAxialSliceOffScreen(float slice_coord_tex[3]){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->highlightPlanes(slice_coord_tex); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::highlightCoronalSliceOffScreen(float slice_coord_tex[3]){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->highlightPlanes(slice_coord_tex); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::highlightSagittalSliceOffScreen(float slice_coord_tex[3]){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->highlightPlanes(slice_coord_tex); }
            }
        }
    }
}

void BaseRaycastingVolumeRenderer3D::setHighlightPlanesActivatedOffScren(bool state){
    QMap<QString, BaseRaycastingShader *>::iterator it;
    for (it = m_offScreenRCSMap.begin(); it != m_offScreenRCSMap.end(); ++it){
        if(m_enableOffScreenMap.contains( it.key() ) ){
            if(m_enableOffScreenMap.value(it.key())){
                 BaseRaycastingShader *brcs = it.value();
                 if(brcs != nullptr){ brcs->highlightPlanesActivated(state); }
            }
        }
    }
}
