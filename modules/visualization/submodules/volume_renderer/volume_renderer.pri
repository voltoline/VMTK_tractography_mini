##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/2d_renderer/BaseSliceRenderer.h \
    $$PWD/2d_renderer/SliceRenderer.h \
    $$PWD/3d_renderer/BaseRaycastingShader.h \
    $$PWD/3d_renderer/BaseRaycastingVolumeRenderer3D.h \
    $$PWD/3d_renderer/RaycastingPreRenderShader.h \
    $$PWD/3d_renderer/RaycastingResultShader.h \
    $$PWD/3d_renderer/RaycastingShader.h \
    $$PWD/3d_renderer/RaycastingVolumeRenderer3D.h \
    $$PWD/DepthRenderer.h \
    $$PWD/OpenGLVolumeRenderingResource.h \
    $$PWD/VisSubModuleBaseVolumeRenderer.h \
    $$PWD/event_handler/EventHandler2DRendering.h \
    $$PWD/event_handler/EventHandler3DRendering.h \
    $$PWD/event_handler/EventHandlerVolume.h \
    $$PWD/model/Model2DRendering.h \
    $$PWD/model/Model3DRendering.h \
    $$PWD/model/ModelTransferFunction.h \
    $$PWD/model/ModelVolume.h \
    $$PWD/transfer_function/vmtkColorPalettes.h \
    $$PWD/transfer_function/vmtkGrayScale.h \
    $$PWD/transfer_function/vmtkTFBuffer.h

SOURCES += \
    $$PWD/2d_renderer/BaseSliceRenderer.cpp \
    $$PWD/2d_renderer/SliceRenderer.cpp \
    $$PWD/3d_renderer/BaseRaycastingShader.cpp \
    $$PWD/3d_renderer/BaseRaycastingVolumeRenderer3D.cpp \
    $$PWD/3d_renderer/RaycastingPreRenderShader.cpp \
    $$PWD/3d_renderer/RaycastingResultShader.cpp \
    $$PWD/3d_renderer/RaycastingShader.cpp \
    $$PWD/3d_renderer/RaycastingVolumeRenderer3D.cpp \
    $$PWD/DepthRenderer.cpp \
    $$PWD/OpenGLVolumeRenderingResource.cpp \
    $$PWD/VisSubModuleBaseVolumeRenderer.cpp \
    $$PWD/event_handler/EventHandler2DRendering.cpp \
    $$PWD/event_handler/EventHandler3DRendering.cpp \
    $$PWD/event_handler/EventHandlerVolume.cpp \
    $$PWD/model/Model2DRendering.cpp \
    $$PWD/model/Model3DRendering.cpp \
    $$PWD/model/ModelTransferFunction.cpp \
    $$PWD/model/ModelVolume.cpp \
    $$PWD/transfer_function/vmtkColorPalettes.cpp \
    $$PWD/transfer_function/vmtkGrayScale.cpp \
    $$PWD/transfer_function/vmtkTFBuffer.cpp
