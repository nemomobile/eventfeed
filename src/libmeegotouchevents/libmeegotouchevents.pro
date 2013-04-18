TEMPLATE = lib

equals(QT_MAJOR_VERSION, 4): TARGET = meegotouchevents
equals(QT_MAJOR_VERSION, 5): TARGET = meegotouchevents-qt5
isEmpty(PREFIX) {
    PREFIX=/usr
}
target.path = $$INSTALL_ROOT$$PREFIX/lib

system(qdbusxml2cpp -c MEventFeedProxy -p meventfeedproxy ../../org.nemomobile.events.EventFeed.xml)

SOURCES = meventfeedproxy.cpp \
          meventfeed.cpp

HEADERS = meventfeedproxy.h \
          meventfeed.h

headers.files = meventfeedproxy.h \
                meventfeed.h
headers.path = $$INSTALL_ROOT$$PREFIX/include/$$TARGET

prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
equals(QT_MAJOR_VERSION, 4): prf.files = meegotouchevents.prf
equals(QT_MAJOR_VERSION, 5): prf.files = meegotouchevents-qt5.prf

INSTALLS += target \
            headers \
            prf
