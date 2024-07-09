##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/controllers/ControllerDTITractographyToolTab.h \
    $$PWD/event_handlers/EventHandlerDTI.h \
    $$PWD/event_handlers/EventHandlerDTIFiberTracking.h \
    $$PWD/models/ModelDTI.h \
    $$PWD/models/ModelUIStateDTI.h \
    $$PWD/models/color_map/ColorMapBase.h \
    $$PWD/models/color_map/VEC2RGBAbsoluteColorMap.h \
    $$PWD/models/tensor_fitter/DTI.h \
    $$PWD/models/tractography/FiberCluster.h \
    $$PWD/models/tractography/FiberClusterSet.h \
    $$PWD/models/tractography/FiberTrackingAlgorithm.h \
    $$PWD/models/tractography/FiberTrackingAlgorithmComputeShader.h \
    $$PWD/models/tractography/FiberTrackingAlgorithmGeometryShader.h \
    $$PWD/models/tractography/FiberTrackingFilterCreator.h \
    $$PWD/models/tractography/FiberTrackingSeedCreator.h \
    $$PWD/renderer/Renderer3DDTIData.h \
    $$PWD/DTIOpenGLResource.h \
    $$PWD/VisSubModuleDTI.h \
    $$PWD/renderer/fiber/FiberRenderer.h \
    $$PWD/event_handlers/EventHandler3DTransformationToDTI.h \
    $$PWD/ui/ToolTractography.h \
    $$PWD/ui/qt/QDTITractsUIHandler.h \
    $$PWD/ui/qt/QDTIroiUIHandler.h \
    $$PWD/ui/qt/QSimpleTableModel.h \
    $$PWD/ui/qt/QTabDTITractography.h \
    $$PWD/ui/qt/UIFactoryDTIQt.h

SOURCES += \
    $$PWD/controllers/ControllerDTITractographyToolTab.cpp \
    $$PWD/event_handlers/EventHandlerDTI.cpp \
    $$PWD/event_handlers/EventHandlerDTIFiberTracking.cpp \
    $$PWD/models/ModelDTI.cpp \
    $$PWD/models/ModelUIStateDTI.cpp \
    $$PWD/models/color_map/ColorMapBase.cpp \
    $$PWD/models/color_map/VEC2RGBAbsoluteColorMap.cpp \
    $$PWD/models/tensor_fitter/DTI.cpp \
    $$PWD/models/tractography/FiberCluster.cpp \
    $$PWD/models/tractography/FiberClusterSet.cpp \
    $$PWD/models/tractography/FiberTrackingAlgorithm.cpp \
    $$PWD/models/tractography/FiberTrackingAlgorithmComputeShader.cpp \
    $$PWD/models/tractography/FiberTrackingAlgorithmGeometryShader.cpp \
    $$PWD/models/tractography/FiberTrackingFilterCreator.cpp \
    $$PWD/models/tractography/FiberTrackingSeedCreator.cpp \
    $$PWD/renderer/Renderer3DDTIData.cpp \
    $$PWD/DTIOpenGLResource.cpp \
    $$PWD/VisSubModuleDTI.tpp \
    $$PWD/renderer/fiber/FiberRenderer.cpp \
    $$PWD/event_handlers/EventHandler3DTransformationToDTI.cpp \
    $$PWD/ui/ToolTractography.cpp \
    $$PWD/ui/qt/QDTITractsUIHandler.cpp \
    $$PWD/ui/qt/QDTIroiUIHandler.cpp \
    $$PWD/ui/qt/QSimpleTableModel.cpp \
    $$PWD/ui/qt/QTabDTITractography.cpp \
    $$PWD/ui/qt/UIFactoryDTIQt.cpp

DISTFILES += \
    $$PWD/models/tractography/diff_particles.geom.glsl \
    $$PWD/models/tractography/diff_particles.vert \
    $$PWD/models/tractography/fiber_tracking.comp.glsl \
    $$PWD/models/tractography/seed_filtering.vert \
    $$PWD/renderer/fiber/diff_particles.frag \
    $$PWD/renderer/fiber/render_fiber.vert

FORMS += \
    $$PWD/ui/qt/QTabDTITractography.ui
