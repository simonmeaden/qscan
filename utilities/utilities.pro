#-------------------------------------------------
#
# Project created by QtCreator 2019-08-07T08:32:26
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = utilities
TEMPLATE = lib
CONFIG += staticlib
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

#===================================================================================================
