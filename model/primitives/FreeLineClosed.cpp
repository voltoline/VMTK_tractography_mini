#include "FreeLineClosed.h"

FreeLineClosed::FreeLineClosed()
{

    m_shaderProgram = nullptr;
    m_sizeData = 0;
    m_sizeIndices = 0;
    m_numTotalObjects = 0;

    m_depthTexture = 0;
    m_depthTextureBack = 0;

    m_gData.clear();
    m_indices.clear();
    m_sizeSubindices.clear();
    m_initialPositionMatrixGeometry.clear();
    m_projectionMatrix = glm::mat4(1.0f);
    m_modelViewMatrix = glm::mat4(1.0f);

    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_statePointSize = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;
    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;

    vectorModelViewMatrixTexture.clear();

    vaos = 0;
}


FreeLineClosed::~FreeLineClosed(){
    if(vaos > 0){
        glDeleteBuffers(1, &ebo);
        ebo = 0;
        glDeleteBuffers(1, &vbo);
        vbo = 0;
        glDeleteVertexArrays(1, &vaos);
        vaos = 0;
        m_indices.clear();
        m_gData.clear();
        m_sizeSubindices.clear();
        vectorModelViewMatrixTexture.clear();
        m_initialPositionMatrixGeometry.clear();
    }
}

void FreeLineClosed::makeIndices(int size){
    m_indices.clear();
    for(int i=0; i<size; i++){
        m_indices.append( static_cast<GLushort>(i) );
    }
}

void FreeLineClosed::makeFreeLineClosed(QVector<glm::vec3> v){

    m_sizeSubindices.append( v.size() );

    QVector<PrimitiveData> data;
    data.reserve( v.size() );
    PrimitiveData p;
    for (int i=0; i< v.size(); i++) {
        p.verCoord = v.at(i);
        glm::vec3 tcV2 = vgf::coordVertToCoordTex(v.at(i));
        p.texCoord = glm::vec2(tcV2.x, tcV2.y);
        p.normal = glm::vec3(1);
        data.append(p);
    }
    m_gData = m_gData + data;
    m_sizeData = static_cast<GLushort>( m_gData.size() );
    makeIndices(m_sizeData);
    m_sizeIndices = static_cast<GLushort>( m_indices.size() );

}

void FreeLineClosed::createAndInitializeMatrices(){

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

void FreeLineClosed::createAndInitializeNewMatrix(){
    m_initialScaleMatrixGeometry.append(glm::mat4(1.0f));
    m_initialPositionMatrixGeometry.append(glm::mat4(1.0f));
    m_initialRotateMatrixGeometry.append(glm::mat4(1.0f));
    m_scaleMatrixGeometry.append(glm::mat4(1.0f));
    m_translateMatrixGeometry.append(glm::mat4(1.0f));
    m_rotateMatrixGeometry.append(glm::mat4(1.0f));
}


void FreeLineClosed::addFreeLineClosed(QVector<glm::vec3> points, glm::vec4 color)
{
    m_numTotalObjects++;
    createAndInitializeNewMatrix();
    m_color.append(color);
    makeFreeLineClosed(points); //add
    updatingBuffersGeometry();
}

void FreeLineClosed::removeMatriz(int id){

    m_initialScaleMatrixGeometry.removeAt(id);
    m_initialPositionMatrixGeometry.removeAt(id);
    m_initialRotateMatrixGeometry.removeAt(id);
    m_scaleMatrixGeometry.removeAt(id);
    m_translateMatrixGeometry.removeAt(id);
    m_rotateMatrixGeometry.removeAt(id);
}

int FreeLineClosed::sumSizesindices(int id){
    int sum = 0;
    for(int i=0;i< id; i++){
        sum = sum + m_sizeSubindices[i];
    }
    return sum;
}

void FreeLineClosed::removeFreeLineClosed(int id)
{
    if(id < 0 || id >= m_numTotalObjects){
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
     m_numTotalObjects--;
    if(id == 0){
        m_gData.remove(0,m_sizeSubindices[0]);
    }
    else{
        m_gData.remove(sumSizesindices(id-1),m_sizeSubindices[id]);
    }
    removeMatriz(id);
    m_color.removeAt(id);
    m_sizeSubindices.removeAt(id);
    m_sizeData = static_cast<GLushort>( m_gData.size() );
    makeIndices(m_sizeData);
    m_sizeIndices = static_cast<GLushort>( m_indices.size() );
    updatingBuffersGeometry();
}

void FreeLineClosed::removeLastMatriz(){
    m_initialScaleMatrixGeometry.removeLast();
    m_initialPositionMatrixGeometry.removeLast();
    m_initialRotateMatrixGeometry.removeLast();
    m_scaleMatrixGeometry.removeLast();
    m_translateMatrixGeometry.removeLast();
    m_rotateMatrixGeometry.removeLast();
}

void FreeLineClosed::removeLastFreeLineClosed()
{
    if(m_numTotalObjects == 0){
        std::cerr<<"Não existem linhas para remover."<<std::endl;
        return;
    }
    if(m_numTotalObjects == 1){
        m_gData.remove(0,m_sizeSubindices[0]);
    }
    else
    {
        m_gData.remove(m_gData.size()-m_sizeSubindices.last(),m_sizeSubindices.last());
    }
    m_numTotalObjects--;
    removeLastMatriz();
    m_color.removeLast();
    m_sizeSubindices.removeLast();
    m_sizeData = static_cast<GLushort>( m_gData.size() );
    makeIndices(m_sizeData);
    m_sizeIndices = static_cast<GLushort>( m_indices.size() );
    updatingBuffersGeometry();
}


void FreeLineClosed::setInitialScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void FreeLineClosed::setInitialScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_initialScaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void FreeLineClosed::setInitialPositionForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void FreeLineClosed::setInitialPositionForEach(int id, glm::vec3 vT){
    if(id < m_numTotalObjects){
        m_initialPositionMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void FreeLineClosed::setInitialRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixGeometry[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void FreeLineClosed::setInitialRotateForEach(int id, glm::vec3 vR){
    if(id < m_numTotalObjects){
        m_initialRotateMatrixGeometry[id] = vgf::rotationFromVectorDegrees(vR);
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void FreeLineClosed::setColorForAll(glm::vec4 color)
{
    for(int i = 0; i < m_numTotalObjects; i++){ m_color[i] = color; }
}

void FreeLineClosed::setColorForEach(int id, glm::vec4 color)
{
    if(id < m_color.size()){ m_color[id] = color; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void FreeLineClosed::setScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixGeometry[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void FreeLineClosed::setScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_scaleMatrixGeometry[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void FreeLineClosed::setTranslateForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixGeometry[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void FreeLineClosed::setTranslateForEach(int id, glm::vec3 vT){
    if(id < m_translateMatrixGeometry.size()){
        m_translateMatrixGeometry[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void FreeLineClosed::setRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixGeometry[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void FreeLineClosed::setRotateForEach(int id, glm::vec3 vR){
    if(id < m_rotateMatrixGeometry.size()){
        m_rotateMatrixGeometry[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}


void FreeLineClosed::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrix=projectionMatrix;
    m_modelViewMatrix=modelViewMatrix;
}

bool FreeLineClosed::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
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

bool FreeLineClosed::setShaderProgram(vmtkManagementShader *mShaderProgram)
{

    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void FreeLineClosed::setIDTextureActivated(GLint lta){
    m_getIDTextureActivated = lta;
}

GLint FreeLineClosed::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *FreeLineClosed::getShaderProgram() const
{
    return m_shaderProgram;
}


void FreeLineClosed::defaultResourceFile()
{
    QManagementResource *vsP =  new QManagementResource();
    QManagementResource *fsP =  new QManagementResource();
    GLuint vertex = vsP->compileResourceFileShader(QManagementResource::vertex,":/geometry_vert");
    GLuint fragment = fsP->compileResourceFileShader(QManagementResource::fragment,":/geometry_frag");

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


void FreeLineClosed::initializePrimitive()
{
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file."<<std::endl;
        defaultResourceFile();
    }
    createBuffers();
    initPrimitive();
}

void FreeLineClosed::createBuffers()
{
    /*create geometry buffers*/
    glGenVertexArrays(1, &vaos);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void FreeLineClosed::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void FreeLineClosed::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}


void FreeLineClosed::initPrimitive()
{
    glBindVertexArray(vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
#ifdef __APPLE__
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( OFFSETOF(PrimitiveData, PrimitiveData::verCoord)  ));
#else
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)( offsetof(PrimitiveData, verCoord) ) );
#endif

    glEnableVertexAttribArray(1);
#ifdef __APPLE__
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(OFFSETOF(PrimitiveData, PrimitiveData::texCoord)));
#else
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *)(offsetof(PrimitiveData, texCoord ) ) );
#endif

    glEnableVertexAttribArray(2);
#ifdef __APPLE__
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( OFFSETOF(PrimitiveData, PrimitiveData::normal)) );
#else
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData), (const void *) ( offsetof(PrimitiveData, normal)) );
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void FreeLineClosed::updatingBuffersGeometry(){
    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_gData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}

void FreeLineClosed::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void FreeLineClosed::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void FreeLineClosed::setDepthTextureBack(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void FreeLineClosed::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}


void FreeLineClosed::drawGeometry()
{
    glBindVertexArray(vaos);
    int accum_ind=0;
    for(int i=0;i<m_numTotalObjects;i++){
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ModelViewMatrix"), 1,
                           GL_FALSE, glm::value_ptr(vectorModelViewMatrixTexture[i]));
        glUniform4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "Color"), 1, glm::value_ptr(m_color[i]) );
        glDrawElements(GL_LINE_LOOP, m_sizeSubindices[i], GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*accum_ind));
        accum_ind=accum_ind+m_sizeSubindices[i];
    }
    glBindVertexArray(0);
}


void FreeLineClosed::geometryRender(){
    if(!m_stateShader){return;}
    GLuint idTextureActivated = m_getIDTextureActivated;

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateTexImage"), false);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"), m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), false);


//    // texture image null or zero.
    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, (GLuint) 0);
    glUniform1i(glGetUniformLocation( m_shaderProgram->getProgram(), "TextureImage"), idTextureActivated);
    idTextureActivated++;


    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTex"),idTextureActivated);
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTexBack"),idTextureActivated);
    idTextureActivated++;

    vectorModelViewMatrixTexture.clear();
    for(int i=0;i<m_numTotalObjects;i++) {
        m_modelViewMatrixGeometry = m_translateMatrixGeometry[i] * m_rotateMatrixGeometry[i] * m_scaleMatrixGeometry[i];
        glm::mat4 initialViewMatrix =  m_initialPositionMatrixGeometry[i] * m_initialRotateMatrixGeometry[i] * m_initialScaleMatrixGeometry[i];
        vectorModelViewMatrixTexture.push_back( m_modelViewMatrix*m_modelViewMatrixGeometry*initialViewMatrix);
    }

    drawGeometry();
    m_lastIDTextureActivated = idTextureActivated;
}


bool FreeLineClosed::bindingRender()
{
    return m_shaderProgram->binding();
}

void FreeLineClosed::releasingRender()
{
    m_shaderProgram->releasing();
}
