##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

#CONFIG += static
#message("static-": $$QMAKE_HOST.name);

INCLUDEPATH += /usr/lib \
                /home/wallace/libs/glew-2.0.0/include\
                /home/wallace/libs/gdcm-2.8.8-install/include/gdcm-2.8\
                /home/wallace/libs/glm-0.9.8.4-install/include \
                ./cimg \
                ./model
#                ./model/dti_fit

 LIBS += -L/home/wallace/libs/gdcm-2.8.8-install/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD -lgdcmopenjp2 \
         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmuuid -lsocketxx -lm -ldl

LIBS += -lGLU
LIBS += -L/home/wallace/libs/glew-2.0.0/lib -lGLEW
LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -L/usr/lib -lGL -lpthread -lXext -lX11
LIBS += -L/usr/lib/x86_64-linux-gnu -lz
