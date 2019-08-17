#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T10:05:27
#
#-------------------------------------------------

DEFINES += LOGGER_ENABLE

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scanner
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

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS +=

TRANSLATIONS += \
  ../translations/Scanner_en-GB.ts \
  ../translations/Scanner_en-US.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

#= System libraries ================================================================================
unix|win32: LIBS += -lyaml-cpp
unix|win32: LIBS += -lqyaml-cpp
unix|win32: LIBS += -ltesseract
unix|win32: LIBS += -lopencv_core
unix|win32: LIBS += -lopencv_photo
unix|win32: LIBS += -L/usr/local/qwt-6.1.4/lib -lqwt
INCLUDEPATH += /usr/local/qwt-6.1.4/include

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utilities/release/ -lutilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utilities/debug/ -lutilities
else:unix: LIBS += -L$$OUT_PWD/../utilities/ -lutilities

INCLUDEPATH += $$PWD/../utilities
DEPENDPATH += $$PWD/../utilities

#= interface lib ===================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../interface/ -linterface

INCLUDEPATH += $$PWD/../interface
DEPENDPATH += $$PWD/../interface

#===================================================================================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../document/release/ -ldocument
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../document/debug/ -ldocument
else:unix: LIBS += -L$$OUT_PWD/../document/ -ldocument

INCLUDEPATH += $$PWD/../document
DEPENDPATH += $$PWD/../document

#===================================================================================================
