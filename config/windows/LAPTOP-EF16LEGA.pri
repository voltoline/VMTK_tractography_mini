##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

CONFIG += static
message("static-": $$QMAKE_HOST.name);

# Transfer to atlas_labeling.pri
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

QMAKE_CXXFLAGS += -DSIMPLE_ITK_2_1
#QMAKE_CXXFLAGS += -DSIMPLE_ITK_2_0


#Includes
INCLUDEPATH += "C:\SimpleITK-master\SimpleITK-Build\ITK-prefix\include\ITK-5.2"
INCLUDEPATH += "C:\SimpleITK-master\Install\include\SimpleITK-2.1"
INCLUDEPATH += ".\ui"


#SimpleITK
LIBS += -LC:\SimpleITK-master\Install\bin\ \
        -lSimpleITKCommon-2.1 \
        -lSimpleITKIO-2.1 \
        -lSimpleITKRegistration-2.1 \
        -lSimpleITKBasicFilters0-2.1 \
        -lSimpleITK_ITKAnisotropicSmoothing-2.1 \
        -lSimpleITK_ITKAntiAlias-2.1 \
        -lSimpleITK_ITKBiasCorrection-2.1 \
        -lSimpleITK_ITKBinaryMathematicalMorphology-2.1 \
        -lSimpleITK_ITKClassifiers-2.1 \
        -lSimpleITK_ITKColormap-2.1 \
        -lSimpleITK_ITKCommon-2.1 \
        -lSimpleITK_ITKConnectedComponents-2.1 \
        -lSimpleITK_ITKConvolution-2.1 \
        -lSimpleITK_ITKCurvatureFlow-2.1 \
        -lSimpleITK_ITKDeconvolution-2.1 \
        -lSimpleITK_ITKDenoising-2.1 \
        -lSimpleITK_ITKDisplacementField-2.1 \
        -lSimpleITK_ITKDistanceMap-2.1 \
        -lSimpleITK_ITKFFT-2.1 \
        -lSimpleITK_ITKFastMarching-2.1 \
        -lSimpleITK_ITKImageCompare-2.1 \
        -lSimpleITK_ITKImageCompose-2.1 \
        -lSimpleITK_ITKImageFeature-2.1 \
        -lSimpleITK_ITKImageFilterBase-2.1 \
        -lSimpleITK_ITKImageFunction-2.1 \
        -lSimpleITK_ITKImageFusion-2.1 \
        -lSimpleITK_ITKImageGradient-2.1 \
        -lSimpleITK_ITKImageGrid-2.1 \
        -lSimpleITK_ITKImageIntensity-2.1 \
        -lSimpleITK_ITKImageLabel-2.1 \
        -lSimpleITK_ITKImageNoise-2.1 \
        -lSimpleITK_ITKImageSources-2.1 \
        -lSimpleITK_ITKImageStatistics-2.1 \
        -lSimpleITK_ITKLabelMap-2.1 \
        -lSimpleITK_ITKLabelVoting-2.1 \
        -lSimpleITK_ITKLevelSets-2.1 \
        -lSimpleITK_ITKMathematicalMorphology-2.1 \
        -lSimpleITK_ITKPDEDeformableRegistration-2.1 \
        -lSimpleITK_ITKRegionGrowing-2.1 \
        -lSimpleITK_ITKRegistrationCommon-2.1 \
        -lSimpleITK_ITKReview-2.1 \
        -lSimpleITK_ITKSmoothing-2.1 \
        -lSimpleITK_ITKThresholding-2.1 \
        -lSimpleITK_ITKTransform-2.1 \
        -lSimpleITK_ITKWatersheds-2.1 \
        -lSimpleITK_SimpleITKFilters-2.1 \
        -lSimpleITKBasicFilters1-2.1 \
        -lSimpleITKIO-2.1

#ITK
LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
        -lITKIOBruker-5.2 \
        -lITKIOHDF5-5.2 \
        -lITKIOLSM-5.2 \
        -lITKIOMINC-5.2

LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\lib -litkminc2-5.2 \

LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
#        -lrt \
        -lITKIOMRC-5.2 \
        -lSimpleITKRegistration-2.1 \
        -lITKOptimizersv4-5.2 \
        -litklbfgs-5.2 \

#SimpleITK
LIBS += -LC:\SimpleITK-master\Install\bin\ \
        -lSimpleITK_ITKAnisotropicSmoothing-2.1 \
        -lSimpleITK_ITKAntiAlias-2.1 \
        -lSimpleITK_ITKBiasCorrection-2.1 \
        -lSimpleITK_ITKBinaryMathematicalMorphology-2.1 \
        -lSimpleITK_ITKClassifiers-2.1 \
        -lSimpleITK_ITKColormap-2.1 \
        -lSimpleITK_ITKCommon-2.1 \
        -lSimpleITK_ITKConnectedComponents-2.1 \
        -lSimpleITK_ITKConvolution-2.1 \
        -lSimpleITK_ITKCurvatureFlow-2.1 \
        -lSimpleITK_ITKDeconvolution-2.1 \
        -lSimpleITK_ITKDenoising-2.1 \
        -lSimpleITK_ITKDisplacementField-2.1 \
        -lSimpleITK_ITKDistanceMap-2.1 \
        -lSimpleITK_ITKFFT-2.1 \
        -lSimpleITK_ITKFastMarching-2.1 \
        -lSimpleITK_ITKImageCompare-2.1 \
        -lSimpleITK_ITKImageCompose-2.1 \
        -lSimpleITK_ITKImageFeature-2.1 \
        -lSimpleITK_ITKImageFilterBase-2.1 \
        -lSimpleITK_ITKImageFunction-2.1 \
        -lSimpleITK_ITKImageFusion-2.1 \
        -lSimpleITK_ITKImageGradient-2.1 \
        -lSimpleITK_ITKImageGrid-2.1 \
        -lSimpleITK_ITKImageIntensity-2.1 \
        -lSimpleITK_ITKImageLabel-2.1 \
        -lSimpleITK_ITKImageNoise-2.1 \
        -lSimpleITK_ITKImageSources-2.1 \
        -lSimpleITK_ITKImageStatistics-2.1 \
        -lSimpleITK_ITKLabelMap-2.1 \
        -lSimpleITK_ITKLabelVoting-2.1 \
        -lSimpleITK_ITKLevelSets-2.1 \
        -lSimpleITK_ITKMathematicalMorphology-2.1 \
        -lSimpleITK_ITKPDEDeformableRegistration-2.1 \
        -lSimpleITK_ITKRegionGrowing-2.1 \
        -lSimpleITK_ITKRegistrationCommon-2.1 \
        -lSimpleITK_ITKReview-2.1 \

LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
        -lITKReview-5.2 \
        -lITKIOGDCM-5.2 \
        -litkgdcmMSFF-5.2 \
        -litkgdcmDICT-5.2 \
        -litkgdcmIOD-5.2 \
        -litkgdcmDSED-5.2 \
        -litkgdcmCommon-5.2 \
        -litkgdcmjpeg8-5.2 \
        -litkgdcmjpeg12-5.2 \
        -litkgdcmjpeg16-5.2 \
        -litkgdcmopenjp2-5.2 \
        -litkgdcmcharls-5.2 \
        #-litkgdcmuuid-5.2 \
        -lITKIOBMP-5.2 \
        -lITKIOBioRad-5.2 \
        -lITKIOGE-5.2 \
        -lITKIOGIPL-5.2 \
        -lITKIOJPEG-5.2 \
        -lITKIOMeta-5.2 \
        -lITKIONIFTI-5.2 \
        -lITKniftiio-5.2 \
        -lITKznz-5.2 \
        -lITKIONRRD-5.2 \
        -lITKNrrdIO-5.2 \
        -lITKIOPNG-5.2 \
        -litkpng-5.2 \
        -lITKIOStimulate-5.2 \
        -lITKIOTIFF-5.2 \
        -litktiff-5.2 \
        -litkjpeg-5.2 \
        -lITKIOVTK-5.2 \
        -lITKIOTransformHDF5-5.2 \
        -litkhdf5_cpp-shared-5.2 \
        -litkhdf5-shared-5.2 \
        -lITKIOTransformInsightLegacy-5.2 \
        -lITKIOTransformMatlab-5.2 \
        -lITKQuadEdgeMesh-5.2 \
        -lITKBiasCorrection-5.2 \
        -lITKPolynomials-5.2 \
#        -lITKBioCell-5.2 \
#        -lITKFEM-5.2 \
        -lITKOptimizers-5.2 \
        -lITKIOSpatialObjects-5.2 \
        -lITKIOSiemens-5.2 \
        -lITKIOIPL-5.2 \
        -lITKIOXML-5.2 \
        -lITKIOImageBase-5.2 \
        -lITKEXPAT-5.2 \
        -lITKKLMRegionGrowing-5.2 \
        -litkopenjpeg-5.2 \
        -lITKVTK-5.2 \

#SimpleITK
LIBS += -LC:\SimpleITK-master\Install\bin\ \
        -lSimpleITK_ITKSmoothing-2.1 \
        -lSimpleITK_ITKThresholding-2.1 \
        -lSimpleITK_ITKTransform-2.1 \
        -lSimpleITK_ITKWatersheds-2.1

#ITK
LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
        -lITKWatersheds-5.2

#SimpleITK
LIBS += -LC:\SimpleITK-master\Install\bin\ \
        -lSimpleITK_SimpleITKFilters-2.1 \
        -lSimpleITKBasicFilters0-2.1 \
        -lSimpleITKCommon-2.1

#ITK
LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
        -lITKIOTransformBase-5.2 \
        -lITKTransformFactory-5.2


#SimpleITK
#LIBS += -LC:\SimpleITK-master\Install\bin\ \
#         -lSimpleITKExplicit-2.1

#ITK
LIBS += -LC:\SimpleITK-master\SimpleITK-Build\ITK-prefix\bin\ \
        -lITKSpatialObjects-5.2 \
        -lITKMesh-5.2 \
        -lITKTransform-5.2 \
        -lITKPath-5.2 \
        -lITKMetaIO-5.2 \
        -litkzlib-5.2 \
        -lITKLabelMap-5.2 \
        -lITKStatistics-5.2 \
        -lITKCommon-5.2 \
        -litksys-5.2 \
        -litkdouble-conversion-5.2 \
        -lITKVNLInstantiation-5.2 \
        -litkvnl_algo-5.2 \
        -litkvnl-5.2 \
#        -litknetlib-5.2 \
        -litkvcl-5.2 \
        -lpthread \
        -lm \
#        -ldl \
        -litkNetlibSlatec-5.2 \
        -litkv3p_netlib-5.2 \
        -lm

INCLUDEPATH += C:\Users\trigu\Desktop\Felippe\VMTK\glew-2.1.0\include \
               C:\Users\trigu\Desktop\Felippe\VMTK\gdcm-3.0.8\Install\bin \
               C:\Users\trigu\Desktop\Felippe\VMTK\glm\ \
               ./cimg \
               ./model
#                ./model/dti_fit

 LIBS += -L"C:\Users\trigu\Desktop\Felippe\VMTK\gdcm-3.0.8\Install\bin" -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls\
         -lgdcmDSED -lgdcmCommon -lgdcmzlib -lgdcmexpat -lgdcmMEXD \#-lgdcmuuid
         -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lsocketxx -lm #-ldl -lgdcmuuid

LIBS += -L"C:\Users\trigu\Desktop\Felippe\VMTK\glew-2.1.0\Install\bin" -lglew32

#shared libraries
#LIBS += -lGLU
#LIBS += -L/usr/lib64 -lGLEW
#LIBS += -L/usr/lib/x86_64-linux-gnu
#LIBS += -L/usr/lib/x86_64-linux-gnu #-lboost_thread -lboost_system
#LIBS += -L/usr/lib -lGL -lpthread -lXext -lX11
#LIBS += -L/usr/lib/x86_64-linux-gnu -lniftiio -lznz -lz
