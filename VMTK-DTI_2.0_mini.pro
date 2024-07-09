#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T15:51:21
#
#-------------------------------------------------
message(PC name: $$QMAKE_HOST.name);
QT       += core gui opengl charts


lessThan(QT_VERSION, 6.0.0){
DEFINES += enableOldQGL
CONFIG += c++11
}

# ensure one "debug" or "release" in CONFIG so they can be used as
#   conditionals instead of writing "CONFIG(debug, debug|release)"...
CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
}

CONFIG(release, debug|release) {
    CONFIG -= debug release
    CONFIG += release
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VMTK-Neuro4.0_mini
QMAKE_CXXFLAGS += -D_CORE_PROFILE -D_DEBUG

!macx {
    QMAKE_CXXFLAGS += -fopenmp -std=gnu++11 ##-march=native  # -O3
    LIBS += -fopenmp
    QMAKE_CFLAGS_RELEASE += -fopenmp
    QMAKE_CFLAGS_DEBUG += -fopenmp
}

DEFINES += "LARGEADDRESSAWARE"

SOURCES +=\
    core/Tool.tpp \
    core/ToolBase.cpp \
    core/UIFactoryBase.cpp \
    modules/visualization/VisualizationModule.cpp \
    renderer/SubRendererBase.cpp \
    ui/QWidgetOpenGL.cpp \
    ui/QMain.cpp \
    ui/QMainWindowVMTK.cpp \
    renderer/vmtkManagementShader.cpp \
    renderer/vmtkFrameBufferObject.cpp \
    renderer/vmtkRender2D.cpp \
    renderer/vmtkRender3D.cpp \
    ui/components/QDialogButton.cpp \
    ui/qt/UIFactoryQt.cpp \
    utilities/DataBuffer.cpp \
    utilities/ImageBuffer.cpp \
    utilities/QManagementResources.cpp \
    utilities/QCheckingOpenGL.cpp \
    utilities/vmath.cpp \
    renderer/vmtkAbstractRender.cpp \
    utilities/vmtkEqualizer.cpp \
    ui/QCheckedTabWidget.cpp \
    ui/QFrameContentWindow.cpp \
    ui/QSplashScreenCopyright.cpp \
    ui/components/QTabLayout.cpp \
    model/BasePlugin.cpp \
    utilities/functions/vmtkGeneralFunctions.cpp \
    utilities/QLoadImage.cpp \
    ui/RenderingSurface.cpp \
    ui/components/RenderingSurfaceSet.cpp \
    model/ModelObserver.cpp \
    ui/CommonGUIFactory.cpp \
    ui/OpenGLVisualizationTab.cpp \
    utilities/cursor/FreeDrawCursor.cpp \
    model/EventNotifier.cpp \
    ui/QCustomProgressDialog.cpp \
    model/ModelSubject.tpp \
    core/BaseModule.cpp \
    core/ModuleManager.cpp \
    modules/visualization/StateManager.cpp \
    core/BaseToolTab.cpp \
    core/MainWindow.cpp \
    core/ToolTab.tpp \
    core/ProgressDialog.cpp \
    core/VisualizationTab.cpp \
    modules/PluginFileManager.cpp \
    modules/visualization/DiffusionRegisterVisualizationModule.cpp

HEADERS  += \
    core/Tool.h \
    core/ToolBase.h \
    core/UIFactoryBase.h \
    renderer/SubRendererBase.h \
    ui/QWidgetOpenGL.h \
    ui/components/QDialogButton.h \
    ui/qt/UIFactoryQt.h \
    utilities/DataBuffer.h \
    utilities/ImageBuffer.h \
    utilities/vmath.h \
    ui/QMainWindowVMTK.h \
    renderer/vmtkAbstractRender.h \
    renderer/vmtkManagementShader.h \
    renderer/vmtkFrameBufferObject.h \
    renderer/vmtkRender2D.h \
    renderer/vmtkRender3D.h \
    utilities/QManagementResources.h \
    utilities/QCheckingOpenGL.h\
    utilities/vmtkEqualizer.h \
    ui/QCheckedTabWidget.h \
    ui/QFrameContentWindow.h \
    ui/QSplashScreenCopyright.h \
    ui/components/QTabLayout.h \
    model/BasePlugin.h \
    utilities/functions/vmtkGeneralFunctions.h \
    utilities/QLoadImage.h \
    model/ModelSubject.h \
    ui/RenderingSurface.h \
    ui/components/RenderingSurfaceSet.h \
    model/ModelObserver.h \
    ui/CommonGUIFactory.h \
    ui/OpenGLVisualizationTab.h \
    utilities/cursor/FreeDrawCursor.h \
    model/Signal.hpp \
    model/EventNotifier.h \
    ui/QCustomProgressDialog.h \
    core/BaseModule.h \
    core/ModuleManager.h \
    modules/visualization/VisualizationModule.h \
    modules/visualization/StateManager.h \
    core/BaseToolTab.h \
    core/MainWindow.h \
    core/ToolTab.h \
    core/ProgressDialog.h \
    core/VisualizationTab.h \
    modules/PluginFileManager.h \
    modules/visualization/DiffusionRegisterVisualizationModule.h

FORMS    += \
    QMainWindowVMTK.ui \
    QSplashScreenCopyright.ui \
    ui/components/QDialogButton.ui \
    ui/components/QTabLayout.ui


MainRoot = $$PWD

INCLUDEPATH += "3rd_party/"

include(config/config.pri)

RESOURCES += \
    Images.qrc \
    Shaders.qrc \
#    tf_files.qrc \
    Fonts.qrc \
    Styles.qrc

OTHER_FILES += \
    Shaders/cube.frag \
    Shaders/drawresult.frag \
    Shaders/position_is_color.frag \
    Shaders/raytrace.frag \
    Shaders/slicing.frag \
    Shaders/cube.vert \
    Shaders/drawresult.vert \
    Shaders/position_is_color.vert \
    Shaders/raytrace.vert \
    Shaders/simple.vert

DISTFILES += \
    Shaders/castregister.frag \
    Shaders/castregister.vert \
    Shaders/coregister_position_is_color.frag \
    Shaders/coregister_position_is_color.vert \
    Shaders/registration_depth.frag \
    Shaders/registration_depth.vert\
    Shaders/castregister2d.frag \
    Shaders/text.frag \
    Shaders/text.vert \
    Shaders/text.geom \
    class_diagram.qmodel

include(modules/modules.pri)
include(model/model.pri)
