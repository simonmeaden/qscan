#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T10:05:27
#
#-------------------------------------------------

DEFINES += LOGGER_ENABLE

QT       += core gui help

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -llog4qt


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qscan/ -lqscan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qscan/ -lqscan
else:unix: LIBS += -L$$OUT_PWD/../qscan -lqscan

INCLUDEPATH += $$PWD/../qscan
DEPENDPATH += $$PWD/../qscan

RESOURCES += \
    icons.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../scanwidgets/release/ -lscanwidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../scanwidgets/debug/ -lscanwidgets
else:unix: LIBS += -L$$OUT_PWD/../scanwidgets/ -lscanwidgets

INCLUDEPATH += $$PWD/../scanwidgets
DEPENDPATH += $$PWD/../scanwidgets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ocr/release/ -locr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ocr/debug/ -locr
else:unix: LIBS += -L$$OUT_PWD/../ocr/ -locr

INCLUDEPATH += $$PWD/../ocr
DEPENDPATH += $$PWD/../ocr

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

unix|win32: LIBS += -lyaml-cpp
#unix|win32: LIBS += -lqyaml-cpp
unix|win32: LIBS += -ltesseract
unix|win32: LIBS += -lopencv_core
unix|win32: LIBS += -lopencv_photo
unix|win32: LIBS += -lqwt-qt5

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/release/ -lqyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/debug/ -lqyaml-cpp
else:unix: LIBS += -L$$OUT_PWD/../qyaml-cpp/ -lqyaml-cpp

INCLUDEPATH += $$PWD/../qyaml-cpp
DEPENDPATH += $$PWD/../qyaml-cpp

