##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

CONFIG += static
message("static-": $$QMAKE_HOST.name);

QMAKE_CXXFLAGS += -Wl,--stack,35544320
QMAKE_LFLAGS_WINDOWS += -Wl,--stack,35544320
#QMAKE_LFLAGS_WINDOWS += -Wl,--large-address-aware


message(Configuracao: maquina do Raphael);
INCLUDEPATH +=  C:\Boost\Static\1_58_0\include\boost-1_58 \
                C:\GDCM\Static\2_4_0\include\gdcm-2.4 \
                C:\Glew\Static\1_12_0\include

INCLUDEPATH += C:\glm\include

QMAKE_CXXFLAGS += -fopenmp -Dcimg_display=0 -std=gnu++11 ##-march=native  # -O3
#QMAKE_CXXFLAGS += -std=gnu++11 ##-march=native  # -O3
LIBS += -fopenmp
QMAKE_CFLAGS_RELEASE += -fopenmp
QMAKE_CFLAGS_DEBUG += -fopenmp

#QMAKE_CXXFLAGS += -D_CREATE_WEB_SERVER

#QT       += websockets webchannel

#SOURCES +=\
#    web/http_server/QtWebApp/httpserver/httpconnectionhandler.cpp \
#    web/http_server/QtWebApp/httpserver/httpconnectionhandlerpool.cpp \
#    web/http_server/QtWebApp/httpserver/httpcookie.cpp \
#    web/http_server/QtWebApp/httpserver/httpglobal.cpp \
#    web/http_server/QtWebApp/httpserver/httplistener.cpp \
#    web/http_server/QtWebApp/httpserver/httprequest.cpp \
#    web/http_server/QtWebApp/httpserver/httprequesthandler.cpp \
#    web/http_server/QtWebApp/httpserver/httpresponse.cpp \
#    web/http_server/QtWebApp/httpserver/httpsession.cpp \
#    web/http_server/QtWebApp/httpserver/httpsessionstore.cpp \
#    web/http_server/QtWebApp/httpserver/staticfilecontroller.cpp \
#    web/webchannel/shared/websocketclientwrapper.cpp \
#    web/webchannel/shared/websockettransport.cpp \
#    web/QWebSocketServerThread.cpp

#HEADERS  += \
#    web/http_server/QtWebApp/httpserver/httpconnectionhandler.h \
#    web/http_server/QtWebApp/httpserver/httpconnectionhandlerpool.h \
#    web/http_server/QtWebApp/httpserver/httpcookie.h \
#    web/http_server/QtWebApp/httpserver/httpglobal.h \
#    web/http_server/QtWebApp/httpserver/httplistener.h \
#    web/http_server/QtWebApp/httpserver/httprequest.h \
#    web/http_server/QtWebApp/httpserver/httprequesthandler.h \
#    web/http_server/QtWebApp/httpserver/httpresponse.h \
#    web/http_server/QtWebApp/httpserver/httpsession.h \
#    web/http_server/QtWebApp/httpserver/httpsessionstore.h \
#    web/http_server/QtWebApp/httpserver/staticfilecontroller.h \
#    web/webchannel/shared/websocketclientwrapper.h \
#    web/webchannel/shared/websockettransport.h \
#    web/QWebSocketServerThread.h

INCLUDEPATH += "..\..\glew\Static\2_1_0\include" "..\..\glm-0.9.8.5\glm" #"..\libs\mingw32_5.3.0\boost_1_55_0"  #".\model\dti_fit"
LIBS += -L"..\..\glew\Static\2_1_0\lib" -lglew32 #-L"..\libs\mingw32_5.3.0\boost_1_55_0\stage\lib" -lboost_thread-mgw53-mt-1_55 -lboost_system-mgw53-mt-1_55

#INCLUDEPATH += "..\libs\mingw64_730\glew-2.1.0\include" "..\libs\glm-0.9.9.7" #"..\libs\mingw32_5.3.0\boost_1_55_0"  #".\model\dti_fit"
#LIBS += -L"..\libs\mingw64_730\glew-2.1.0\lib" -lglew32 #-L"..\libs\mingw32_5.3.0\boost_1_55_0\stage\lib" -lboost_thread-mgw53-mt-1_55 -lboost_system-mgw53-mt-1_55


#QMAKE_CXXFLAGS += -DCOMPILE_LABELING_MODULE

#SOURCES +=\
#    modules/visualization/submodules/atlas_labeling/model/ModelAtlasLabeling.cpp \
#    modules/visualization/submodules/atlas_labeling/model/affine_registration.cpp \
#    modules/visualization/submodules/atlas_labeling/model/bspline_registration.cpp \
#    modules/visualization/submodules/atlas_labeling/model/iterationupdate.cpp \
#    modules/visualization/submodules/atlas_labeling/model/iterationupdate_affine.cpp \
#    modules/visualization/submodules/atlas_labeling/model/iterationupdate_bspline.cpp \
#    modules/visualization/submodules/atlas_labeling/model/multiresolutioniterationupdate.cpp \
#    modules/visualization/submodules/atlas_labeling/model/utils_registration.cpp \

#HEADERS  += \
#    modules/visualization/submodules/atlas_labeling/model/ModelAtlasLabeling.h \
#    modules/visualization/submodules/atlas_labeling/model/affine_registration.h \
#    modules/visualization/submodules/atlas_labeling/model/bspline_registration.h \
#    modules/visualization/submodules/atlas_labeling/model/Iterationupdate.h \
#    modules/visualization/submodules/atlas_labeling/model/Iterationupdate_affine.h \
#    modules/visualization/submodules/atlas_labeling/model/iterationupdate_bspline.h \
#    modules/visualization/submodules/atlas_labeling/model/multiresolutioniterationupdate.h \
#    modules/visualization/submodules/atlas_labeling/model/utils_registration.h \

# for simple itk (dynamic) and without
#############################################################################################################################

#QMAKE_CXXFLAGS += -D_HAS_SIMPLEITK

equals(has_simple_elastix, 0){

INCLUDEPATH += "..\..\glew\Static\2_1_0\include" "..\include" \
            "..\..\gdcm\install\include\gdcm-2.8"

LIBS  += -L"..\..\glew\Static\2_1_0\lib" \
         -L"..\..\gdcm\install\lib" \
         -lglew32 -lglu32 -lglu32 -lopengl32 -lOPENGL32 -lGLU32 \
         -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm \
         -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 \
         -ladvapi32 -lwsock32 -lodbc32 -lgdcmDICT -lgdcmMSFF -lgdcmDSED -lgdcmIOD -lgdcmexpat -lgdcmzlib -lsocketxx  \
          -lgdcmcharls -lgdcmgetopt  -lgdcmjpeg16 -lgdcmjpeg12 -lgdcmjpeg8  -lgdcmMEXD -lgdcmopenjp2 -lgdcmCommon



#INCLUDEPATH += "..\libs\mingw32_5.3.0\glew-2.0.0\include" "..\include" \
#            "..\libs\mingw64_730\gdcm-2.8.2\include\gdcm-2.8"

#LIBS  += -L"..\libs\mingw64_730\glew-2.1.0\lib" \
#         -L"..\libs\mingw64_730\gdcm-2.8.2\lib" \
#         -lglew32 -lglu32 -lglu32 -lopengl32 -lOPENGL32 -lGLU32 \
#         -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm \
#         -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 \
#         -ladvapi32 -lwsock32 -lodbc32 -lgdcmDICT -lgdcmMSFF -lgdcmDSED -lgdcmIOD -lgdcmexpat -lgdcmzlib -lsocketxx  \
#         -lgdcmcharls -lgdcmgetopt  -lgdcmjpeg16 -lgdcmjpeg12 -lgdcmjpeg8  -lgdcmMEXD -lgdcmopenjp2 -lgdcmCommon

}

# for simple itk (dynamic)
#############################################################################################################################

##Includes

#INCLUDEPATH += "C:/build/SimpleITK-master/build/ITK-prefix/include/ITK-5.1"
#INCLUDEPATH += "C:/build/SimpleITK-master/install/include/SimpleITK-2.0"

##SimpleITK
#LIBS += -LC:/build/SimpleITK-master/install/lib \
#        -lSimpleITKCommon-2.0 \
#        -lSimpleITKIO-2.0 \
#        -lSimpleITKRegistration-2.0 \
#        -lSimpleITKBasicFilters0-2.0 \
#        -lSimpleITK_ITKAnisotropicSmoothing-2.0 \
#        -lSimpleITK_ITKAntiAlias-2.0 \
#        -lSimpleITK_ITKBiasCorrection-2.0 \
#        -lSimpleITK_ITKBinaryMathematicalMorphology-2.0 \
#        -lSimpleITK_ITKClassifiers-2.0 \
#        -lSimpleITK_ITKColormap-2.0 \
#        -lSimpleITK_ITKCommon-2.0 \
#        -lSimpleITK_ITKConnectedComponents-2.0 \
#        -lSimpleITK_ITKConvolution-2.0 \
#        -lSimpleITK_ITKCurvatureFlow-2.0 \
#        -lSimpleITK_ITKDeconvolution-2.0 \
#        -lSimpleITK_ITKDenoising-2.0 \
#        -lSimpleITK_ITKDisplacementField-2.0 \
#        -lSimpleITK_ITKDistanceMap-2.0 \
#        -lSimpleITK_ITKFFT-2.0 \
#        -lSimpleITK_ITKFastMarching-2.0 \
#        -lSimpleITK_ITKImageCompare-2.0 \
#        -lSimpleITK_ITKImageCompose-2.0 \
#        -lSimpleITK_ITKImageFeature-2.0 \
#        -lSimpleITK_ITKImageFilterBase-2.0 \
#        -lSimpleITK_ITKImageFunction-2.0 \
#        -lSimpleITK_ITKImageFusion-2.0 \
#        -lSimpleITK_ITKImageGradient-2.0 \
#        -lSimpleITK_ITKImageGrid-2.0 \
#        -lSimpleITK_ITKImageIntensity-2.0 \
#        -lSimpleITK_ITKImageLabel-2.0 \
#        -lSimpleITK_ITKImageNoise-2.0 \
#        -lSimpleITK_ITKImageSources-2.0 \
#        -lSimpleITK_ITKImageStatistics-2.0 \
#        -lSimpleITK_ITKLabelMap-2.0 \
#        -lSimpleITK_ITKLabelVoting-2.0 \
#        -lSimpleITK_ITKLevelSets-2.0 \
#        -lSimpleITK_ITKMathematicalMorphology-2.0 \
#        -lSimpleITK_ITKPDEDeformableRegistration-2.0 \
#        -lSimpleITK_ITKRegionGrowing-2.0 \
#        -lSimpleITK_ITKRegistrationCommon-2.0 \
#        -lSimpleITK_ITKReview-2.0 \
#        -lSimpleITK_ITKSmoothing-2.0 \
#        -lSimpleITK_ITKThresholding-2.0 \
#        -lSimpleITK_ITKTransform-2.0 \
#        -lSimpleITK_ITKWatersheds-2.0 \
#        -lSimpleITK_SimpleITKFilters-2.0 \
#        -lSimpleITKBasicFilters1-2.0 \
#        -lSimpleITKIO-2.0

##ITK
#LIBS += -LC:/build/SimpleITK-master/build/ITK-prefix/lib \
#        -lITKIOBruker-5.1 \
#        -lITKIOHDF5-5.1 \
#        -lITKIOLSM-5.1 \
#        -lITKIOMINC-5.1 \
#        -litkminc2-5.1 \
##        -lrt \
#        -lITKIOMRC-5.1 \
#        -lSimpleITKRegistration-2.0 \
#        -lITKOptimizersv4-5.1 \
#        -litklbfgs-5.1 \

##SimpleITK
#LIBS += -LC:/build/SimpleITK-master/install/lib \
#        -lSimpleITK_ITKAnisotropicSmoothing-2.0 \
#        -lSimpleITK_ITKAntiAlias-2.0 \
#        -lSimpleITK_ITKBiasCorrection-2.0 \
#        -lSimpleITK_ITKBinaryMathematicalMorphology-2.0 \
#        -lSimpleITK_ITKClassifiers-2.0 \
#        -lSimpleITK_ITKColormap-2.0 \
#        -lSimpleITK_ITKCommon-2.0 \
#        -lSimpleITK_ITKConnectedComponents-2.0 \
#        -lSimpleITK_ITKConvolution-2.0 \
#        -lSimpleITK_ITKCurvatureFlow-2.0 \
#        -lSimpleITK_ITKDeconvolution-2.0 \
#        -lSimpleITK_ITKDenoising-2.0 \
#        -lSimpleITK_ITKDisplacementField-2.0 \
#        -lSimpleITK_ITKDistanceMap-2.0 \
#        -lSimpleITK_ITKFFT-2.0 \
#        -lSimpleITK_ITKFastMarching-2.0 \
#        -lSimpleITK_ITKImageCompare-2.0 \
#        -lSimpleITK_ITKImageCompose-2.0 \
#        -lSimpleITK_ITKImageFeature-2.0 \
#        -lSimpleITK_ITKImageFilterBase-2.0 \
#        -lSimpleITK_ITKImageFunction-2.0 \
#        -lSimpleITK_ITKImageFusion-2.0 \
#        -lSimpleITK_ITKImageGradient-2.0 \
#        -lSimpleITK_ITKImageGrid-2.0 \
#        -lSimpleITK_ITKImageIntensity-2.0 \
#        -lSimpleITK_ITKImageLabel-2.0 \
#        -lSimpleITK_ITKImageNoise-2.0 \
#        -lSimpleITK_ITKImageSources-2.0 \
#        -lSimpleITK_ITKImageStatistics-2.0 \
#        -lSimpleITK_ITKLabelMap-2.0 \
#        -lSimpleITK_ITKLabelVoting-2.0 \
#        -lSimpleITK_ITKLevelSets-2.0 \
#        -lSimpleITK_ITKMathematicalMorphology-2.0 \
#        -lSimpleITK_ITKPDEDeformableRegistration-2.0 \
#        -lSimpleITK_ITKRegionGrowing-2.0 \
#        -lSimpleITK_ITKRegistrationCommon-2.0 \
#        -lSimpleITK_ITKReview-2.0 \

#LIBS += -LC:/build/SimpleITK-master/build/ITK-prefix/lib \
#        -lITKReview-5.1 \
#        -lITKIOGDCM-5.1 \
#        -litkgdcmMSFF-5.1 \
#        -litkgdcmDICT-5.1 \
#        -litkgdcmIOD-5.1 \
#        -litkgdcmDSED-5.1 \
#        -litkgdcmCommon-5.1 \
#        -litkgdcmjpeg8-5.1 \
#        -litkgdcmjpeg12-5.1 \
#        -litkgdcmjpeg16-5.1 \
#        -litkgdcmopenjp2-5.1 \
#        -litkgdcmcharls-5.1 \
##        -litkgdcmuuid-5.1 \
#        -lITKIOBMP-5.1 \
#        -lITKIOBioRad-5.1 \
#        -lITKIOGE-5.1 \
#        -lITKIOGIPL-5.1 \
#        -lITKIOJPEG-5.1 \
#        -lITKIOMeta-5.1 \
#        -lITKIONIFTI-5.1 \
#        -lITKniftiio-5.1 \
#        -lITKznz-5.1 \
#        -lITKIONRRD-5.1 \
#        -lITKNrrdIO-5.1 \
#        -lITKIOPNG-5.1 \
#        -litkpng-5.1 \
#        -lITKIOStimulate-5.1 \
#        -lITKIOTIFF-5.1 \
#        -litktiff-5.1 \
#        -litkjpeg-5.1 \
#        -lITKIOVTK-5.1 \
#        -lITKIOTransformHDF5-5.1 \
#        -litkhdf5_cpp \
#        -litkhdf5 \
#        -lITKIOTransformInsightLegacy-5.1 \
#        -lITKIOTransformMatlab-5.1 \
#        -lITKQuadEdgeMesh-5.1 \
#        -lITKBiasCorrection-5.1 \
#        -lITKPolynomials-5.1 \
##        -lITKBioCell-5.1 \
##        -lITKFEM-5.1 \
#        -lITKOptimizers-5.1 \
#        -lITKIOSpatialObjects-5.1 \
#        -lITKIOSiemens-5.1 \
#        -lITKIOIPL-5.1 \
#        -lITKIOXML-5.1 \
#        -lITKIOImageBase-5.1 \
#        -lITKEXPAT-5.1 \
#        -lITKKLMRegionGrowing-5.1 \
#        -litkopenjpeg-5.1 \
#        -lITKVTK-5.1 \

##SimpleITK
#LIBS += -LC:/build/SimpleITK-master/install/lib \
#        -lSimpleITK_ITKSmoothing-2.0 \
#        -lSimpleITK_ITKThresholding-2.0 \
#        -lSimpleITK_ITKTransform-2.0 \
#        -lSimpleITK_ITKWatersheds-2.0

##ITK
#LIBS += -LC:/build/SimpleITK-master/build/ITK-prefix/lib \
#        -lITKWatersheds-5.1

##SimpleITK
#LIBS += -LC:/build/SimpleITK-master/install/lib \
#        -lSimpleITK_SimpleITKFilters-2.0 \
#        -lSimpleITKBasicFilters0-2.0 \
#        -lSimpleITKCommon-2.0

##ITK
#LIBS += -LC:/build/SimpleITK-master/build/ITK-prefix/lib \
#        -lITKIOTransformBase-5.1 \
#        -lITKTransformFactory-5.1


##SimpleITK
##LIBS += -LC:/build/SimpleITK-master/install/lib
##         -lSimpleITKExplicit-2.0

##ITK
#LIBS += -LC:/build/SimpleITK-master/build/ITK-prefix/lib \
#        -lITKSpatialObjects-5.1 \
#        -lITKMesh-5.1 \
#        -lITKTransform-5.1 \
#        -lITKPath-5.1 \
#        -lITKMetaIO-5.1 \
#        -litkzlib-5.1 \
#        -lITKLabelMap-5.1 \
#        -lITKStatistics-5.1 \
#        -lITKCommon-5.1 \
#        -litksys-5.1 \
#        -litkdouble-conversion-5.1 \
#        -lITKVNLInstantiation-5.1 \
#        -litkvnl_algo-5.1 \
#        -litkvnl-5.1 \
##        -litknetlib-5.1 \
#        -litkvcl-5.1 \
#        -lpthread \
#        -lm \
##        -ldl \
#        -litkNetlibSlatec-5.1 \
#        -litkv3p_netlib-5.1 \
#        -lm

#############################################################################################################################

equals(has_simple_elastix, 1){

    # for simple elastix (static)
    ######################################################################################################################

    QMAKE_CXXFLAGS += -DSIMPLE_ITK_2_0

    INCLUDEPATH += "D:/build/SimpleElastix-master/build/ITK-prefix/include/ITK-5.1"
    INCLUDEPATH += "D:/build/SimpleElastix-master/install/include/SimpleITK-2.0"

    #SimpleITK
    LIBS += -LD:/build/SimpleElastix-master/install/lib \
            -lSimpleITKCommon-2.0 \
            -lSimpleITKIO-2.0 \
            -lSimpleITKRegistration-2.0 \
            -lSimpleITKBasicFilters0-2.0 \
            -lSimpleITK_ITKAnisotropicSmoothing-2.0 \
            -lSimpleITK_ITKAntiAlias-2.0 \
            -lSimpleITK_ITKBiasCorrection-2.0 \
            -lSimpleITK_ITKBinaryMathematicalMorphology-2.0 \
            -lSimpleITK_ITKClassifiers-2.0 \
            -lSimpleITK_ITKColormap-2.0 \
            -lSimpleITK_ITKCommon-2.0 \
            -lSimpleITK_ITKConnectedComponents-2.0 \
            -lSimpleITK_ITKConvolution-2.0 \
            -lSimpleITK_ITKCurvatureFlow-2.0 \
            -lSimpleITK_ITKDeconvolution-2.0 \
            -lSimpleITK_ITKDenoising-2.0 \
            -lSimpleITK_ITKDisplacementField-2.0 \
            -lSimpleITK_ITKDistanceMap-2.0 \
            -lSimpleITK_ITKFFT-2.0 \
            -lSimpleITK_ITKFastMarching-2.0 \
            -lSimpleITK_ITKImageCompare-2.0 \
            -lSimpleITK_ITKImageCompose-2.0 \
            -lSimpleITK_ITKImageFeature-2.0 \
            -lSimpleITK_ITKImageFilterBase-2.0 \
            -lSimpleITK_ITKImageFunction-2.0 \
            -lSimpleITK_ITKImageFusion-2.0 \
            -lSimpleITK_ITKImageGradient-2.0 \
            -lSimpleITK_ITKImageGrid-2.0 \
            -lSimpleITK_ITKImageIntensity-2.0 \
            -lSimpleITK_ITKImageLabel-2.0 \
            -lSimpleITK_ITKImageNoise-2.0 \
            -lSimpleITK_ITKImageSources-2.0 \
            -lSimpleITK_ITKImageStatistics-2.0 \
            -lSimpleITK_ITKLabelMap-2.0 \
            -lSimpleITK_ITKLabelVoting-2.0 \
            -lSimpleITK_ITKLevelSets-2.0 \
            -lSimpleITK_ITKMathematicalMorphology-2.0 \
            -lSimpleITK_ITKPDEDeformableRegistration-2.0 \
            -lSimpleITK_ITKRegionGrowing-2.0 \
            -lSimpleITK_ITKRegistrationCommon-2.0 \
            -lSimpleITK_ITKReview-2.0 \
            -lSimpleITK_ITKSmoothing-2.0 \
            -lSimpleITK_ITKThresholding-2.0 \
            -lSimpleITK_ITKTransform-2.0 \
            -lSimpleITK_ITKWatersheds-2.0 \
            -lSimpleITK_SimpleITKFilters-2.0 \
            -lSimpleITKBasicFilters1-2.0 \
            -lSimpleITKIO-2.0 \
            -lElastixImageFilter-2.0 \
            -lTransformixImageFilter-2.0


    LIBS += -LD:/build/SimpleElastix-master/build/Elastix-prefix/lib \
            -lelastix-5.0 \
            -lAdaGrad-5.0 \
            -lAdaptiveStochasticGradientDescent-5.0 \
            -lAdaptiveStochasticLBFGS-5.0 \
            -lAdaptiveStochasticVarianceReducedGradient-5.0 \
            -lAdvancedAffineTransformElastix-5.0 \
            -lAdvancedBSplineTransform-5.0 \
            -lAdvancedKappaStatisticMetric-5.0 \
            -lAdvancedMattesMutualInformationMetric-5.0 \
            -lAdvancedMeanSquaresMetric-5.0 \
            -lAdvancedNormalizedCorrelationMetric-5.0 \
            -lAffineDTITransformElastix-5.0 \
            -lAffineLogStackTransform-5.0 \
            -lAffineLogTransformElastix-5.0 \
            -lBSplineInterpolator-5.0 \
            -lBSplineInterpolatorFloat-5.0 \
            -lBSplineResampleInterpolator-5.0 \
            -lBSplineResampleInterpolatorFloat-5.0 \
            -lBSplineStackTransform-5.0 \
            -lCorrespondingPointsEuclideanDistanceMetric-5.0 \
            -lDeformationFieldTransform-5.0 \
            -lDisplacementMagnitudePenalty-5.0 \
            -lelxCommon-5.0 \
            -lelxCore-5.0 \
            -lEulerStackTransform-5.0 \
            -lEulerTransformElastix-5.0 \
            -lFixedGenericPyramid-5.0 \
            -lFixedRecursivePyramid-5.0 \
            -lFixedShrinkingPyramid-5.0 \
            -lFixedSmoothingPyramid-5.0 \
            -lGridSampler-5.0 \
            -lLinearInterpolator-5.0 \
            -lLinearResampleInterpolator-5.0 \
            -lmevisdcmtiff-5.0 \
            -lMovingGenericPyramid-5.0 \
            -lMovingRecursivePyramid-5.0 \
            -lMovingShrinkingPyramid-5.0 \
            -lMovingSmoothingPyramid-5.0 \
            -lMultiInputRandomCoordinateSampler-5.0 \
            -lMultiMetricMultiResolutionRegistration-5.0 \
            -lMultiResolutionRegistration-5.0 \
            -lMultiResolutionRegistrationWithFeatures-5.0 \
            -lMutualInformationHistogramMetric-5.0 \
            -lMyStandardResampler-5.0 \
            -lNearestNeighborInterpolator-5.0 \
            -lNearestNeighborResampleInterpolator-5.0 \
            -lNormalizedGradientCorrelationMetric-5.0 \
            -lNormalizedMutualInformationMetric-5.0 \
            -lparam-5.0 \
            -lPCAMetric-5.0 \
            -lPCAMetric2-5.0 \
            -lPreconditionedStochasticGradientDescent-5.0 \
            -lQuasiNewtonLBFGS-5.0 \
            -lRandomCoordinateSampler-5.0 \
            -lRandomSampler-5.0 \
            -lRandomSamplerSparseMask-5.0 \
            -lRecursiveBSplineTransform-5.0 \
            -lReducedDimensionBSplineInterpolator-5.0 \
            -lReducedDimensionBSplineResampleInterpolator-5.0 \
            -lSimilarityTransformElastix-5.0 \
            -lSplineKernelTransform-5.0 \
            -lStandardGradientDescent-5.0 \
            -lSumOfPairwiseCorrelationCoefficientsMetric-5.0 \
            -lSumSquaredTissueVolumeDifferenceMetric-5.0 \
            -lTransformBendingEnergyPenalty-5.0 \
            -ltransformix-5.0 \
            -lTransformRigidityPenalty-5.0 \
            -lTranslationStackTransform-5.0 \
            -lTranslationTransformElastix-5.0 \
            -lVarianceOverLastDimensionMetric-5.0 \
            -lWeightedCombinationTransformElastix-5.0 \
            -lxoutlib-5.0 \
            -lelxCommon-5.0

    #ITK
    LIBS += -LD:/build/SimpleElastix-master/build/ITK-prefix/lib \
            -lITKIOBruker-5.1 \
            -lITKIOHDF5-5.1 \
            -lITKIOLSM-5.1 \
            -lITKIOMINC-5.1 \
            -litkminc2-5.1 \
            -lITKIOMRC-5.1 \
            -lSimpleITKRegistration-2.0 \
            -lITKOptimizersv4-5.1 \
            -litklbfgs-5.1 \
            -lITKIOMeshBase-5.1 \
            -lITKIOMeshBYU-5.1 \
            -lITKIOMeshGifti-5.1 \
            -lITKgiftiio-5.1 \
            -lITKIOMeshOFF-5.1 \
            -lITKIOMeshOBJ-5.1 \
            -lITKIOMeshFreeSurfer-5.1 \
            -lITKIOMeshVTK-5.1

    #SimpleITK
    LIBS += -LD:/build/SimpleElastix-master/install/lib \
            -lSimpleITK_ITKAnisotropicSmoothing-2.0 \
            -lSimpleITK_ITKAntiAlias-2.0 \
            -lSimpleITK_ITKBiasCorrection-2.0 \
            -lSimpleITK_ITKBinaryMathematicalMorphology-2.0 \
            -lSimpleITK_ITKClassifiers-2.0 \
            -lSimpleITK_ITKColormap-2.0 \
            -lSimpleITK_ITKCommon-2.0 \
            -lSimpleITK_ITKConnectedComponents-2.0 \
            -lSimpleITK_ITKConvolution-2.0 \
            -lSimpleITK_ITKCurvatureFlow-2.0 \
            -lSimpleITK_ITKDeconvolution-2.0 \
            -lSimpleITK_ITKDenoising-2.0 \
            -lSimpleITK_ITKDisplacementField-2.0 \
            -lSimpleITK_ITKDistanceMap-2.0 \
            -lSimpleITK_ITKFFT-2.0 \
            -lSimpleITK_ITKFastMarching-2.0 \
            -lSimpleITK_ITKImageCompare-2.0 \
            -lSimpleITK_ITKImageCompose-2.0 \
            -lSimpleITK_ITKImageFeature-2.0 \
            -lSimpleITK_ITKImageFilterBase-2.0 \
            -lSimpleITK_ITKImageFunction-2.0 \
            -lSimpleITK_ITKImageFusion-2.0 \
            -lSimpleITK_ITKImageGradient-2.0 \
            -lSimpleITK_ITKImageGrid-2.0 \
            -lSimpleITK_ITKImageIntensity-2.0 \
            -lSimpleITK_ITKImageLabel-2.0 \
            -lSimpleITK_ITKImageNoise-2.0 \
            -lSimpleITK_ITKImageSources-2.0 \
            -lSimpleITK_ITKImageStatistics-2.0 \
            -lSimpleITK_ITKLabelMap-2.0 \
            -lSimpleITK_ITKLabelVoting-2.0 \
            -lSimpleITK_ITKLevelSets-2.0 \
            -lSimpleITK_ITKMathematicalMorphology-2.0 \
            -lSimpleITK_ITKPDEDeformableRegistration-2.0 \
            -lSimpleITK_ITKRegionGrowing-2.0 \
            -lSimpleITK_ITKRegistrationCommon-2.0 \
            -lSimpleITK_ITKReview-2.0 \

    LIBS += -LD:/build/SimpleElastix-master/build/ITK-prefix/lib \
            -lITKReview-5.1 \
            -lITKIOGDCM-5.1 \
            -litkgdcmMSFF-5.1 \
            -litkgdcmDICT-5.1 \
            -litkgdcmIOD-5.1 \
            -litkgdcmDSED-5.1 \
            -litkgdcmCommon-5.1 \
            -litkgdcmjpeg8-5.1 \
            -litkgdcmjpeg12-5.1 \
            -litkgdcmjpeg16-5.1 \
            -litkgdcmopenjp2-5.1 \
            -litkgdcmcharls-5.1 \
            -lITKIOBMP-5.1 \
            -lITKIOBioRad-5.1 \
            -lITKIOGE-5.1 \
            -lITKIOGIPL-5.1 \
            -lITKIOJPEG-5.1 \
            -lITKIOJPEG2000-5.1 \
            -lITKIOMeta-5.1 \
            -lITKIONIFTI-5.1 \
            -lITKniftiio-5.1 \
            -lITKznz-5.1 \
            -lITKIONRRD-5.1 \
            -lITKNrrdIO-5.1 \
            -lITKIOPNG-5.1 \
            -litkpng-5.1 \
            -lITKIOStimulate-5.1 \
            -lITKIOTIFF-5.1 \
            -litktiff-5.1 \
            -litkjpeg-5.1 \
            -lITKIOVTK-5.1 \
            -lITKIOTransformHDF5-5.1 \
            -llibitkhdf5_cpp \
            -llibitkhdf5 \
            -lITKIOTransformInsightLegacy-5.1 \
            -lITKRegistrationMethodsv4-5.1 \
            -lITKIOTransformMatlab-5.1 \
            -lITKQuadEdgeMesh-5.1 \
            -lITKBiasCorrection-5.1 \
            -lITKPolynomials-5.1 \
            -lITKOptimizers-5.1 \
            -lITKIOSpatialObjects-5.1 \
            -lITKIOSiemens-5.1 \
            -lITKIOIPL-5.1 \
            -lITKIOXML-5.1 \
            -lITKIOImageBase-5.1 \
            -lITKEXPAT-5.1 \
            -lITKKLMRegionGrowing-5.1 \
            -litkopenjpeg-5.1 \
            -lITKVTK-5.1 \
            -lITKDenoising-5.1

    #SimpleITK
    LIBS += -LD:/build/SimpleElastix-master/install/lib \
            -lSimpleITK_ITKSmoothing-2.0 \
            -lSimpleITK_ITKThresholding-2.0 \
            -lSimpleITK_ITKTransform-2.0 \
            -lSimpleITK_ITKWatersheds-2.0

    #ITK
    LIBS += -LD:/build/SimpleElastix-master/build/ITK-prefix/lib \
            -lITKWatersheds-5.1

    #SimpleITK
    LIBS += -LD:/build/SimpleElastix-master/install/lib \
            -lSimpleITK_SimpleITKFilters-2.0 \
            -lSimpleITKBasicFilters0-2.0 \
            -lSimpleITKCommon-2.0

    #ITK
    LIBS += -LD:/build/SimpleElastix-master/build/ITK-prefix/lib \
            -lITKIOTransformBase-5.1 \
            -lITKTransformFactory-5.1

    #ITK
    LIBS += -LD:/build/SimpleElastix-master/build/ITK-prefix/lib \
            -lITKSpatialObjects-5.1 \
            -lITKMesh-5.1 \
            -lITKTransform-5.1 \
            -lITKPath-5.1 \
            -lITKMetaIO-5.1 \
            -litkzlib-5.1 \
            -lITKLabelMap-5.1 \
            -lITKStatistics-5.1 \
            -lITKCommon-5.1 \
            -litksys-5.1 \
            -lITKSmoothing-5.1 \
            -litkdouble-conversion-5.1 \
            -lITKVNLInstantiation-5.1 \
            -litkvnl_algo-5.1 \
            -litkvnl-5.1 \
            -litkvcl-5.1 \
            -lpthread \
            -lm \
            -litkNetlibSlatec-5.1 \
            -litkv3p_netlib-5.1 \
            -lm \
            -lpsapi

    INCLUDEPATH += "..\libs\mingw32_5.3.0\glew-2.0.0\include" "..\include"

    LIBS  += -L"..\libs\mingw64_730\glew-2.1.0\lib" \
             -lglew32 -lglu32 -lglu32 -lopengl32 -lOPENGL32 -lGLU32 \
             -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm \
             -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 \
             -ladvapi32 -lwsock32 -lodbc32

}
