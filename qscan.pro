TEMPLATE = subdirs

SUBDIRS += \
    qyaml-cpp \
    document \
    logger \
    ocr \
    qscan \
    scanwidgets \
    Scanner

logger.subdir = logger

ocr.subdir = ocr
ocr.depends = logger qyaml-cpp

qscan.subdir = qscan
qscan.depends = logger qyaml-cpp

scanwidgets.subdir = scanwidgets
scanwidgets.depends = qscan ocr logger qyaml-cpp

Scanner.subdir = Scanner
Scanner.depends = qscan ocr scanwidgets logger qyaml-cpp

