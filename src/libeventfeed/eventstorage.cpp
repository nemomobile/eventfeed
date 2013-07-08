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

#include <QDir>
#include <QtCore/QVariant>
#include "eventstorage.h"

static const QString DB_NAME = "events.data";
static const QString DATA_SUBDIR = "eventfeed";

EventStorage::EventStorage(QObject *parent)
  : QObject(parent)
{
    m_dbname = dataDir().filePath(DB_NAME);
    m_db = QSqlDatabase::addDatabase("QSQLITE", "EventStorage");
    m_db.setDatabaseName(m_dbname);
}

EventStorage::~EventStorage()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void EventStorage::open()
{
    if (!dataDir().mkpath(".")) {
        qFatal("Can't create directory for database files.");
    }

    if (!m_db.open() || !isSchemaValid()) {
        reset();
    } else {
        QSqlQuery query(m_db);

        if (!query.exec("PRAGMA foreign_keys = ON")) {
            qFatal("No foreign keys support in SQLite");
        }

        if (!query.exec("SELECT count(*) FROM events")) {
            qFatal("Wrong DB schema");
        }
    
        query.next();
        m_itemCount = query.value(0).toLongLong();
    }
}

void EventStorage::close()
{
    m_db.close();
}

qlonglong EventStorage::addItem(const QVariantMap &parameters)
{
    QSqlQuery query(m_db);
    qlonglong id;

    purgeOutdatedItems();

    m_db.transaction();
    query.prepare("INSERT INTO events (title, body, timestamp, footer, url, "
                                      "sourceName, sourceDisplayName) "
                  "VALUES (:title, :body, :timestamp, :footer, :url, "
                          ":sourceName, :sourceDisplayName)");
    query.bindValue(":title",      parameters["title"].toString());
    query.bindValue(":body",       parameters["body"].toString());
    query.bindValue(":timestamp",  parameters["timestamp"].toString());
    query.bindValue(":footer",     parameters["footer"].toString());
    query.bindValue(":url",        parameters["url"].toString());
    query.bindValue(":sourceName", parameters["sourceName"].toString());
    query.bindValue(":sourceDisplayName",
            parameters["sourceDisplayName"].toString());
    query.exec();
    QVariant last_id = query.lastInsertId();

    if (!last_id.isValid()) {
        m_db.rollback();
        return -1;
    }

    id = last_id.toLongLong();
    addImages(id, parameters);
    addMetaData(id, parameters);
    m_db.commit();

    m_itemCount++;

    return id;
}

void EventStorage::addImages(const qlonglong &id, const QVariantMap &parameters)
{
    QSqlQuery query(m_db);
    QString icon = parameters["icon"].toString();
    if (!icon.isEmpty()) {
        query.prepare("INSERT INTO images (id, position, originalPath, type) VALUES "
                                         "(:id, :position, :originalPath, 'image')");
        query.bindValue(":id", id);
        query.bindValue(":position", -1);
        query.bindValue(":originalPath", icon);
        query.exec();
    }

    int counter = 0;
    foreach (const QString &imgpath, parameters["imageList"].toStringList()) {
        query.prepare("INSERT INTO images (id, position, originalPath, type) VALUES "
                                         "(:id, :position, :originalPath, :type)");
        query.bindValue(":id", id);
        query.bindValue(":position", counter);
        query.bindValue(":originalPath", imgpath);
        if (counter == 0 && parameters["video"].toBool()) {
            query.bindValue(":type", "video");
        } else {
            query.bindValue(":type", "image");
        }
        query.exec();
        counter++;
    }
}

void EventStorage::addMetaData(const qlonglong &id, const QVariantMap &parameters)
{
    QVariantMap metaData(parameters);
    metaData.remove("id");
    metaData.remove("icon");
    metaData.remove("title");
    metaData.remove("body");
    metaData.remove("imageList");
    metaData.remove("timestamp");
    metaData.remove("footer");
    metaData.remove("video");
    metaData.remove("url");
    metaData.remove("sourceName");
    metaData.remove("sourceDisplayName");
    QSqlQuery query(m_db);
    foreach (const QString &key, metaData.keys()) {
        query.prepare("INSERT INTO metadata (id, key, value) VALUES "
                                         "(:id, :key, :value)");
        query.bindValue(":id", id);
        query.bindValue(":key", key);
        query.bindValue(":value", metaData.value(key).toString());
        query.exec();
    }
}

QDir EventStorage::dataDir()
{
    static const QDir xdgDataHome = QProcessEnvironment::systemEnvironment()
        .value("XDG_DATA_HOME", QDir::home().filePath(".config"));
    static const QDir dataDir = xdgDataHome.filePath(DATA_SUBDIR);
    return dataDir;
}

void EventStorage::updateItem(const qlonglong &id, const QVariantMap &parameters)
{
    removeItem(id);

    QSqlQuery query(m_db);
    m_db.transaction();
    query.prepare("INSERT INTO events (id, title, body, timestamp, footer, url, "
                                      "sourceName, sourceDisplayName) "
                  "VALUES (:id, :title, :body, :timestamp, :footer, :url, "
                          ":sourceName, :sourceDisplayName)");
    query.bindValue(":id",         id);
    query.bindValue(":title",      parameters["title"].toString());
    query.bindValue(":body",       parameters["body"].toString());
    query.bindValue(":timestamp",  parameters["timestamp"].toString());
    query.bindValue(":footer",     parameters["footer"].toString());
    query.bindValue(":url",        parameters["url"].toString());
    query.bindValue(":sourceName", parameters["sourceName"].toString());
    query.bindValue(":sourceDisplayName",
            parameters["sourceDisplayName"].toString());
    query.exec();
    addImages(id, parameters);
    addMetaData(id, parameters);
    m_db.commit();

    m_itemCount++;
}

bool EventStorage::removeItem(const qlonglong &id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM events WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.numRowsAffected() > 0) {
        m_itemCount--;
        return true;
    } else {
        return false;
    }
}

const QList<qlonglong> EventStorage::removeItemsBySourceName(const QString &sourceName)
{
    QSqlQuery query(m_db);
    QList<qlonglong> ids;
    m_db.transaction();
    query.prepare("SELECT id FROM events WHERE sourceName = :sourceName");
    query.bindValue(":sourceName", sourceName);
    query.exec();
    while (query.next()) {
        ids.append(query.value(0).toLongLong());
    }
    query.prepare("DELETE FROM events WHERE sourceName = :sourceName");
    query.bindValue(":sourceName", sourceName);
    query.exec();
    m_db.commit();
    m_itemCount -= ids.size();
    return ids;
}

QList<Event *> EventStorage::getAllItems()
{
    QList<Event *> events;
    QSqlQuery query(m_db), query_img(m_db), query_metadata(m_db);
    query.exec("SELECT e.id, e.title, e.body, e.timestamp, e.footer, "
                      "e.url, e.sourceName, e.sourceDisplayName, "
                      "i.originalPath FROM events e "
               "LEFT OUTER JOIN images i ON i.id = e.id AND "
                                           "i.position = -1 "
               "ORDER BY e.timestamp DESC, e.id DESC");
    while (query.next()) {
        qlonglong id = query.value(0).toLongLong();
        QString title = query.value(1).toString();
        QString body = query.value(2).toString();
        QString timestamp = query.value(3).toString();
        QString footer = query.value(4).toString();
        QString url = query.value(5).toString();
        QString sourceName = query.value(6).toString();
        QString sourceDisplayName = query.value(7).toString();
        QString icon = query.value(8).toString();

        query_img.prepare("SELECT originalPath, type FROM images "
                          "WHERE id = :id AND position >= 0 "
                          "ORDER BY position ASC");
        query_img.bindValue(":id", id);
        query_img.exec();
        bool video = false;
        QStringList images;
        while (query_img.next()) {
            images.append(query_img.value(0).toString());
            video |= query_img.value(1).toString() == "video";
        }

        query_metadata.prepare("SELECT * FROM metadata WHERE ID = :id");
        query_metadata.bindValue(":id", id);
        query_metadata.exec();
        QVariantMap metaData;
        while (query_metadata.next()) {
            metaData.insert(query_metadata.value(1).toString(), query_metadata.value(2));
        }

        Event* event = new Event;
        event->setId(id);
        event->setIcon(icon);
        event->setTitle(title);
        event->setBody(body);
        event->setImageList(images);
        event->setTimestamp(QDateTime::fromString(timestamp, Qt::ISODate));
        event->setFooter(footer);
        event->setIsVideo(video);
        event->setUrl(url);
        event->setSourceName(sourceName);
        event->setSourceDisplayName(sourceDisplayName);
        event->setMetaData(metaData);
        events.append(event);
    }
    return events;
}

void EventStorage::reset()
{
    if (QFile::exists(m_dbname)) {
        if (!QFile::remove(m_dbname)) {
            qFatal("Can't delete existing events db file");
        }
    }

    if (!m_db.open()) {
        qFatal("Can't create events db file");
    }

    bool ret;
    ret = QSqlQuery(m_db).exec(
            "CREATE TABLE events ("
                "id INTEGER PRIMARY KEY, "
                "title TEXT, body TEXT, "
                "timestamp TEXT, footer TEXT, "
                "action TEXT, url TEXT, "
                "sourceName TEXT, "
                "sourceDisplayName TEXT)");
    ret = ret && QSqlQuery(m_db).exec(
            "CREATE TABLE images ("
                "id INTEGER, position INTEGER, "
                "originalPath TEXT, "
                "type TEXT, PRIMARY KEY(id, position) "
                "FOREIGN KEY(id) REFERENCES events(id) ON DELETE CASCADE)");
    ret = ret && QSqlQuery(m_db).exec(
            "CREATE TABLE metadata ("
                "id INTEGER, key TEXT, "
                "value TEXT, "
                "FOREIGN KEY(id) REFERENCES events(id) ON DELETE CASCADE)");
    ret = ret && QSqlQuery(m_db).exec("PRAGMA user_version=" STR(DB_SCHEMA_VERSION));
    ret = ret && QSqlQuery(m_db).exec("PRAGMA foreign_keys = ON");
    if (!ret) {
        qFatal("Can't create db schema");
    }
    m_itemCount = 0;
}

bool EventStorage::isSchemaValid()
{
    QSqlQuery query(m_db);

    if (!query.exec("PRAGMA user_version")) {
        return false;
    }
    query.next();
    if (query.value(0).toLongLong() != DB_SCHEMA_VERSION) {
        return false;
    }

    return true;
}

void EventStorage::purgeOutdatedItems()
{
    QList<qlonglong> ids;

    if (m_itemCount > MAX_EVENT_ITEMS) {
        QSqlQuery query(m_db);
        m_db.transaction();
        query.exec("SELECT id FROM events ORDER BY timestamp ASC, id ASC LIMIT "
                    STR(EVENT_COUNT_HYSTERESIS));
        while (query.next()) {
            ids.append(query.value(0).toLongLong());
        }
        query.exec("DELETE FROM events WHERE id IN "
                        "(SELECT id FROM events ORDER BY timestamp ASC, id ASC LIMIT "
                          STR(EVENT_COUNT_HYSTERESIS) ")");
        m_db.commit();
        m_itemCount -= ids.size();
        emit itemsOutdated(ids);
    }
}
