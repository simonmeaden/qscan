#-------------------------------------------------
#
# Project created by QtCreator 2019-07-08T08:46:36
#
#-------------------------------------------------

QT       += core widgets svg

TEMPLATE = lib
TARGET = qscanplugin
CONFIG += c++14
DESTDIR = $$OUT_PWD/..

DEFINES += QSCANPLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "QSCAN_VERSION_MAJOR=$$VERSION_MAJOR" \
       "QSCAN_VERSION_MINOR=$$VERSION_MINOR" \
       "QSCAN_VERSION_BUILD=$$VERSION_BUILD"


SOURCES += \
#        book.pb.cc \
        iscanlibrary.cpp \
        qscan.cpp \
        qscanplugin.cpp \
        scandevice.cpp \
        scanoptions.cpp \
        version.cpp

HEADERS += \
#        book.pb.h \
        iscanlibrary.h \
        qscan.h \
        qscan_global.h \
        qscanplugin.h \
        qscanplugin_global.h  \
        scandevice.h \
        scanoptions.h \
        version.h

unix {
    SOURCES += \
        unix/saneworker.cpp \
        unix/sanelibrary.cpp \

    HEADERS += \
        unix/saneworker.h \
        unix/sanelibrary.h \

    LIBS += -lsane
#    INCLUDEPATH += /usr/include
#    INCLUDEPATH += /usr/local/include

#    target.path = /usr/lib
#    INSTALLS += target
}

win32:win64 {
    SOURCES += \
        win/twainlibrary.cpp

    HEADERS += \
        win/twainlibrary.h
}

TRANSLATIONS += \
  ../translations/qscan_en-GB.ts \
  ../translations/qscan_en-US.ts

DISTFILES += \
    book.proto \
    win/twain-dsm/dsm.def

FORMS +=


#= utilities library ===============================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../utilities/release/ -lutilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../utilities/debug/ -lutilities
else:unix: LIBS += -L$$OUT_PWD/../../utilities/ -lutilities

INCLUDEPATH += $$PWD/../../utilities
DEPENDPATH += $$PWD/../../utilities

#= document library =================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../document/release/ -ldocument
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../document/debug/ -ldocument
else:unix: LIBS += -L$$OUT_PWD/../../document/ -ldocument

INCLUDEPATH += $$PWD/../../document
DEPENDPATH += $$PWD/../../document

#= interface library ===============================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

#= windows only twain library ======================================================================

#win32:win64: LIBS += -L$$PWD/../../../../../../usr/x86_64-w64-mingw32/sys-root/mingw/lib/ -ltwaindsm.dll

#INCLUDEPATH += $$PWD/../../../../../../usr/x86_64-w64-mingw32/sys-root/mingw/include
#DEPENDPATH += $$PWD/../../../../../../usr/x86_64-w64-mingw32/sys-root/mingw/include

#win32:win64:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../../../usr/x86_64-w64-mingw32/sys-root/mingw/lib/twaindsm.dll.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../../../../usr/x86_64-w64-mingw32/sys-root/mingw/lib/libtwaindsm.dll.a

#===================================================================================================
