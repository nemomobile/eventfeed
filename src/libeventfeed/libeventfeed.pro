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

PUBLICHEADERS = $$HEADERS
publicheaders.files = $$PUBLICHEADERS
publicheaders.path = /usr/include/eventfeed

INSTALLS += target publicheaders
