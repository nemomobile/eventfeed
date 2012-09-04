/*
 * Copyright (C) 2012 Jolla Ltd.
 * Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QDBusConnection>
#include "eventmodel.h"

static const QString SERVICE_NAME = "org.nemomobile.events.EventFeed";

EventModel::EventModel(QObject *parent)
    : QAbstractListModel(parent),
      m_feeder(NULL)
{
    QHash<int, QByteArray> roles;
    roles[IdRole]                = "item_id";
    roles[IconRole]              = "icon";
    roles[TitleRole]             = "title";
    roles[BodyRole]              = "body";
    roles[ImageListRole]         = "imageList";
    roles[TimestampRole]         = "timestamp";
    roles[FooterRole]            = "footer";
    roles[VideoRole]             = "video";
    roles[UrlRole]               = "url";
    roles[SourceNameRole]        = "sourceName";
    roles[SourceDisplayNameRole] = "sourceDisplayName";
    setRoleNames(roles);
    // TODO: add all roles

    m_watcher = new QDBusServiceWatcher(SERVICE_NAME,
            QDBusConnection::sessionBus(),
            QDBusServiceWatcher::WatchForRegistration |
            QDBusServiceWatcher::WatchForUnregistration,
            this);
    connect(m_watcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(connectFeeder(QString)));
    connect(m_watcher, SIGNAL(serviceUnregistered(QString)),
            this, SLOT(disconnectFeeder(QString)));

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(SERVICE_NAME)) {
        connectFeeder();
    }

    m_storage.open(); // FIXME: new and delete after getAllItems() ?
    m_events = m_storage.getAllItems();
    m_storage.close();
}

EventModel::~EventModel()
{
    foreach (Event *event, m_events) {
        delete event;
    }
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    return m_events.count();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_events.count()) {
        return QVariant();
    }

    const Event *event = m_events[index.row()];
    switch (role) {
        case IdRole:
            return event->id;
        case IconRole:
            return event->icon();
        case TitleRole:
            return event->title();
        case BodyRole:
            return event->body();
        case ImageListRole:
            return event->imageList();
        case TimestampRole:
            return event->timestamp();
        case FooterRole:
            return event->footer();
        case VideoRole:
            return event->video();
        case UrlRole:
            return event->url();
        case SourceNameRole:
            return event->sourceName();
        case SourceDisplayNameRole:
            return event->sourceDisplayName();
    }
    return QVariant();
}

void EventModel::addEvent(const QVariantMap &ev)
{
    Event* event = new Event(ev["id"].toLongLong(),
                             ev["icon"].toString(),
                             ev["title"].toString(),
                             ev["body"].toString(),
                             ev["imageList"].toStringList(),
                             ev["timestamp"].toString(),
                             ev["footer"].toString(),
                             ev["video"].toBool(),
                             ev["url"].toString(),
                             ev["sourceName"].toString(),
                             ev["sourceDisplayName"].toString(),
                             ev["iconThumbnail"].toString(),
                             ev["thumbnailList"].toStringList());
    beginInsertRows(QModelIndex(), 0, 0);
    m_events.prepend(event);
    endInsertRows();
}

void EventModel::removeEvents(const QList<qlonglong> &ids)
{
    for (int i = m_events.size() - 1; i >= 0; i--) {
        if (ids.contains(m_events.at(i)->id)) {
            beginRemoveRows(QModelIndex(), i, i);
            delete m_events.at(i);
            m_events.removeAt(i);
            endRemoveRows();
        }
    }
}

void EventModel::connectFeeder(QString str)
{
    if (m_feeder) {
        return;
    }

    m_feeder = new EventFeedProxy(SERVICE_NAME,
            "/org/nemomobile/events/EventFeed", QDBusConnection::sessionBus(), this);
    connect(m_feeder, SIGNAL(eventAdded(const QVariantMap&)),
            this, SLOT(addEvent(const QVariantMap&)));
    connect(m_feeder, SIGNAL(eventsRemoved(const QList<qlonglong>&)),
            this, SLOT(removeEvents(const QList<qlonglong>&)));
}

void EventModel::disconnectFeeder(QString) {
    if (m_feeder) {
        delete m_feeder;
        m_feeder = NULL;
    }
}

void EventModel::removeUIEvent(const qlonglong &id)
{
    connectFeeder();
    m_feeder->removeItem(id);
}

void EventModel::removeUIEventsBySourceName(const QString &sourceName)
{
    connectFeeder();
    m_feeder->removeItemsBySourceName(sourceName);
}
