##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/VisSubModuleRegisterVolumeRenderer.h \
    $$PWD/controllers/ControllerVolumeBlendingTool.h \
    $$PWD/ext_volume_renderer/RegisterRaycastingShader.h \
    $$PWD/ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.h \
    $$PWD/ext_volume_renderer/RegisterSliceRenderer.h \
    $$PWD/ui/ToolVolumeBlending.h \
    $$PWD/ui/qt/QToolVolumeBlending.h \
    $$PWD/ui/qt/UIFactoryRegisteredVolumeQt.h

SOURCES += \
    $$PWD/VisSubModuleRegisterVolumeRenderer.tpp \
    $$PWD/controllers/ControllerVolumeBlendingTool.cpp \
    $$PWD/ext_volume_renderer/RegisterRaycastingShader.cpp \
    $$PWD/ext_volume_renderer/RegisterRaycastingVolumeRenderer3D.cpp \
    $$PWD/ext_volume_renderer/RegisterSliceRenderer.cpp \
    $$PWD/ui/ToolVolumeBlending.cpp \
    $$PWD/ui/qt/QToolVolumeBlending.cpp \
    $$PWD/ui/qt/UIFactoryRegisteredVolumeQt.cpp

FORMS += \
    $$PWD/ui/qt/QToolVolumeBlending.ui


