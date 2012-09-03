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
#include "eventfeedservice.h"
#include "eventfeedadaptor.h"
#include "eventprocessor.h"

EventFeedService::EventFeedService(QObject *parent)
  : QObject(parent),
    m_thumbnailer(NULL)
{
    m_thumbnailer = new Thumbnails::Thumbnailer();
    m_storage.open();
    m_timer.setInterval(20000);
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SIGNAL(idle()));

    new EventFeedAdaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService("org.nemomobile.events.EventFeed")) {
        qFatal("Can't register service on session bus");
    }
    if (!connection.registerObject("/org/nemomobile/events/EventFeed", this)) {
        qFatal("Can't register object for the D-Bus service.");
    }

    m_timer.start();
}

EventFeedService::~EventFeedService()
{
    m_storage.close();
    delete m_thumbnailer;
}

qlonglong EventFeedService::addItem(const QVariantMap &parameters)
{
    qlonglong id;
    m_timer.stop();

    id = m_storage.addItem(parameters);
    EventProcessor *processor = new EventProcessor(id, parameters, this);
    connect(processor, SIGNAL(thumbnailReady(const qlonglong&, const int&, const QString&)),
            &m_storage, SLOT(saveThumbnail(const qlonglong&, const int&, const QString&)));
    if (processor->process(m_thumbnailer)) {
        emit eventAdded(processor->getEventData());
        processor->deleteLater();
        m_timer.start();
        return id;
    }
    connect(processor, SIGNAL(eventReady(QVariantMap)),
            this, SIGNAL(eventAdded(QVariantMap)));

    m_timer.start();
    return id;
}

void EventFeedService::removeItem(qlonglong id)
{
    m_timer.stop();
    if (m_storage.removeItem(id)) {
        QList<qlonglong> ids;
        ids.append(id);
        emit eventsRemoved(ids);
    }
    m_timer.start();
}

void EventFeedService::removeItemsBySourceName(const QString &sourceName)
{
    QList<qlonglong> ids;
    m_timer.stop();
    ids = m_storage.removeItemsBySourceName(sourceName);
    if (!ids.isEmpty()) {
        emit eventsRemoved(ids);
    }
    m_timer.start();
}
