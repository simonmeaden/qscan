#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T08:56:56
#
#-------------------------------------------------

#QT       -= gui

TARGET = document
TEMPLATE = lib

DEFINES += DOCUMENT_LIBRARY

CONFIG += staticlib
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
        block.cpp \
        document.cpp \
        documentdata.cpp \
        logging.cpp \
        paragraphblock.cpp \
        styledstring.cpp \
        textblock.cpp \
        titleblock.cpp

HEADERS += \
        block.h \
        document.h \
        document_global.h  \
        documentdata.h \
        logging.h \
        paragraphblock.h \
        styledstring.h \
        textblock.h \
        titleblock.h



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

#===================================================================================================

