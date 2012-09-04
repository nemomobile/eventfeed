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

Event::Event(const qlonglong &eid,
             const QString &icon,
             const QString &title,
             const QString &body,
             const QStringList &imageList,
             const QString &timestamp,
             const QString &footer,
             const bool &video,
             const QString &url,
             const QString &sourceName,
             const QString &sourceDisplayName,
             const QString &iconThumbnail,
             const QStringList &thumbnailList)
    : id(eid),
      m_icon(icon),
      m_title(title),
      m_body(body),
      m_imageList(imageList),
      m_timestamp(timestamp),
      m_footer(footer),
      m_video(video),
      m_url(url),
      m_sourceName(sourceName),
      m_sourceDisplayName(sourceDisplayName),
      m_iconThumbnail(iconThumbnail),
      m_thumbnailList(thumbnailList)
{
}

const QString Event::icon() const
{
    return m_icon;
}

const QString Event::title() const
{
    return m_title;
}

const QString Event::body() const
{
    return m_body;
}

const QStringList Event::imageList() const
{
    return m_imageList;
}

const QString Event::timestamp() const
{
    return m_timestamp;
}

const QString Event::footer() const
{
    return m_footer;
}

const bool Event::video() const
{
    return m_video;
}

const QString Event::url() const
{
    return m_url;
}

const QString Event::sourceName() const
{
    return m_sourceName;
}

const QString Event::sourceDisplayName() const
{
    return m_sourceDisplayName;
}
