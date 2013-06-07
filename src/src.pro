TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = libmeegotouchevents \
          libeventfeed \
          eventfeed \
          fakefeeder \

!nodoc {
    SUBDIRS += libmeegotouchevents/doc
}

equals(QT_MAJOR_VERSION, 4): SUBDIRS += widget
