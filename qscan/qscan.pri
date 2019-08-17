
VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "QSCAN_VERSION_MAJOR=$$VERSION_MAJOR" \
       "QSCAN_VERSION_MINOR=$$VERSION_MINOR" \
       "QSCAN_VERSION_BUILD=$$VERSION_BUILD"

SOURCES += \
  $$PWD/logging.cpp \
  book.pb.cc \
  iscanlibrary.cpp \
  qscan.cpp \
  scandevice.cpp \
  scanoptions.cpp \
  version.cpp

HEADERS += \
  $$PWD/logging.h \
  book.pb.h \
  iscanlibrary.h \
  qscan.h \
  qscan_global.h  \
  scandevice.h \
  scanoptions.h \
  version.h \

TRANSLATIONS += \
  ../translations/qscan_en-GB.ts \
  ../translations/qscan_en-US.ts

FORMS +=

DISTFILES += \
  book.proto \
  qscan.pri

unix {
    SOURCES += \
        unix/saneworker.cpp \
        unix/sanelibrary.cpp

    HEADERS += \
        unix/saneworker.h \
        unix/sanelibrary.h

    LIBS += -lsane

#    target.path = /usr/lib
#    INSTALLS += target
}

win32:win64 {
    SOURCES += \
        win/twainlibrary.cpp \
        win/twain-dsm/apps.cpp \
        win/twain-dsm/dsm.cpp \
        win/twain-dsm/hook.cpp \
        win/twain-dsm/log.cpp \
        win/twainlibrary.cpp

    HEADERS += \
        win/twainlibrary.h \
        win/twain-dsm/dsm.h \
        win/twain-dsm/dsm.rc \
        win/twain-dsm/resource.h \
        win/twain-dsm/twain.h \
        win/twainlibrary.h

    #= windows only twain library ======================================================================
    LIBS += -ltwain_32.dll

    DISTFILES += \
        win/twain-dsm/DSM_Translations.txt \
        win/twain-dsm/dsm.def \
        win/twain-dsm/readme.doc

}
