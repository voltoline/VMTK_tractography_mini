##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

macx {
    include(macos/macos.pri)
}

#windows#
win32 {
    include(windows/windows.pri)
}

#Linux#
unix:!macx {
    include(unix/unix.pri)
}
