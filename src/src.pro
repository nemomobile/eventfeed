TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = libmeegotouchevents \
          libeventfeed \
          eventfeed \
          fakefeeder \
          libmeegotouchevents/doc

equals(QT_MAJOR_VERSION, 4): SUBDIRS += widget
