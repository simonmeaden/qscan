
VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "QSCAN_VERSION_MAJOR=$$VERSION_MAJOR" \
       "QSCAN_VERSION_MINOR=$$VERSION_MINOR" \
       "QSCAN_VERSION_BUILD=$$VERSION_BUILD"

SOURCES += \
  logging.cpp \
  book.pb.cc \
  iscanlibrary.cpp \
  qscan.cpp \
  scandevice.cpp \
  scanoptions.cpp \
  version.cpp

HEADERS += \
  logging.h \
  book.pb.h \
  iscanlibrary.h \
  qscan.h \
  qscan_global.h  \
  scandevice.h \
  scanoptions.h \
  version.h

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
}

win32:win64 {
    SOURCES += \
        win/twainlibrary.cpp \

    HEADERS += \
        win/twainlibrary.h \
        win/twpp.hpp  \
        win/twpp/application.hpp \
        win/twpp/audio.hpp \
        win/twpp/capability.hpp \
        win/twpp/cie.hpp \
        win/twpp/curveresponse.hpp \
        win/twpp/customdata.hpp \
        win/twpp/datasource.hpp \
        win/twpp/deviceevent.hpp \
        win/twpp/element8.hpp \
        win/twpp/enums.hpp \
        win/twpp/env.hpp \
        win/twpp/event.hpp \
        win/twpp/exception.hpp \
        win/twpp/extimageinfo.hpp \
        win/twpp/filesystem.hpp \
        win/twpp/fix32.hpp \
        win/twpp/frame.hpp \
        win/twpp/identity.hpp \
        win/twpp/imageinfo.hpp \
        win/twpp/imagelayout.hpp \
        win/twpp/imagememxfer.hpp \
        win/twpp/imagenativexfer.hpp \
        win/twpp/internal.hpp \
        win/twpp/jpegcompression.hpp \
        win/twpp/memory.hpp \
        win/twpp/memoryops.hpp \
        win/twpp/palette8.hpp \
        win/twpp/passthrough.hpp \
        win/twpp/pendingxfers.hpp \
        win/twpp/setupfilexfer.hpp \
        win/twpp/setupmemxfer.hpp \
        win/twpp/status.hpp \
        win/twpp/strings.hpp \
        win/twpp/types.hpp \
        win/twpp/typesops.hpp \
        win/twpp/userinterface.hpp \
        win/twpp/utils.hpp


    #= windows only twain library ======================================================================
    LIBS += -ltwain_32.dll

    DISTFILES += \
        win/twain-dsm/DSM_Translations.txt \
        win/twain-dsm/dsm.def \
        win/twain-dsm/readme.doc

}
