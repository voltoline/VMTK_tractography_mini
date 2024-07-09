#include "Rectangle_Square.h"

Rectangle_Square::Rectangle_Square()
{

    m_shaderProgram = nullptr;
    m_sizeData = 0;
    m_sizeIndices = 0;
    m_numTotalObjects = 0;
    modelVewMatrixTextureBufferObject = 0;
    colorTextureBufferObject = 0;
    m_colorSamplerLocationID = 0;
    m_modelVewMatrixSamplerLocationID = 0;
    m_depthTexture = 0;
    m_depthTextureBack = 0;

    m_qImageToTexture = nullptr;
    createLoadImageToTexture(m_qImageToTexture);

    m_gData.clear();
    m_indices.clear();
    m_scaleHeigthForAll.clear();
    m_scaleWidthForAll.clear();
    m_initialPositionMatrixGeometry.clear();
    m_projectionMatrix = glm::mat4(1.0f);
    m_modelViewMatrix = glm::mat4(1.0f);

    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_dataRectangleCreated = false;
    m_statePointSize = false;
    m_stateTextureImage = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;

    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;
    vectorModelViewMatrixTexture.clear();
    vectorColorTexture.clear();
}


Rectangle_Square::~Rectangle_Square(){
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
        m_indices.clear();
        m_gData.clear();
        vectorModelViewMatrixTexture.clear();
        vectorColorTexture.clear();
        m_initialPositionMatrixGeometry.clear();
        m_scaleHeigthForAll.clear();
        m_scaleWidthForAll.clear();
        delete m_qImageToTexture;
    }
}

void Rectangle_Square::makeRectangle(){

    m_dataRectangleCreated = true;
    m_sizeData = 4;
    m_sizeIndices = 4;
    m_gData.clear();
    m_gData.reserve(m_sizeData);
    m_indices.clear();
    m_indices.reserve(m_sizeIndices);

    std::vector<glm::vec3> v;
    v.push_back( glm::vec3( 1.0f,  1.0f,  0.0f) );
    v.push_back( glm::vec3(-1.0f,  1.0f,  0.0f) );
    v.push_back( glm::vec3(-1.0f, -1.0f,  0.0f) );
    v.push_back( glm::vec3( 1.0f, -1.0f,  0.0f) );
    glm::vec3 n = vgf::computeNormal(v.at(0), v.at(1),v.at(2));

    for (GLuint i=0; i< v.size(); i++) {
        m_gData[i].verCoord = v.at(i);
        glm::vec3 tcV2 = vgf::coordVertToCoordTex(v.at(i));
        m_gData[i].texCoord = glm::vec2(tcV2.x, tcV2.y);
        m_gData[i].normal=n;
        m_indices[i] = static_cast<GLushort>(i);
    }
    v.clear();
}

void Rectangle_Square::createAndInitializeMatrices(){

    m_initialScaleMatrixGeometry.clear();
    m_initialPositionMatrixGeometry.clear();
    m_initialRotateMatrixGeometry.clear();
    m_scaleMatrixGeometry.clear();
    m_translateMatrixGeometry.clear();
    m_rotateMatrixGeometry.clear();
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixGeometry.push_back(glm::mat4(1.0f));
        m_initialPositionMatrixGeometry.push_back(glm::mat4(1.0f));
        m_initialRotateMatrixGeometry.push_back(glm::mat4(1.0f));
        m_scaleMatrixGeometry.push_back(glm::mat4(1.0f));
        m_translateMatrixGeometry.push_back(glm::mat4(1.0f));
        m_rotateMatrixGeometry.push_back(glm::mat4(1.0f));
    }
}

void Rectangle_Square::createAndInitializeNewMatrix(){
    m_initialScaleMatrixGeometry.push_back(glm::mat4(1.0f));
    m_initialPositionMatrixGeometry.push_back(glm::mat4(1.0f));
    m_initialRotateMatrixGeometry.push_back(glm::mat4(1.0f));
    m_scaleMatrixGeometry.push_back(glm::mat4(1.0f));
    m_translateMatrixGeometry.push_back(glm::mat4(1.0f));
    m_rotateMatrixGeometry.push_back(glm::mat4(1.0f));
}


void Rectangle_Square::createAndInitializeSettings(glm::vec4 color, GLfloat scaleWidth, GLfloat scaleHeight){
    m_color.clear();
    m_scaleWidthForAll.clear();
    m_scaleHeigthForAll.clear();
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_color.push_back(color);
        m_scaleWidthForAll.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(scaleWidth, 1.0f, 1.0f) ));
        m_scaleHeigthForAll.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) ));
    }
}

void Rectangle_Square::createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleWidth, GLfloat scaleHeight){
    m_color.push_back(color);
    m_scaleWidthForAll.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(scaleWidth, 1.0f, 1.0f) ));
    m_scaleHeigthForAll.push_back(glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) ));
}


void Rectangle_Square::addRectanglesDefaultForAll(GLushort numRectangles, GLfloat scaleWidth, GLfloat scaleHeight, glm::vec4 color)
{
    m_numTotalObjects = numRectangles;
    if(!m_dataRectangleCreated){
        makeRectangle();
        initializePrimitive();
    }
    createAndInitializeMatrices();
    createAndInitializeSettings(color,scaleWidth,scaleHeight);
}

void Rectangle_Square::addRectangleCustomized(GLfloat scaleWidth, GLfloat scaleHeight, glm::vec4 color)
{
    m_numTotalObjects++;
    if(!m_dataRectangleCreated){
        makeRectangle();
        initializePrimitive();
    }
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleWidth,scaleHeight);
}

void Rectangle_Square::setScaleWidthForAll(GLfloat scaleWidth){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_scaleWidthForAll[i] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleWidth, 1.0f, 1.0f) );
    }
}

void Rectangle_Square::setScaleWidthForEach(GLuint id, GLfloat scaleWidth){
    if( id < m_numTotalObjects ){
        m_scaleWidthForAll[id] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleWidth, 1.0f, 1.0f) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setScaleHeigthForAll(GLfloat scaleHeight){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_scaleHeigthForAll[i] =  glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) );
    }
}

void Rectangle_Square::setScaleHeigthForEach(GLuint id, GLfloat scaleHeight){
    if (id < m_numTotalObjects ){
        m_scaleHeigthForAll[id] = glm::scale( glm::mat4(1.0f), glm::vec3(1.0f, scaleHeight, 1.0f) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setInitialScaleForAll(glm::vec3 vS){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Rectangle_Square::setInitialScaleForEach(GLuint id, glm::vec3 vS){
    if( id < m_numTotalObjects ){
        m_initialScaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setInitialPositionForAll(glm::vec3 vT){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Rectangle_Square::setInitialPositionForEach(GLuint id, glm::vec3 vT){
    if( id < m_numTotalObjects ){
        m_initialPositionMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setInitialRotateForAll(glm::vec3 vR){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixGeometry[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void Rectangle_Square::setInitialRotateForEach(GLuint id, glm::vec3 vR){
    if( id < m_numTotalObjects ){
        m_initialRotateMatrixGeometry[id] = vgf::rotationFromVectorDegrees(vR);
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setColorForAll(glm::vec4 color)
{
    for(GLuint i = 0; i < m_numTotalObjects; i++){ m_color[i] = color; }
}

void Rectangle_Square::setColorForEach(GLuint id, glm::vec4 color)
{
    if(id < m_color.size()){ m_color[id] = color; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Rectangle_Square::setScaleForAll(glm::vec3 vS){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Rectangle_Square::setScaleForEach(GLuint id, glm::vec3 vS){
    if( id < m_numTotalObjects ){
        m_scaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setTranslateForAll(glm::vec3 vT){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Rectangle_Square::setTranslateForEach(GLuint id, glm::vec3 vT){
    if(id < m_translateMatrixGeometry.size()){
        m_translateMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Rectangle_Square::setRotateForAll(glm::vec3 vR){
    for(GLuint i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixGeometry[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void Rectangle_Square::setRotateForEach(GLuint id, glm::vec3 vR){
    if(id < m_rotateMatrixGeometry.size()){
        m_rotateMatrixGeometry[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}


void Rectangle_Square::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrix=projectionMatrix;
    m_modelViewMatrix=modelViewMatrix;
}

bool Rectangle_Square::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
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

        deletePointer(m_shaderProgram)
                return m_stateAddedShader = m_stateShader = false;

    }
    else{ return m_stateAddedShader = m_stateShader = true; }
}

bool Rectangle_Square::setShaderProgram(vmtkManagementShader *mShaderProgram)
{

    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void Rectangle_Square::setIDTextureActivated(GLuint lta){
    m_getIDTextureActivated = lta;
}

GLuint Rectangle_Square::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *Rectangle_Square::getShaderProgram() const
{
    return m_shaderProgram;
}


void Rectangle_Square::defaultResourceFile()
{
    QManagementResource *vsP =  new QManagementResource();
    QManagementResource *fsP =  new QManagementResource();
    GLuint vertex = vsP->compileResourceFileShader(QManagementResource::vertex,":/primitive_object_vert");
    GLuint fragment = fsP->compileResourceFileShader(QManagementResource::fragment,":/primitive_object_frag");

    if( vertex==0 || fragment==0)
    {
        m_stateShader=false;
    }
    m_shaderProgram = new vmtkManagementShader;
    m_shaderProgram->setShader(vertex);
    m_shaderProgram->setShader(fragment);
    if(!m_shaderProgram->linking()){
        m_stateShader = false;
        deletePointer(m_shaderProgram);
        std::cerr<<"The program "<< m_shaderProgram->getProgram() << " not compiled."<<std::endl;
    }
    else{ m_stateShader = true; }
    deletePointer(vsP);
    deletePointer(fsP);
}


void Rectangle_Square::initializePrimitive()
{
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file (Rectangle_Square)."<<std::endl;
        defaultResourceFile();
    }
    createBuffers();
    initPrimitive();
}

void Rectangle_Square::createBuffers()
{
    /*create geometry buffers*/
    glGenVertexArrays(1, &vaos);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Rectangle_Square::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void Rectangle_Square::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}

void Rectangle_Square::initPrimitive()
{
    glBindVertexArray(vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
#ifdef __APPLE__
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( OFFSETOF(PrimitiveData, PrimitiveData::verCoord)  ));
#else
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *>( offsetof(PrimitiveData, verCoord) ) );
#endif

    glEnableVertexAttribArray(1);
#ifdef __APPLE__
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(OFFSETOF(PrimitiveData, PrimitiveData::texCoord)));
#else
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *>(offsetof(PrimitiveData, texCoord ) ) );
#endif

    glEnableVertexAttribArray(2);
#ifdef __APPLE__
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( OFFSETOF(PrimitiveData, PrimitiveData::normal)) );
#else
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), reinterpret_cast<const GLvoid *>( offsetof(PrimitiveData, normal)) );
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);


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


void Rectangle_Square::updatingBuffersGeometry(){
    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_gData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}

void Rectangle_Square::createLoadImageToTexture(QLoadImage* qli){
    if(qli==nullptr){
        qli = new QLoadImage();
    }
}

void Rectangle_Square::loadImageTexture(const char *image_path1)
{
    m_qImageToTexture->loadImageToTexture(image_path1);
    m_stateTextureImage = true;
}

void Rectangle_Square::deleteImageTexture()
{
    if(m_qImageToTexture != nullptr){
        delete m_qImageToTexture;
        m_stateTextureImage = false;
    }
    else{
        if(m_stateTextureImage == true){
            m_stateTextureImage = false;
        }
    }
}

void Rectangle_Square::enableImageTexture(bool state)
{
    if(m_qImageToTexture == nullptr){
        m_stateTextureImage = false;
    }
    else{
        m_stateTextureImage = state;
    }
}

void Rectangle_Square::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void Rectangle_Square::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void Rectangle_Square::setDepthTextureBack(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void Rectangle_Square::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}




void Rectangle_Square::drawGeometry(TypeFilled mode , GLuint idActivateTexture)
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
        glDrawElementsInstanced(GL_POINTS, m_sizeIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid *>(sizeof(GLushort)*0), static_cast<int>(m_numTotalObjects));
    }
    else{

        glActiveTexture(GL_TEXTURE0+idActivateTexture);
        if(m_stateTextureImage){
            glBindTexture(GL_TEXTURE_2D, m_qImageToTexture->getTextureID());
        }
        else{
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), static_cast<int>(idActivateTexture) );

        glDrawElementsInstanced(  GL_TRIANGLE_FAN, m_sizeIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid *>(sizeof(GLushort)*0),static_cast<int>(m_numTotalObjects));
    }
    glBindVertexArray(0);
}


void Rectangle_Square::geometryRender( TypeFilled mode ){
    if(!m_stateShader){return;}
    GLuint idTextureActivated = m_getIDTextureActivated;

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateTexImage"), m_stateTextureImage);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"), m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), /*m_fixedAlpha*/ false);

    vectorColorTexture.clear();
    vectorModelViewMatrixTexture.clear();
    for(GLuint i=0;i<m_numTotalObjects;i++) {
        m_modelViewMatrixGeometry = m_translateMatrixGeometry[i] * m_rotateMatrixGeometry[i] * m_scaleMatrixGeometry[i];
        glm::mat4 initialScale = m_initialScaleMatrixGeometry[i]*m_scaleHeigthForAll[i]*m_scaleWidthForAll[i];
        glm::mat4 initialViewMatrix =  m_initialPositionMatrixGeometry[i] * m_initialRotateMatrixGeometry[i] * initialScale;
        vectorModelViewMatrixTexture.push_back( m_modelViewMatrix*m_modelViewMatrixGeometry*initialViewMatrix);
        vectorColorTexture.push_back(m_color[i]);
    }

    // Bind the weight TBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(glm::vec4)*m_numTotalObjects, vectorColorTexture.data());
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_BUFFER, colorTextureBufferObject);
    glUniform1i(m_colorSamplerLocationID, static_cast<GLint>(idTextureActivated));
    idTextureActivated++;

    // Bind the weight VBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(glm::mat4)*m_numTotalObjects, vectorModelViewMatrixTexture.data());
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_BUFFER, modelVewMatrixTextureBufferObject);
    glUniform1i(m_modelVewMatrixSamplerLocationID, static_cast<GLint>(idTextureActivated));
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTex"),static_cast<GLint>(idTextureActivated));
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTexBack"),static_cast<GLint>(idTextureActivated));
    idTextureActivated++;


    drawGeometry(mode,idTextureActivated);
    m_lastIDTextureActivated = idTextureActivated;
}


bool Rectangle_Square::bindingRender()
{
    return m_shaderProgram->binding();
}

void Rectangle_Square::releasingRender()
{
    m_shaderProgram->releasing();
}
