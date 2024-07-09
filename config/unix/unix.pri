##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

# destination directories
debug:   DESTDIR = build/debug
release: DESTDIR = build/release

QMAKE_CXXFLAGS += -DQT_COMPILING_QSTRING_COMPAT_CPP -Dcimg_display=0

equals(QMAKE_HOST.name , "wallace-P151EMx"){
    include(wallace-P151EMx.pri)
}

equals(QMAKE_HOST.name , "rodrigo-Vostro"){
    include(rodrigo-Vostro.pri)
}

equals(QMAKE_HOST.name , "pc12-lca"){
    include(pc12-lca.pri)
}

equals(QMAKE_HOST.name , "pc09-lca"){
    include(pc09-lca.pri)
}

equals(QMAKE_HOST.name, "ting-Inspiron-7560") {
    include(ting-Inspiron-7560.pri)
}

equals(QMAKE_HOST.name, "ivan-Aspire-E5-571G") {
    include(ivan-Aspire-E5-571G.pri)
}

equals(QMAKE_HOST.name, "PC-DCA") {
    include(IvanDCA.pri)
}

equals(QMAKE_HOST.name, "comp-Ting-01") {
    include(comp-Ting-01.pri)
}


!equals(QMAKE_HOST.name , "comp-Ting-01") {
!equals(QMAKE_HOST.name , "ivan-Aspire-E5-571G") {
!equals(QMAKE_HOST.name , "ting-Inspiron-7560") {
!equals(QMAKE_HOST.name , "wallace-P151EMx"){
!equals(QMAKE_HOST.name , "pc09-lca"){
!equals(QMAKE_HOST.name , "pc12-lca"){
!equals(QMAKE_HOST.name , "pc13-lca"){
!equals(QMAKE_HOST.name , "rodrigo-Vostro") {
    include(others.pri)
}
}
}
}
}
}
}
}

equals(QMAKE_HOST.name, "pc13-lca") {
    include(pc13-lca.pri)
}

# INCLUDEPATH += /usr/lib \
#                /usr/local/include/gdcm-2.0 \
#                ./cimg
# LIBS += -lGLU
# LIBS += -L/usr/lib64 -lGLEW
# LIBS += -L/usr/lib/x86_64-linux-gnu -lglut
# LIBS += -L/usr/lib/x86_64-linux-gnu -lboost_thread -lboost_system
# LIBS += -L/usr/lib -lGL -lpthread -lXt -lXext -lX11
# LIBS += -L/usr/lib/x86_64-linux-gnu -lniftiio -lznz -lz
# LIBS += -L/usr/local/lib -lgdcmCommon
# PRE_TARGETDEPS += /usr/local/lib/libgdcmCommon.a
# LIBS += -L/usr/local/lib/ -lgdcmIOD
# PRE_TARGETDEPS += /usr/local/lib/libgdcmIOD.a
# LIBS += -L/usr/local/lib/ -lgdcmMSFF
# PRE_TARGETDEPS += /usr/local/lib/libgdcmMSFF.a
# LIBS += -L/usr/local/lib/ -lgdcmcharls
# PRE_TARGETDEPS += /usr/local/lib/libgdcmcharls.a
# LIBS += -L/usr/local/lib/ -lgdcmDICT
# PRE_TARGETDEPS += /usr/local/lib/libgdcmDICT.a
# LIBS += -L/usr/local/lib/ -lgdcmDSED
# PRE_TARGETDEPS += /usr/local/lib/libgdcmDSED.a
# LIBS += -L/usr/local/lib/ -lgdcmexpat
# PRE_TARGETDEPS += /usr/local/lib/libgdcmexpat.a
# LIBS += -L/usr/local/lib/ -lgdcmjpeg8
# PRE_TARGETDEPS += /usr/local/lib/libgdcmjpeg8.a
# LIBS += -L/usr/local/lib/ -lgdcmjpeg12
# PRE_TARGETDEPS += /usr/local/lib/libgdcmjpeg12.a
# LIBS += -L/usr/local/lib/ -lgdcmjpeg16
# PRE_TARGETDEPS += /usr/local/lib/libgdcmjpeg16.a
# LIBS += -L/usr/local/lib/ -lgdcmopenjpeg
# PRE_TARGETDEPS += /usr/local/lib/libgdcmopenjpeg.a
# LIBS += -L/usr/local/lib/ -lgdcmuuid
# PRE_TARGETDEPS += /usr/local/lib/libgdcmuuid.a
# LIBS += -L/usr/local/lib/ -lgdcmcharls
# PRE_TARGETDEPS += /usr/local/lib/libgdcmcharls.a

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = ./ui

DISTFILES +=
