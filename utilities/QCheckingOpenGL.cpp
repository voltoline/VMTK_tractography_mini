#include "utilities/QCheckingOpenGL.h"

QCheckingOpenGL::QCheckingOpenGL()
{
}


#ifdef enableOldQGL
void QCheckingOpenGL::checkingProfile(QGLFormat format)
{
    std::string formatString="Unknow Profile";
    std::string versionString="Unknow Version";
    if (format.profile()==QGLFormat::CoreProfile){formatString="CoreProfile";}
    if (format.profile()==QGLFormat::CompatibilityProfile){formatString="CompatibilityProfile";}
    if (format.profile()==QGLFormat::NoProfile){formatString="NoProfile";}
    int ma_v=format.majorVersion();
    int me_v=format.minorVersion();
    std::ostringstream temp;
    temp<<ma_v<<"."<<me_v;
    versionString=temp.str();
    std::cout<<std::endl;
    std::cout<<"***Checking New Format Profile***"<<std::endl;
    std::cout<<"Format Profile: "<<formatString<<std::endl;
    std::cout<<"OpenGL Version Profile: "<<versionString<<std::endl;
    std::cout<<std::endl;
}

void QCheckingOpenGL::settingGLFormat(QGLFormat &format)
{
    format.setVersion(4,3);
    format.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    format.setDepth(true);
    format.setAlpha(true);
    std::cout<<std::endl;
    std::cout<<"***Setting GLFormat***"<<std::endl;
    std::cout<<std::endl;
}

void QCheckingOpenGL::initializeGlew(QGLFormat format)
{
    //Ting: It causes GL_INVALID_ENUM on GL 3.2+ core context
    //as soon as glewInit()​ is called
    //See: https://www.opengl.org/wiki/OpenGL_Loading_Library

    std::cout<<std::endl;
    std::cout<<"*****************Initializing Glew*****************"<<std::endl;

    if (format.profile()==QGLFormat::CoreProfile){
        glewExperimental = GL_TRUE; /**< GLEW setting some functions as unsupported */
        std::cout<<"Experimental Glew is enabled"<<std::endl;
    }else{
        std::cout<<"Experimental Glew is disabled"<<std::endl;
    }
    //glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cout << "Glew isn't ok" << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        std::cout << "Initialize Glew is ok" << std::endl;
        //Ting: just igore the error (4/12/2016)
        glGetError();
    }
    std::cout<<std::endl;
}

#else

void QCheckingOpenGL::checkingProfile(QSurfaceFormat format)
{
    std::string formatString="Unknow Profile";
    std::string versionString="Unknow Version";
    if (format.profile()==QSurfaceFormat::CoreProfile){formatString="CoreProfile";}
    if (format.profile()==QSurfaceFormat::CompatibilityProfile){formatString="CompatibilityProfile";}
    if (format.profile()==QSurfaceFormat::NoProfile){formatString="NoProfile";}
    int ma_v=format.majorVersion();
    int me_v=format.minorVersion();
    std::ostringstream temp;
    temp<<ma_v<<"."<<me_v;
    versionString=temp.str();
    std::cout<<std::endl;
    std::cout<<"***Checking New Format Profile***"<<std::endl;
    std::cout<<"Format Profile: "<<formatString<<std::endl;
    std::cout<<"OpenGL Version Profile: "<<versionString<<std::endl;
    std::cout<<std::endl;
}


void QCheckingOpenGL::settingGLFormat(QSurfaceFormat &format)

{
    format.setVersion(4,3);
    format.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
    std::cout<<std::endl;
    std::cout<<"***Setting QSurfaceFormat***"<<std::endl;
    std::cout<<std::endl;
}

void QCheckingOpenGL::initializeGlew(QSurfaceFormat format)
{
    //Ting: It causes GL_INVALID_ENUM on GL 3.2+ core context
    //as soon as glewInit()​ is called
    //See: https://www.opengl.org/wiki/OpenGL_Loading_Library

    std::cout<<std::endl;
    std::cout<<"*****************Initializing Glew*****************"<<std::endl;
   // std::cout << "OpenGL System Current Version: " << glGetString(GL_VERSION) <<std::endl;
    if (format.profile()==QSurfaceFormat::CoreProfile){
        glewExperimental = GL_TRUE; /**< GLEW setting some functions as unsupported */
        std::cout<<"Experimental Glew is enabled"<<std::endl;
    }else{
        std::cout<<"Experimental Glew is disabled"<<std::endl;
    }
    //glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cout << "Glew isn't ok" << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        std::cout << "Initialize Glew is ok" << std::endl;
        //Ting: just igore the error (4/12/2016)
        glGetError();
        //QCheckingOpenGL::_check_gl_error(__FILE__,__LINE__);
    }
    std::cout<<std::endl;
}

#endif


void QCheckingOpenGL::checkingCurrentSystemProfile()
{
    std::cout<<std::endl;
    std::cout<<"***Checking Current System Profile***"<<std::endl;
    #ifdef _COMPATIBILITY_PROFILE
        std::cout << "Format: CompatibilityProfile"<< std::endl;
    #elif _CORE_PROFILE
        std::cout << "Format: CoreProfile"<< std::endl;
    #else
        std::cout << "Format: NoProfile"<< std::endl;
    #endif
    std::cout<<std::endl;
}

void QCheckingOpenGL::_check_gl_error(const char *file, int line) {
    //Ting: sugestao para chamada
    //_check_gl_error(__FILE__,__LINE__)
    GLenum err (glGetError());
    while(err!=GL_NO_ERROR) {
        std::string error;
        switch(err) {
            case GL_INVALID_OPERATION: error="INVALID_OPERATION"; break;
            case GL_INVALID_ENUM: error="INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error="INVALID_VALUE"; break;
            case GL_OUT_OF_MEMORY: error="OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error="INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_STACK_UNDERFLOW: error="GL_STACK_UNDERFLOW"; break;
            case GL_STACK_OVERFLOW: error="GL_STACK_OVERFLOW"; break;
        }
        std::cout << "GL_" << error.c_str() <<" - "<< file << ":" << line << std::endl;
        err=glGetError();
    }
}
