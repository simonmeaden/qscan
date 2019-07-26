TEMPLATE = subdirs

SUBDIRS += \
    interface \
    plugins \
    document \
    Scanner \
    tests

plugins.subdir = plugins
plugins.depends = interface

Scanner.subdir = Scanner
Scanner.depends = document interface

tests.subdir = tests
tests.depends = interface

