import QtQuick 1.1
import com.nokia.meego 1.0
import org.nemomobile.events 1.0

PageStackWindow {
    initialPage: mainPage

    Page {
        id: mainPage

        EventsWidget {
            anchors { left: parent.left; right: parent.right }
            id: events
        }
    }
}
