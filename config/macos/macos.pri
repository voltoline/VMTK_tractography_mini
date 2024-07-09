##----------------------------------------------------------
## Project organization source
## Template created by Raphael Voltoline Ramos - 2021
##
##----------------------------------------------------------

equals(QMAKE_HOST.name , "MacBook-Pro-de-Daniel.local"){
    include(macbook_daniel.pri)
}

!equals(QMAKE_HOST.name , "MacBook-Pro-de-Daniel.local"){
    include(others.pri)
}
