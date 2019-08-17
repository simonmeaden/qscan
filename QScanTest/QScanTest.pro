#-------------------------------------------------
#
# Project created by QtCreator 2019-08-12T09:52:32
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QScanTest
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
  ../translations/Scanner_en-GB.ts \
  ../translations/Scanner_en-US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#= System libraries ================================================================================
unix|win32: LIBS += -lyaml-cpp
unix|win32: LIBS += -lqyaml-cpp
unix|win32: LIBS += -ltesseract
unix|win32: LIBS += -lopencv_core
unix|win32: LIBS += -lopencv_photo
unix|win32: LIBS += -L/usr/local/qwt-6.1.4/lib -lqwt
INCLUDEPATH += /usr/local/qwt-6.1.4/include

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
