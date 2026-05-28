import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: qsTr("Hello Layouts")

    color: "black"
    palette.windowText: "white"

    property int currentImageIndex: 0

    Component {
        id: thumbnailDelegate

        Image {
            id: thumbnail

            required property int index

            source: Qt.resolvedUrl(`assets/image${index}.jpg`)
            asynchronous: true

            Layout.preferredWidth: 100
            Layout.preferredHeight: 100

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border {
                    color: "white"
                    width: window.currentImageIndex === thumbnail.index ? 2 : 0
                }
            }

            Label {
                anchors.centerIn: parent
                font.pixelSize: 30
                style: Text.Outline
                styleColor: "black"
                text: thumbnail.index
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentImageIndex = thumbnail.index
            }
        }
    }

    RowLayout {
        anchors {
            fill: parent
            margins: 5
        }

        Flickable {
            id: flickable

            contentHeight: gridLayout.height
            contentWidth: gridLayout.width

            Layout.fillHeight: true
            Layout.preferredWidth: gridLayout.width

            clip: true

            GridLayout {
                id: gridLayout

                columns: 2

                Repeater {
                    model: 10
                    delegate: thumbnailDelegate
                }
            }
        }

        ColumnLayout {
            Image {
                id: previewImage
                Layout.fillWidth: true
                Layout.fillHeight: true

                source: Qt.resolvedUrl(`assets/image${window.currentImageIndex}.jpg`)
                asynchronous: true
            }
            RowLayout {
                id: imagDetails

                Layout.minimumHeight: 40
                Label {
                    id: sourceTitle
                    text: qsTr("Source")
                    font.bold: true
                }

                Label {
                    id: sourceValue
                    text: previewImage.source
                }

                Label {
                    id: widthTitle
                    text: qsTr("Width")
                    font.bold: true
                }

                Label {
                    id: widthValue
                    text: previewImage.sourceSize.width
                }

                Label {
                    id: heightTitle
                    text: qsTr("Height")
                    font.bold: true
                }

                Label {
                    id: heihtValue
                    text: previewImage.sourceSize.height
                }
            }
        }
    }
}
