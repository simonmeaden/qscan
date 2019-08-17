#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T08:56:56
#
#-------------------------------------------------

#QT       -= gui

TARGET = document
TEMPLATE = lib

DEFINES += DOCUMENT_LIBRARY

#CONFIG += staticlib
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
        block.cpp \
        document.cpp \
        documentdata.cpp \
        paragraphblock.cpp \
        styledstring.cpp \
        textblock.cpp \
        titleblock.cpp

HEADERS += \
        block.h \
        document.h \
        document_global.h  \
        documentdata.h \
        paragraphblock.h \
        styledstring.h \
        textblock.h \
        titleblock.h



#===================================================================================================

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utilities/release/ -lutilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utilities/debug/ -lutilities
else:unix: LIBS += -L$$OUT_PWD/../utilities/ -lutilities

INCLUDEPATH += $$PWD/../utilities
DEPENDPATH += $$PWD/../utilities
#===================================================================================================
