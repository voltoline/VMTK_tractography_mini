##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/ext_volume_renderer_reg/DiffusionRegisterRaycastingShader.h \
    $$PWD/ext_volume_renderer_reg/DiffusionRegisterSliceRenderer.h \
    $$PWD/ext_volume_renderer_reg/RegisteredDiffusionRaycastingVolumeRenderer3D.h \
    $$PWD/VisSubModuleRegisterDiffusionVolumeRenderer.h

SOURCES += \
    $$PWD/VisSubModuleRegisterDiffusionVolumeRenderer.tpp \
    $$PWD/ext_volume_renderer_reg/DiffusionRegisterRaycastingShader.cpp \
    $$PWD/ext_volume_renderer_reg/DiffusionRegisterSliceRenderer.cpp \
    $$PWD/ext_volume_renderer_reg/RegisteredDiffusionRaycastingVolumeRenderer3D.cpp


