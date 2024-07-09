#include "Cylinder.h"

Cylinder::Cylinder()
{
    m_shaderProgram = nullptr;
    m_sizeData = 0;
    m_sizeIndices = 0;
    m_numTotalObjects = 0;
    m_numImages = 0;
    modelVewMatrixTextureBufferObject = 0;
    colorTextureBufferObject = 0;
    m_colorSamplerLocationID = 0;
    m_modelVewMatrixSamplerLocationID = 0;
    m_depthTexture = 0;
    m_depthTextureBack = 0;

    m_qImageToTexture.clear();


    m_gData.clear();
    m_indices.clear();
    m_vectorSizeSubindices = nullptr;

    m_scaleHeigthForAll.clear();
    m_initialPositionMatrixGeometry.clear();
    m_projectionMatrix = glm::mat4(1.0f);
    m_modelViewMatrix = glm::mat4(1.0f);

    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_dataCylinderCreated = false;
    m_statePointSize = false;
    m_stateTextureImage = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;
    m_fixedAlpha = false;

    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;

    vectorModelViewMatrixTexture.clear();
    vectorColorTexture.clear();
}


Cylinder::~Cylinder(){
    if(vaos > 0){
        glDeleteBuffers(1, &ebo);
        ebo = 0;
        glDeleteBuffers(1, &vbo);
        vbo = 0;
        glDeleteBuffers(1, &modelViewMatrixBuffer);
        modelViewMatrixBuffer = 0;
        glDeleteBuffers(1, &colorBuffer);
        colorBuffer = 0;
        glDeleteVertexArrays(1, &vaos);
        vaos = 0;
        glDeleteTextures(1, &modelVewMatrixTextureBufferObject);
        modelVewMatrixTextureBufferObject=0;
        m_gData.clear();
        m_indices.clear();
        vectorModelViewMatrixTexture.clear();
        vectorColorTexture.clear();
        m_initialPositionMatrixGeometry.clear();
        delete m_vectorSizeSubindices;
    }
}

void Cylinder::makeCylinder(GLushort resolution){

    m_dataCylinderCreated = true;
    GLfloat radius = 1.0f;
    GLfloat h = 1.0f;
    GLushort it = resolution;
    m_sizeData = it * 4 + 2;
    m_sizeIndices = it * 4 + 2;
    m_gData.clear();
    m_gData.reserve(m_sizeData);
    m_indices.clear();
    m_indices.reserve(m_sizeIndices);
    m_vectorSizeSubindices = new int[3];
    m_vectorSizeSubindices[0]= 2*it;
    m_vectorSizeSubindices[1]= it+1;
    m_vectorSizeSubindices[2]= it+1;

    int ind=0, c=0;
    for (int i = 0; i < it; i++) {
        float angle = static_cast<float>(2*M_PI) / static_cast<float>(it-1);
        int ni=0; if(i==it-1){ni=0;}else{ni=i;}
        float x = sinf(ni * angle);
        float z = cosf(ni * angle);
        float u = static_cast<float>(i) / static_cast<float>(it-1);
        glm::vec2 tCoord(u, 0);
        glm::vec3 vCoord(x*radius, h/2, z*radius);
        glm::vec3 normals(x, 0, z);
        normals = glm::normalize(normals);
        m_gData[c].verCoord=vCoord;
        m_gData[c].texCoord=tCoord;
        m_gData[c].normal=normals;
        m_indices[ind] = static_cast<GLushort>(ind);
        ind++; c++;
        tCoord = glm::vec2(u, 1);
        vCoord = glm::vec3(x*radius, -h/2, z*radius);
        normals = glm::vec3(x, 0, z);
        normals = glm::normalize(normals);
        m_gData[c].verCoord=vCoord;
        m_gData[c].texCoord=tCoord;
        m_gData[c].normal=normals;
        m_indices[ind] = static_cast<GLushort>(ind);
        ind++; c++;
    }

    int signalNormal=-1;
    for(int i = 0; i<2;i++){
        glm::vec2 tCoord(0.5, 0.5);
        signalNormal *=-1;
        glm::vec3 vCoord( glm::vec3(0, -h/2, 0) * static_cast<float>(signalNormal) );
        glm::vec3 normals = vCoord;
        normals = glm::normalize(normals);
        m_gData[c+(it+1)*i].verCoord = vCoord;
        m_gData[c+(it+1)*i].texCoord = tCoord;
        m_gData[c+(it+1)*i].normal = normals;
        m_indices[ind+(it+1)*i] = static_cast<GLushort>(ind+(it+1)*i);
    }
    ind++; c++;

    for(int i = 0; i<it;i++){
        float angle = static_cast<float>(2*M_PI) / static_cast<float>(it-1);
        int ni=0; if(i==it-1){ni=0;}else{ni=i;}
        float x = sinf(ni * angle);
        float z = cosf(ni * angle);
        glm::vec2 tCoord(x, z);
        tCoord = vgf::coordVertToCoordTex(tCoord);
        glm::vec3 vCoord( glm::vec3(-x*radius, -h/2, z*radius));
        glm::vec3 normals = glm::vec3(0, -h/2, 0);
        normals = glm::normalize(normals);
        m_gData[c].verCoord = vCoord;
        m_gData[c].texCoord = tCoord;
        m_gData[c].normal = normals;
        m_indices[ind] = static_cast<GLushort>(ind);

        tCoord = glm::vec2(x, -z);
        tCoord = vgf::coordVertToCoordTex(tCoord);
        vCoord = glm::vec3(x*radius, h/2, z*radius);
        normals = glm::vec3(0, h/2, 0);
        normals = glm::normalize(normals);
        m_gData[c+it+1].verCoord=vCoord;
        m_gData[c+it+1].texCoord=tCoord;
        m_gData[c+it+1].normal=normals;
        m_indices[ind+it+1]= static_cast<GLushort>(ind+it+1);
        c++;
        ind++;
    }
}

void Cylinder::createAndInitializeMatrices(){

    m_initialScaleMatrixGeometry.clear();
    m_initialPositionMatrixGeometry.clear();
    m_initialRotateMatrixGeometry.clear();
    m_scaleMatrixGeometry.clear();
    m_translateMatrixGeometry.clear();
    m_rotateMatrixGeometry.clear();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixGeometry.append(glm::mat4(1.0f));
        m_initialPositionMatrixGeometry.append(glm::mat4(1.0f));
        m_initialRotateMatrixGeometry.append(glm::mat4(1.0f));
        m_scaleMatrixGeometry.append(glm::mat4(1.0f));
        m_translateMatrixGeometry.append(glm::mat4(1.0f));
        m_rotateMatrixGeometry.append(glm::mat4(1.0f));
    }
}

void Cylinder::createAndInitializeNewMatrix(){
    m_scaleHeigthForAll.append(glm::mat4(1.0f));
    m_scaleRadiusForAll.append(glm::mat4(1.0f));
    m_initialScaleMatrixGeometry.append(glm::mat4(1.0f));
    m_initialPositionMatrixGeometry.append(glm::mat4(1.0f));
    m_initialRotateMatrixGeometry.append(glm::mat4(1.0f));
    m_scaleMatrixGeometry.append(glm::mat4(1.0f));
    m_translateMatrixGeometry.append(glm::mat4(1.0f));
    m_rotateMatrixGeometry.append(glm::mat4(1.0f));
}


void Cylinder::createAndInitializeSettings(glm::vec4 color, GLfloat scaleRadius, GLfloat scaleHeight){
    m_color.clear();
    m_scaleRadiusForAll.clear();
    m_scaleHeigthForAll.clear();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_color.append(color);
        m_scaleRadiusForAll.append(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, 1.0f, scaleRadius) ));
        m_scaleHeigthForAll.append(glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) ));
    }
}

void Cylinder::createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleRadius, GLfloat scaleHeight){
    m_color.append(color);
    m_scaleRadiusForAll.append(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, 1.0f, scaleRadius) ));
    m_scaleHeigthForAll.append(glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) ));
}


void Cylinder::addCylindersDefaultForAll(GLushort numCylinders, GLfloat scaleRadius, GLfloat scaleHeight, GLushort resolution, glm::vec4 color)
{
    m_numTotalObjects = numCylinders;
    if(!m_dataCylinderCreated){
        makeCylinder(resolution);
        initializePrimitive();
    }
    createAndInitializeMatrices();
    createAndInitializeSettings(color,scaleRadius,scaleHeight);

}

void Cylinder::addCylinderCustomized(GLfloat scaleRadius, GLfloat scaleHeight, GLushort resolution, glm::vec4 color)
{
    m_numTotalObjects++;
    if(!m_dataCylinderCreated){
        makeCylinder(resolution);
        initializePrimitive();

    }
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleRadius,scaleHeight);

}

void Cylinder::setScaleRadiusForAll(GLfloat scaleRadius){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleRadiusForAll[i] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, 1.0f, scaleRadius) );
    }
}

void Cylinder::setScaleRadiusForEach(int id, GLfloat scaleRadius){
    if(id < m_numTotalObjects){
        m_scaleRadiusForAll[id] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, 1.0f, scaleRadius) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setScaleHeigthForAll(GLfloat scaleHeight){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleHeigthForAll[i] =  glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) );
    }
}

void Cylinder::setScaleHeigthForEach(int id, GLfloat scaleHeight){
    if(id < m_numTotalObjects){
        m_scaleHeigthForAll[id] = glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setInitialScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Cylinder::setInitialScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_initialScaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setInitialPositionForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Cylinder::setInitialPositionForEach(int id, glm::vec3 vT){
    if(id < m_numTotalObjects){
        m_initialPositionMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setInitialRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixGeometry[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void Cylinder::setInitialRotateForEach(int id, glm::vec3 vR){
    if(id < m_numTotalObjects){
        m_initialRotateMatrixGeometry[id] = vgf::rotationFromVectorDegrees(vR);
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setColorForAll(glm::vec4 color)
{
    for(int i = 0; i < m_numTotalObjects; i++){ m_color[i] = color; }
}

void Cylinder::setColorForEach(int id, glm::vec4 color)
{
    if(id < m_color.size()){ m_color[id] = color; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Cylinder::setScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Cylinder::setScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_scaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setTranslateForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Cylinder::setTranslateForEach(int id, glm::vec3 vT){
    if(id < m_translateMatrixGeometry.size()){
        m_translateMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Cylinder::setRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixGeometry[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void Cylinder::setRotateForEach(int id, glm::vec3 vR){
    if(id < m_rotateMatrixGeometry.size()){
        m_rotateMatrixGeometry[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}


void Cylinder::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_modelViewMatrix = modelViewMatrix;
}

bool Cylinder::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
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
    delete qmr;
    if(!m_shaderProgram->linking()){
        deletePointer(m_shaderProgram);
        return m_stateAddedShader = m_stateShader = false;
    }
    else{ return m_stateAddedShader = m_stateShader = true; }
}

bool Cylinder::setShaderProgram(vmtkManagementShader *mShaderProgram)
{

    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void Cylinder::setIDTextureActivated(GLint lta){
    m_getIDTextureActivated = lta;
}

GLint Cylinder::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *Cylinder::getManagementShader() const
{
    return m_shaderProgram;
}

void Cylinder::defaultResourceFile()
{
    QManagementResource *vsG =  new QManagementResource();
    QManagementResource *fsG =  new QManagementResource();
    GLuint vertex = vsG->compileResourceFileShader(QManagementResource::vertex,":/geometry_vert");
    GLuint fragment = fsG->compileResourceFileShader(QManagementResource::fragment,":/geometry_frag");

    vsG->printSourceCode();

    if( vertex==0 || fragment==0)
    {
        m_stateShader=false;
    }
    m_shaderProgram = new vmtkManagementShader;
    m_shaderProgram->setShader(vertex);
    m_shaderProgram->setShader(fragment);

    if(!m_shaderProgram->linking()){
        m_stateShader=false;
        deletePointer(m_shaderProgram);
        std::cerr<<"The program "<< m_shaderProgram->getProgram() << " not compiled."<<std::endl;
    }
    else{ m_stateShader = true; }
}


void Cylinder::initializePrimitive()
{
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file (Cylinder)."<<std::endl;
        defaultResourceFile();
    }
    createBuffers();
    initPrimitive();
}

void Cylinder::createBuffers()
{
    /*create geometry buffers*/
    glGenVertexArrays(1, &vaos);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Cylinder::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void Cylinder::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}



void Cylinder::initPrimitive()
{
    glBindVertexArray(vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
#ifdef __APPLE__
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( OFFSETOF(PrimitiveData, PrimitiveData::verCoord)  ));
#else
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( offsetof(PrimitiveData, PrimitiveData::verCoord) ) );
#endif

    glEnableVertexAttribArray(1);
#ifdef __APPLE__
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(OFFSETOF(PrimitiveData, PrimitiveData::texCoord)));
#else
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(offsetof(PrimitiveData, PrimitiveData::texCoord ) ) );
#endif

    glEnableVertexAttribArray(2);
#ifdef __APPLE__
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( OFFSETOF(PrimitiveData, PrimitiveData::normal)) );
#else
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( offsetof(PrimitiveData, PrimitiveData::normal)) );
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);


    // Create the buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glGenTextures(1, &colorTextureBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, colorTextureBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, colorBuffer);
    m_colorSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "colorTexture");

    // Create the buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &modelViewMatrixBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glGenTextures(1, &modelVewMatrixTextureBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, modelVewMatrixTextureBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, modelViewMatrixBuffer);
    m_modelVewMatrixSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "modelViewMatrixTexture");

    glBindBuffer(GL_TEXTURE_BUFFER,0);
}

void Cylinder::updatingInstance(){
    // update the buffer and attach it to the buffer texture
    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), vectorColorTexture.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::mat4), vectorModelViewMatrixTexture.data(), GL_DYNAMIC_DRAW);
}

void Cylinder::updatingBuffersGeometry(){
    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_gData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}

void Cylinder::createLoadImagesToTexture(int numImages){
    for(int i = 0; i<numImages; i++){
        m_qImageToTexture.append(QLoadImage());
    }
}

void Cylinder::loadImageTexture(const char *image_path1)
{
    m_numImages = 1;
    int i = 0;
    createLoadImagesToTexture(m_numImages);
    m_qImageToTexture[i++].loadImageToTexture(image_path1);
    m_stateTextureImage = true;
}

void Cylinder::loadImageTexture(const char *image_path1, const char * image_path2)
{
    m_numImages = 2;
    createLoadImagesToTexture(m_numImages);
    m_qImageToTexture[m_numImages-2].loadImageToTexture(image_path1);
    m_qImageToTexture[m_numImages-1].loadImageToTexture(image_path2);
    m_stateTextureImage = true;
}

void Cylinder::loadImageTexture(const char *image_path1, const char *image_path2, const char *image_path3)
{
    m_numImages = 3;
    createLoadImagesToTexture(m_numImages);
    m_qImageToTexture[m_numImages-3].loadImageToTexture(image_path1);
    m_qImageToTexture[m_numImages-2].loadImageToTexture(image_path2);
    m_qImageToTexture[m_numImages-1].loadImageToTexture(image_path3);
    m_stateTextureImage = true;
}

void Cylinder::deleteImageTexture()
{
    if(!m_qImageToTexture.isEmpty()){
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

void Cylinder::enableImageTexture(bool state)
{
    if(m_qImageToTexture.isEmpty()){
        m_stateTextureImage = false;
    }
    else{
        m_stateTextureImage = state;
    }
}

void Cylinder::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void Cylinder::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void Cylinder::setDepthTextureBack(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void Cylinder::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}

void Cylinder::setEnableFixedAlpha(bool state)
{
        m_fixedAlpha = state;
}

void Cylinder::drawGeometry(TypeFilled mode , GLint idActivateTexture)
{
    glBindVertexArray(vaos);
    if(mode == points){
        if(m_statePointSize){
            glEnable(GL_PROGRAM_POINT_SIZE); // enable state GL_PROGRAM_POINT_SIZE
            glPointSize(m_pointSize);
        }
        else{
            glDisable(GL_PROGRAM_POINT_SIZE); // It changes the state to the default state (disable).
        }

        glDrawElementsInstanced(GL_POINTS, m_sizeIndices, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*0),m_numTotalObjects);
    }
    else{
        int accum_ind=0;
        glActiveTexture(GL_TEXTURE0+idActivateTexture);
        if(!m_stateTextureImage){
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idActivateTexture);
        }
        for(int i = 0; i < 3; i++) {
            if( m_numImages == 1 && i < 1){
                glBindTexture(GL_TEXTURE_2D, m_qImageToTexture[i].getTextureID());
                glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idActivateTexture);
            }
            if( m_numImages == 2 && i < 2){
                glBindTexture(GL_TEXTURE_2D, m_qImageToTexture[i].getTextureID());
                glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idActivateTexture);
            }
            if( m_numImages == 3 && i < 3){
                glBindTexture(GL_TEXTURE_2D, m_qImageToTexture[i].getTextureID());
                glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idActivateTexture);
            }
            if(i==0){
                glDrawElementsInstanced(GL_TRIANGLE_STRIP, m_vectorSizeSubindices[i], GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*accum_ind),m_numTotalObjects);
            }
            else{
                accum_ind = accum_ind + m_vectorSizeSubindices[i-1];
                glDrawElementsInstanced(  GL_TRIANGLE_FAN, m_vectorSizeSubindices[i], GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*accum_ind),m_numTotalObjects);
            }
        } // end for
    }
    glBindVertexArray(0);
}


void Cylinder::geometryRender( TypeFilled mode ){
    if(!m_stateShader){return;}
    GLuint idTextureActivated = m_getIDTextureActivated;

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));


    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateTexImage"), m_stateTextureImage);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"),  m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), m_fixedAlpha);

    vectorModelViewMatrixTexture.clear();
    vectorColorTexture.clear();
    for(int i=0;i<m_numTotalObjects;i++) {
        m_modelViewMatrixGeometry = m_translateMatrixGeometry[i] * m_rotateMatrixGeometry[i] * m_scaleMatrixGeometry[i];
        glm::mat4 initialScale = m_initialScaleMatrixGeometry[i]*m_scaleHeigthForAll[i]*m_scaleRadiusForAll[i];
        glm::mat4 initialViewMatrix =  m_initialPositionMatrixGeometry[i] * m_initialRotateMatrixGeometry[i] * initialScale;
        vectorModelViewMatrixTexture.push_back( m_modelViewMatrix*m_modelViewMatrixGeometry*initialViewMatrix);
        vectorColorTexture.push_back(m_color[i]);
    }

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

    drawGeometry(mode,idTextureActivated);
    m_lastIDTextureActivated = idTextureActivated;
}


void Cylinder::bindingRender()
{
    m_shaderProgram->binding();
}

void Cylinder::releasingRender()
{
    m_shaderProgram->releasing();
}
