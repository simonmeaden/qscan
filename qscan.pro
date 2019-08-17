TEMPLATE = subdirs

SUBDIRS += \
#    tests \
    interface \
    plugins \
    document \
    utilities \
    QScanTest \
    Scanner


utilities.subdir = utilities

document.subdir = document
document.depends = utilities

interface.subdir = interface
interface.depends = document utilities

plugins.subdir = plugins
plugins.depends = interface document utilities

QScanTest.subdir = QScanTest
QScanTest.depends = interface document utilities

Scanner.subdir = Scanner
Scanner.depends = interface document utilities

tests.subdir = tests
tests.depends = interface document utilities plugins

