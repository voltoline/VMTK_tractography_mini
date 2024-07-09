#include "Sphere.h"

Sphere::Sphere()
{
    m_shaderProgram = nullptr;
    m_sizeData = 0;
    m_sizeIndices = 0;
    m_numTotalObjects = 0;
    modelVewMatrixTextureBufferObject = 0;
    enableViewBufferObject = 0;
    colorTextureBufferObject = 0;
    m_enableViewSamplerLocationID = 0;
    m_colorSamplerLocationID = 0;
    m_modelVewMatrixSamplerLocationID = 0;
    m_depthTexture = 0;
    m_depthTextureBack = 0;

    m_qImageToTexture.clear();

    m_gData.clear();
    m_indices.clear();

    m_projectionMatrixGlobal = glm::mat4(1.0f);
    m_modelViewMatrixGlobal = glm::mat4(1.0f);

    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_dataSphereCreated = false;
    m_statePointSize = false;
    m_stateTextureImage = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;
    m_fixedAlpha = false;

    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;
    vectorEnableViewTexture.clear();
    vectorColorTexture.clear();
    vectorModelViewMatrixTexture.clear();
    m_translateMatrixPrimitive.clear();
}


Sphere::~Sphere(){
    if(vaos > 0){
        glDeleteBuffers(1, &ebo);
        ebo = 0;
        glDeleteBuffers(1, &vbo);
        vbo = 0;
        glDeleteBuffers(1, &modelViewMatrixBuffer);
        modelViewMatrixBuffer = 0;
        glDeleteBuffers(1, &colorBuffer);
        colorBuffer = 0;
        glDeleteBuffers(1, &enableViewBuffer);
        enableViewBuffer = 0;
        glDeleteVertexArrays(1, &vaos);
        vaos = 0;
        glDeleteTextures(1, &modelVewMatrixTextureBufferObject);
        modelVewMatrixTextureBufferObject=0;
        m_gData.clear();
        m_indices.clear();
        clearMatrices();
        clearSettings();
        m_qImageToTexture.clear();
        vectorEnableViewTexture.clear();
        vectorColorTexture.clear();
        vectorModelViewMatrixTexture.clear();
        m_translateMatrixPrimitive.clear();
    }
    deletePointer(m_shaderProgram);
}


void Sphere::makeSphere(GLushort verticalResolution, GLushort horizontalResolution)
{
    m_dataSphereCreated = true;
    GLfloat radius = 1.0f; // normalized radius
    double const V = 1./static_cast<double>( verticalResolution-1 );
    double const H = 1./static_cast<double>( horizontalResolution-1 );
    m_sizeData = verticalResolution * horizontalResolution;
    m_sizeIndices = (verticalResolution-1) * (horizontalResolution-1) * 6;

    m_gData.empty();
    m_indices.empty();
    m_gData.reserve(m_sizeData);
    m_indices.reserve(m_sizeIndices);

    size_t c=0, i=0;
    for(int v = 0; v < verticalResolution; v++){
        for(int h = 0; h < horizontalResolution; h++) {
            float const y = static_cast<float>( sin( -M_PI_2 + M_PI * v * V ) );
            float const x = static_cast<float>( cos(2*M_PI * h * H) * sin( M_PI * v * V ) );
            float const z = static_cast<float>( sin(2*M_PI * h * H) * sin( M_PI * v * V ) );
            glm::vec2 tCoord(h*H, v*V);
            glm::vec3 vCoord(x*radius, y*radius, z*radius);
            glm::vec3 normals(x, y, z);
            m_gData[c].verCoord=vCoord;
            m_gData[c].texCoord=tCoord;
            m_gData[c].normal=normals;
            c++;
            if(verticalResolution-1!=v &&horizontalResolution-1!=h){
                m_indices[i++] = static_cast<GLushort>(  v * horizontalResolution + h );
                m_indices[i++] = static_cast<GLushort>(  (v + 1) * horizontalResolution + h );
                m_indices[i++] = static_cast<GLushort>(  (v + 1) * horizontalResolution + (h + 1) );
                m_indices[i++] = static_cast<GLushort>(  (v + 1) * horizontalResolution + (h + 1) );
                m_indices[i++] = static_cast<GLushort>(  v * horizontalResolution + (h + 1) );
                m_indices[i++] = static_cast<GLushort>(  v * horizontalResolution + h );
            }
        }
    }
}

void Sphere::clearMatrices(){
    m_initialScaleMatrixPrimitive.clear();
    m_initialPositionMatrixPrimitive.clear();
    m_initialRotateMatrixPrimitive.clear();
    m_scaleMatrixPrimitive.clear();
    m_translateMatrixPrimitive.clear();
    m_rotateMatrixPrimitive.clear();
}

void Sphere::clearSettings(){
    m_colorVector.clear();
    m_scaleRadiusVector.clear();
    m_enableViewVector.clear();
}

void Sphere::removeAllSpheres(){
    clearMatrices();
    clearSettings();
    m_numTotalObjects = 0;
}

void Sphere::removeMatriz(int id){

    vgf::removeVectorElementByIndex( m_initialScaleMatrixPrimitive, id);
    vgf::removeVectorElementByIndex( m_initialPositionMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_initialRotateMatrixPrimitive, id);
    vgf::removeVectorElementByIndex( m_scaleMatrixPrimitive, id);
    vgf::removeVectorElementByIndex( m_translateMatrixPrimitive, id);
    vgf::removeVectorElementByIndex( m_rotateMatrixPrimitive, id);
}

void Sphere::removeSetting(int id){
    vgf::removeVectorElementByIndex(m_colorVector, id);
    vgf::removeVectorElementByIndex(m_scaleRadiusVector, id);
    vgf::removeVectorElementByIndex(m_enableViewVector, id);
}

void Sphere::removeSphere(unsigned short id){
    if(id >= m_numTotalObjects){
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
    removeMatriz(id);
    removeSetting(id);
    m_numTotalObjects--;
}

void Sphere::removeLastMatriz(){
    m_initialScaleMatrixPrimitive.pop_back();
    m_initialPositionMatrixPrimitive.pop_back();
    m_initialRotateMatrixPrimitive.pop_back();
    m_scaleMatrixPrimitive.pop_back();
    m_translateMatrixPrimitive.pop_back();
    m_rotateMatrixPrimitive.pop_back();
}

void Sphere::removeLastSetting(){
    m_colorVector.pop_back();
    m_scaleRadiusVector.pop_back();
    m_enableViewVector.pop_back();
}

void Sphere::removeLastSphere(){
    if(m_numTotalObjects == 0){
        std::cerr<<"."<<std::endl;
        return;
    }
    removeLastMatriz();
    removeLastSetting();
    m_numTotalObjects--;
}

void Sphere::createAndInitializeMatrices(){
    clearMatrices();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_initialPositionMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_initialRotateMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_scaleMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_translateMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_rotateMatrixPrimitive.push_back(glm::mat4(1.0f));
    }
}

void Sphere::createAndInitializeNewMatrix(){
    m_initialScaleMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_initialPositionMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_initialRotateMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_scaleMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_translateMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_rotateMatrixPrimitive.push_back(glm::mat4(1.0f));
}


void Sphere::createAndInitializeSettings(glm::vec4 color, GLfloat scaleRadius){
    clearSettings();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_colorVector.push_back(color);
        m_scaleRadiusVector.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) ));
        m_enableViewVector.push_back(true);
    }
}

void Sphere::createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleRadius){
    m_colorVector.push_back(color);
    m_scaleRadiusVector.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) ));
    m_enableViewVector.push_back(true);
}

void Sphere::addSpheresDefaultForAll(GLushort numSpheres, GLfloat scaleRadius, GLushort verticalResolution, GLushort horizontalResolution, glm::vec4 color)
{
    m_numTotalObjects = numSpheres;
    if(!m_dataSphereCreated){
        makeSphere(verticalResolution, horizontalResolution);
        updatingBuffersPrimitive();
    }
    createAndInitializeMatrices();
    createAndInitializeSettings(color,scaleRadius);
    updatingInstance();
}

void Sphere::addFirstSphereCustomized(GLfloat scaleRadius, GLushort verticalResolution, GLushort horizontalResolution, glm::vec4 color)
{
    m_numTotalObjects++;
    if(!m_dataSphereCreated){
        makeSphere(verticalResolution, horizontalResolution);
        updatingBuffersPrimitive();
    }
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleRadius);
    updatingInstance();
}

void Sphere::addSphereCustomized(GLfloat scaleRadius, glm::vec4 color)
{
    m_numTotalObjects++;
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleRadius);
    updatingInstance();
}

void Sphere::setColorForAll(glm::vec4 color)
{
    for(unsigned short i = 0; i < m_numTotalObjects; i++){ m_colorVector[i] = color; }
}

void Sphere::setColorForEach(unsigned short id, glm::vec4 color)
{
    if(id < m_colorVector.size()){ m_colorVector[id] = color; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Sphere::setColorForLast(glm::vec4 color)
{
     m_colorVector.back() = color;
}

void Sphere::setEnableViewForAll(bool enable)
{
    for(unsigned short i = 0; i < m_numTotalObjects; i++){  m_enableViewVector[i] = enable; }
}

void Sphere::setEnableViewForEach(unsigned short id, bool enable)
{
    if(id < m_enableViewVector.size()) { m_enableViewVector[id] = enable; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Sphere::setEnableViewForLast(bool enable)
{
     m_enableViewVector.back() = enable;
}

std::vector<bool> Sphere::getEnableViewVector(){
    return m_enableViewVector;
}

void Sphere::setScaleRadiusForAll(GLfloat scaleRadius){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_scaleRadiusVector[i] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
    }
}

void Sphere::setScaleRadiusForEach(unsigned short id, GLfloat scaleRadius){
    if(id < m_numTotalObjects){
        m_scaleRadiusVector[id] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setScaleRadiusForLast(GLfloat scaleRadius){
        m_scaleRadiusVector.back() = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
}

void Sphere::setInitialScaleForAll(glm::vec3 vS){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Sphere::setInitialScaleForEach(unsigned short id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_initialScaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setInitialScaleForLast(glm::vec3 vS){
        m_initialScaleMatrixPrimitive.back() = glm::scale( glm::mat4(1.0f), vS );
}

void Sphere::setInitialPositionForAll(glm::vec3 vT){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Sphere::setInitialPositionForEach(int id, glm::vec3 vT){
    if(id < m_numTotalObjects){
        m_initialPositionMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setInitialPositionForLast(glm::vec3 vT){
        m_initialPositionMatrixPrimitive.back() = glm::translate( glm::mat4(1.0),  vT );
}


void Sphere::setInitialRotateForAll(glm::vec3 vR){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixPrimitive[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void Sphere::setInitialRotateForEach(unsigned short id, glm::vec3 vR){
    if(id < m_numTotalObjects){
        m_initialRotateMatrixPrimitive[id] = vgf::rotationFromVectorDegrees(vR);
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setInitialRotateForLast(glm::vec3 vR){
        m_initialRotateMatrixPrimitive.back() = vgf::rotationFromVectorDegrees(vR);
}

void Sphere::setInitialRotateForLast(glm::mat4 mR){
        m_initialRotateMatrixPrimitive.back() = mR;
}

void Sphere::setScaleForAll(glm::vec3 vS){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Sphere::setScaleForEach(unsigned short id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_scaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setTranslateForAll(glm::vec3 vT){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Sphere::setTranslateForAll(glm::mat4 mT){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixPrimitive[i] = mT;
    }
}

void Sphere::setTranslateForEach(unsigned short id, glm::vec3 vT){
    if(id < m_translateMatrixPrimitive.size()){
        m_translateMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setTranslateForEach(unsigned short id, glm::mat4 mT){
    if(id < m_translateMatrixPrimitive.size()){
        m_translateMatrixPrimitive[id] = mT;
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setTranslateForLast(glm::vec3 vT){
    if( m_numTotalObjects >0){
        m_translateMatrixPrimitive.back() = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Sphere::setTranslateForLast(glm::mat4 mT){
    if( m_numTotalObjects >0){
        m_translateMatrixPrimitive.back() = mT;
    }
}


void Sphere::setRotateForAll(glm::vec3 vR){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixPrimitive[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void Sphere::setRotateForAll(glm::mat4 mR){
    for(unsigned short i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixPrimitive[i] = mR;
    }
}

void Sphere::setRotateForEach(unsigned short id, glm::vec3 vR){
    if(id < m_rotateMatrixPrimitive.size()){
        m_rotateMatrixPrimitive[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setRotateForEach(unsigned short id, glm::mat4 mR){
    if(id < m_rotateMatrixPrimitive.size()){
        m_rotateMatrixPrimitive[id] = mR;
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Sphere::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrixGlobal = projectionMatrix;
    m_modelViewMatrixGlobal = modelViewMatrix;
}

bool Sphere::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
{
    QManagementResource *qmr = new QManagementResource;
    GLuint idShader = qmr->compileResourceFileShader(tos,resourceFile);
    if(idShader==0)
    {
        deletePointer(qmr);
        return m_stateAddedShader = m_stateShader = false;
    }
    if(m_shaderProgram==nullptr){
        m_shaderProgram = new vmtkManagementShader;
    }

    m_shaderProgram->setShader(idShader);
    deletePointer(qmr);

    if(!m_shaderProgram->linking()){
        deletePointer(m_shaderProgram);
        return m_stateAddedShader = m_stateShader = false;
    }
    else{ return m_stateAddedShader = m_stateShader = true; }
}

bool Sphere::setShaderProgram(vmtkManagementShader *mShaderProgram)
{
    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void Sphere::setIDTextureActivated(unsigned short idta){
    m_getIDTextureActivated = idta;
}

GLint Sphere::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *Sphere::getManagementShader() const
{
    return m_shaderProgram;
}


void Sphere::defaultResourceFile(){
    QManagementResource *vsG =  new QManagementResource();
    QManagementResource *fsG =  new QManagementResource();
    GLuint vertex = vsG->compileResourceFileShader(QManagementResource::vertex,":/primitive_object_v2_vert");
    GLuint fragment = fsG->compileResourceFileShader(QManagementResource::fragment,":/primitive_object_v2_frag");

    if( vertex==0 || fragment==0){ m_stateShader=false; }
    m_shaderProgram = new vmtkManagementShader;
    m_shaderProgram->setShader(vertex);
    m_shaderProgram->setShader(fragment);
    deletePointer(vsG);
    deletePointer(fsG);

    if(!m_shaderProgram->linking()){
        m_stateShader=false;
        deletePointer(m_shaderProgram);
        std::cerr<<"The program "<< m_shaderProgram->getProgram() << " not compiled."<<std::endl;
    }
    else{ m_stateShader = true; }
}

void Sphere::initializePrimitive(){
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file (Sphere)."<<std::endl;
        defaultResourceFile();
    }
    createBuffers();
    initPrimitive();
}

void Sphere::createBuffers(){
    /*create Primitive buffers*/
    glGenVertexArrays(1, &vaos);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Sphere::setModelViewMatrixGlobal( glm::mat4 modelViewMatrixGlobal){
    m_modelViewMatrixGlobal = modelViewMatrixGlobal;
}

void Sphere::setProjectionMatrixGlobal( glm::mat4 projectionMatrixGlobal){
    m_projectionMatrixGlobal = projectionMatrixGlobal;
}

int Sphere::getNumTotalObjects() const
{
    return m_numTotalObjects;
}

void Sphere::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void Sphere::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}



void Sphere::initPrimitive()
{
    glBindVertexArray(vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
#ifdef __APPLE__
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( OFFSETOF(PrimitiveData, PrimitiveData::verCoord)  ));
#else
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *> ( offsetof(PrimitiveData, verCoord) ) );
#endif

    glEnableVertexAttribArray(1);
#ifdef __APPLE__
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(OFFSETOF(PrimitiveData, PrimitiveData::texCoord)));
#else
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *> (offsetof(PrimitiveData, texCoord ) ) );
#endif
    glEnableVertexAttribArray(2);

#ifdef __APPLE__
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( OFFSETOF(PrimitiveData, PrimitiveData::normal)) );
#else
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *> ( offsetof(PrimitiveData, normal)) );
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);

    // Create the view buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &enableViewBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(int), nullptr, GL_DYNAMIC_DRAW);
    glGenTextures(1, &enableViewBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, enableViewBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, enableViewBuffer);
    m_enableViewSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "enableViewTexture");

    // Create the buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glGenTextures(1, &colorTextureBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, colorTextureBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, colorBuffer);
    m_colorSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "colorTexture");

    // Create the buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &modelViewMatrixBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glGenTextures(1, &modelVewMatrixTextureBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, modelVewMatrixTextureBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, modelViewMatrixBuffer);
    m_modelVewMatrixSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "modelViewMatrixTexture");

    glBindBuffer(GL_TEXTURE_BUFFER,0);
}

void Sphere::updatingInstance(){
    // update the buffer and attach it to the buffer texture

    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(int), vectorEnableViewTexture.data(), GL_DYNAMIC_DRAW);


    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), vectorColorTexture.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::mat4), vectorModelViewMatrixTexture.data(), GL_DYNAMIC_DRAW);
}

void Sphere::updatingBuffersPrimitive(){
    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_gData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}



void Sphere::createLoadImagesToTexture(int numImages){
    for(int i = 0; i<numImages; i++){
        m_qImageToTexture.push_back(QLoadImage());
    }
}

void Sphere::loadImageTexture(const char *image_path1)
{
    createLoadImagesToTexture(1);
    m_qImageToTexture[0].loadImageToTexture(image_path1);
    m_stateTextureImage = true;
}


void Sphere::deleteImageTexture()
{
    if(!m_qImageToTexture.empty()){
        foreach (QLoadImage imageTexture, m_qImageToTexture) {
            imageTexture.deleteTextureImage();
        }
        m_qImageToTexture.clear();
        m_stateTextureImage = false;
    }
    else{
        if(m_stateTextureImage == true){
            m_stateTextureImage = false;
        }
    }
}

void Sphere::enableImageTexture(bool state)
{
    if(m_qImageToTexture.empty()){
        m_stateTextureImage = false;
    }
    else{
        m_stateTextureImage = state;
    }
}

void Sphere::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void Sphere::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void Sphere::setDepthTextureBack(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void Sphere::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}

void Sphere::setEnableFixedAlpha(bool state){
        m_fixedAlpha = state;
}

void Sphere::drawPrimitive(TypeFilled mode , GLushort idActivateTexture)
{
    glBindVertexArray(vaos);
    if(mode == points){
        if(m_statePointSize){
            glEnable(GL_PROGRAM_POINT_SIZE); // enable state GL_PROGRAM_POINT_SIZE
            glPointSize(m_pointSize);
        }
        else
        {
            glDisable(GL_PROGRAM_POINT_SIZE); // It changes the state to the default state (disable).
        }
        glDrawElementsInstanced(GL_POINTS, m_sizeIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid *>(sizeof(GLushort)*0),m_numTotalObjects);
    }
    else{
        glActiveTexture(GL_TEXTURE0+idActivateTexture);
        if(m_stateTextureImage){
            glBindTexture(GL_TEXTURE_2D, m_qImageToTexture[0].getTextureID());
        }
        else{
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idActivateTexture);

        glDrawElementsInstanced(  GL_TRIANGLES, m_sizeIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid *>(sizeof(GLushort)*0),m_numTotalObjects);
    }
    glBindVertexArray(0);
}


void Sphere::primitiveRender( TypeFilled mode ){


    if(!m_stateShader){return;}
    GLushort idTextureActivated = m_getIDTextureActivated;

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrixGlobal));
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateTexImage"), m_stateTextureImage);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"),  m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), m_fixedAlpha);

    vectorEnableViewTexture.clear();
    vectorColorTexture.clear();
    vectorModelViewMatrixTexture.clear();
    for(unsigned short i=0;i<m_numTotalObjects;i++) {
            m_modelViewMatrixPrimitive = m_translateMatrixPrimitive[i] * m_rotateMatrixPrimitive[i] * m_scaleMatrixPrimitive[i];
            glm::mat4 initialScale = m_initialScaleMatrixPrimitive[i] * m_scaleRadiusVector[i];
            glm::mat4 initialViewMatrix =  m_initialPositionMatrixPrimitive[i] * m_initialRotateMatrixPrimitive[i] * initialScale;
            vectorModelViewMatrixTexture.push_back( m_modelViewMatrixGlobal*m_modelViewMatrixPrimitive*initialViewMatrix);
            glm::vec4 color = m_colorVector[i];
            int visible = m_enableViewVector.at(i) ? color.a : 0;
            if(!visible){
                color.a = 0.0;
            }
            vectorColorTexture.push_back( color );
            vectorEnableViewTexture.push_back( visible );
    }

    // Bind the weight TBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(int)*m_numTotalObjects, vectorEnableViewTexture.data());
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_BUFFER, enableViewBufferObject);
    glUniform1i(m_enableViewSamplerLocationID, idTextureActivated);
    idTextureActivated++;

    // Bind the weight TBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(glm::vec4)*m_numTotalObjects, vectorColorTexture.data());
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_BUFFER, colorTextureBufferObject);
    glUniform1i(m_colorSamplerLocationID, idTextureActivated);
    idTextureActivated++;

    // Bind the weight VBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(glm::mat4)*m_numTotalObjects, vectorModelViewMatrixTexture.data());
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_BUFFER, modelVewMatrixTextureBufferObject);
    glUniform1i(m_modelVewMatrixSamplerLocationID, idTextureActivated);
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTex"),idTextureActivated);
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTexBack"),idTextureActivated);
    idTextureActivated++;

    drawPrimitive(mode,idTextureActivated);
    m_lastIDTextureActivated = idTextureActivated;
}


bool Sphere::bindingRender()
{
    return m_shaderProgram->binding();
}

void Sphere::releasingRender()
{
    m_shaderProgram->releasing();
}
