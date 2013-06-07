TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = src tests

check.depends = all
check.CONFIG = phony recursive
QMAKE_EXTRA_TARGETS += check
