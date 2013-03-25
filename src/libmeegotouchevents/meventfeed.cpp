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


#include "meventfeed.h"

MEventFeed::MEventFeed() :
    proxy("org.nemomobile.events.EventFeed",
          "/org/nemomobile/events/EventFeed", QDBusConnection::sessionBus())
{
}

MEventFeed::~MEventFeed()
{
}

MEventFeed *MEventFeed::instance()
{
    static MEventFeed eventFeedInstance;
    return &eventFeedInstance;
}

qlonglong MEventFeed::addItem(const QString &icon, const QString &title, const QString &body, const QStringList &imageList, const QDateTime &timestamp, const QString &footer, bool video, const QUrl &url, const QString &sourceName, const QString &sourceDisplayName)
{
    QVariantMap parameters;
    if (!icon.isEmpty()) {
        parameters.insert("icon", icon);
    }
    if (!title.isEmpty()) {
        parameters.insert("title", title);
    }
    if (!body.isEmpty()) {
        parameters.insert("body", body);
    }
    if (!imageList.isEmpty()) {
        parameters.insert("imageList", imageList);
    }
    if (timestamp.isValid()) {
        parameters.insert("timestamp", timestamp.toString(Qt::ISODate));
    }
    if (!footer.isEmpty()) {
        parameters.insert("footer", footer);
    }
    if (video) {
        parameters.insert("video", video);
    }
    if (!url.isEmpty()) {
        parameters.insert("url", url.toString());
    }
    if (!sourceName.isEmpty()) {
        parameters.insert("sourceName", sourceName);
    }
    if (!sourceDisplayName.isEmpty()) {
        parameters.insert("sourceDisplayName", sourceDisplayName);
    }
    return proxy.addItem(parameters);
}

void MEventFeed::updateItem(qlonglong id, const QString &icon, const QString &title, const QString &body, const QStringList &imageList, const QDateTime &timestamp, const QString &footer, bool video, const QUrl &url, const QString &sourceName, const QString &sourceDisplayName)
{
    QVariantMap parameters;
    if (!icon.isEmpty()) {
        parameters.insert("icon", icon);
    }
    if (!title.isEmpty()) {
        parameters.insert("title", title);
    }
    if (!body.isEmpty()) {
        parameters.insert("body", body);
    }
    if (!imageList.isEmpty()) {
        parameters.insert("imageList", imageList);
    }
    if (timestamp.isValid()) {
        parameters.insert("timestamp", timestamp.toString(Qt::ISODate));
    }
    if (!footer.isEmpty()) {
        parameters.insert("footer", footer);
    }
    if (video) {
        parameters.insert("video", video);
    }
    if (!url.isEmpty()) {
        parameters.insert("url", url.toString());
    }
    if (!sourceName.isEmpty()) {
        parameters.insert("sourceName", sourceName);
    }
    if (!sourceDisplayName.isEmpty()) {
        parameters.insert("sourceDisplayName", sourceDisplayName);
    }
    proxy.updateItem(id, parameters);
}

void MEventFeed::removeItem(qlonglong id)
{
    proxy.removeItem(id);
}

void MEventFeed::removeItemsBySourceName(const QString &sourceName)
{
    proxy.removeItemsBySourceName(sourceName);
}
