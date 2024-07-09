#-----------------------------------------------------
#
# Project created by Iván Rubianes 2020-08-09T03:19:25
#
#-----------------------------------------------------

## Declare path and name of the dynamic library ##
##################################################
dir_path_install = $$MainRoot/internal_libraries/Cursor
target_lib = cursor
dir_module = modules/visualization/submodules/cursor
projectname = cursor
targetDirLib = $$MainRoot/$$DESTDIR
usingLib = false
#message("targetDirLib: "$$targetDirLib)
#################################################
static{
    include($$MainRoot/$$dir_module/$$projectname".pri")
    message( "$$projectname: Using source code." )
}
else{
    equals(usingLib, "false"){
        include($$MainRoot/$$dir_module/$$projectname".pri")
        message( "$$projectname: Using source code." )
#        message("PWD: " $$PWD)
    }
    else{
        dir_path_lib = $$dir_path_install/lib
        dir_path_headers = $$dir_path_install/include
        win32:lib_file = "lib"$$target_lib".dll"
        unix:!macx:lib_file = "lib"$$target_lib".so"
        macx:lib_file = "lib"$$target_lib".dylib"
#        message("lib_file: " $$lib_file)
        path_lib_file = $$dir_path_lib/$$lib_file
        !exists( $$path_lib_file ){
            DEFINES += makinglib_$$target_lib # Muda para cada um dos modulos
#            message("DEFINES: " $$DEFINES)
            message( "$$projectname: The specified library does not exist!!" )
            message( "$$projectname: First make the dynamic library" )
            include($$MainRoot/$$dir_module/$$projectname".pro")
        }
        else{
            DEFINES += usinglib_$$target_lib # Muda para cada um dos modulos
            libname = "-l"$$target_lib
            INCLUDEPATH += $$dir_path_headers
    #        message(dir_path_headers: $$dir_path_headers)
            LIBS += "-L"$$dir_path_lib $$libname
            message($$projectname: The specified library exist!!! )
            message($$projectname: includepath = $$dir_path_headers)
            message($$projectname: libs = -L$$dir_path_lib $$libname)

        }
    }
}

