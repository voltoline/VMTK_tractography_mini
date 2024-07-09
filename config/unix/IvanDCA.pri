##----------------------------------------------------------
## Project organization source
## Template created by Iván - 2022
##
##----------------------------------------------------------

#CONFIG += static
#message("static-": $$QMAKE_HOST.name);

QMAKE_LIBS = -lpng
QMAKE_CFLAGS_ISYSTEM = -I
CONFIG += c++11
#DEFINES += GLM_ENABLE_EXPERIMENTAL

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH +=  /usr/local/include/gdcm-3.0 \
                /usr/local/include/glm \
                ./cimg \
                ./model

INCLUDEPATH += /usr/include

#statics libraries
static {
message("static build")
#LIBS += /usr/lib/libniftiio.a /usr/lib/libznz.a libz.a
LIBS += /usr/lib/x86_64-linux-gnu/libGLU.a /usr/lib64/libGLEW.a
#LIBS += -L/usr/lib -lGL -lpthread
LIBS += -L/usr/lib/x86_64-linux-gnu -lGL -lpthread -lXext -lX11




LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD \
         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx -lm -ldl

LIBS += -L/usr/lib/x86_64-linux-gnu -lniftiio -lznz -lz

} else {
message("dynamic build")
#INCLUDEPATH += /usr/include

#lib_files = $$files(/usr/local/lib/*)
#message("list libraries: " $$lib_files)

LIBS += -L/opt/Qt/5.14.2/gcc_64/lib -lQt5Core

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

LIBS += -L/usr/lib64/ -lGLEW
#LIBS += -L/usr/lib64/libGLEW.so



}

#QMAKE_LIBS = -lpng
#CONFIG += c++11



#shared libraries
#LIBS += -L/usr/lib/x86_64-linux-gnu -lXext -lX11
