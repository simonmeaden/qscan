#-------------------------------------------------
#
# Project created by QtCreator 2019-08-17T08:11:47
#
#-------------------------------------------------

#QT       -= gui

TARGET = qscan
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

DEFINES += QSCAN_LIBRARY

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

CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $$OUT_PWD/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += \
  book.pb.cc \
  iscanlibrary.cpp \
  qscan.cpp \
  scandevice.cpp \
  scanoptions.cpp \
  version.cpp

HEADERS += \
  book.pb.h \
  iscanlibrary.h \
  qscan.h \
  qscan_global.h  \
  scandevice.h \
  scanoptions.h \
  version.h \

TRANSLATIONS += \
  ../translations/qscan_en-GB.ts \
  ../translations/qscan_en-US.ts

FORMS +=

DISTFILES += \
  book.proto

unix {
    SOURCES += \
        unix/saneworker.cpp \
        unix/sanelibrary.cpp

    HEADERS += \
        unix/saneworker.h \
        unix/sanelibrary.h

    LIBS += -lsane

#    target.path = /usr/lib
#    INSTALLS += target
}

win32:win64 {
    SOURCES += \
        win/twainlibrary.cpp \
        win/twain-dsm/apps.cpp \
        win/twain-dsm/dsm.cpp \
        win/twain-dsm/hook.cpp \
        win/twain-dsm/log.cpp \
        win/twainlibrary.cpp

    HEADERS += \
        win/twainlibrary.h \
        win/twain-dsm/dsm.h \
        win/twain-dsm/dsm.rc \
        win/twain-dsm/resource.h \
        win/twain-dsm/twain.h \
        win/twainlibrary.h

    #= windows only twain library ======================================================================
    LIBS += -ltwain_32.dll

    DISTFILES += \
        win/twain-dsm/DSM_Translations.txt \
        win/twain-dsm/dsm.def \
        win/twain-dsm/readme.doc

}


#= logging library =================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logging/release/ -llogging
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logging/debug/ -llogging
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logging/release -llogging
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logging/debug -llogging

INCLUDEPATH += $$PWD/../logging
DEPENDPATH += $$PWD/../logging

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/liblogging.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/liblogging.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/logging.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/logging.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/liblogging.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/liblogging.a

#= utilities library ===============================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utilities/release/ -lutilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utilities/debug/ -lutilities
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utilities/release -lutilities
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utilities/debug -lutilities

INCLUDEPATH += $$PWD/../utilities
DEPENDPATH += $$PWD/../utilities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/release/libutilities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/debug/libutilities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/release/utilities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/debug/utilities.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/release/libutilities.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../utilities/debug/libutilities.a

#= document library ================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../document/release/ -ldocument
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../document/debug/ -ldocument
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../document/release -ldocument
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../document/debug -ldocument

INCLUDEPATH += $$PWD/../document
DEPENDPATH += $$PWD/../document

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/release/libdocument.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/debug/libdocument.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/release/document.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/debug/document.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/release/libdocument.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../document/debug/libdocument.a

#= interface library ===============================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../interface/debug/ -linterface
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../interface/release -linterface
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../interface/debug -linterface

INCLUDEPATH += $$PWD/../interface
DEPENDPATH += $$PWD/../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/release/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/debug/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/release/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/debug/interface.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/release/libinterface.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/debug/libinterface.a

#===================================================================================================
