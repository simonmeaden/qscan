TEMPLATE = subdirs

SUBDIRS += \
#    tests  \
    document \
    utilities \
    interface \
    plugins \
    qscanner \
    widgets \
#    Scanner \
    QScanTest


utilities.subdir = utilities
utilities.depends =

document.subdir = document
document.depends = utilities

interface.subdir = interface
interface.depends = utilities document

qscanner.subdir = qscanner
qscanner.depends = utilities document interface

widgets.subdir = widgets
widgets.depends = utilities document interface qscanner

plugins.subdir = plugins
plugins.depends = utilities document interface

QScanTest.subdir = QScanTest
QScanTest.depends = utilities document interface qscanner widgets

Scanner.subdir = Scanner
Scanner.depends = utilities document interface qscanner widgets

# not much has tests yet
tests.subdir = tests
tests.depends = utilities

