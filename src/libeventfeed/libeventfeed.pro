TEMPLATE = lib

CONFIG += qt

QT += sql

isEmpty(PREFIX) {
    PREFIX=/usr
}

equals(QT_MAJOR_VERSION, 4): TARGET = eventfeed
equals(QT_MAJOR_VERSION, 5): TARGET = eventfeed-qt5
target.path = $$INSTALL_ROOT$$PREFIX/lib

HEADERS = eventstorage.h \
          event.h

SOURCES = eventstorage.cpp \
          event.cpp

headers.files = $$HEADERS
equals(QT_MAJOR_VERSION, 4): headers.path = /usr/include/eventfeed
equals(QT_MAJOR_VERSION, 5): headers.path = /usr/include/eventfeed-qt5
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
equals(QT_MAJOR_VERSION, 4): prf.files = eventfeed.prf
equals(QT_MAJOR_VERSION, 5): prf.files = eventfeed-qt5.prf

INSTALLS += target headers prf
