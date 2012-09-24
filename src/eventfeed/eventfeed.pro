TEMPLATE = app

TARGET = eventfeedd

isEmpty(PREFIX) {
    PREFIX=/usr
}
target.path = $$INSTALL_ROOT$$PREFIX/bin

system(qdbusxml2cpp -c EventFeedAdaptor -a eventfeedadaptor ../../org.nemomobile.events.EventFeed.xml)

# FIXME: is sql really needed here?
QT += dbus sql

HEADERS = eventfeedservice.h \
          eventfeedadaptor.h

SOURCES = eventfeedservice.cpp \
          eventfeedadaptor.cpp \
          main.cpp

INCLUDEPATH += ../libeventfeed
LIBS += -L../libeventfeed -leventfeed

services.files = *.service
services.path = $$INSTALL_ROOT$$PREFIX/share/dbus-1/services

INSTALLS += target services
