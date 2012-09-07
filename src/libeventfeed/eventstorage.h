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

#ifndef EVENTSTORAGE_H
#define EVENTSTORAGE_H

#include <QtSql>
#include "event.h"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define MAX_EVENT_ITEMS 250
#define EVENT_COUNT_HYSTERESIS 20
#define DB_SCHEMA_VERSION 1

class EventStorage : public QObject
{
    Q_OBJECT
public:
    explicit EventStorage(QObject *parent = 0);
    virtual ~EventStorage();

public slots:
    void open();
    void close();
    qlonglong addItem(const QVariantMap &parameters);
    bool removeItem(const qlonglong &id);
    const QList<qlonglong> removeItemsBySourceName(const QString &sourceName);
    QList<Event *> getAllItems();
    void saveThumbnail(const qlonglong &id, const int &position,
                       const QString &image);

signals:
    void itemsOutdated(const QList<qlonglong>& ids);

private:
    QString m_dbname;
    QSqlDatabase m_db;
    qlonglong m_itemCount;

private slots:
    void reset();
    bool isSchemaValid();
    void purgeOutdatedItems();
};
#endif
