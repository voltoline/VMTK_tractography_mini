#include "LineP2P.h"

glm::vec4 LineP2P::m_defaultColor = glm::vec4(0.f,1.f,0.f,0.5f);
LineP2P::LineP2P()
{

    m_shaderProgram = nullptr;
    m_sizeData = 0;
    m_sizeIndices = 0;
    m_numTotalObjects = 0;
    ebo = 0;
    vbo = 0;
    vaos = 0;
    m_depthTexture = 0;
    m_depthTextureBack = 0;

    /** clear primitive vector data **/
    clearData();

    /** clear primitive vector matrix**/
    clearMatrices();
    /** clear vector settings **/
    clearSettings();

    m_projectionMatrixGlobal = glm::mat4(1.0f);
    m_modelViewMatrixGlobal = glm::mat4(1.0f);
    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_dataLineP2PCreated = false;
    initPrimitiveCreated = false;
    m_statePointSize = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;
    m_fixedAlpha = false;
    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;
    m_colorVector.clear();
    m_enableViewVector.clear();

}


LineP2P::~LineP2P(){

    /** delete buffers and vertex array **/
    if( ebo > 0 ){ glDeleteBuffers(1, &ebo); }
    if( vbo > 0 ){ glDeleteBuffers(1, &vbo); }
    if( vaos > 0 ){ glDeleteVertexArrays(1, &vaos); }

    /** clear primitive vector data **/
    clearData();

    /** clear primitive vector matrix**/
    clearMatrices();
    /** clear vector settings **/
    clearSettings();
}

void LineP2P::clearData(){
    vgf::freeMemoryVectorSTD(m_indices);
    vgf::freeMemoryVectorSTD(m_primitiveData);
    vgf::freeMemoryVectorSTD(m_sizeSubindices);
    vgf::freeMemoryVectorSTD(m_points);
}

void LineP2P::makeLineP2P(glm::vec3 point1, glm::vec3 point2)
{
    m_dataLineP2PCreated = true;
    std::vector<PrimitiveData> data;
    std::vector<glm::vec3> v = { point1, point2 };
    glm::vec3 n = glm::cross( v.at(0),v.at(1) );
    PrimitiveData pm;
    m_sizeSubindices.push_back( static_cast<int>(v.size()) );
    for (size_t i=0; i< v.size(); i++) {
        pm.verCoord = v.at(i);
        glm::vec3 tcV2 = vgf::coordVertToCoordTex(v.at(i));
        pm.texCoord = glm::vec2(tcV2.x, tcV2.y);
        pm.normal=n;
        data.push_back(pm);
    }    
    m_primitiveData = vgf::concatSTDVector(m_primitiveData , data);
    m_sizeData = static_cast<GLushort>( m_primitiveData.size() );
    makeIndices(m_sizeData);
    m_sizeIndices = static_cast<GLushort>( m_indices.size() );
    vgf::freeMemoryVectorSTD(v);
    vgf::freeMemoryVectorSTD(data);
}

void LineP2P::updateMakeLineP2P(size_t id, glm::vec3 point1, glm::vec3 point2)
{
    m_dataLineP2PCreated = true;
    std::vector<PrimitiveData> data;
    std::vector<glm::vec3> v = { point1, point2 };
    glm::vec3 n = glm::cross( v.at(0),v.at(1) );
    PrimitiveData pm;

    for (size_t i=0; i< v.size(); i++) {
        pm.verCoord = v.at(i);
        glm::vec3 tcV2 = vgf::coordVertToCoordTex(v.at(i));
        pm.texCoord = glm::vec2(tcV2.x, tcV2.y);
        pm.normal=n;
        data.push_back(pm);
    }
    int ind1, ind2;
    if(id==0){
        ind1 = 0;
    }
    else{
        ind1 =  sumSizesindices(id-1);
    }
    ind2 = ind1 + static_cast<int>(v.size());
    std::vector<PrimitiveData> lower, upper;
    vgf::splitSTDVector(m_primitiveData, ind1, ind2, lower, upper );
    m_primitiveData = vgf::concatSTDVector( vgf::concatSTDVector(lower, data), upper );
    m_sizeSubindices[id] =  ind2;

    vgf::freeMemoryVectorSTD(v);
    vgf::freeMemoryVectorSTD(data);
}

void LineP2P::createAndInitializeMatrices(){

    clearMatrices();
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_initialPositionMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_initialRotateMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_scaleMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_translateMatrixPrimitive.push_back(glm::mat4(1.0f));
        m_rotateMatrixPrimitive.push_back(glm::mat4(1.0f));
    }
}

void LineP2P::initializeNewMatrix(){
    m_initialScaleMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_initialPositionMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_initialRotateMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_scaleMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_translateMatrixPrimitive.push_back(glm::mat4(1.0f));
    m_rotateMatrixPrimitive.push_back(glm::mat4(1.0f));
}

void LineP2P::clearMatrices()
{
    vgf::freeMemoryVectorSTD( m_initialScaleMatrixPrimitive );
    vgf::freeMemoryVectorSTD( m_initialPositionMatrixPrimitive );
    vgf::freeMemoryVectorSTD( m_initialRotateMatrixPrimitive );
    vgf::freeMemoryVectorSTD( m_scaleMatrixPrimitive );
    vgf::freeMemoryVectorSTD( m_translateMatrixPrimitive );
    vgf::freeMemoryVectorSTD( m_rotateMatrixPrimitive );
}

void LineP2P::clearSettings()
{
    vgf::freeMemoryVectorSTD(m_colorVector);
    vgf::freeMemoryVectorSTD(m_enableViewVector);
}

void LineP2P::removeMatriz(size_t id)
{
    vgf::removeVectorElementByIndex( m_initialScaleMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_initialPositionMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_initialRotateMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_scaleMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_translateMatrixPrimitive, id );
    vgf::removeVectorElementByIndex( m_rotateMatrixPrimitive, id );
}

void LineP2P::removeSetting(size_t id)
{
    vgf::removeVectorElementByIndex( m_colorVector, id );
    vgf::removeVectorElementByIndex( m_enableViewVector, id );
}

void LineP2P::removeLastMatriz()
{
    m_initialScaleMatrixPrimitive.pop_back();
    m_initialPositionMatrixPrimitive.pop_back();
    m_initialRotateMatrixPrimitive.pop_back();
    m_scaleMatrixPrimitive.pop_back();
    m_translateMatrixPrimitive.pop_back();
    m_rotateMatrixPrimitive.pop_back();
}

void LineP2P::removeLastSetting()
{
    m_colorVector.pop_back();
    m_enableViewVector.pop_back();
}

void LineP2P::createAndInitializeSettings(glm::vec4 color, bool view)
{
    clearSettings();
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_colorVector.push_back(color);
        m_enableViewVector.push_back(view);
    }
}


void LineP2P::initializeNewSetting(glm::vec4 color, bool view){
    m_colorVector.push_back(color);
    m_enableViewVector.push_back(view);
}


void LineP2P::addLineP2P(glm::vec3 point1, glm::vec3 point2, glm::vec4 color, bool view)
{
    m_numTotalObjects++;
    initializeNewMatrix();
    initializeNewSetting(color,view);
    makeLineP2P(point1, point2);
    updatingBuffersPrimitive();
}

void LineP2P::addLineP2P(std::pair< glm::vec3, glm::vec3> points, glm::vec4 color, bool view)
{
    addLineP2P(points.first, points.second, color, view);
}

void LineP2P::updatePointsLineP2P(size_t id, glm::vec3 point1, glm::vec3 point2)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    updateMakeLineP2P(id, point1, point2);
    updatingBuffersPrimitive();
}

void LineP2P::updatePointsLineP2P(size_t id, std::pair< glm::vec3, glm::vec3> points)
{
    updatePointsLineP2P(id, points.first,points.second);
    updatingBuffersPrimitive();
}

void LineP2P::updateLastPointsLineP2P(glm::vec3 point1, glm::vec3 point2)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    size_t last_id = m_numTotalObjects;
    updateMakeLineP2P(last_id, point1, point2);
    updatingBuffersPrimitive();
}

void LineP2P::updateLastPointsLineP2P(std::pair< glm::vec3, glm::vec3> points)
{
    updateLastPointsLineP2P(points.first,points.second);
    updatingBuffersPrimitive();
}

void LineP2P::updateFirstPointLineP2P(size_t id, glm::vec3 point){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    size_t ind;
    if(id==0){ ind = 0; }
    else{ ind =  static_cast<size_t>( sumSizesindices(id-1) ); }
    updateMakeLineP2P(id, point, m_primitiveData[ind+1].verCoord);
    updatingBuffersPrimitive();
}


void LineP2P::updateSecondPointLineP2P(size_t id, glm::vec3 point)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    size_t ind;
    if(id==0){ ind = 0; }
    else{ ind =  static_cast<size_t>( sumSizesindices(id-1) ); }
    updateMakeLineP2P(id, m_primitiveData[ind].verCoord, point);
    updatingBuffersPrimitive();
}

void LineP2P::updateLastFirstPointsLineP2P(glm::vec3 point)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    size_t id = m_sizeSubindices.size()-1, ind;
    if(id==0){ ind = 0; }
    else{ ind =  static_cast<size_t>( sumSizesindices(id-1) ); }
    updateMakeLineP2P(id, point, m_primitiveData[ind+1].verCoord);
    updatingBuffersPrimitive();
}


void LineP2P::updateLastSecondPointsLineP2P(glm::vec3 point)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    size_t id = m_sizeSubindices.size()-1, ind;
    if(id==0){ ind = 0; }
    else{ ind =  static_cast<size_t>( sumSizesindices(id-1) ); }
    updateMakeLineP2P(id, m_primitiveData[ind].verCoord, point);
    updatingBuffersPrimitive();
}

void LineP2P::updateAllPrimitivesP2P(std::vector<glm::vec3> point1Vector, std::vector<glm::vec3> point2Vector, std::vector<glm::vec4> colorVector, std::vector<bool> viewVector){
    removeAllPrimitives();
    m_numTotalObjects = point1Vector.size();
    createAndInitializeMatrices();
    for(size_t i = 0; i < m_numTotalObjects; i++){
        makeLineP2P(point1Vector[i], point2Vector[i]);
       initializeNewSetting(colorVector[i],viewVector[i]);
    }
    updatingBuffersPrimitive();
}

void LineP2P::updateAllPrimitivesP2P(std::vector< std::pair< glm::vec3, glm::vec3>> points, std::vector<glm::vec4> colorVector, std::vector<bool> viewVector){
    std::vector<glm::vec3> vP1, vP2;
    vgf::pairToIndependentVector(points, vP1, vP2);
    updateAllPrimitivesP2P(vP1, vP2, colorVector,viewVector);
    updatingBuffersPrimitive();
}

void LineP2P::setInitialScaleForAll(glm::vec3 vS){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void LineP2P::setInitialScaleForLast(glm::vec3 vS){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_initialScaleMatrixPrimitive.back() = glm::scale( glm::mat4(1.0f), vS );
}

void LineP2P::setInitialScaleForEach(size_t id, glm::vec3 vS){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_initialScaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
}

void LineP2P::setInitialPositionForAll(glm::vec3 vT){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void LineP2P::setInitialPositionForLast(glm::vec3 vT){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_initialPositionMatrixPrimitive.back() = glm::translate( glm::mat4(1.0),  vT );
}

void LineP2P::setInitialPositionForEach(size_t id, glm::vec3 vT){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_initialPositionMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
}

void LineP2P::setInitialRotateForAll(glm::vec3 vR){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixPrimitive[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void LineP2P::setInitialRotateForLast(glm::vec3 vR){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_initialRotateMatrixPrimitive.back() = vgf::rotationFromVectorDegrees(vR);
}

void LineP2P::setInitialRotateForEach(size_t id, glm::vec3 vR){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_initialRotateMatrixPrimitive[id] = vgf::rotationFromVectorDegrees(vR);
}

void LineP2P::setColorForAll(glm::vec4 color)
{
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_colorVector[i] = color;
    }
}

void LineP2P::setColorForLast(glm::vec4 color)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_colorVector.back() = color;
}

void LineP2P::setColorForEach(size_t id, glm::vec4 color)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_colorVector[id] = color;
}

void LineP2P::setEnableViewForAll(bool enable)
{
    for(size_t i = 0; i < m_numTotalObjects; i++){  m_enableViewVector[i] = enable; }
}

void LineP2P::setEnableViewForEach(size_t id, bool enable)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_enableViewVector[id] = enable;
}

void LineP2P::setEnableViewForLast(bool enable)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_enableViewVector.back() = enable;
}

void LineP2P::setScaleForAll(glm::vec3 vS){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void LineP2P::setScaleForEach(size_t id, glm::vec3 vS){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_scaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
}

void LineP2P::setScaleForLast(glm::vec3 vS)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_scaleMatrixPrimitive.back() = glm::scale( glm::mat4(1.0f), vS );
}

void LineP2P::setTranslateForAll(glm::vec3 vT){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void LineP2P::setTranslateForEach(size_t id, glm::vec3 vT){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id >= m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_translateMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
}

void LineP2P::setTranslateForLast(glm::vec3 vT)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_translateMatrixPrimitive.back() = glm::translate( glm::mat4(1.0),  vT );
}

void LineP2P::setRotateForAll(glm::vec3 vR){
    for(size_t i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixPrimitive[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void LineP2P::setRotateForEach(size_t id, glm::vec3 vR){
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    m_rotateMatrixPrimitive[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
}

void LineP2P::setRotateForLast(glm::vec3 vR)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    m_rotateMatrixPrimitive.back() = glm::rotate( glm::mat4(1.0), 1.0f, vR );
}


void LineP2P::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrixGlobal=projectionMatrix;
    m_modelViewMatrixGlobal=modelViewMatrix;
}

bool LineP2P::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
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

bool LineP2P::setShaderProgram(vmtkManagementShader *mShaderProgram)
{
    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void LineP2P::setIDTextureActivated(GLuint lta){
    m_getIDTextureActivated = lta;
}

GLuint LineP2P::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *LineP2P::getShaderProgram() const
{
    return m_shaderProgram;
}

void LineP2P::defaultResourceFile()
{
    QManagementResource *vsP =  new QManagementResource();
    QManagementResource *fsP =  new QManagementResource();
    GLuint vertex = vsP->compileResourceFileShader(QManagementResource::vertex,":/primitiveLineP2P_vert");
    GLuint fragment = fsP->compileResourceFileShader(QManagementResource::fragment,":/primitiveLineP2P_frag");

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

void LineP2P::initializePrimitive()
{
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file (LineP2P)."<<std::endl;
        defaultResourceFile();
    }
    createBuffers();
    initPrimitive();
}

void LineP2P::createBuffers()
{
    /*create geometry buffers*/
    if( vaos == 0 ){ glGenVertexArrays(1, &vaos); }
    if( vbo == 0 ){ glGenBuffers(1, &vbo); }
    if( ebo == 0 ){  glGenBuffers(1, &ebo); }
}

void LineP2P::setEnableFixedAlpha(bool enableFixedAlpha){
    m_fixedAlpha = enableFixedAlpha;
}

void LineP2P::setModelViewMatrixGlobal(const glm::mat4 &modelViewMatrixGlobal)
{
    m_modelViewMatrixGlobal = modelViewMatrixGlobal;
}

void LineP2P::removeAllPrimitives()
{
    clearMatrices();
    clearSettings();
    clearData();
    m_numTotalObjects = 0;
    updatingBuffersPrimitive();
}

void LineP2P::removePrimitive(size_t id)
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }
    if(id > m_numTotalObjects){ std::cerr<<"Index out of range."<<std::endl; return; }
    removeMatriz(id);
    removeSetting(id);
    m_numTotalObjects--;

   if(id == 0){
       m_primitiveData.erase(m_primitiveData.begin(), m_primitiveData.begin() + m_sizeSubindices[0]);
   }
   else{
       m_primitiveData.erase(m_primitiveData.begin()+sumSizesindices(id-1),m_primitiveData.begin()+sumSizesindices(id));
   }
   vgf::removeVectorElementByIndex(m_sizeSubindices,id);
   m_sizeData = static_cast<GLushort>( m_primitiveData.size() );
   makeIndices(m_sizeData);
   m_sizeIndices = static_cast<GLushort>( m_indices.size() );
   updatingBuffersPrimitive();
}

void LineP2P::removeLastPrimitive()
{
    if( m_numTotalObjects == 0){ std::cerr<<"There are no lines created."<<std::endl; return; }

    if(m_numTotalObjects == 1){
        m_primitiveData.erase(m_primitiveData.begin(), m_primitiveData.begin() + m_sizeSubindices[0]);
    }
    else
    {
        int size = static_cast<int>( m_primitiveData.size() );
        m_primitiveData.erase( m_primitiveData.begin() + ( size - m_sizeSubindices.back() ),
                               m_primitiveData.begin() + m_sizeSubindices.back());
    }

    removeLastMatriz();
    removeLastSetting();
    m_numTotalObjects--;

    m_sizeSubindices.pop_back();
    m_sizeData = static_cast<GLushort>( m_primitiveData.size() );
    makeIndices(m_sizeData);
    m_sizeIndices = static_cast<GLushort>( m_indices.size() );
    updatingBuffersPrimitive();
}

void LineP2P::makeIndices(GLushort size){
    m_indices.clear();
    for(GLushort i=0; i<size; i++){
        m_indices.push_back(i );
    }
}

int LineP2P::sumSizesindices(size_t id)
{
    int sum = 0;
    for(size_t i=0;i< id; i++){
        sum = sum + m_sizeSubindices[i];
    }
    return sum;
}

void LineP2P::setProjectionMatrixGlobal(const glm::mat4 &projectionMatrixGlobal)
{
    m_projectionMatrixGlobal = projectionMatrixGlobal;
}

void LineP2P::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void LineP2P::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}

void LineP2P::initPrimitive()
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

void LineP2P::updatingBuffersPrimitive(){

    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_primitiveData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_primitiveData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}

void LineP2P::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void LineP2P::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void LineP2P::setDepthBackTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void LineP2P::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}


void LineP2P::drawPrimitive()
{
    glBindVertexArray(vaos);
    size_t accum_ind=0;
    for(size_t i=0;i<m_numTotalObjects;i++) {
        glm::mat4 modelViewMatrixPrimitive = m_translateMatrixPrimitive[i] * m_rotateMatrixPrimitive[i] * m_scaleMatrixPrimitive[i];
        glm::mat4 initialViewMatrix =  m_initialPositionMatrixPrimitive[i] * m_initialRotateMatrixPrimitive[i] * m_initialScaleMatrixPrimitive[i];
        glm::mat4 modelViewatrix = m_modelViewMatrixGlobal * modelViewMatrixPrimitive* initialViewMatrix;
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewatrix) );
        glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "EnableView"), m_enableViewVector[i]);
        glUniform4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "Color"), 1, glm::value_ptr(m_colorVector[i]) );
        glDrawElements(GL_LINES, m_sizeSubindices[i], GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid *>(sizeof(GLushort)*accum_ind) );
        accum_ind = accum_ind + static_cast<size_t>( m_sizeSubindices[i] );
    }
    glBindVertexArray(0);
}


void LineP2P::primitiveRender(){

    if( !m_stateShader ){ return; }

    GLuint idTextureActivated = m_getIDTextureActivated;
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrixGlobal));
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"), m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), m_fixedAlpha);

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTex"), static_cast<GLint>(idTextureActivated) );
    idTextureActivated++;

    glActiveTexture(GL_TEXTURE0+idTextureActivated);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "DepthTexBack"),static_cast<GLint>(idTextureActivated) );
    idTextureActivated++;
    drawPrimitive();
    m_lastIDTextureActivated = idTextureActivated;
}


bool LineP2P::bindingRender()
{
    return m_shaderProgram->binding();
}

void LineP2P::releasingRender()
{
    m_shaderProgram->releasing();
}
