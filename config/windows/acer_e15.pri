##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

#CONFIG += static
#message("static-": $$QMAKE_HOST.name);

CONFIG += -std=c++1z
#CONFIG += warn_off
message(Configuration: $$QMAKE_HOST.name);
INCLUDEPATH +=  C:\Eigen3\include \
                C:\glm\0_9_9_6\Static\include
static {
 # this is a static build
message(ACER-E15 - static );
INCLUDEPATH +=  C:\GDCM\Static\3_0_1\include\gdcm-3.0 \
                C:\Glew\Static\2_1_0\include
LIBS += -L"C:\Glew\Static\2_1_0\lib" -lglew32
LIBS += -L"C:\GDCM\Static\3_0_1\lib" -lgdcmMSFF -lgdcmCommon -lgdcmDICT -lgdcmDSED \
        -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg8 -lgdcmjpeg12 -lgdcmjpeg16 \
        -lgdcmopenjp2 -lgdcmzlib -lgdcmcharls -lgdcmgetopt -lsocketxx -lws2_32
} else {
message(ACER-E15 - dynamic );
INCLUDEPATH +=  C:\GDCM\Dynamic\3_0_1\include\gdcm-3.0 \
                C:\Glew\Dynamic\2_1_0\include
LIBS += -L"C:\Glew\Dynamic\2_1_0\lib" -llibglew32.dll
LIBS += -L"C:\GDCM\Dynamic\3_0_1\lib" -lgdcmMSFF -lgdcmCommon -lgdcmDICT -lgdcmDSED \
        -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg8 -lgdcmjpeg12 -lgdcmjpeg16 \
        -lgdcmopenjp2 -lgdcmzlib -lgdcmcharls -lgdcmgetopt -lsocketxx -lws2_32
}
DEFINES += stateExternalEigen=1
