import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        anchors.centerIn: parent
        spacing: 20

        Rectangle {
            id: recClickTest
            width: 100
            height: 100
            gradient: Gradient {
                GradientStop {
                    id: gradStop1
                    position: 0.0
                    color: "pink"
                }
                GradientStop {
                    id: gradStop2
                    position: 1.0
                    color: "green"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    gradStop1.color = "blue"
                }
            }
        }

        Rectangle {
            width: 100
            height: 100
            color: "blue"
            border.color: "black"
            radius: height
        }

        Item {
            width: 100
            height: 50

            Text {
                color: "blue"
                width: parent.width
                font.family: "Courier New"
                font.pixelSize: 22
                text: qsTr("this is a text")
                wrapMode: Text.WordWrap
            }
        }

        Image {
            width: 100
            height: 100
            source: Qt.resolvedUrl("music-app-icon.png")
        }

        MyComponent {
            width: 100
            height: 100
            imageUrl: Qt.resolvedUrl("music-app-icon.png")
        }
    }
}
