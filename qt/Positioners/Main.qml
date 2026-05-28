import QtQuick

Window {
    width: 1280
    height: 800
    visible: true
    title: qsTr("Hello World")

    Component {
       id: imageDelegate

       Image {
            id: image

            required property int index

            width: 100
            height: 100

            source: Qt.resolvedUrl(`assets/image${index}.jpg`)
            asynchronous: true

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
                style: Text.Outline
                styleColor: "black"
                text: image.index
            }

            Rectangle {
                color: "transparent"
                anchors.fill: parent
                border.width: 5
                border.color: image.Positioner.isFirstItem ? "green" : image.Positioner.isLastItem ? "red" : "transparent"
            }
        }
    }

    Rectangle {
        color: "grey"
        anchors.fill: positioner
    }

    Flow {
        id: positioner
        anchors.fill: parent
        spacing: 10
        padding: 10
        layoutDirection: Qt.RightToLeft
        flow: Grid.TopToBottom
        Repeater {
            model: 10

            delegate: imageDelegate
        }
    }
}
