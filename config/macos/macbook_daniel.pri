##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

#QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
#QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -I/usr/local/include

CONFIG += static
message("static-": $$QMAKE_HOST.name);

DEFINES += GL_SILENCE_DEPRECATION
QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -I/usr/local/include
QMAKE_LFLAGS += -lomp

QMAKE_LFLAGS += -Bstatic
#QMAKE_LFLAGS += -headerpad_max_install_names
#QMAKE_LFLAGS  = -Wl,-rpath,@executable_path/../Frameworks

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

QMAKE_CXXFLAGS += -fvisibility=hidden -Dcimg_display=0

 INCLUDEPATH += "/usr/local/Cellar/gdcm/3.0.8_1/include/gdcm-3.0"
 INCLUDEPATH += "/opt/local/include/GL"
 INCLUDEPATH += "./cimg"
 INCLUDEPATH += "/usr/local/Cellar/glm/0.9.9.5/include"
 INCLUDEPATH += "/usr/local/Cellar/glew/2.2.0_1/include/GL"
 INCLUDEPATH += "/usr/local/Cellar/glew/2.2.0_1/include"
 INCLUDEPATH += "./ui"

 LIBS        += "/usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.a"
# LIBS        += -L/opt/lib -lgdcmcharls -lgdcmCommon -lgdcmDICT -lgdcmDSED \
#                -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 \
#                -lgdcmopenjpeg -lgdcmMSFF -lgdcmzlib -lsocketxx -lgdcmuuid
 LIBS        += -L/opt/lib -lgdcmcharls -lgdcmCommon -lgdcmDICT -lgdcmDSED \
                -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 \
                -lgdcmopenjp2 -lgdcmMSFF -lgdcmzlib -lsocketxx -lgdcmuuid

LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib

UI_DIR = ./ui

ICON = Images/vmtk_mac_icon1.icns

# destination directories
#release: DESTDIR = build\release
#debug:   DESTDIR = build\debug/System/Library/Frameworks/OpenGL.framework/Versions/A/

DESTDIR = build
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc

unix:QMAKE_RPATHDIR += /Users/daniel_mac/Qt/5.12.3/clang_64/lib
