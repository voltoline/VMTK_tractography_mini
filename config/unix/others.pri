##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

#CONFIG += static
#message("static-": $$QMAKE_HOST.name);

INCLUDEPATH += /usr/include \
#                /usr/local/include/gdcm-2.0 \
#                /usr/local/include/gdcm-2.4 \
                /usr/local/include/gdcm-2.8 \
                /usr/local/include/glm \
                ./cimg \
                ./model
#                ./model/dti_fit

 LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD \
         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx -lm -ldl

#statics libraries
#LIBS += /usr/local/lib/libboost_thread.a /usr/local/lib/libboost_system.a
#LIBS += /usr/lib/libniftiio.a /usr/lib/libznz.a /usr/lib/x86_64-linux-gnu/libz.a
#LIBS += /usr/lib/x86_64-linux-gnu/libGLU.a /usr/lib64/libGLEW.a -L/usr/lib -lGL -lpthread

#shared libraries
LIBS += -lGLU
LIBS += -L/usr/lib64 -lGLEW
LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -L/usr/lib/x86_64-linux-gnu #-lboost_thread -lboost_system
LIBS += -L/usr/lib -lGL -lpthread -lXext -lX11
LIBS += -L/usr/lib/x86_64-linux-gnu -lniftiio -lznz -lz
