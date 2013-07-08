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

#include "event.h"

Event::Event(QObject *parent) : QObject(parent)
{
}

Event::~Event()
{
}

void Event::setId(const qlonglong id)
{
    qlonglong oldId = id_;

    id_ = id;

    if (id_ != oldId) {
        emit idChanged();
    }
}

const qlonglong Event::id() const
{
    return id_;
}

void Event::setIcon(const QString &icon)
{
    QString oldIcon = icon_;

    icon_ = icon;

    if (icon_ != oldIcon) {
        emit iconChanged();
    }
}

const QString& Event::icon() const
{
    return icon_;
}

void Event::setTitle(const QString &title)
{
    QString oldTitle = title_;

    title_ = title;

    if (title_ != oldTitle) {
        emit titleChanged();
    }
}

const QString& Event::title() const
{
    return title_;
}

void Event::setBody(const QString &body)
{
    QString oldBody = body_;

    body_ = body;

    if (body_ != oldBody) {
        emit bodyChanged();
    }
}

const QString& Event::body() const
{
    return body_;
}

void Event::setSourceName(const QString &sourceName)
{
    QString oldSourceName = sourceName_;

    sourceName_ = sourceName;

    if (sourceName_ != oldSourceName) {
        emit sourceNameChanged();
    }
}

const QString& Event::sourceName() const
{
    return sourceName_;
}

void Event::setSourceDisplayName(const QString &sourceDisplayName)
{
    QString oldSourceDisplayName = sourceDisplayName_;

    sourceDisplayName_ = sourceDisplayName;

    if (sourceDisplayName_ != oldSourceDisplayName) {
        emit sourceDisplayNameChanged();
    }
}

const QString& Event::sourceDisplayName() const
{
    return sourceDisplayName_;
}

void Event::setImageList(const QStringList &list)
{
    imageList_ = list;

    emit imageListChanged();
}

const QStringList& Event::imageList() const
{
    return imageList_;
}

void Event::setIsVideo(bool video)
{
    bool oldVideo = isVideo_;

    isVideo_ = video;

    if (isVideo_ != oldVideo) {
        emit isVideoChanged();
    }
}

bool Event::isVideo() const
{
    return isVideo_;
}

void Event::setTimestamp(const QDateTime &timestamp)
{
    QDateTime oldTimestamp = timestamp_;

    timestamp_ = timestamp;

    if (timestamp_ != oldTimestamp) {
        emit timestampChanged();
    }
}

const QDateTime& Event::timestamp() const
{
    return timestamp_;
}

void Event::setFooter(const QString &footer)
{
    QString oldFooter = footer_;

    footer_ = footer;

    if (footer_ != oldFooter) {
        emit footerChanged();
    }
}

const QString& Event::footer() const
{
    return footer_;
}

void Event::setUrl(const QString &url)
{
    QString oldUrl = url_;

    url_ = url;

    if (url_ != oldUrl) {
        emit urlChanged();
    }
}

const QString& Event::url() const
{
    return url_;
}

void Event::setMetaData(const QVariantMap &metaData)
{
    QVariantMap oldMetaData(metaData_);

    metaData_ = metaData;

    if (metaData_ != oldMetaData) {
        emit metaDataChanged();
    }
}

const QVariantMap& Event::metaData() const
{
    return metaData_;
}
