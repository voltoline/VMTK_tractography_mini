#include "vmtkGeneralFunctions.h"

glm::vec3 vmtkGeneralFunctions::getVecScaleFromMat4Scale(glm::mat4 m)
{ return glm::vec3((float) m[0][0]/m[3][3], (float) m[1][1]/m[3][3], (float) m[2][2]/m[3][3]); }

vmath::Vector2<float> vmtkGeneralFunctions::glmTovmathV2(glm::vec2 v)
{
    return vmath::Vector2<float>(v[0],v[1]);
}

vmath::Vector3f vmtkGeneralFunctions::glmTovmathV3(glm::vec3 v)
{
    return vmath::Vector3f(v[0],v[1],v[2]);
}

glm::vec3 vmtkGeneralFunctions::vmathToglmV3(vmath::Vector3f v)
{
    return glm::vec3(v[0],v[1],v[2]);
}

vmath::Vector4f vmtkGeneralFunctions::glmTovmathV4(glm::vec4 v)
{
    return vmath::Vector4f(v[0],v[1],v[2],v[3]);
}

glm::vec4 vmtkGeneralFunctions::vmathToglmV4(vmath::Vector4f v)
{
    return glm::vec4(v[0],v[1],v[2],v[3]);
}

glm::vec4 vmtkGeneralFunctions::v3Tov4(glm::vec3 v)
{
    return glm::vec4(v,1.0f);
}

void vmtkGeneralFunctions::v3Tov4( float *v3, float *v4)
{
    v4[0] = v3[0]; v4[1] = v3[1]; v4[2] = v3[2]; v4[3] = 1.0f;
}

glm::vec3 vmtkGeneralFunctions::v4Tov3(glm::vec4 v)
{
    return glm::vec3(v[0]/v[3],v[1]/v[3],v[2]/v[3]);
}

void vmtkGeneralFunctions::v4Tov3( float *v4, float *v3)
{
    if( v4[3] == 0.0f ){ v4[3] = 1.0f; }
    v3[0] = v4[0]/v4[3]; v3[1] = v4[1]/v4[3]; v3[2] = v4[2]/v4[3];
}

void vmtkGeneralFunctions::vec3ToArray(glm::vec3 v, float *f){
    f[0] = v[0]; f[1] = v[1]; f[2] = v[2];
}

glm::mat4 vmtkGeneralFunctions::mat3GLMTomat4GLM(glm::mat3 m3){
    glm::mat4 m4;
    m4[0] = glm::vec4(m3[0], 0);
    m4[1] = glm::vec4(m3[1], 0);
    m4[2] = glm::vec4(m3[2], 0);
    m4[3] = glm::vec4(0, 0, 0, 1);
    return m4;
}

float vmtkGeneralFunctions::coordVertToCoordTex(float value)
{
    return (value+1)/2;
}

glm::vec2 vmtkGeneralFunctions::coordVertToCoordTex(glm::vec2 v)
{
    return glm::vec2(coordVertToCoordTex(v.x),coordVertToCoordTex(v.y));
}

glm::vec3 vmtkGeneralFunctions::coordVertToCoordTex(glm::vec3 v)
{
    return glm::vec3(coordVertToCoordTex(v.x),coordVertToCoordTex(v.y),coordVertToCoordTex(v.z));
}

glm::vec4 vmtkGeneralFunctions::coordVertToCoordTex(glm::vec4 v4)
{
    glm::vec3 v = vgf::v4Tov3(v4);
    return  v3Tov4(coordVertToCoordTex(v));
}

float vmtkGeneralFunctions::toCoordVert(float value)
{
    return (value*2)-1;
}

glm::vec2 vmtkGeneralFunctions::toCoordVert(glm::vec2 v)
{
    return glm::vec2(toCoordVert(v.x),toCoordVert(v.y));
}

glm::vec3 vmtkGeneralFunctions::toCoordVert(glm::vec3 v)
{
    return glm::vec3(toCoordVert(v.x),toCoordVert(v.y),toCoordVert(v.z));
}

float vmtkGeneralFunctions::coordVertToCoordTexFactor(float value, float scaleFactor)
{
    return value/(2.*scaleFactor) + 0.5;
}

glm::vec3 vmtkGeneralFunctions::coordVertToCoordTexFactor(glm::vec3 v, glm::vec3 sF)
{
    return glm::vec3(coordVertToCoordTexFactor(v.x,sF.x),
                           coordVertToCoordTexFactor(v.y,sF.y),
                           coordVertToCoordTexFactor(v.z,sF.z));
}

void vmtkGeneralFunctions::resetViewportToSquare(int width, int height){
    if (width < height) {
        glViewport (0, (height-width)/2, width, width);
    } else {
        glViewport ((width-height)/2, 0, height, height);
    }
}

void vmtkGeneralFunctions::getCurrentViewport(GLint (&viewport)[4]){
    glGetIntegerv( GL_VIEWPORT, viewport);
}

glm::vec4 vmtkGeneralFunctions::getCurrentViewport(){
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport);
    return glm::make_vec4(viewport);
}



void vmtkGeneralFunctions::resetAndUpdateViewportToSquare(int width, int height, GLint (&viewport)[4]){
    resetViewportToSquare(width, height);
    getCurrentViewport(viewport);
}


glm::vec2 vmtkGeneralFunctions::getNormalizedPositionFromMouseCoordinate(glm::vec2 mousePosition, int height){
    GLfloat winX, winY;
    GLint vp[4]; getCurrentViewport(vp);
    orientedMousePosition(mousePosition, height, winX, winY);
    return normalizedPosition(vp,winX,winY);
}

glm::vec2 vmtkGeneralFunctions::getNormalizedPositionFromMouseCoordinate(int x ,int y, int height){
    GLfloat winX, winY;
    GLint vp[4]; getCurrentViewport(vp);
    orientedMousePosition(glm::vec2((float)x,(float)y), height, winX, winY);
    return normalizedPosition(vp,winX,winY);
}

glm::vec2 vmtkGeneralFunctions::getNormalizedPositionFromMouseCoordinate(glm::vec2 mousePosition, GLint viewport[4], int height){
    GLfloat winX, winY;
    orientedMousePosition(mousePosition, height, winX, winY);
    return normalizedPosition(viewport,winX,winY);
}

void vmtkGeneralFunctions::orientedMousePosition(glm::vec2 pos2D, int height, GLfloat &winX, GLfloat &winY){
    winX = (float) pos2D.x;
    winY = (float) height - (float) pos2D.y;
}

bool vmtkGeneralFunctions::validatePositionOnViewport(GLint viewport[4], int winX, int winY){
    if(winX<viewport[0] || winX>viewport[0]+viewport[2]
            || winY<viewport[1] || winY>viewport[1]+viewport[3]){ return false; }
    else{ return true; }
}

glm::vec2 vmtkGeneralFunctions::normalizedPosition(GLint viewport[4], GLfloat winX, GLfloat winY){
    glm::vec2 p2DNormalized( (winX-(float)viewport[0])/(float)viewport[2], (winY-(float)viewport[1])/(float)viewport[3] );
    return p2DNormalized;
}

void vmtkGeneralFunctions::printVec3(glm::vec3 v){
    std::cout<<"["<<v.x<<","<<v.y<<","<<v.z<<"]"<<std::endl;
}

glm::mat4 vmtkGeneralFunctions::rotationFromVectorDegrees(glm::vec3 v){

    // Reversing angles to correct orientation.
    glm::vec3 vAnglesRadians = -glm::radians(v);
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), vAnglesRadians.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), vAnglesRadians.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), vAnglesRadians.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rot = rotX*rotY*rotZ;

    return rot;
}

void vmtkGeneralFunctions::displacementToCameraFront(glm::mat4 mv, float &nearCamera, float &farCamera)
{
    glm::vec4 tmp[8];

    tmp[0][0]=-1.0; tmp[0][1]=-1.0; tmp[0][2]=-1.0; tmp[0][3] = 1.0;
    tmp[1][0]=1.0; tmp[1][1]=-1.0; tmp[1][2]=-1.0; tmp[1][3] = 1.0;
    tmp[2][0]=1.0; tmp[2][1]=1.0; tmp[2][2]=-1.0; tmp[2][3] = 1.0;
    tmp[3][0]=-1.0; tmp[3][1]=1.0; tmp[3][2]=-1.0; tmp[3][3] = 1.0;
    tmp[4][0]=-1.0; tmp[4][1]=-1.0; tmp[4][2]=1.0; tmp[4][3] = 1.0;
    tmp[5][0]=1.0; tmp[5][1]=-1.0; tmp[5][2]=1.0; tmp[5][3] = 1.0;
    tmp[6][0]=1.0; tmp[6][1]=1.0; tmp[6][2]=1.0; tmp[6][3] = 1.0;
    tmp[7][0]=-1.0; tmp[7][1]=1.0; tmp[7][2]=1.0; tmp[7][3] = 1.0;

    nearCamera = -1.f;  // arbitrary large number
    farCamera = 1.f;
    for (int i = 0; i < 8; i++) {
        tmp[i] = mv*tmp[i];
        if (tmp[i][2] > nearCamera){ nearCamera = tmp[i][2]; }
        if (tmp[i][2] < farCamera){ farCamera = tmp[i][2]; }
    }
}

glm::mat4 vmtkGeneralFunctions::makeNearProjectionMatrix(glm::mat4 mv, float left, float right, float bottom, float top){
    glm::mat4 traslateAux, orthoAux;
    float zNear, zFar;
    displacementToCameraFront( mv, zNear, zFar);
    if (zNear > 0.f && zFar < 0.f) {
        traslateAux = glm::translate( glm::mat4(1.0f), glm::vec3(0.f, 0.f, -zNear) );
        orthoAux = glm::ortho(left, right, bottom, top, 0.f, zNear-zFar);
    } else {
        traslateAux = glm::translate( glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f) );
        orthoAux = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    return orthoAux * traslateAux;
}

GLint vmtkGeneralFunctions::currentActivateTexture(){
    GLint cAT;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &cAT);
    return cAT-GL_TEXTURE0;
}



glm::vec3 vmtkGeneralFunctions::computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    return glm::fastNormalize(glm::cross(c - a, b - a));
}

glm::ivec2 vmtkGeneralFunctions::orientedMousePosition(glm::ivec2 pos2D, int height){
    return glm::ivec2(pos2D.x, height - pos2D.y);
}

void vmtkGeneralFunctions::resetAndGetViewportToSquare(int width, int height, GLint (&viewport)[4]){
    resetViewportToSquare(width, height);
    getCurrentViewport(viewport);
}

void vmtkGeneralFunctions::resetAndGetViewportToSquare(int width, int height, glm::vec4 &viewport){
    GLint vp[4];
    resetViewportToSquare(width, height);
    getCurrentViewport(vp);
    viewport = glm::make_vec4( vp );
}

glm::vec4 vmtkGeneralFunctions::resetAndGetViewportToSquare(int width, int height){
    resetViewportToSquare(width, height);
    return getCurrentViewport();
}

bool vmtkGeneralFunctions::validatePositionOnViewport(glm::vec4 viewport, int winX, int winY){
    if(winX<viewport[0] || winX>viewport[0]+viewport[2]
            || winY<viewport[1] || winY>viewport[1]+viewport[3]){ return false; }
    else{ return true; }
}

void vmtkGeneralFunctions::setViewport(glm::vec4 viewport){
    glm::ivec4 v = viewport;
    glViewport(v.x,v.y,v.z,v.w);
}


std::vector<int> vmtkGeneralFunctions::vectorBool2VectorInt(std::vector<bool> vb)
{
    return std::vector<int>(vb.begin(), vb.end());
}




float vmtkGeneralFunctions::distanceFromVertToCoordTexFactor(glm::vec3 v1, glm::vec3 v2, glm::vec3 sF){
    glm::vec3 vf1 = coordVertToCoordTexFactor(v1,sF);
    glm::vec3 vf2 = coordVertToCoordTexFactor(v2,sF);
    return glm::distance(vf1,vf2);
}



/** geometry functions **/

glm::vec3 vmtkGeneralFunctions::posView2DFrom3DPos(glm::vec3 global3DPosition, AxisPlane axisViewPlane, ViewOrientation viewOrientation)
{
    glm::vec3 p = global3DPosition, pv2d(0.0f);
    if(viewOrientation == ViewOrientation::LAS){
        if(axisViewPlane == AxisPlane::Sagittal) pv2d = glm::vec3(p.y, p.z, p.x);
        if(axisViewPlane == AxisPlane::Coronal) pv2d = glm::vec3(p.x, p.z, p.y);
        if(axisViewPlane == AxisPlane::Axial) pv2d = glm::vec3(p.x, -p.y, p.z);
    }
    else{
        if(axisViewPlane == AxisPlane::Sagittal) pv2d = glm::vec3(-p.y, p.z, p.x);
        if(axisViewPlane == AxisPlane::Coronal) pv2d = glm::vec3(-p.x, p.z, p.y);
        if(axisViewPlane == AxisPlane::Axial) pv2d = glm::vec3(-p.x, -p.y, p.z);
    }
    return pv2d;
}

glm::vec4 vmtkGeneralFunctions::planeView2DFrom3DPos(float posPlane, vmtkGeneralFunctions::AxisPlane axisViewPlane)
{
    glm::vec4 plane(0.0f);
    if(axisViewPlane == AxisPlane::Sagittal) plane = glm::vec4(1, 0, 0, posPlane);
    if(axisViewPlane == AxisPlane::Coronal) plane = glm::vec4(0, 1, 0, posPlane);
    if(axisViewPlane == AxisPlane::Axial) plane = glm::vec4(0, 0, 1, posPlane);
    return plane;
}

glm::vec4 vmtkGeneralFunctions::planeView2DFrom3DPos(glm::vec3 global3DPosition, vmtkGeneralFunctions::AxisPlane axisViewPlane)
{
    glm::vec3 p = global3DPosition;
    glm::vec4 plane(0.0f);
    if(axisViewPlane == AxisPlane::Sagittal) plane = glm::vec4(1, 0, 0, p.x);
    if(axisViewPlane == AxisPlane::Coronal) plane = glm::vec4(0, 1, 0, p.y);
    if(axisViewPlane == AxisPlane::Axial) plane = glm::vec4(0, 0, 1, p.z);
    return plane;
}

glm::vec3 vmtkGeneralFunctions::normalFromPlaneEq(glm::vec4 planeEquation){
    return glm::vec3(planeEquation.x, planeEquation.y, planeEquation.z);
}

float vmtkGeneralFunctions::distancePointPlane(glm::vec3 p, glm::vec4 Plane) // por ver
{
    glm::vec3 n = normalFromPlaneEq(Plane);
    return ( glm::dot(n, p) - Plane.w ) / sqrtf( glm::dot(n, n) ) ;
}

/**
 * @brief vmtkGeneralFunctions::radiusCircleInterPlaneSphere
 * @param planaEquation Plane equation in format glm::vec4
 * @param positionSphere Center of the sphere
 * @param radiusSphere Radius of the sphere.
 * @param typeIntersected Type integer. If value is: 1 (is intersected) , 2 (is tangency), 3 (is not intersected).
 * @return
 */
float vmtkGeneralFunctions::radiusCircleInterPlaneSphere(glm::vec4 planaEquation, glm::vec3 positionSphere, float radiusSphere, int &typeIntersected){
    float distanceSpherePlane = distancePointPlane(positionSphere, planaEquation);
    if(radiusSphere>distanceSpherePlane){
        typeIntersected = 0;
        return  sqrtf(radiusSphere*radiusSphere - distanceSpherePlane*distanceSpherePlane);
    }
    else if( isApproximatelyEqual(radiusSphere, distanceSpherePlane) )
    {
        typeIntersected = 1;
        return 0.0;
    }
    else{
        typeIntersected = 2;
        return 0.0;
    }
}

float vmtkGeneralFunctions::radiusCircleInterPlaneSphere(glm::vec4 planaEquation, glm::vec3 positionSphere, float radiusSphere){
    int typeIntersected = 0;
    return radiusCircleInterPlaneSphere( planaEquation, positionSphere, radiusSphere, typeIntersected);
}

glm::vec3 vmtkGeneralFunctions::ClosestPointOnPlane(glm::vec4 plane, glm::vec3 point)
{
    float distance = distancePointPlane(point, plane);
    return point - distance*normalFromPlaneEq(plane);
}


float vmtkGeneralFunctions::randfromARange(double fMin, double fMax)
{
    double f = static_cast<double>(rand())/ RAND_MAX;
    return static_cast<float>(fMin + f * (fMax - fMin));
}

/** Line and Plane **/

bool vmtkGeneralFunctions::isLineLiesOnThePlaneAxis(Line L, AxisPlane AP){
    bool state = false;
    glm::vec3 p1 = L.initialPoint, p2 = L.finalPoint;
    if( AP == AxisPlane::Axial && isApproximatelyEqual( p1.z, p2.z ) ){ state = true; }
    if( AP == AxisPlane::Coronal && isApproximatelyEqual( p1.y, p2.y ) ){ state = true; }
    if( AP == AxisPlane::Sagittal && isApproximatelyEqual( p1.x, p2.x ) ){ state = true; }
    return state;
}

int vmtkGeneralFunctions::intersectionLinePLane( Line L, Plane Pl, glm::vec3 &intersectionPoint ){
    glm::vec3 u = L.direction;
    glm::vec3 w = L.initialPoint - Pl.pointPl;

    float D = dot(Pl.normalPl, u);
    float N = -dot(Pl.normalPl, w);

    if (std::fabs(D) < std::numeric_limits<float>::epsilon()) {   // segment is parallel to plane
        if ( isApproximatelyEqual (N, 0.f) )                      // segment lies in plane
            return 2;
        else
            return 0;                    // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    intersectionPoint = L.initialPoint + sI * u;                  // compute segment intersect point
    return 1;
}

