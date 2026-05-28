import QtQuick

Window {
    width: 1200
    height: 800
    visible: true
    title: qsTr("Hello Anchors")

    contentItem {
        scale: 0.1
        transformOrigin: Item.TopLeft
    }

    Image {
        id: image0
        source: Qt.resolvedUrl("assets/image0")
    }

    Image {
        id: image1
        anchors.left: image0.right
        source: Qt.resolvedUrl("assets/image1")
    }

    Image {
        id: image2
        anchors.left: image1.right
        source: Qt.resolvedUrl("assets/image2")
    }

    Image {
        id: image3
        anchors.left: image2.right
        source: Qt.resolvedUrl("assets/image3")
    }

    Image {
        id: image4
        anchors.left: image3.right
        source: Qt.resolvedUrl("assets/image4")
    }

    Image {
        id: image5
        anchors.left: image4.right
        source: Qt.resolvedUrl("assets/image5")
    }

    Image {
        id: image6
        anchors.left: image5.right
        source: Qt.resolvedUrl("assets/image6")
    }

    Image {
        id: image7
        anchors.left: image6.right
        source: Qt.resolvedUrl("assets/image7")
    }

    Image {
        id: image8
        anchors.left: image7.right
        source: Qt.resolvedUrl("assets/image8")
    }

    Image {
        id: image9
        anchors.left: image8.right
        source: Qt.resolvedUrl("assets/image9")
    }
}
