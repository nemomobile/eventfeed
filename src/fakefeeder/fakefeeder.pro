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
equals(QT_MAJOR_VERSION, 4): LIBS += -L../libmeegotouchevents -lmeegotouchevents
equals(QT_MAJOR_VERSION, 5): LIBS += -L../libmeegotouchevents -lmeegotouchevents-qt5

target.path = /usr/bin
INSTALLS += target
