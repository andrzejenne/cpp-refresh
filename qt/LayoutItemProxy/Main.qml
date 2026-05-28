import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: qsTr("Hello LayoutItemProxy")

    color: "black"
    palette.windowText: "white"

    property int currentImageIndex: 0

    readonly property int tabletBreakpoint: 1000
    readonly property int mobileBreakpoint: 600
    readonly property bool isDesktopLayout: window.width > tabletBreakpoint
    readonly property bool isTabledLayout: window.width > mobileBreakpoint && window.width <= tabletBreakpoint
    readonly property bool isMobileLayout: window.width <= mobileBreakpoint


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

    Flickable {
        id: flickable

        contentHeight: gridLayout.height
        contentWidth: gridLayout.width

        clip: true

        GridLayout {
            id: gridLayout

            columns: window.isDesktopLayout ? 2 : window.isTabledLayout ? 1 : -1
            rows: window.isMobileLayout ? 1 : -1

            Repeater {
                model: 10
                delegate: thumbnailDelegate
            }
        }
    }

    Image {
        id: previewImage
        Layout.fillWidth: true
        Layout.fillHeight: true

        source: Qt.resolvedUrl(`assets/image${window.currentImageIndex}.jpg`)
        asynchronous: true
    }

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
        id: heightValue
        text: previewImage.sourceSize.height
    }


    RowLayout {
        // desktop

        visible: window.isDesktopLayout

        anchors {
            fill: parent
            margins: 5
        }

        LayoutItemProxy {
            target: flickable

            Layout.fillHeight: true
            Layout.preferredWidth: gridLayout.width
        }

        ColumnLayout {
            LayoutItemProxy {
                target: previewImage
            }

            RowLayout {
                Layout.minimumHeight: 40

                LayoutItemProxy {
                    target: sourceTitle
                }
                LayoutItemProxy {
                    target: sourceValue
                }
                LayoutItemProxy {
                    target: widthTitle
                }
                LayoutItemProxy {
                    target: widthValue
                }
                LayoutItemProxy {
                    target: heightTitle
                }
                LayoutItemProxy {
                    target: heightValue
                }
            }
        }
    }

    RowLayout {
        // tablet

        visible: window.isTabledLayout

        anchors {
            fill: parent
            margins: 5
        }

        LayoutItemProxy {
            target: flickable

            Layout.fillHeight: true
            Layout.preferredWidth: gridLayout.width
        }

        LayoutItemProxy {
            target: previewImage
        }

        ColumnLayout {

            Layout.alignment: Qt.AlignTop
            Layout.minimumHeight: 40

            LayoutItemProxy {
                target: sourceTitle
            }
            LayoutItemProxy {
                target: sourceValue
            }
            LayoutItemProxy {
                target: widthTitle
            }
            LayoutItemProxy {
                target: widthValue
            }
            LayoutItemProxy {
                target: heightTitle
            }
            LayoutItemProxy {
                target: heightValue
            }
        }
    }

    ColumnLayout {
        // mobile

        visible: window.isMobileLayout

        anchors {
            fill: parent
            margins: 5
        }

        LayoutItemProxy {
            target: previewImage
        }

        GridLayout {
            columns: 4

            LayoutItemProxy {
                target: sourceTitle
            }
            LayoutItemProxy {
                target: sourceValue
            }
            LayoutItemProxy {
                target: widthTitle
            }
            LayoutItemProxy {
                target: widthValue
            }
            LayoutItemProxy {
                target: heightTitle
            }
            LayoutItemProxy {
                target: heightValue
            }
        }

        LayoutItemProxy {
            target: flickable

            Layout.fillWidth: true
            Layout.preferredHeight: gridLayout.height
        }
    }
}
