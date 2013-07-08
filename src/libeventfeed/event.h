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

#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QDateTime>

class Event : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(QString footer READ footer WRITE setFooter NOTIFY footerChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList imageList READ imageList WRITE setImageList NOTIFY imageListChanged)
    Q_PROPERTY(bool isVideo READ isVideo WRITE setIsVideo NOTIFY isVideoChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(QString sourceName READ sourceName WRITE setSourceName NOTIFY sourceNameChanged)
    Q_PROPERTY(QString sourceDisplayName READ sourceDisplayName WRITE setSourceDisplayName NOTIFY sourceDisplayNameChanged)
    Q_PROPERTY(QVariantMap metaData READ metaData WRITE setMetaData NOTIFY metaDataChanged)

public:
    Event(QObject *parent = 0);
    virtual ~Event();

    void setId(const qlonglong id);
    const qlonglong id() const;
    void setIcon(const QString& iconId);
    const QString& icon() const;
    void setTitle(const QString& name);
    const QString& title() const;
    void setBody(const QString& body);
    const QString& body() const;
    void setImageList(const QStringList& list);
    const QStringList& imageList() const;
    void setIsVideo(bool isVideo);
    bool isVideo() const;
    void setTimestamp(const QDateTime& timestamp);
    const QDateTime& timestamp() const;
    void setFooter(const QString& footer);
    const QString& footer() const;
    void setUrl(const QString &url);
    const QString& url() const;
    void setSourceName(const QString &sourceName);
    const QString& sourceName() const;
    void setSourceDisplayName(const QString &sourceDisplayName);
    const QString& sourceDisplayName() const;
    void setMetaData(const QVariantMap &metaData);
    const QVariantMap& metaData() const;

signals:
    void idChanged();
    void iconChanged();
    void titleChanged();
    void bodyChanged();
    void footerChanged();
    void urlChanged();
    void imageListChanged();
    void timestampChanged();
    void sourceNameChanged();
    void sourceDisplayNameChanged();
    void isVideoChanged();
    void metaDataChanged();

private:
    qlonglong id_;
    QString icon_;
    QString title_;
    QString body_;
    QString footer_;
    QStringList imageList_;
    QDateTime timestamp_;
    QString sourceName_;
    QString sourceDisplayName_;
    bool isVideo_;
    QString url_;
    QVariantMap metaData_;
};

#endif /* EVENT_H */
