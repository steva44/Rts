#*********************************************************************
#                                                                    *
# This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
# as a result of work performed in the research group of Professor   *
# Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
# Columbia in Vancouver. The code is part of the computer program    *
# Rts, which is an extension of the computer program Rt developed by *
# Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
#                                                                    *
# The content of this file is the product of thesis-related work     *
# performed at the University of British Columbia (UBC). Therefore,  *
# the Rights and Ownership are held by UBC.                          *
#                                                                    *
# Please be cautious when using this code. It is provided “as is”    *
# without warranty of any kind, express or implied.                  *
#                                                                    *
# Contributors to this file:                                         *
#   - Mojtaba Mahsuli                                                *
#   - Stevan Gavrilovic                                              *
#   - Terje Haukaas                                                  *
#   - Rodrigo Costa                                                  *
#                                                                    *
#********************************************************************/

# These should always compile together, with all on if desired
DEFINES += DETAILEDBUILDINGS
DEFINES += SIMPLEBUILDINGS
DEFINES += HAZARDS
DEFINES += GSL
DEFINES += OSG
DEFINES += BOOST
DEFINES += IFC      # Both BOOST and OSG defines need to ON when IFC is enabled
DEFINES += OPENSEES
DEFINES += XLINT    # Library to output results to an excel file

# This is Rodrigo's testing ground
#DEFINES += NETWORKS
#DEFINES += GIS
contains (DEFINES, NETWORKS)  { include(./Extended/Networks/Networks.pri) }
contains (DEFINES, GIS)   {
QT += positioning core xml widgets sensors
CONFIG += esri_runtime_qt100_0_0
win32:CONFIG +=   embed_manifest_exe
include($$PWD/arcgisruntime.pri)
}

# C++14 support
CONFIG += c++14
TEMPLATE = app
TARGET = Rts
DESTDIR = ./Bin
QMAKE_MAC_SDK = macosx10.15
QT += network script scripttools svg printsupport opengl gui
RESOURCES += ./Core/GUI/Rts.qrc
win32: RC_FILE = ./Core/GUI/RIcon/Rts.rc
mac: ICON = ./Core/GUI/RIcon/RtsIcon.icns

# CONFIG+=debug_and_release
# CONFIG+=build_all

include(./Core/Core.pri)

contains (DEFINES, OPENSEES) {

include(./Libraries/OpenSees/OpenSees/qmake.conf)
include(./Libraries/OpenSees/OpenSees/OpenSees.pri)
include(./Libraries/OpenSees/OpenSees/OpenSeesLibs.pri)
include(./Extended/OpenSeesInterface/OpenSeesInterface.pri)

}

contains (DEFINES, DETAILEDBUILDINGS)   { include(./Extended/DetailedBuildings/DetailedBuildings.pri) }
contains (DEFINES, SIMPLEBUILDINGS)     { include(./Extended/SimpleBuildings/SimpleBuildings.pri) }
contains (DEFINES, HAZARDS)             { include(./Extended/Hazards/Hazards.pri) }
contains (DEFINES, BOOST)               { include(./Libraries/BOOST/BOOST.pri) }
contains (DEFINES, IFC)                 { include(./Libraries/IFC/IFC.pri) }
contains (DEFINES, GSL)                 { include(./Libraries/GSL/GSL.pri) }
contains (DEFINES, OSG)                 { include(./Libraries/OSG/OSG.pri) }
contains (DEFINES, XLINT)               { include(./Libraries/xlnt/xlnt.pri) }

# OS specific configs
win32-msvc* {
    message(Building for Windows using Qt $$QT_VERSION)
    QMAKE_CXXFLAGS += /bigobj # allow big objects (essentially templates that are not supported in MSVC 2005 and under)
    # remove possible other optimization flags
    CONFIG(release, debug|release) {
        CONFIG += optimize_full
    }
}
else:linux-* {
    message(Building for Linux using Qt $$QT_VERSION)
    QMAKE_CXXFLAGS += -std=c++14

    QMAKE_CXX = ccache g++

    QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN/lib'"

    # MPI Settings
    QMAKE_CXX = ccache mpicxx
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_CC = mpicc

    QMAKE_LINK = $$QMAKE_CXX
    QMAKE_CFLAGS += $$system(mpicc --showme:compile)
    QMAKE_LFLAGS += $$system(mpicxx --showme:link)
    QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
    QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

    QMAKE_CXXFLAGS_RELEASE += -O3 #-Wall -g#-Werror

    # Need to link with the ldl library
    LIBS += -L/usr/lib/x86_64-linux-gnu/ -ldl

}
else:macx {
    message(Building for Mac OS X using Qt $$QT_VERSION)
    LIBS += -framework Cocoa
    QMAKE_CFLAGS_RELEASE += -fvisibility=hidden
    QMAKE_CXXFLAGS += -std=c++14
    QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden \
                              -fvisibility-inlines-hidden

    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
    QMAKE_LFLAGS_RELEASE -= -O1

#    CONFIG(release, debug|release) {
#        CONFIG += optimize_full
#    }

}


INCLUDEPATH += $$PWD/Libraries/OpenSees/OpenSees/OTHER/SuperLU_MT/pdsp_defs.h




