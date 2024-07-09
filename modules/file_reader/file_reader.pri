##----------------------------------------------------------
## Project organization source
## Template created by José Angel Iván Rubianes Silva - 2020
##
##----------------------------------------------------------

HEADERS += \
    $$PWD/FileManagerModule.h \
    $$PWD/ModelAcquisitionSet.h \
    $$PWD/nifti/NiftiFileLoader3DAcquisition.h \
    $$PWD/nifti/NiftiFileLoaderDiffusionAcquisition.h \
    $$PWD/dicom/vmtkAcquisition.h \
    $$PWD/dicom/vmtkAcquisitionAbstract.h \
    $$PWD/dicom/vmtkDiffAcquisition.h \
    $$PWD/dicom/vmtkReorientImageVolumeToLPS.h

SOURCES += \
    $$PWD/FileManagerModule.cpp \
    $$PWD/ModelAcquisitionSet.cpp \
    $$PWD/nifti/NiftiFileLoader3DAcquisition.cpp \
    $$PWD/nifti/NiftiFileLoaderDiffusionAcquisition.cpp \
    $$PWD/dicom/vmtkAcquisition.cpp \
    $$PWD/dicom/vmtkAcquisitionAbstract.cpp \
    $$PWD/dicom/vmtkDiffAcquisition.cpp \
    $$PWD/dicom/vmtkReorientImageVolumeToLPS.cpp
