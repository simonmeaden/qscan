TEMPLATE = subdirs

SUBDIRS += \
    qscan \
#    qlogsystem \
    Scanner


qscan.subdir = qscan

Scanner.subdir = Scanner
Scanner.depends = qscan

#qlogsystem.subdir = qlogsystem
