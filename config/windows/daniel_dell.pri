##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

message(Configuracao: $$QMAKE_HOST.name);

INCLUDEPATH +=  C:\dev\libs\gdcm\Dynamic\3_1_0\include\gdcm-3.0 \
                C:\dev\libs\Eigen \
                C:\dev\libs\glew\Static\1_12_0\include \
                C:\dev\libs\glm
LIBS += -L"C:\dev\libs\glew\Static\1_12_0\lib" -lglew32

LIBS += -L"C:\dev\libs\gdcm\Dynamic\3_1_0\lib" -lgdcmMSFF -lgdcmCommon -lgdcmDICT -lgdcmDSED \
        -lgdcmIOD -lgdcmMEXD -lgdcmexpat -lgdcmjpeg8 -lgdcmjpeg12 -lgdcmjpeg16 \
        -lgdcmopenjp2 -lgdcmzlib -lgdcmcharls -lsocketxx

LIBS += -lopengl32

QMAKE_CXXFLAGS += -fopenmp -Dcimg_display=0 -std=gnu++11 ##-march=native  # -O3
#QMAKE_CXXFLAGS += -std=gnu++11 ##-march=native  # -O3
LIBS += -fopenmp
QMAKE_CFLAGS_RELEASE += -fopenmp
QMAKE_CFLAGS_DEBUG += -fopenmp
