import QtQuick

Item {
    id: root
    property string imageUrl: ""

    Rectangle {
        width: root.width
        height: root.height
        border.width: 2
        border.color: "black"
        Image {
            anchors.fill: parent
            source: root.imageUrl
        }
    }
}
