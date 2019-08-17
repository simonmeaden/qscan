#-------------------------------------------------
#
# Project created by QtCreator 2019-08-05T11:37:32
#
#-------------------------------------------------

QT       += widgets

TARGET = qscan-addons
TEMPLATE = lib
CONFIG += c++14
DESTDIR = $$OUT_PWD/..

DEFINES += QSCANADDONS_LIBRARY

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
        baseeditor.cpp \
        basescanimage.cpp \
        imagedelegate.cpp \
        imageeditdialog.cpp \
        imagelistmodel.cpp \
        imageview.cpp \
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
        texteditdialog.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

unix|win32: LIBS += -ltesseract
unix|win32: LIBS += -lqwt
#unix|win32: LIBS += -lqwtpolar
#unix|win32: LIBS += -lqwtplot3d-qt5

unix|win32: LIBS += -L/usr/local/qwt-6.1.4/lib -lqwt
INCLUDEPATH += /usr/local/qwt-6.1.4/include

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR -lqscanplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR -lqscanplugin
else:unix: LIBS += -L$$DESTDIR -lqscanplugin

INCLUDEPATH += $$PWD/../qscanplugin
DEPENDPATH += $$PWD/../qscanplugin

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../utilities/release/ -lutilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../utilities/debug/ -lutilities
else:unix: LIBS += -L$$OUT_PWD/../../utilities/ -lutilities

INCLUDEPATH += $$PWD/../../utilities
DEPENDPATH += $$PWD/../../utilities

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../document/release/ -ldocument
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../document/debug/ -ldocument
else:unix: LIBS += -L$$OUT_PWD/../../document/ -ldocument

INCLUDEPATH += $$PWD/../../document
DEPENDPATH += $$PWD/../../document

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

#===================================================================================================



