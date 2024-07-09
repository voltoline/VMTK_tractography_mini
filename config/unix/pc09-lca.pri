##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

CONFIG += static
message("static-": $$QMAKE_HOST.name);

INCLUDEPATH += /usr/include \
#sys/time.h
                /usr/include/x86_64-linux-gnu \
#omp.h
                /usr/lib/gcc/x86_64-linux-gnu/5/include \
                /opt/include/gdcm-2.8 \
                /usr/include/glm \
                ./cimg \
                ./model
#                ./model/dti_fit

LIBS += /opt/lib/libgdcmMSFF.a /opt/lib/libgdcmDICT.a /opt/lib/libgdcmIOD.a /opt/lib/libgdcmcharls.a \
        /opt/lib/libgdcmDSED.a /opt/lib/libgdcmCommon.a /opt/lib/libgdcmzlib.a \
        /opt/lib/libgdcmexpat.a /opt/lib/libgdcmMEXD.a /opt/lib/libgdcmuuid.a /opt/lib/libgdcmjpeg12.a \
        /opt/lib/libgdcmjpeg16.a /opt/lib/libgdcmjpeg8.a /opt/lib/libgdcmopenjp2.a \
        /opt/lib/libgdcmuuid.a /opt/lib/libsocketxx.a -lm -ldl

#statics libraries
LIBS += /usr/lib/libniftiio.a /usr/lib/libznz.a /usr/lib/x86_64-linux-gnu/libz.a
LIBS += /usr/lib64/libGLEW.a /usr/lib/x86_64-linux-gnu/libGLU.a -L/usr/lib -lGL -lpthread

##shared libraries
#LIBS += -lGLU
#LIBS += -L/usr/lib64 -lGLEW
#LIBS += -L/usr/lib -lGL -lpthread -lXext -lX11
#LIBS += -L/usr/lib/x86_64-linux-gnu -lniftiio -lznz -lz
