##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

equals(QMAKE_HOST.name, "PC3079") {
    include(daniel_dell.pri)
}

equals(QMAKE_HOST.name, "LAPTOP-EF16LEGA") {
    include(LAPTOP-EF16LEGA.pri)
}

equals(QMAKE_HOST.name , "PC09-LCA"){
    include(pc09_lca.pri)
}

equals(QMAKE_HOST.name , "PC12-LCA"){
    include(pc12_lca.pri)
}

equals(QMAKE_HOST.name , "ACER-E15"){
    include(acer_e15.pri)
}

!equals(QMAKE_HOST.name , "PC12-LCA"){
!equals(QMAKE_HOST.name , "ACER-E15"){
!equals(QMAKE_HOST.name , "PC09-LCA"){
!equals(QMAKE_HOST.name, "PC3079") {

    include(raphael.pri)
}
}
}
}

# destination directories
release: DESTDIR = build\release
debug:   DESTDIR = build\debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = ./ui

DISTFILES += \
    $$PWD/daniel_dell.pri
