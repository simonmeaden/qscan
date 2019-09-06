TEMPLATE = subdirs

SUBDIRS += \
#    tests  \
    document \
    utilities \
    interface \
    plugins \
    qscan \
    widgets \
#    Scanner \
    QScanTest


utilities.subdir = utilities
utilities.depends =

document.subdir = document
document.depends = utilities

interface.subdir = interface
interface.depends = utilities document

qscan.subdir = qscan
qscan.depends = utilities document interface

widgets.subdir = widgets
widgets.depends = utilities document interface qscan

plugins.subdir = plugins
plugins.depends = utilities document interface

QScanTest.subdir = QScanTest
QScanTest.depends = utilities document interface qscan widgets

Scanner.subdir = Scanner
Scanner.depends = utilities document interface qscan widgets

# not much has tests yet
tests.subdir = tests
tests.depends = utilities

