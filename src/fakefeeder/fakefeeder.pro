QT += core
QT += dbus
QT -= gui

TARGET = fakefeeder
CONFIG   += console meegotouchevents
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    ffeedcmd.cpp \
    feeder.cpp

HEADERS += \
    feeder.h

INCLUDEPATH += ../libmeegotouchevents
LIBS += -L../libmeegotouchevents -lmeegotouchevents

target.path = /usr/bin
INSTALLS += target
