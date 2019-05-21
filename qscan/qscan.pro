#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T09:11:49
#
#-------------------------------------------------

DEFINES += LOGGER_ENABLE

QT       += widgets svg xml multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = qscan
TEMPLATE = lib

DEFINES += SCAN_LIBRARY

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qscan.cpp \
    version.cpp  \
    scaninterface.cpp \
    scanoptions.cpp

HEADERS += \
    qscan.h \
    qscan_global.h \
    version.h \
    scaninterface.h  \
    scanoptions.h

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

unix|win32: LIBS += -llog4qt

DISTFILES += \
    SIZES.md

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger
