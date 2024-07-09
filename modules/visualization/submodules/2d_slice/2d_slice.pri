##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/CommonObjects2DSlice.h \
    $$PWD/SliceSynchronizer.h \
    $$PWD/controller/ControllerToolSliceOrientation.h \
    $$PWD/event_handler/EventHandler2DSlice.h \
    $$PWD/model/Model2DSlice.h \
    $$PWD/model/Model2DSliceUIState.h \
    $$PWD/ui/ToolSliceOrientation.h \
    $$PWD/ui/qt/QToolSliceOrientation.h \
    $$PWD/ui/qt/UIFactory2DSliceQt.h

SOURCES += \
    $$PWD/CommonObjects2DSlice.cpp \
    $$PWD/SliceSynchronizer.cpp \
    $$PWD/controller/ControllerToolSliceOrientation.cpp \
    $$PWD/event_handler/EventHandler2DSlice.cpp \
    $$PWD/model/Model2DSlice.cpp \
    $$PWD/model/Model2DSliceUIState.cpp \
    $$PWD/ui/ToolSliceOrientation.cpp \
    $$PWD/ui/qt/QToolSliceOrientation.cpp \
    $$PWD/ui/qt/UIFactory2DSliceQt.cpp

FORMS += \
    $$PWD/ui/qt/QToolSliceOrientation.ui


