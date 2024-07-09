##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/controllers/ControllerDiffusionTool.h \
    $$PWD/event_handlers/EventHandlerDiffusion.h \
    $$PWD/ext_volume_renderer/DiffusionRaycastingShader.h \
    $$PWD/ext_volume_renderer/EventHandlerDiffusionVolume.h \
    $$PWD/ext_volume_renderer/DiffusionRaycastingVolumeRenderer3D.h \
    $$PWD/ext_volume_renderer/DiffusionSliceRenderer.h \ \
    $$PWD/models/ModelDiffusionUI.h \
    $$PWD/ui/ToolDiffusion.h \
    $$PWD/ui/qt/QToolDiffusion.h \
    $$PWD/ui/qt/UIFactoryDiffusionRendererQt.h

SOURCES += \
    $$PWD/controllers/ControllerDiffusionTool.cpp \
    $$PWD/event_handlers/EventHandlerDiffusion.cpp \
    $$PWD/ext_volume_renderer/DiffusionRaycastingShader.cpp \
    $$PWD/ext_volume_renderer/DiffusionRaycastingVolumeRenderer3D.cpp \
    $$PWD/ext_volume_renderer/DiffusionSliceRenderer.cpp \
    $$PWD/ext_volume_renderer/EventHandlerDiffusionVolume.tpp \
    $$PWD/models/ModelDiffusionUI.cpp \
    $$PWD/ui/ToolDiffusion.cpp \
    $$PWD/ui/qt/QToolDiffusion.cpp \
    $$PWD/ui/qt/UIFactoryDiffusionRendererQt.cpp

FORMS += \
    $$PWD/ui/qt/QToolDiffusion.ui


