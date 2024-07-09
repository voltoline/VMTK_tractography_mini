##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------


#CONFIG += static
#message("static-": $$QMAKE_HOST.name);

QMAKE_LIBS = -lpng
#CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += /usr/lib \
                /usr/local/include/gdcm-3.0 \
                /usr/local/include/glm \
                ./cimg \
                ./model



#statics libraries
static {
message("static build")
LIBS += /usr/lib/libniftiio.a /usr/lib/libznz.a libz.a
LIBS += /usr/lib/x86_64-linux-gnu/libGLU.a /usr/lib64/libGLEW.a
LIBS += -L/usr/lib -lGL -lpthread
LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD \
         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx -lm -ldl

} else {
message("dynamic build")
#INCLUDEPATH += /usr/include
LIBS += -L/opt/Qt5.14.2/5.14.2/gcc_64/lib -lQt5Core

LIBS += -L/usr/lib -lznz
LIBS += -L/usr/lib -lniftiio
LIBS += -L/usr/lib/x86_64-linux-gnu -lz
LIBS += -L/usr/lib/x86_64-linux-gnu -lGL
LIBS += -L/usr/lib/x86_64-linux-gnu -lGLU
LIBS += -L/usr/lib/x86_64-linux-gnu -lpthread
LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx
#LIBS += -lm -ldl
#LIBS += -L/usr/lib/x86_64-linux-gnu -ldl

LIBS += -L/lib/x86_64-linux-gnu -lGLEW
#LIBS += -L/usr/lib64/libGLEW.so

QMAKE_LIBS = -lpng
CONFIG += c++11

}

DEFINES += GLM_ENABLE_EXPERIMENTAL

#shared libraries
#LIBS += -L/usr/lib/x86_64-linux-gnu -lXext -lX11
