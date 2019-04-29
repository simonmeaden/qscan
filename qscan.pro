TEMPLATE = subdirs

SUBDIRS += \
    ocr \
    qscan \
    Scanner


qscan.subdir = qscan

Scanner.subdir = Scanner
Scanner.depends = qscan

