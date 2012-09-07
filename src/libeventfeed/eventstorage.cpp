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

#include <QDebug>
#include <QDir>
#include <QtCore/QVariant>
#include "eventstorage.h"

static const QString DB_NAME = "events.data";
static const QString DB_DIR = ".config/eventfeed";

EventStorage::EventStorage(QObject *parent)
  : QObject(parent)
{
    m_dbname = QDir::homePath() + "/" + DB_DIR + "/" + DB_NAME;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
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
    QDir dir(QDir::homePath());

    if (!dir.mkpath(DB_DIR)) {
        qFatal("Can't create directory for database files.");
    }

    if (!m_db.open() || !isSchemaValid()) {
        reset();
    } else {
        QSqlQuery query;

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
    QSqlQuery query;
    qlonglong id;
    QString icon = parameters["icon"].toString();

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
    if (!icon.isEmpty()) {
        query.prepare("INSERT INTO images (id, position, originalPath) VALUES "
                                         "(:id, :position, :originalPath)");
        query.bindValue(":id", id);
        query.bindValue(":position", -1);
        query.bindValue(":originalPath", icon);
        query.exec();
    }

    qDebug() << __FILE__ << __LINE__ << "imageList:" << parameters["imageList"].toStringList();
    int counter = 0;
    foreach (const QString &imgpath, parameters["imageList"].toStringList()) {
        qDebug() << "Adding " << imgpath << " to event " << id;
        query.prepare("INSERT INTO images (id, position, originalPath) VALUES "
                                         "(:id, :position, :originalPath)");
        query.bindValue(":id", id);
        query.bindValue(":position", counter);
        query.bindValue(":originalPath", imgpath);
        query.exec();
        counter++;
    }
    m_db.commit();

    m_itemCount++;

    return id;
}

bool EventStorage::removeItem(const qlonglong &id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM events WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.numRowsAffected() > 0) {
        m_itemCount--;
        query.prepare("DELETE FROM images WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
        return true;
    } else {
        return false;
    }
}

const QList<qlonglong> EventStorage::removeItemsBySourceName(const QString &sourceName)
{
    QSqlQuery query;
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
    query.prepare("DELETE FROM images WHERE id IN "
                    "(SELECT id FROM events WHERE sourceName = :sourceName)");
    query.bindValue(":sourceName", sourceName);
    query.exec();
    m_db.commit();
    m_itemCount -= ids.size();
    return ids;
}

QList<Event *> EventStorage::getAllItems()
{
    QList<Event *> events;
    QSqlQuery query, query_img;
    query.exec("SELECT e.id, e.title, e.body, e.timestamp, e.footer, "
                      "e.url, e.sourceName, e.sourceDisplayName, "
                      "i.originalPath, i.thumbnailPath FROM events e "
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
        QString iconThumbnail = query.value(9).toString();

        // TODO: try to reduce the amount of SQL queries
        query_img.prepare("SELECT originalPath, thumbnailPath FROM images "
                          "WHERE id = :id AND position >= 0 "
                          "ORDER BY position ASC");
        query_img.bindValue(":id", id);
        query_img.exec();
        QStringList images, thumbnails;
        while (query_img.next()) {
            images.append(query_img.value(0).toString());
            thumbnails.append(query_img.value(1).toString());
        }

        Event* event = new Event(id, icon, title, body, images,
                                 timestamp, footer, false, url, sourceName,
                                 sourceDisplayName, iconThumbnail,
                                 thumbnails);
        events.append(event);
    }
    return events;
}

void EventStorage::saveThumbnail(const qlonglong &id, const int &position,
                                 const QString &image)
{
    qDebug() << __FILE__ << __LINE__ << "updating " << id << position << image;
    QSqlQuery query;
    query.prepare("UPDATE images SET thumbnailPath = :path"
                  "WHERE id = :id AND " "position = :position");
    query.bindValue(":path", image);
    query.bindValue(":id", id);
    query.bindValue(":position", position);
    query.exec();
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
    ret = QSqlQuery().exec(
            "CREATE TABLE events ("
                "id INTEGER PRIMARY KEY, "
                "title TEXT, body TEXT, "
                "timestamp TEXT, footer TEXT, "
                "action TEXT, url TEXT, "
                "sourceName TEXT, "
                "sourceDisplayName TEXT)");
    ret = ret && QSqlQuery().exec(
            "CREATE TABLE images ("
                "id INTEGER, position INTEGER, "
                "originalPath TEXT, thumbnailPath TEXT, "
                "type TEXT, PRIMARY KEY(id, position))");
    ret = ret && QSqlQuery().exec("PRAGMA user_version=" STR(DB_SCHEMA_VERSION));
    if (!ret) {
        qFatal("Can't create db schema");
    }
    m_itemCount = 0;
}

bool EventStorage::isSchemaValid()
{
    QSqlQuery query;

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
        QSqlQuery query;
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
