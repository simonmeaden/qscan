#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T19:13:15
#
#-------------------------------------------------

QT += core svg

DEFINES += LOGGER_ENABLE

TARGET = ocr
TEMPLATE = lib

DEFINES += OCR_LIBRARY

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
    ocrframe.cpp \
    ocrimage.cpp \
    ocrtools.cpp \
    ocrworker.cpp \
    tesstools.cpp \
    texteditdialog.cpp

HEADERS += \
        ocr_global.h \
        ocrframe.h \
        ocrimage.h \
        ocrtools.h \
        ocrworker.h \
        tesstools.h \
        texteditdialog.h

TRANSLATIONS += \
  ../translations/ocr_en-GB.ts \
  ../translations/ocr_en-US.ts

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../scanwidgets/release/ -lscanwidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../scanwidgets/debug/ -lscanwidgets
else:unix: LIBS += -L$$OUT_PWD/../scanwidgets/ -lscanwidgets

INCLUDEPATH += $$PWD/../scanwidgets
DEPENDPATH += $$PWD/../scanwidgets

unix|win32: LIBS += -ltesseract
unix|win32: LIBS += -lopencv_core
unix|win32: LIBS += -lopencv_imgcodecs
unix|win32: LIBS += -lopencv_photo
unix|win32: LIBS += -lqwt-qt5

FORMS +=
