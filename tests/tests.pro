TEMPLATE = subdirs
SUBDIRS = \
        libeventfeed \

check.depends = all
check.CONFIG = phony recursive
QMAKE_EXTRA_TARGETS += check
