TEMPLATE = subdirs

SUBDIRS += \
    qscan \
    Scanner


qscan.subdir = qscan

Scanner.subdir = Scanner
Scanner.depends = qscan

