TEMPLATE = subdirs

SUBDIRS += \
    logger \
    ocr \
    qscan \
    scanwidgets \
    Scanner

logger.subdir = logger

ocr.subdir = ocr
ocr.depends = logger

qscan.subdir = qscan
qscan.depends = logger

scanwidgets.subdir = scanwidgets
scanwidgets.depends = qscan ocr logger

Scanner.subdir = Scanner
Scanner.depends = qscan ocr scanwidgets logger

