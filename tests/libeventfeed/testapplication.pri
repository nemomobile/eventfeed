include(../testapplication.pri)

# TODO: eventstorage.h should not include <QtSql>
QT += sql

INCLUDEPATH += ../../src/libeventfeed
QMAKE_LIBDIR = ../../src/libeventfeed
LIBS = -leventfeed$${DASH_QT_VERSION}

check.depends = all
check.commands = '\
    cd $${OUT_PWD} \
    && export XDG_DATA_HOME="`pwd`/.config" \
    && rm -rf \$\${XDG_DATA_HOME} \
    && export LD_LIBRARY_PATH="$${OUT_PWD}/../../src/libeventfeed:\$\${LD_LIBRARY_PATH}" \
    && ./$${TARGET}'
check.CONFIG = phony
QMAKE_EXTRA_TARGETS += check
