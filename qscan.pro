TEMPLATE = subdirs

SUBDIRS += \
    interface \
    plugins \
    document \
    Scanner

plugins.subdir = plugins
plugins.depends = interface

Scanner.subdir = Scanner
Scanner.depends = document interface

