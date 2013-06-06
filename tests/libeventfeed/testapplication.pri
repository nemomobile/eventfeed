include(../testapplication.pri)

# TODO: eventstorage.h should not include <QtSql>
QT += sql

INCLUDEPATH += ../../src/libeventfeed
LIBS += -L../../src/libeventfeed -leventfeed$${DASH_QT_VERSION}
