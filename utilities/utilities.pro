#-------------------------------------------------
#
# Project created by QtCreator 2019-08-07T08:32:26
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = utilities
TEMPLATE = lib

#CONFIG += staticlib
CONFIG += c++14

DEFINES += UTILITIES_LIBRARY

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
        docutil.cpp \
        imageconverter.cpp \
        papersize.cpp \
        qmenuutils.cpp \
        utillogging.cpp

HEADERS += \
        docutil.h \
        imageconverter.h \
        papersize.h \
        qmenuutils.h \
        utillogging.h


unix|win32: LIBS += -lopencv_imgproc
