Overview
========

The eventfeed package provides

1. `eventfeedd` daemon which is responsible for:

  - registering the D-Bus service `org.nemomobile.events.EventFeed`,
  - event storage maintenance,
  - sending requests to a thumbnailer service,
  - emitting signals upon adding or removal events from the storage.

2. trivial wrapper library (libmeegotouchevents) around a proxy to
   `org.nemomobile.events.EventFeed`. This library serves as public API.

3. QtQuick component listening to signals from the D-Bus service and
   updating component's view.

4. `libeventfeed` used by `eventfeedd` and by the QtQuick component
   to deal with event storage.

In order to decrease startup time the QtQuick components initializes its
view by loading event items from the storage directly.

Event storage
=============

`eventfeed` uses SQLite database as a storage for event items. Its schema is
identical to what's been used in MeeGo Harmattan::

  CREATE TABLE events (id INTEGER PRIMARY KEY, title TEXT, body TEXT,
                       timestamp TEXT, footer TEXT, action TEXT, url TEXT,
                       sourceName TEXT, sourceDisplayName TEXT);
  CREATE TABLE images (id INTEGER, position INTEGER, originalPath TEXT,
                       thumbnailPath TEXT, type TEXT,
                       PRIMARY KEY(id, position));

Notes
=====

Currently there is no working thumbnailer service in Nemo thus the code
using the libthumbnailer API is useless. The QtQuick component ignores
the content of the thumbnailPath field and uses the thumbnailer plugin
from the package `nemo-qml-plugins`.

If we decide not to use any D-Bus based thumbnailer, but to rely on the
QML plugin then this separate daemon providing
`org.nemomobile.events.EventFeed` service may be considered an
overcomplication. Instead the service might be a part of the QtQuick
component and the API library might be rewritten to update the
event storage directly.

So instead of::

                                                        --> event model 1
                                                        v
  client -> dump library -> dbus -> event service -> dbus <-> event model 2
                                                        ^
                                                        ---> event model 3

the scheme might look like this::

  client -> smart library -> dbus -> single event model

OTOH if go this way then

1. event feed bindings for other programming languages will get complicated,

2. changes in one event view won't be reflected in all others given that
   we want to allow running many event veiws in different processes at once.

The field `events.action` seems to be redundant.
