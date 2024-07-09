##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/VisSubModule2D3DSlicing.h \
    $$PWD/VisSubModule2D3DSlicing.h \
    $$PWD/controllers/Controller2D3DSlicingTool.h \
    $$PWD/controllers/ControllerMouseHandler2D3DSlicing.h \
    $$PWD/ui/Tool2D3DSlicing.h \
    $$PWD/ui/qt/QTool2D3DSlicing.h \
    $$PWD/ui/qt/UIFactory2D3DSlicingQt.h

SOURCES += \
    $$PWD/VisSubModule2D3DSlicing.tpp \
    $$PWD/VisSubModule2D3DSlicing.tpp \
    $$PWD/controllers/Controller2D3DSlicingTool.cpp \
    $$PWD/controllers/ControllerMouseHandler2D3DSlicing.cpp \
    $$PWD/ui/Tool2D3DSlicing.cpp \
    $$PWD/ui/qt/QTool2D3DSlicing.cpp \
    $$PWD/ui/qt/UIFactory2D3DSlicingQt.cpp

FORMS += \
    $$PWD/ui/qt/QTool2D3DSlicing.ui
