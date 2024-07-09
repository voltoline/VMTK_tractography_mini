/**
  * @file Checking_OGL.h
  * @brief Check and set values the GLFormat.
  * @author José Angel Ivan Rubianes Silva
  * @date January 2015
  * @note This code is created by @author.
  */

#ifndef VMTKCHECKINGOPENGL_H
#define VMTKCHECKINGOPENGL_H

/**
  * @def GLEW_STATIC
  * @brief binds the code to the <a href=¨http://glew.sourceforge.net/¨>GLEW</a>´s static library
  */
#ifndef GLEW_STATIC
    #define GLEW_STATIC
#endif

#ifdef __WIN32__
    #include <GL/glew.h>
#elif __linux__
    #include <GL/glew.h>
#elif __APPLE__
    #include <glew.h>
#endif

#include <iostream>
#include <sstream>
#include <QtGlobal>

#define XSTR(x) STR(x)
#define STR(x) #x



#ifdef enableOldQGL
    #include <QGLFormat>
#else
    #include <QSurfaceFormat>
#endif


class QCheckingOpenGL
{
public:
    QCheckingOpenGL();

    /**
     * @brief Check the System Current Profile for OpenGL.
     */
    static void checkingCurrentSystemProfile();



#ifdef enableOldQGL
    /**
     * @brief The OpenGL format settings are established.
     * @param format: Parameter used to load format settings.
     */
    static void settingGLFormat(QGLFormat &format);

    /**
     * @brief Check the format settings and displays the information of the current profile.
     * @param format: Parameter used to read the current format settings.
     */
    static void checkingProfile(QGLFormat format);

    /**
     * @brief Initialize Glew functions.
     * @param format: Parameter used to read the current format settings.
     * @param flagGlew: Parameter used to load current state of glewExperimental (GL_TRUE or_GL_False).
     */
    static void initializeGlew(QGLFormat format);
#else
    static void settingGLFormat(QSurfaceFormat &format);

    static void checkingProfile(QSurfaceFormat format);

    static void initializeGlew(QSurfaceFormat format);
#endif





    /**
     * @brief Check opengl error
     * @param file File to be checked
     * @param line Line to be checked
     */
    static void _check_gl_error(const char *file, int line);   //added by Ting (3/12/2016)
};

#endif // VMTKCHECKINGOPENGL_H
