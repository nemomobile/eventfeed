PROJECT_NAME = events

TEMPLATE = lib
CONFIG += qt plugin

# FIXME: is sql really needed here?
QT += declarative dbus sql

TARGET = qmleventsplugin
target.path = $$[QT_INSTALL_IMPORTS]/org/nemomobile/$$PROJECT_NAME

system(qdbusxml2cpp -c EventFeedProxy -p eventfeedproxy -N ../../org.nemomobile.events.EventFeed.xml)

SOURCES = eventsqmlplugin.cpp \
          eventmodel.cpp \
          eventfeedproxy.cpp
HEADERS = eventsqmlplugin.h \
          eventmodel.h \
          eventfeedproxy.h

INCLUDEPATH += ../libeventfeed
LIBS += -L../libeventfeed -leventfeed

# qml API we provide
qml_api.files = api/*
qml_api.path = $$[QT_INSTALL_IMPORTS]/org/nemomobile/$$PROJECT_NAME

INSTALLS += target qml_api
