#-------------------------------------------------
#
# Project created by QtCreator 2019-08-17T08:07:12
#
#-------------------------------------------------

QT       += widgets

TARGET = widgets
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

DEFINES += WIDGETS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
  baseeditor.cpp \
  basescanimage.cpp \
  imagedelegate.cpp \
  imageeditdialog.cpp \
  imagelistmodel.cpp \
  imageview.cpp \
  logging.cpp \
  ocreditor.cpp \
  ocrimage.cpp \
  ocrtools.cpp \
  ocrworker.cpp \
  pageview.cpp \
  scaneditor.cpp \
  scanimage.cpp \
  scanlist.cpp \
  stackableframe.cpp \
  tesstools.cpp \
  texteditdialog.cpp

HEADERS += \
        baseeditor.h \
        basescanimage.h \
        imagedelegate.h \
        imageeditdialog.h \
        imagelistmodel.h \
        imageview.h \
        logging.h \
        ocreditor.h \
        ocrimage.h \
        ocrtools.h \
        ocrworker.h \
        pageview.h \
        scaneditor.h \
        scanimage.h \
        scanlist.h \
        stackableframe.h \
        tesstools.h \
        texteditdialog.h \
        widgets_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#= logging library =================================================================================
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logging/release/ -llogging
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logging/debug/ -llogging
#else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logging/release -llogging
#else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logging/debug -llogging

#INCLUDEPATH += $$PWD/../logging
#DEPENDPATH += $$PWD/../logging

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/liblogging.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/liblogging.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/logging.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/logging.lib
#else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/release/liblogging.a
#else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logging/debug/liblogging.a

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

#= qscan library ===================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qscan/release/ -lqscan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qscan/debug/ -lqscan
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qscan/release -lqscan
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qscan/debug -lqscan

INCLUDEPATH += $$PWD/../qscan
DEPENDPATH += $$PWD/../qscan

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/release/libqscan.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/debug/libqscan.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/release/qscan.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/debug/qscan.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/release/libqscan.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qscan/debug/libqscan.a

#===================================================================================================
