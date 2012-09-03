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

// TODO: DROP THIS IF WE GO WITH nemo-qml-plugins-thumbnailer

#include <QDebug>
#include <QString>
#include <QHash>
#include <QVariantMap>
#include <QUrl>
#include <QStringList>
#include <thumbnailer/Thumbnailer>

static QString fileToMime(const QString &file) {

	static QHash<QString, QString> mimes;

	if(mimes.isEmpty()) {
		mimes.insert(".jpeg", "image/jpeg");
		mimes.insert(".jpg",  "image/jpeg");
		mimes.insert(".gif",  "image/gif");
		mimes.insert(".png",  "image/png");
		mimes.insert(".bmp",  "image/bmp");
		mimes.insert(".tif",  "image/tiff");
		mimes.insert(".tiff", "image/tiff");
	}

	foreach(QString mime, mimes.keys()) {
		if(file.endsWith(mime, Qt::CaseInsensitive)) {
			return mimes[mime];
		}
	}

	return QString();
}




class EventProcessor : public QObject
{
    Q_OBJECT
public:
    EventProcessor(const qlonglong &id, QVariantMap eventdata, QObject *parent)
      : QObject(parent),
        m_event(eventdata)
    {
        m_event["id"] = id;

    }

    bool process(Thumbnails::Thumbnailer *thumbnailer) {
        bool ready = true;
        QString icon = m_event["icon"].toString();
        QStringList mimes;
        QList<QUrl> urls;

        if (icon.startsWith("/")) {
            m_imgpos.insert(icon, -1);
            urls << QUrl(QString("file://") + icon);
            mimes << fileToMime(icon);
            ready = false;
            qDebug() << __FILE__ << __LINE__ << "not ready";
        }

        int i = -1;
        if (m_event.count("imageList") == 0) { // TODO optimize
            m_event["imageList"] = QStringList();
        }
        qDebug() << __FILE__ << __LINE__ << "imageList: " << m_event["imageList"].toStringList();
        foreach (const QString& img, m_event["imageList"].toStringList()) {
            i++;
            if (!img.isEmpty()) {
                ready = false;
                qDebug() << __FILE__ << __LINE__ << "img: " << img << "i:" << i;
                m_imgpos.insert(img, i);
                urls << img;
                mimes << fileToMime(img);
            }
        }

        if (ready) {
            qDebug() << "return from EventProcessor::process() (ready)";
            return true;
        }

        connect(thumbnailer, SIGNAL(thumbnail(QUrl, QUrl, QPixmap, QString)),
                this, SLOT(thumbnail(QUrl, QUrl)));
        connect(thumbnailer, SIGNAL(error(QString, QUrl)),
                this, SLOT(error(QString, QUrl)));
        thumbnailer->request(urls, mimes);
        qDebug() << "return from EventProcessor::process() (false)";
        return false;
    }

    QVariantMap getEventData() const { // TODO: return reference
        return m_event;
    };

signals:
    void thumbnailReady(const qlonglong &id, const int &position, const QString &image);
    void eventReady(const QVariantMap &eventdata);

private slots:
    void thumbnail(const QUrl &fileUri, const QUrl &thumbUri) {
        qDebug() << "fileUri: " << fileUri << "\nthumbUri: " << thumbUri;

        QString origFile = fileUri.toString(QUrl::RemoveScheme | QUrl::RemoveAuthority);
        QString thumbFile = thumbUri.toString(QUrl::RemoveScheme | QUrl::RemoveAuthority);
        int pos = m_imgpos.take(origFile);
        m_imgmap.insert(origFile, thumbFile);

        emit thumbnailReady(m_event["id"].toLongLong(), pos, thumbFile);

        if (pos == -1) {
            m_event["iconThumbnail"] = thumbFile;
        }

        if (m_imgpos.empty()) {
            fillThumbnailList();
        }
    }

    void error(const QString &error, const QUrl &uri) {
        qCritical() << __FILE__ << __LINE__ << m_event["id"] << uri << error;

        QString origFile = uri.toString(QUrl::RemoveScheme | QUrl::RemoveAuthority);
        m_imgpos.remove(origFile);

        if (m_imgpos.empty()) {
            fillThumbnailList();
        }
    }

private:
    void fillThumbnailList() {
        QStringList thumbnails;
        foreach (const QString &orig, m_event["imageList"].toStringList()) {
            thumbnails << m_imgmap[orig];
        }
        m_event["thumbnailList"] = thumbnails;
        emit eventReady(m_event);
        deleteLater(); // FIXME: is this right way to deallocate memory?
    }

private:
    QVariantMap m_event;
    QMap<QString, int> m_imgpos;
    QMap<QString, QString> m_imgmap;
};

