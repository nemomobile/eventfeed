// Copyright (C) 2012 Jolla Ltd
// Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>

import QtQuick 1.1
import com.nokia.meego 1.0
import org.nemomobile.thumbnailer 1.0

ListView {
    id: eventsView
    model: events
    height: 800
    cacheBuffer: 10
    spacing: 30

    delegate: Item {
        anchors { left: parent.left; right: parent.right }
        height: 40 + bodyText.height + imageListView.height
        Image {
            anchors { left: parent.left; leftMargin: 10 }
            height: 80
            width: 80
            sourceSize.width: 80
            sourceSize.height: 80
            source: icon.indexOf("/") == 0 ? "image://nemoThumbnail/" + icon : "image://theme/" + icon;
        }
        Column {
            anchors { left: parent.left; right: parent.right }
            spacing: 5
            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: 100; rightMargin: 20 }
                text: title
                font.weight: Font.Bold
                font.pixelSize: 16
            }
            Text {
                id: bodyText
                anchors { left: parent.left; right: parent.right; leftMargin: 100; rightMargin: 20 }
                text: body
                wrapMode: Text.WordWrap
                font.pixelSize: 16
            }
            ListView {
                id: imageListView
                model: imageList
                anchors { left: parent.left; right: parent.right; leftMargin: 100; }
                height: 200 * imageList.length
                delegate: Image {
                    anchors { left: parent.left }
                    height: 200
                    width: 200
                    sourceSize.width: 200
                    sourceSize.height: 200
                    source: "image://nemoThumbnail/" + modelData
                }
            }
            Row {
                spacing: 5
                anchors { left: parent.left; right: parent.right; leftMargin: 100; rightMargin: 20 }
                Text {
                    text: timestamp
                    font.pixelSize: 12
                }
                Rectangle {
                    height: 8
                    width: 8
                    radius: 4
                    color: "black"
                }
                Text {
                    text: footer
                    font.pixelSize: 12
                }
            }
        }
        MouseArea {
            anchors { fill: parent }
            onPressAndHold: {
                deleteItemButton.itemId = item_id;
                deleteAllButton.itemType = sourceName;
                deleteAllButton.itemDisplayType = sourceDisplayName;
                eventMenu.open();
            }
            onClicked: Qt.openUrlExternally(url)
        }
    }

    Menu {
        id: eventMenu
        visualParent: eventsView
        MenuLayout {
            MenuItem {
                id: deleteItemButton
                text: "Delete this item"
                property int itemId
                onClicked: {
                    console.log("Clicked Delete this item " + itemId);
                    events.removeUIEvent(itemId);
                }
            }
            MenuItem {
                id: deleteAllButton
                text: "Clear " + itemDisplayType
                property string itemType
                property string itemDisplayType
                onClicked: {
                    console.log("Clicked Delete all these " + itemType + "  " + itemDisplayType);
                    events.removeUIEventsBySourceName(itemType);
                }
            }
        }
    }

    EventModel {
        id: events
    }
}
