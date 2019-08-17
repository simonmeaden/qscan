TEMPLATE = subdirs

SUBDIRS += \
#    tests  \
    logging\
    document \
    utilities \
    interface \
    plugins \
    qscan \
    widgets \
#    Scanner \
    QScanTest


logging.subdir = logging

utilities.subdir = utilities
utilities.depends = logging

document.subdir = document
document.depends = logging utilities

interface.subdir = interface
interface.depends = logging utilities document

qscan.subdir = qscan
qscan.depends = logging utilities document interface

widgets.subdir = widgets
widgets.depends = logging utilities document interface qscan

plugins.subdir = plugins
plugins.depends = logging utilities document interface

QScanTest.subdir = QScanTest
QScanTest.depends = logging utilities document interface qscan widgets

Scanner.subdir = Scanner
Scanner.depends = logging utilities document interface qscan widgets

# not much has tests yet
tests.subdir = tests
tests.depends = logging utilities

