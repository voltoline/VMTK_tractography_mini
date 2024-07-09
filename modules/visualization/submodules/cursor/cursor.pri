##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/Controller2DCursorToolTab.h \
    $$PWD/ControllerMouseHandlerCursor2D.h \
    $$PWD/ControllerMouseHandlerCursor3D.h \
    $$PWD/Cursor2D.h \
    $$PWD/Cursor3D.h \
    $$PWD/CursorRenderer2D.h \
    $$PWD/CursorRenderer3D.h \
    $$PWD/ModelCursor.h \
    $$PWD/VisSubModuleCursor.h \
    $$PWD/VisSubModuleCursor.tpp \
    $$PWD/event_handler/EventHandler2DTransformationToCursor.h \
    $$PWD/event_handler/EventHandler3DTransformationToCursor.h \
    $$PWD/ui/ToolCursor.h \
    $$PWD/ui/qt/QToolCursor.h \
    $$PWD/ui/qt/UIFactoryCursorQt.h

SOURCES += \
    $$PWD/Controller2DCursorToolTab.cpp \
    $$PWD/ControllerMouseHandlerCursor2D.cpp \
    $$PWD/ControllerMouseHandlerCursor3D.cpp \
    $$PWD/Cursor2D.cpp \
    $$PWD/Cursor3D.cpp \
    $$PWD/CursorRenderer2D.cpp \
    $$PWD/CursorRenderer3D.cpp \
    $$PWD/ModelCursor.cpp \
    $$PWD/event_handler/EventHandler2DTransformationToCursor.cpp \
    $$PWD/event_handler/EventHandler3DTransformationToCursor.cpp \
    $$PWD/ui/ToolCursor.cpp \
    $$PWD/ui/qt/QToolCursor.cpp \
    $$PWD/ui/qt/UIFactoryCursorQt.cpp

FORMS += \
    $$PWD/ui/qt/QToolCursor.ui


