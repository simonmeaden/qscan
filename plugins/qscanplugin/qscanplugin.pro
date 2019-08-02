#-------------------------------------------------
#
# Project created by QtCreator 2019-07-08T08:46:36
#
#-------------------------------------------------

QT       += widgets svg

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
        iscaninterface.cpp \
        pageview.cpp \
        qscan.cpp \
        qscanplugin.cpp \
        scaneditor.cpp \
        scanimage.cpp \
        scaninterface.cpp \
        scanlist.cpp \
        scanoptions.cpp \
        version.cpp \
        imageeditdialog.cpp \
        ocreditor.cpp \
        ocrimage.cpp \
        ocrtools.cpp \
        ocrworker.cpp \
        tesstools.cpp \
        texteditdialog.cpp

HEADERS += \
        iscaninterface.h \
        pageview.h \
        qscan.h \
        qscan_global.h \
        qscanplugin.h \
        qscanplugin_global.h  \
        scaneditor.h \
        scanimage.h \
        scaninterface.h \
        scanlist.h \
        scanoptions.h \
        version.h \
        imageeditdialog.h \
        ocreditor.h \
        ocrimage.h \
        ocrtools.h \
        ocrworker.h \
        tesstools.h \
        texteditdialog.h

unix {
    SOURCES += \
        unix/saneworker.cpp \
        unix/sanelibrary.cpp \

    HEADERS += \
        unix/saneworker.h \
        unix/sanelibrary.h \

    LIBS += -lsane

    target.path = /usr/lib
    INSTALLS += target
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


FORMS +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/interface.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a

unix|win32: LIBS += -L/usr/local/qwt-6.1.4/lib -lqwt
INCLUDEPATH += /usr/local/qwt-6.1.4/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../document/release/ -ldocument
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../document/debug/ -ldocument
else:unix: LIBS += -L$$OUT_PWD/../../document/ -ldocument

INCLUDEPATH += $$PWD/../../document
DEPENDPATH += $$PWD/../../document
