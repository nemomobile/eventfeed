TEMPLATE = lib

CONFIG += qt

QT += sql

isEmpty(PREFIX) {
    PREFIX=/usr
}

TARGET = eventfeed
target.path = $$INSTALL_ROOT$$PREFIX/lib

HEADERS = eventstorage.h \
          event.h

SOURCES = eventstorage.cpp \
          event.cpp

headers.files = $$HEADERS
headers.path = /usr/include/eventfeed
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = eventfeed.prf

INSTALLS += target headers prf
