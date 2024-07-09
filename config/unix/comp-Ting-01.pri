##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

CONFIG += static
message("static-": $$QMAKE_HOST.name);

INCLUDEPATH +=  /usr/include/c++/9/ \
                /usr/include/x86_64-linux-gnu \
                /usr/lib/gcc/x86_64-linux-gnu/5/include \
                /usr/local/include/gdcm-3.0 \
                /usr/local/include/glm \
                ./cimg \
                ./model

#statics libraries
LIBS += /usr/lib/libniftiio.a /usr/lib/libznz.a /usr/lib/x86_64-linux-gnu/libz.a
LIBS += /usr/lib64/libGLEW.a /usr/lib/x86_64-linux-gnu/libGLU.a  -L/usr/lib -lGL -lpthread

#shared libraries
#LIBS += -L/usr/lib/x86_64-linux-gnu -lXext -lX11
if (equals(has_simple_elastix, 1)|equals(has_atlas_labeling,1)) {

    QMAKE_CXXFLAGS += -DSIMPLE_ITK_2_0

    # for simple elastix (static)
    ######################################################################################################################
     INCLUDEPATH += /opt/SimpleElastix/include/SimpleElastix
     INCLUDEPATH += /opt/SimpleElastix/include/ITK-5.1
     INCLUDEPATH += /opt/SimpleElastix/include/SimpleITK-2.0
#    INCLUDEPATH += "C:/build/SimpleElastix-master/build/ITK-prefix/include/ITK-5.1"
#    INCLUDEPATH += "C:/build/SimpleElastix-master/install/include/SimpleITK-2.0"

     LIBS += -L/opt/SimpleElastix/lib/ \
             -L/opt/SimpleElastix/lib/Elastix \
             -L/opt/SimpleElastix/lib/ITK \
             -L/opt/SimpleElastix/lib

    #SimpleITK
#    LIBS += -L/opt/SimpleElastix/lib/ \
#    LIBS += -LC:/build/SimpleElastix-master/install/lib \
     LIBS +=-lSimpleITKCommon-2.0 \
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

#    LIBS += -LC:/build/SimpleElastix-master/build/Elastix-prefix/lib \
    LIBS += -lelastix-5.0 \
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
#   LIBS += -LC:/build/SimpleElastix-master/build/ITK-prefix/lib \
    LIBS += -lITKIOBruker-5.1 \
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
#    LIBS += -LC:/build/SimpleElastix-master/install/lib \
     LIBS += -lSimpleITK_ITKAnisotropicSmoothing-2.0 \
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

#    LIBS += -LC:/build/SimpleElastix-master/build/ITK-prefix/lib \
    LIBS += -lITKReview-5.1 \
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
#            -llibf5_cpp \
#            -llibitkhdf5 \
            -litkhdf5_cpp \
            -litkhdf5 \
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
#    LIBS += -LC:/build/SimpleElastix-master/install/lib \
     LIBS += -lSimpleITK_ITKSmoothing-2.0 \
            -lSimpleITK_ITKThresholding-2.0 \
            -lSimpleITK_ITKTransform-2.0 \
            -lSimpleITK_ITKWatersheds-2.0

    #ITK
#    LIBS += -LC:/build/SimpleElastix-master/build/ITK-prefix/lib \
    LIBS += -lITKWatersheds-5.1

    #SimpleITK
#    LIBS += -LC:/build/SimpleElastix-master/install/lib \
    LIBS += -lSimpleITK_SimpleITKFilters-2.0 \
            -lSimpleITKBasicFilters0-2.0 \
            -lSimpleITKCommon-2.0

    #ITK
#    LIBS += -LC:/build/SimpleElastix-master/build/ITK-prefix/lib \
    LIBS += -lITKIOTransformBase-5.1 \
            -lITKTransformFactory-5.1

    #ITK
#    LIBS += -LC:/build/SimpleElastix-master/build/ITK-prefix/lib \
    LIBS += -lITKSpatialObjects-5.1 \
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
            -lm
#            -lpsapi

#LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
#         -lgdcmexpat -lgdcmMEXD \
#         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx -lm -ldl

#    INCLUDEPATH += "..\libs\mingw32_5.3.0\glew-2.0.0\include" "..\include"

#    LIBS  += -L"..\libs\mingw64_730\glew-2.1.0\lib" \
#             -lglew32 -lglu32 -lglu32 -lopengl32 -lOPENGL32 -lGLU32 \
#             -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm \
#             -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 \
#             -ladvapi32 -lwsock32 -lodbc32

}
LIBS += -L/usr/local/lib -lgdcmMSFF -lgdcmDICT -lgdcmIOD -lgdcmcharls -lgdcmDSED -lgdcmCommon -lgdcmzlib \
         -lgdcmexpat -lgdcmMEXD \
         -lgdcmuuid -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjp2 -lgdcmuuid -lsocketxx -lm -ldl

