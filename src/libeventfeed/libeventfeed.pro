include(../../common.pri)

TEMPLATE = lib

CONFIG += qt

QT += sql

isEmpty(PREFIX) {
    PREFIX=/usr
}

TARGET = eventfeed$${DASH_QT_VERSION}
target.path = $$INSTALL_ROOT$$PREFIX/lib

equals(QT_MAJOR_VERSION, 5): DEFINES *= USE_PRIVILEGED_DATA_DIR

HEADERS = eventstorage.h \
          event.h

SOURCES = eventstorage.cpp \
          event.cpp

headers.files = $$HEADERS
headers.path = /usr/include/eventfeed$${DASH_QT_VERSION}
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = eventfeed$${DASH_QT_VERSION}.prf

INSTALLS += target headers prf
