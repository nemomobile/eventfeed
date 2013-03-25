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

#ifndef MEVENTFEED_H
#define MEVENTFEED_H

#include "meventfeedproxy.h"
#include <qglobal.h>

/*!
 * An interface to the event feed. Uses the org.nemomobile.events.EventFeed D-Bus
 * interface with which application developers can create and manage event
 * feed items.
 */
class MEventFeed
{
private:
    /*!
     * Constructs an interface for the event feed.
     */
    MEventFeed();

    /*!
     * Destroys an interface for the event feed.
     */
    virtual ~MEventFeed();

    //! The D-Bus proxy object for the event feed interface.
    MEventFeedProxy proxy;

public:
    /*!
     * Returns an instance of the event feed interface.
     *
     * \return an instance of the event feed interface
     */
    static MEventFeed *instance();

    /*!
     * Adds an item to the event feed.
     *
     * \param icon the icon id for the feed item or absolute path to an image file. The icon must exist when addItem() is called. If the icon image is unique its path must be unique (same path should not be reused for different images).
     * \param title the title for the feed item
     * \param body the body text for the feed item
     * \param imageList the image list for the feed item. If the images in list start with /, they're interpreted as absolute paths, otherwise as image IDs from the theme. The images must exist when addItem() is called. If an image is unique its path must be unique (same path should not be reused for different images).
     * \param timestamp timestamp for the feed item
     * \param footer the footer text for the feed item. If there is no visible text to be shown in the footer the footer should be empty.
     * \param video if true, marks that the event item contains video content. The video content is limited to one thumbnail in the image list. The thumbnail will have a play button overlay rendered on top of it.
     * \param url the url to be executed when item is clicked. Executed action for URL is the default action provided by libcontentaction for the URL's scheme.
     * \param sourceName identifier for the event source e.g. application name, which should be persistent.
     * \param sourceDisplayName the source description in localized form which will be displayed in the UI in event feed item's object menu.
     * \return unique id for the event feed item or -1 if values for the mandatory keys are not provided
     */
    qlonglong addItem(const QString &icon, const QString &title, const QString &body, const QStringList &imageList, const QDateTime &timestamp, const QString &footer, bool video, const QUrl &url, const QString &sourceName, const QString &sourceDisplayName);

    /*!
     * Updates an item in the event feed.
     *
     * \param id unique id for the event feed item to be updated
     * \param icon the icon id for the feed item or absolute path to an image file. The icon must exist when addItem() is called. If the icon image is unique its path must be unique (same path should not be reused for different images).
     * \param title the title for the feed item
     * \param body the body text for the feed item
     * \param imageList the image list for the feed item. If the images in list start with /, they're interpreted as absolute paths, otherwise as image IDs from the theme. The images must exist when addItem() is called. If an image is unique its path must be unique (same path should not be reused for different images).
     * \param timestamp timestamp for the feed item
     * \param footer the footer text for the feed item. If there is no visible text to be shown in the footer the footer should be empty.
     * \param video if true, marks that the event item contains video content. The video content is limited to one thumbnail in the image list. The thumbnail will have a play button overlay rendered on top of it.
     * \param url the url to be executed when item is clicked. Executed action for URL is the default action provided by libcontentaction for the URL's scheme.
     * \param sourceName identifier for the event source e.g. application name, which should be persistent.
     * \param sourceDisplayName the source description in localized form which will be displayed in the UI in event feed item's object menu.
     */
    void updateItem(qlonglong id, const QString &icon, const QString &title, const QString &body, const QStringList &imageList, const QDateTime &timestamp, const QString &footer, bool video, const QUrl &url, const QString &sourceName, const QString &sourceDisplayName);

    /*!
     * Removes an item from the event feed.
     *
     * \param id the id of the feed item
     */
    void removeItem(qlonglong id);

    /*!
     * Removes all events with the specified source name from the event feed.
     *
     * \param sourceName identifier for the event source
     */
    void removeItemsBySourceName(const QString &sourceName);
};

#endif /* MEVENTFEED_H */
