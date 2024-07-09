#-----------------------------------------------------
#
# Project created by Iván Rubianes 2020-08-09T02:18:41
#
#-----------------------------------------------------

TARGET = $$target_lib
#message(target_lib: $$target_lib)
TEMPLATE = lib
CONFIG += plugin
#CONFIG += staticlib # Solo si se desea construir bibliotecas estáticas (En Settings[name_project] : Dentro de static adicionar este archivo *.pro)

name_define = $$upper($$target_lib)_LIBRARY
#message($$name_define)
DEFINES += $$name_define

currentDestDir = $$DESTDIR
currentHeaders = $$HEADERS
currentSources = $$SOURCES

DESTDIR = $$targetDirLib
SOURCES = \
#        $$PWD/"$$target_lib".cpp

HEADERS = \
        $$PWD/"$$target_lib".h \
        $$PWD/"$$target_lib"_global.h

include($$projectname".pri")


## Installation settings##
headersDataFiles.files += $$HEADERS
headersDataFiles.path = $$dir_path_headers
#win32: libraryFiles.files = $$targetDirLib/$$target_lib".dll*"
#unix:!macx: libraryFiles.files = $$targetDirLib/$$target_lib".so*"
#macx: libraryFiles.files = $$targetDirLib/$$target_lib".dylib*"
libraryFiles.files = $$targetDirLib/$$lib_file"*"
#message("libraryFiles.files: " $$libraryFiles.files )
libraryFiles.path = $$dir_path_lib
#message(pathlibraryPath: $$libraryFiles.path)
INSTALLS += headersDataFiles
INSTALLS += libraryFiles

#HEADERS += $$currentHeaders
#SOURCES += $$currentSources


