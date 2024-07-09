#include "Circle.h"

Circle::Circle()
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

    m_projectionMatrix = glm::mat4(1.0f);
    m_modelViewMatrix = glm::mat4(1.0f);

    m_pointSize = 1.0;
    m_stateAddedShader = false;
    m_stateShader = false;
    m_dataCircleCreated = false;
    m_statePointSize = false;
    m_stateTextureImage = false;
    m_stateDepthTexture = false;
    m_stateDepthTextureBack = false;
    m_fixedAlpha = false;
    //    m_stateTexture3D = false;
    m_getIDTextureActivated = 0;
    m_lastIDTextureActivated = 0;
    vectorEnableViewTexture.clear();
    vectorColorTexture.clear();
    vectorModelViewMatrixTexture.clear();
    m_translateMatrixPrimitive.clear();
    m_modeDraw = TypeFilled::solid;
}


Circle::~Circle(){
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
}

void Circle::initializePrimitive(TypeFilled mode)
{
    if(!m_stateAddedShader){
        std::cerr<<"Load default shader file."<<std::endl;
        defaultResourceFile();
    }

    m_modeDraw = mode;
    createBuffers();
    initPrimitive();
}

void Circle::createBuffers()
{
    /*create primitive buffers*/
    glGenVertexArrays(1, &vaos);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Circle::initPrimitive()
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
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveData),
                           (const void *)(offsetof(PrimitiveData, PrimitiveData::texCoord ) ) );
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

    // Create the view buffer, initialize it and attach it to the buffer texture
    glGenBuffers(1, &enableViewBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glGenTextures(1, &enableViewBufferObject);
    glBindTexture(GL_TEXTURE_BUFFER, enableViewBufferObject);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, enableViewBuffer);
    m_enableViewSamplerLocationID = glGetUniformLocation(m_shaderProgram->getProgram(), "enableViewTexture");

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

    // End codes commented  //
}

void Circle::makeCircle(GLushort resolution)
{
    m_dataCircleCreated = true;


    GLfloat radius = 1.0f;
    GLfloat h = 0.0f;
    GLushort it = resolution;
    m_sizeData = it * 3+3;
    m_sizeIndices = it * 3+3 ;
    m_gData.clear();
    m_gData.reserve(m_sizeData);
    m_indices.clear();
    m_indices.reserve(m_sizeIndices);

    int ind=0, c=0;

    if(m_modeDraw == TypeFilled::solid || m_modeDraw == TypeFilled::points ||  m_modeDraw == TypeFilled::line_strip){
        glm::vec3 centerV(0);
        glm::vec2 centerT(0.5,0.5);
        glm::vec2 posT_temp(0);
        glm::vec3 posV_temp(0);


        for(int i = 0; i<=it;i++){
            float angle = static_cast<float>(2*M_PI) / static_cast<float>(it);
            int ni=0; if(i==it){ni=0;}else{ni=i;}
            float x = sinf(ni * angle);
            float y = cosf(ni * angle);

            glm::vec2 tCoord(x, y);
            tCoord = vgf::coordVertToCoordTex(tCoord);
            glm::vec3 vCoord( glm::vec3(x*radius, y*radius, h ));
            glm::vec3 normals = glm::vec3(0, 0, 1);
            normals = glm::normalize(normals);
            if(i==0){
                posT_temp = tCoord;
                posV_temp = vCoord;
            }
            else{
                m_gData[c].verCoord = centerV;
                m_gData[c].texCoord = centerT;
                m_gData[c].normal = normals;
                m_indices[ind] = static_cast<GLushort>(ind);
                c++; ind++;
                m_gData[c].verCoord = posV_temp;
                m_gData[c].texCoord = posT_temp;
                m_gData[c].normal = normals;
                m_indices[ind] = static_cast<GLushort>(ind);
                c++; ind++;
                m_gData[c].verCoord = vCoord;
                m_gData[c].texCoord = tCoord;
                m_gData[c].normal = normals;
                m_indices[ind] = static_cast<GLushort>(ind);
                c++; ind++;
                posT_temp = tCoord;
                posV_temp = vCoord;
            }
        }

    }
    else{

        for(int i = 0; i<it;i++){
            float angle = static_cast<float>(2*M_PI) / static_cast<float>(it);
            int ni=0; if(i==it){ni=0;}else{ni=i;}
            float x = sinf(ni * angle);
            float y = cosf(ni * angle);

            glm::vec2 tCoord(x, y);
            tCoord = vgf::coordVertToCoordTex(tCoord);
            glm::vec3 vCoord( glm::vec3(x*radius, y*radius, h ));
            glm::vec3 normals = glm::vec3(0, 0, 1);
            normals = glm::normalize(normals);
            m_gData[c].verCoord = vCoord;
            m_gData[c].texCoord = tCoord;
            m_gData[c].normal = normals;
            m_indices[ind] = static_cast<GLushort>(ind);
            c++; ind++;
        }
    }

}

void Circle::clearMatrices(){
    m_initialScaleMatrixPrimitive.clear();
    m_initialPositionMatrixPrimitive.clear();
    m_initialRotateMatrixPrimitive.clear();
    m_scaleMatrixPrimitive.clear();
    m_translateMatrixPrimitive.clear();
    m_rotateMatrixPrimitive.clear();
}

void Circle::clearSettings(){
    m_color.clear();
    m_scaleRadiusList.clear();
    m_enableViewList.clear();
}

void Circle::removeAllCircles(){
    clearMatrices();
    clearSettings();
    m_numTotalObjects = 0;
}

void Circle::removeMatriz(int id){

    m_initialScaleMatrixPrimitive.removeAt(id);
    m_initialPositionMatrixPrimitive.removeAt(id);
    m_initialRotateMatrixPrimitive.removeAt(id);
    m_scaleMatrixPrimitive.removeAt(id);
    m_translateMatrixPrimitive.removeAt(id);
    m_rotateMatrixPrimitive.removeAt(id);
}

void Circle::removeSetting(int id){
    m_color.removeAt(id);
    m_scaleRadiusList.removeAt(id);
    m_enableViewList.removeAt(id);
}

void Circle::removeCircle(int id){
    if(id < 0 || id >= m_numTotalObjects){
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
    removeMatriz(id);
    removeSetting(id);
    m_numTotalObjects--;
}

void Circle::removeLastMatriz(){
    m_initialScaleMatrixPrimitive.removeLast();
    m_initialPositionMatrixPrimitive.removeLast();
    m_initialRotateMatrixPrimitive.removeLast();
    m_scaleMatrixPrimitive.removeLast();
    m_translateMatrixPrimitive.removeLast();
    m_rotateMatrixPrimitive.removeLast();
}

void Circle::removeLastSetting(){
    m_color.removeLast();
    m_scaleRadiusList.removeLast();
    m_enableViewList.removeLast();
}

void Circle::removeLastCircle(){
    if(m_numTotalObjects == 0){
        std::cerr<<"."<<std::endl;
        return;
    }
    removeLastMatriz();
    removeLastSetting();
    m_numTotalObjects--;
}

void Circle::createAndInitializeMatrices(){
    clearMatrices();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive.append(glm::mat4(1.0f));
        m_initialPositionMatrixPrimitive.append(glm::mat4(1.0f));
        m_initialRotateMatrixPrimitive.append(glm::mat4(1.0f));
        m_scaleMatrixPrimitive.append(glm::mat4(1.0f));
        m_translateMatrixPrimitive.append(glm::mat4(1.0f));
        m_rotateMatrixPrimitive.append(glm::mat4(1.0f));
    }
}

void Circle::createAndInitializeNewMatrix(){
    m_initialScaleMatrixPrimitive.append(glm::mat4(1.0f));
    m_initialPositionMatrixPrimitive.append(glm::mat4(1.0f));
    m_initialRotateMatrixPrimitive.append(glm::mat4(1.0f));
    m_scaleMatrixPrimitive.append(glm::mat4(1.0f));
    m_translateMatrixPrimitive.append(glm::mat4(1.0f));
    m_rotateMatrixPrimitive.append(glm::mat4(1.0f));
}


void Circle::createAndInitializeSettings(glm::vec4 color, GLfloat scaleRadius){
    clearSettings();
    for(int i = 0; i < m_numTotalObjects; i++){
        m_color.append(color);
        m_scaleRadiusList.append(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) ));
        m_enableViewList.append(true);
    }
}

void Circle::createAndInitializeNewSetting(glm::vec4 color, GLfloat scaleRadius){
    m_color.append(color);
    m_scaleRadiusList.append(glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) ));
    m_enableViewList.append(true);
}

void Circle::addCirclesDefaultForAll(GLushort numCircles, GLfloat scaleRadius, GLushort resolution, glm::vec4 color)
{
    m_numTotalObjects = numCircles;
    if(!m_dataCircleCreated){
        makeCircle(resolution);
        updatingBuffersPrimitive();
    }
    createAndInitializeMatrices();
    createAndInitializeSettings(color,scaleRadius);
    updatingInstance();
}

void Circle::addFirstCircleSettingResolution(GLfloat scaleRadius, GLushort resolution, glm::vec4 color)
{
    m_numTotalObjects++;
    if(!m_dataCircleCreated){
        makeCircle(resolution);
        updatingBuffersPrimitive();
    }
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleRadius);
    updatingInstance();
}

void Circle::addCircle(GLfloat scaleRadius, glm::vec4 color)
{
    m_numTotalObjects++;
    if(!m_dataCircleCreated){
        makeCircle(24);
        updatingBuffersPrimitive();
    }
    createAndInitializeNewMatrix();
    createAndInitializeNewSetting(color,scaleRadius);
    updatingInstance();
}

void Circle::setColorForAll(glm::vec4 color)
{
    for(int i = 0; i < m_numTotalObjects; i++){ m_color[i] = color; }
}

void Circle::setColorForEach(int id, glm::vec4 color)
{
    if(id < m_color.size()){ m_color[id] = color; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Circle::setColorForLast(glm::vec4 color)
{
     m_color.last() = color;
}

void Circle::setEnableViewForAll(bool enable)
{
    for(int i = 0; i < m_numTotalObjects; i++){  m_enableViewList[i] = enable; }
}

void Circle::setEnableViewForEach(int id, bool enable)
{
    if(id < m_enableViewList.size()) { m_enableViewList[id] = enable; }
    else{ std::cerr<<"Index out of range."<<std::endl; return; }
}

void Circle::setEnableViewForLast(bool enable)
{
     m_enableViewList.last() = enable;
}

QList<bool> Circle::getEnableViewList(){
    return m_enableViewList;
}

void Circle::setScaleRadiusForAll(GLfloat scaleRadius){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleRadiusList[i] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
    }
}

void Circle::setScaleRadiusForEach(int id, GLfloat scaleRadius){
    if(id < m_numTotalObjects){
        m_scaleRadiusList[id] = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setScaleRadiusForLast(GLfloat scaleRadius){
        m_scaleRadiusList.last() = glm::scale( glm::mat4(1.0f), glm::vec3(scaleRadius, scaleRadius, scaleRadius) );
}

void Circle::setInitialScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialScaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Circle::setInitialScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_initialScaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setInitialScaleForLast(glm::vec3 vS){
        m_initialScaleMatrixPrimitive.last() = glm::scale( glm::mat4(1.0f), vS );
}

void Circle::setInitialPositionForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialPositionMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Circle::setInitialPositionForEach(int id, glm::vec3 vT){
    if(id < m_numTotalObjects){
        m_initialPositionMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setInitialPositionForLast(glm::vec3 vT){
        m_initialPositionMatrixPrimitive.last() = glm::translate( glm::mat4(1.0),  vT );
}


void Circle::setInitialRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_initialRotateMatrixPrimitive[i] = vgf::rotationFromVectorDegrees(vR);
    }
}

void Circle::setInitialRotateForEach(int id, glm::vec3 vR){
    if(id < m_numTotalObjects){
        m_initialRotateMatrixPrimitive[id] = vgf::rotationFromVectorDegrees(vR);
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setInitialRotateForLast(glm::vec3 vR){
        m_initialRotateMatrixPrimitive.last() = vgf::rotationFromVectorDegrees(vR);
}

void Circle::setScaleForAll(glm::vec3 vS){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_scaleMatrixPrimitive[i]= glm::scale( glm::mat4(1.0f), vS );
    }
}

void Circle::setScaleForEach(int id, glm::vec3 vS){
    if(id < m_numTotalObjects){
        m_scaleMatrixPrimitive[id] = glm::scale( glm::mat4(1.0f), vS );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setTranslateForAll(glm::vec3 vT){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_translateMatrixPrimitive[i] = glm::translate( glm::mat4(1.0),  vT );
    }
}

void Circle::setTranslateForEach(int id, glm::vec3 vT){
    if(id < m_translateMatrixPrimitive.size()){
        m_translateMatrixPrimitive[id] = glm::translate( glm::mat4(1.0),  vT );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setRotateForAll(glm::vec3 vR){
    for(int i = 0; i < m_numTotalObjects; i++){
        m_rotateMatrixPrimitive[i] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
}

void Circle::setRotateForEach(int id, glm::vec3 vR){
    if(id < m_rotateMatrixPrimitive.size()){
        m_rotateMatrixPrimitive[id] = glm::rotate( glm::mat4(1.0), 1.0f, vR );
    }
    else{
        std::cerr<<"Index out of range."<<std::endl;
        return;
    }
}

void Circle::setGlobalTransformations(glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_modelViewMatrix = modelViewMatrix;
}

bool Circle::addResourceFile(QManagementResource::TypeOfShader tos, const char *resourceFile)
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

bool Circle::setShaderProgram(vmtkManagementShader *mShaderProgram)
{

    m_shaderProgram = mShaderProgram;
    if(m_shaderProgram==nullptr || m_shaderProgram->getProgram()==0){
        delete m_shaderProgram;
        return m_stateAddedShader = m_stateShader = false;
    }
    return m_stateAddedShader = m_stateShader = true;
}

void Circle::setIDTextureActivated(GLint lta){
    m_getIDTextureActivated = lta;
}

GLint Circle::getLastTextureActivated(){
    return m_lastIDTextureActivated;
}


vmtkManagementShader *Circle::getManagementShader() const
{
    return m_shaderProgram;
}


void Circle::defaultResourceFile()
{
    QManagementResource *vsG =  new QManagementResource();
    QManagementResource *fsG =  new QManagementResource();
    GLuint vertex = vsG->compileResourceFileShader(QManagementResource::vertex,":/primitive_object_vert");
    GLuint fragment = fsG->compileResourceFileShader(QManagementResource::fragment,":/primitive_object_frag");

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



int Circle::getNumTotalObjects() const
{
    return m_numTotalObjects;
}

void Circle::setStatePointSize(bool statePointSize)
{
    m_statePointSize = statePointSize;
}

void Circle::setPointSize(GLfloat pointSize)
{
    m_pointSize = pointSize;
}



void Circle::updatingInstance(){
    // update the buffer and attach it to the buffer texture

    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(bool), vectorEnableViewTexture.data(), GL_DYNAMIC_DRAW);


    glBindBuffer(GL_TEXTURE_BUFFER, colorBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::vec4), vectorColorTexture.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_TEXTURE_BUFFER, modelViewMatrixBuffer);
    glBufferData(GL_TEXTURE_BUFFER, m_numTotalObjects * sizeof(glm::mat4), vectorModelViewMatrixTexture.data(), GL_DYNAMIC_DRAW);
}

void Circle::updatingBuffersPrimitive(){
    glBindVertexArray( vaos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sizeData * sizeof(PrimitiveData), m_gData.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_sizeData * sizeof(PrimitiveData), m_gData.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeIndices * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER , 0, m_sizeIndices * sizeof(GLushort), m_indices.data());
    glBindVertexArray(0);
}



void Circle::createLoadImagesToTexture(int numImages){
    for(int i = 0; i<numImages; i++){
        m_qImageToTexture.append(QLoadImage());
    }
}

void Circle::loadImageTexture(const char *image_path1)
{
    createLoadImagesToTexture(1);
    m_qImageToTexture[0].loadImageToTexture(image_path1);
    m_stateTextureImage = true;
}


void Circle::deleteImageTexture()
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

void Circle::enableImageTexture(bool state)
{
    if(m_qImageToTexture.isEmpty()){
        m_stateTextureImage = false;
    }
    else{
        m_stateTextureImage = state;
    }
}

void Circle::setDepthTexture(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTexture = idTexture;
        m_stateDepthTexture = true;
    }
}

void Circle::enableDepthTexture(bool state)
{
    if(m_depthTexture == 0){
        m_stateDepthTexture = false;
    }
    else{
        m_stateDepthTexture = state;
    }
}

void Circle::setDepthTextureBack(GLuint idTexture)
{
    if(idTexture != 0){
        m_depthTextureBack = idTexture;
        m_stateDepthTextureBack = true;
    }
}

void Circle::enableDepthTextureBack(bool state)
{
    if(m_depthTextureBack == 0){
        m_stateDepthTextureBack = false;
    }
    else{
        m_stateDepthTextureBack = state;
    }
}

void Circle::setEnableFixedAlpha(bool state)
{
        m_fixedAlpha = state;
}

void Circle::drawPrimitive( GLint idActivateTexture)
{
    glBindVertexArray(vaos);
    if(m_modeDraw == points){
        if(m_statePointSize){
            glEnable(GL_PROGRAM_POINT_SIZE); // enable state GL_PROGRAM_POINT_SIZE
            glPointSize(m_pointSize);
        }
        else
        {
            glDisable(GL_PROGRAM_POINT_SIZE); // It changes the state to the default state (disable).
        }
        glDrawElementsInstanced(GL_POINTS, m_sizeIndices, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*0),m_numTotalObjects);
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
        if(m_modeDraw == TypeFilled::solid){
            glDrawElementsInstanced(  GL_TRIANGLES, m_sizeIndices, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*0),m_numTotalObjects);
        }
        if(m_modeDraw == TypeFilled::line_strip){
            glDrawElementsInstanced(  GL_LINE_STRIP, m_sizeIndices, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*0),m_numTotalObjects);
        }
        if(m_modeDraw == TypeFilled::wire){
            glDrawElementsInstanced(  GL_LINE_STRIP, m_sizeIndices, GL_UNSIGNED_SHORT, (const GLvoid *)(sizeof(GLushort)*0),m_numTotalObjects);
        }
    }
    glBindVertexArray(0);
}


void Circle::primitiveRender(){


    if(!m_stateShader){return;}
    GLint idTextureActivated = m_getIDTextureActivated;

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->getProgram(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateTexImage"), m_stateTextureImage);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTex"),  m_stateDepthTexture);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "StateDepthTexBack"), m_stateDepthTextureBack);
    glUniform1i(glGetUniformLocation(m_shaderProgram->getProgram(), "fixedAlpha"), m_fixedAlpha);

    vectorEnableViewTexture.clear();
    vectorColorTexture.clear();
    vectorModelViewMatrixTexture.clear();
    for(int i=0;i<m_numTotalObjects;i++) {
            m_modelViewMatrixPrimitive = m_translateMatrixPrimitive[i] * m_rotateMatrixPrimitive[i] * m_scaleMatrixPrimitive[i];
            glm::mat4 initialScale = m_initialScaleMatrixPrimitive[i] * m_scaleRadiusList[i];
            glm::mat4 initialViewMatrix =  m_initialPositionMatrixPrimitive[i] * m_initialRotateMatrixPrimitive[i] * initialScale;
            vectorModelViewMatrixTexture.push_back( m_modelViewMatrix*m_modelViewMatrixPrimitive*initialViewMatrix);
            bool visible = m_enableViewList.at(i);
            glm::vec4 color = m_color[i];

            if(!visible){
                color.a = 0.0;
            }
            vectorColorTexture.push_back( color );
            vectorEnableViewTexture.push_back( visible );
    }

    // Bind the weight TBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, enableViewBuffer);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(bool)*m_numTotalObjects, vectorEnableViewTexture.data());
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

    drawPrimitive(idTextureActivated);
    m_lastIDTextureActivated = idTextureActivated;
}


bool Circle::bindingRender()
{
    return m_shaderProgram->binding();
}

void Circle::releasingRender()
{
    m_shaderProgram->releasing();
}
