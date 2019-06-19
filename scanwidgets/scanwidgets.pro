#-------------------------------------------------
#
# Project created by QtCreator 2019-05-01T05:44:59
#
#-------------------------------------------------

DEFINES += LOGGER_ENABLE

QT       += widgets svg

TARGET = scanwidgets
TEMPLATE = lib

DEFINES += SCANWIDGETS_LIBRARY

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
        basescanimage.cpp \
        documentdata.cpp \
        imageconverter.cpp \
        imagedelegate.cpp \
        imagelistmodel.cpp \
        imageview.cpp \
        pageview.cpp \
        scaneditor.cpp \
        scanimage.cpp \
        scanlist.cpp \
        util.cpp

HEADERS += \
        basescanimage.h \
        documentdata.h \
        imageconverter.h \
        imagedelegate.h \
        imagelistmodel.h \
        imageview.h \
        pageview.h \
        scaneditor.h \
        scanimage.h \
        scanlist.h \
        scanwidgets_global.h  \
        util.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qscan/release/ -lqscan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qscan/debug/ -lqscan
else:unix: LIBS += -L$$OUT_PWD/../qscan/ -lqscan

INCLUDEPATH += $$PWD/../qscan
DEPENDPATH += $$PWD/../qscan

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ocr/release/ -locr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ocr/debug/ -locr
else:unix: LIBS += -L$$OUT_PWD/../ocr/ -locr

INCLUDEPATH += $$PWD/../ocr
DEPENDPATH += $$PWD/../ocr

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/release/ -lqyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/debug/ -lqyaml-cpp
else:unix: LIBS += -L$$OUT_PWD/../qyaml-cpp/ -lqyaml-cpp

INCLUDEPATH += $$PWD/../qyaml-cpp
DEPENDPATH += $$PWD/../qyaml-cpp
