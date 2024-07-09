##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

CONFIG += static
message("static-": $$QMAKE_HOST.name);

message(Configuracao: $$QMAKE_HOST.name);
INCLUDEPATH +=  C:\GDCM\Static\2_8_6\include\gdcm-2.8 \
                C:\Eigen\include \
                C:\Glew\Static\1_12_0\include \
                C:\glm\include

LIBS += -L"C:\Glew\Static\1_12_0\lib" -lglew32
LIBS += -L"C:\GDCM\Static\2_8_6\lib" -lgdcmMSFF -lgdcmCommon -lgdcmDICT -lgdcmDSED \
        -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg8 -lgdcmjpeg12 -lgdcmjpeg16 \
        -lgdcmopenjp2 -lgdcmzlib -lgdcmcharls -lgdcmgetopt -lsocketxx
DEFINES += stateExternalEigen=1
