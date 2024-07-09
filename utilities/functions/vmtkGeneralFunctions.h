#ifndef VMTKGENERALFUNCTIONS_H
#define VMTKGENERALFUNCTIONS_H
#include "utilities/QCheckingOpenGL.h"
#include "utilities/vmath.h"
#include <list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <cassert>
#include "modules/file_reader/dicom/vmtkAcquisition.h"
#include <QString>

#define deletePointer(x) {delete x; x = nullptr;}
#define varName(x) #x
#define cast_offset(X) ((const GLvoid *) (std::uintptr_t) (X))
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT) )
#define repeat(n) for (int _##n = 0; _##n < n; _##n++)
#define repetir(typename, n) for (typename i = 0; i < n; i++)
class vmtkGeneralFunctions
{

public:
    static glm::vec3 getVecScaleFromMat4Scale(glm::mat4 m);
    static vmath::Vector2f glmTovmathV2(glm::vec2 v);
    static vmath::Vector3f glmTovmathV3(glm::vec3 v);
    static glm::vec3 vmathToglmV3(vmath::Vector3f v);
    static glm::vec4 v3Tov4(glm::vec3 v);
    static void v3Tov4(float *v3, float *v4);
    static glm::vec3 v4Tov3(glm::vec4 v);
    static void v4Tov3(float *v4, float *v3);
    static vmath::Vector4f glmTovmathV4(glm::vec4 v);
    static glm::vec4 vmathToglmV4(vmath::Vector4f v);
    static void vec3ToArray(glm::vec3 v, float *f);
    static float coordVertToCoordTex(float value);
    static glm::vec2 coordVertToCoordTex(glm::vec2 v);
    static glm::vec3 coordVertToCoordTex(glm::vec3 v);
    static glm::vec4 coordVertToCoordTex(glm::vec4 v4);
    static float toCoordVert(float value);
    static glm::vec2 toCoordVert(glm::vec2 v);
    static glm::vec3 toCoordVert(glm::vec3 v);
    static void resetViewportToSquare(int width, int height);
    static void getCurrentViewport(GLint (&viewport)[4]);
    static void resetAndUpdateViewportToSquare(int width, int height, GLint (&viewport)[4]);
    static void printVec3(glm::vec3 v);
    static glm::mat4 rotationFromVectorDegrees(glm::vec3 v);


    /**
 * @brief Compute the nearst and furtherst z-value in VCR
 * @param[in] mv modelview transformation matrix
 * @param[out] nearCamera nearest z
 * @param[out] farCamera furtherst z
 */
    static void displacementToCameraFront(glm::mat4 mv, float &nearCamera, float &farCamera);
    static glm::mat4 makeNearProjectionMatrix(glm::mat4 mv, float left, float right, float bottom, float top);
    static void orientedMousePosition(glm::vec2 pos2D, int height, GLfloat &winX, GLfloat &winY);
    static bool validatePositionOnViewport(GLint viewport[], int winX, int winY);
    static glm::vec2 normalizedPosition(GLint viewport[], GLfloat winX, GLfloat winY);
    static glm::vec2 getNormalizedPositionFromMouseCoordinate(glm::vec2 mousePosition, GLint viewport[], int height);
    static glm::vec2 getNormalizedPositionFromMouseCoordinate(int x, int y, int height);
    static glm::vec2 getNormalizedPositionFromMouseCoordinate(glm::vec2 mousePosition, int height);
    static float coordVertToCoordTexFactor(float value, float scaleFactor);
    static glm::vec3 coordVertToCoordTexFactor(glm::vec3 v, glm::vec3 sF);
    static GLint currentActivateTexture();


    static glm::vec3 computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    static glm::ivec2 orientedMousePosition(glm::ivec2 pos2D, int height);
    static void resetAndGetViewportToSquare(int width, int height, GLint (&viewport)[4]);
    static void resetAndGetViewportToSquare(int width, int height, glm::vec4 &viewport);
    static glm::vec4 resetAndGetViewportToSquare(int width, int height);
    static bool validatePositionOnViewport(glm::vec4 viewport, int winX, int winY);
    static void setViewport(glm::vec4 viewport);
    static glm::vec4 getCurrentViewport();

    static std::vector<int> vectorBool2VectorInt(std::vector<bool> vb);

    static glm::mat4 mat3GLMTomat4GLM(glm::mat3 m3);

    static float distanceFromVertToCoordTexFactor(glm::vec3 v1, glm::vec3 v2, glm::vec3 sF);

    /** functions using template (only in header) **/
    template <typename T, int N>
    static void coordVertToCoordTex(T v[N],T (&vt)[N]){
        assert(N<=4);
        if( N == 4 ){
            T v_temp[N-1];
            vmtkGeneralFunctions::v4Tov3(v,v_temp);
            for(int i = 0; i < N-1; i++){
                v_temp[i] = vmtkGeneralFunctions::coordVertToCoordTex(v_temp[i]);
            }
            vmtkGeneralFunctions::v3Tov4(v_temp, vt);
        }
        else{
            for(int i = 0; i < N; i++){
                vt[i] = vmtkGeneralFunctions::coordVertToCoordTex(v[i]);
            }
        }
    }

    template <typename T, int N>
    static void divArray(T v1[N], T v2[N],float (&vd)[N]){
        assert(N<=4);
        for(int i=0; i<N; i++){
            assert(v2[i]>0);
            vd[i] = (float) v1[i]/v2[i];
        }
    }

    template <typename T, int N>
    static void printArray(T (&v)[N]){
        std::cout<<"[";
        for(int i=0; i<N; i++){ if(i==N-1){ std::cout<<v[i]; }else{ std::cout<<v[i]<<", "; } }
        std::cout<<"]"<<std::endl;
    }

    template <typename T, int N, typename genType>
    static void arrayToVecGLM(T (&v_array)[N], genType &v_glm){
        if(N != v_glm.length()){
            std::cerr<<"Error: arrayToVecGLM(T, genTYpe): parameters size not correct."<<std::endl;
            return;
        }
        for(int i=0; i<N; i++){
            v_glm[i] = v_array[i];
        }
    }


    template <typename T, int N>
    static void initialUniqueValueArray( T value, T (&v)[N])
    {
        if(std::is_same<T, int>::value)
            std::cout<<"is int"<<std::endl;
        if (std::is_same<T, double>::value)
            std::cout<<"is double"<<std::endl;
        if (std::is_same<T, long double>::value)
            std::cout<<"is long double"<<std::endl;
        if (std::is_same<T, float>::value)
            std::cout<<"is float"<<std::endl;
        for(int i=0; i<N; i++){
            v[i]=value;
        }
    }


    template <typename T>
    static void tvec4ToArray(glm::tvec4<T> v, T *f){

        for(int i=0; i< v.length(); i++){
            f[i]=v[i];
        }

    }

    template <typename T>
    static void tvec3ToArray(glm::tvec3<T> v, T *f){

        for(int i=0; i< v.length(); i++){
            f[i]=v[i];
        }

    }

    template <typename T>
    static void tvec2ToArray(glm::tvec2<T> v, T *f){

        for(int i=0; i< v.length(); i++){
            f[i]=v[i];
        }

    }

    template<typename genType>
    static QString vecToString(const genType& v)
    {
        QString out;
        for(int i=0; i< v.length(); i++){
            if(i == v.length()-1){
                out =  out + QString::number(v[i]);
            }
            else{
                out = out + QString::number(v[i])+" ";
            }
        }
        return out;
    }

    template<typename genType>
    static QString vecToString(const genType v, QString separateElemets)
    {
        QString out;
        for(int i=0; i< v.length(); i++){
            if(i == v.length()-1){
                out = out + QString::number( v[i] );
            }
            else{
                out = out + QString::number( v[i] ) + separateElemets;
            }
        }
        return out;
    }



    static std::vector<std::string> split(const std::string& s, char delimiter)
    {
       std::vector<std::string> tokens;
       std::string token;
       std::istringstream tokenStream(s);
       while (std::getline(tokenStream, token, delimiter))
       {
          tokens.push_back(token);
       }
       return tokens;
    }

    template<typename T,  template<typename> class C>
    static QSet<T> toQSet(const C<T> &container)
    {
      return QSet<T>(container.begin(), container.end());
    }


    template<typename T,  template<typename> class C>
    static std::vector<T> toStdVector(const C<T> &container)
    {
      return std::vector<T>(container.begin(), container.end());
    }

    static std::string removeSpaces(std::string str)
    {
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        return str;
    }

    template<typename genType>
    static void string2VecGLM(std::string str, genType &v){

        std::size_t f = str.find("(");
        std::size_t l = str.find(")");
        std::string str2 = str.substr (f+1,l-f-1);
        str2 = removeSpaces(str2);
        std::vector<std::string> vs = split(str2,',');
        if(vs.size()!=v.length()){
            std::cerr<<"The number of data and the vector are not the same."<<std::endl;
            return;
        }
        for(int i=0; i< v.length(); i++){
            v[i] = stof(vs[i]);
        }
    }

    template <typename T>
    static std::vector<T> concatSTDVector(std::vector<T> a, std::vector<T> b) {
        std::vector<T> ret = std::vector<T>();
        copy(a.begin(), a.end(), back_inserter(ret));
        copy(b.begin(), b.end(), back_inserter(ret));
        return ret;
    }

    template<typename T>
    static void stringToNumber(const std::string& numberAsString, T &valor){
        std::stringstream stream(numberAsString);
        stream >> valor;
        if (stream.fail()) {
            std::runtime_error e(numberAsString);
            throw e;
        }
    }

    template<typename T>
    static std::string numtoString(const T& value)
    {
        std::ostringstream oss(std::stringstream::out);
        oss << value;
        return oss.str();
    }

    template <typename T>
    static std::list<T> vector2List(std::vector<T> &a) {
        std::list<T> ret = std::list<T>();
        copy(a.begin(), a.end(), back_inserter(ret));
        return ret;
    }

    template <typename T>
    static std::vector<T> list2Vector(std::list<T> &a) {
        std::vector<T> ret = std::vector<T>();
        copy(a.begin(), a.end(), back_inserter(ret));
        return ret;
    }

    template <typename T>
    static void removeVectorElementByIndex(std::vector<T>& vec, size_t pos)
    {
        if(vec.size()<pos){ std::cerr<< "The index entered exceeds the size of the vector."<<std::endl; return;}
        typename std::vector<T>::iterator it = vec.begin();
        std::advance(it, pos);
        vec.erase(it);
    }

    template <typename T>
    static void removeListElementByIndex(std::list<T>& list, size_t pos)
    {
        if(list.size()<pos){ std::cerr<< "The index entered exceeds the size of the list."<<std::endl; return;}
        typename std::list<T>::iterator it = list.begin();
        std::advance(it, pos);
        list.erase(it);
    }

    template <typename T>
    static T listElementByIndex(std::list<T>& list, size_t pos)
    {
        assert(list.size()<pos);
        std::cerr<< "The index entered exceeds the size of the list."<<std::endl;
        typename std::list<T>::iterator it = list.begin();
        std::advance(it, pos);
        return *it;
    }

    template<typename T>
    static void freeMemoryVectorSTD(std::vector<T>& vec) {
       vec.clear();
       std::vector<T>().swap(vec);
    }


    template<class T1, class T2>
    static std::vector<T1> firstVecFromPairVec(const std::vector<std::pair<T1, T2> >& v) {
        std::vector<T1> v1;
        v1.resize(v.size());
        for (size_t i = 0; i < v.size(); i++) {
            v1[i] = v[i].first;
        }
        return v1;
    }

    template<class T1, class T2>
    static std::vector<T2> secondVecFromPairVec(const std::vector<std::pair<T1, T2> >& v) {
        std::vector<T2> v2;
        v2.resize(v.size());
        for (size_t i = 0; i < v.size(); i++) {
            v2[i] = v[i].second;
        }
        return v2;
    }

    template<class T1, class T2>
    static void pairToIndependentVector(std::vector< std::pair<T1, T2> > p, std::vector<T1> &v1, std::vector<T2> &v2){
        for (auto it = std::make_move_iterator(p.begin()),
                 end = std::make_move_iterator(p.end()); it != end; ++it)
        {
            v1.push_back(std::move(it->first));
            v2.push_back(std::move(it->second));
        }
    }

    template<typename T>
    static void splitSTDVector(std::vector<T>v, int ind1, int ind2, std::vector<T>&vs1, std::vector<T>&vs2){
        vs1 = std::vector<T>( v.begin(), v.begin() + ind1 );
        vs2 = std::vector<T>( v.begin() + ind2, v.end() );
    }


    static size_t getAddress(std::function<void ()> f) {
        typedef void fnType;
        fnType ** fnPointer = f.template target<fnType*>();
        return (size_t)(*fnPointer);
    }

    template<typename T>
    static bool isApproximatelyEqual(T a, T b, T tolerance = std::numeric_limits<T>::epsilon())
    {
        T diff = std::fabs(a - b);
        if (diff <= tolerance)
            return true;

        if (diff < std::fmax(std::fabs(a), std::fabs(b)) * tolerance)
            return true;

        return false;
    }




    /** geometry functions **/

    /**
    * @enum TAxis
    * @brief types of axis
    */
    enum AxisPlane
    {
        Axial = 0,    /**< z-axis */
        Sagittal = 1,    /**< x-axis */
        Coronal = 2     /**< y-axis */
    };

    enum ViewOrientation
    {
        LAS = 0,    /**< LeftPosteriorSuperior (Radiological) */
        RAS = 1     /**< RightAnteriorSuperior (Neurological) */
    };




    static glm::vec3 posView2DFrom3DPos(glm::vec3 global3DPosition, AxisPlane axisViewPlane, ViewOrientation viewOrientation = ViewOrientation::LAS);


    /** Sphere and Plane **/
//    template<typename T>
    typedef struct _Plane{
        glm::vec3 normalDirection;
        glm::vec3 normalPl;
        glm::vec3 pointPl;
        float distancePl;
        glm::vec4 equationPl;
        _Plane(): normalDirection( 0, 0, 0 ), normalPl( 0, 0, 0 ), pointPl( 0, 0, 0 ),  distancePl(0), equationPl( 0, 0, 0, 0 ) { }
        _Plane( glm::vec3 normalDir, glm::vec3 point ) : normalDirection(normalDir), normalPl( glm::normalize(normalDir) ),
            pointPl(point), distancePl( -glm::dot( point, glm::normalize(normalDir) ) ), equationPl( glm::normalize(normalDir), -glm::dot( point, glm::normalize(normalDir) ) ) { }
        void constructFromPointVectors(glm::vec3 point, glm::vec3 v1, glm::vec3 v2){
            glm::vec3 normalDir = glm::cross(v1, v2);
            _Plane(normalDir, point);
        }
        void constructFromPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
            glm::vec3 normalDir = glm::cross(p2 - p1, p3 - p1);
            _Plane(normalDir, p1);
        }
        void constructFromPlaneEquation(glm::vec4 planeEquation){
//            glm::vec3 normalNormalized( glm::make_vec3( planeEquation ) );  ///Ting: unvalid usage (30/04/2020)
            glm::vec3 normalNormalized( glm::vec3(planeEquation.x,planeEquation.y, planeEquation.z));
            _Plane(normalNormalized, (-(normalNormalized/planeEquation.w) ) );
        }
    } Plane;

    static glm::vec3 normalFromPlaneEq(glm::vec4 planeEquation);

    static float distancePointPlane(glm::vec3 p, glm::vec4 Plane);

    static float radiusCircleInterPlaneSphere(glm::vec4 planaEquation, glm::vec3 positionSphere, float radiusSphere, int &typeIntersected);
    static float radiusCircleInterPlaneSphere(glm::vec4 planaEquation, glm::vec3 positionSphere, float radiusSphere);
    static glm::vec3 ClosestPointOnPlane(glm::vec4 plane, glm::vec3 point);
    static float coordTextToCoordVertFactor(float ct, float scaleFactor);
    static glm::vec3 coordTextToCoordVertFactor(glm::vec3 v, glm::vec3 sF);
    static glm::vec3 coordTextToCoordVertFactor(glm::vec3 sliceNormalized, vmtkAcquisition *data);
    static glm::vec4 planeView2DFrom3DPos(float posPlane, vmtkGeneralFunctions::AxisPlane axisViewPlane);
    static glm::vec4 planeView2DFrom3DPos(glm::vec3 global3DPosition, vmtkGeneralFunctions::AxisPlane axisViewPlane);
    static float randfromARange(double fMin, double fMax);


    /** Line and Plane **/
    typedef struct _Line{
        glm::vec3 initialPoint;
        glm::vec3 finalPoint;
        glm::vec3 direction;
        _Line(): initialPoint( 0, 0, 0 ), finalPoint( 0, 0, 0 ), direction( 0, 0, 0 ) {}
        _Line( glm::vec3 initialP, glm::vec3 finalP ) : initialPoint(initialP), finalPoint(finalP), direction(finalP-initialP) {}
    } Line;

    inline bool isLineLiesOnThePlaneAxis(Line L, AxisPlane AP);


    //    Return: 0 = no intersection
    //            1 = exist intersection (intersectionPoint)
    //            2 = the segment lies in the plane
    inline int intersectionLinePLane( Line L, Plane Pl, glm::vec3 &intersectionPoint );



};
using vgf = vmtkGeneralFunctions;
#endif // VMTKGENERALFUNCTIONS_H
