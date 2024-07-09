##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/ModelROI.h \
    $$PWD/VisSubModuleROI.h \
    $$PWD/controllers/Controller2DViewROI.h \
    $$PWD/controllers/Controller3DViewROI.h \
    $$PWD/controllers/ControllerROIToolTab.h \
    $$PWD/event_handler/EventHandler2DTransformationToROI.h \
    $$PWD/event_handler/EventHandlerDTIToROI.h \
    $$PWD/event_handler/EventHandlerROI.h \
    $$PWD/event_handler/EventHandlerSlice2DToROI.h \
    $$PWD/renderer/ROIRenderer2D.h \
    $$PWD/renderer/ROIRenderer3D.h

SOURCES += \
    $$PWD/ModelROI.cpp \
    $$PWD/VisSubModuleROI.tpp \
    $$PWD/controllers/Controller2DViewROI.cpp \
    $$PWD/controllers/Controller3DViewROI.cpp \
    $$PWD/controllers/ControllerROIToolTab.cpp \
    $$PWD/event_handler/EventHandler2DTransformationToROI.cpp \
    $$PWD/event_handler/EventHandlerDTIToROI.cpp \
    $$PWD/event_handler/EventHandlerROI.cpp \
    $$PWD/event_handler/EventHandlerSlice2DToROI.cpp \
    $$PWD/renderer/ROIRenderer2D.cpp \
    $$PWD/renderer/ROIRenderer3D.cpp


