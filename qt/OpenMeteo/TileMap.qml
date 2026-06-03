import QtQuick 6.8
import App

// A minimal OpenStreetMap "slippy map" built from raster tiles, with no
// dependency on QtLocation. Supports drag-to-pan, wheel-to-zoom, and emits
// mapClicked(lat, lon) on a (non-drag) click using Web Mercator projection.
Item {
    id: root

    // --- public API: configured / observed by the parent ---
    property real centerLat: 50.0833 // Prague-ish default
    property real centerLon: 14.4167
    property int zoom: 5
    property string tileServer: "https://tile.openstreetmap.org"

    // last clicked location marker
    property bool hasMarker: false
    property real markerLat: 0
    property real markerLon: 0

    signal mapClicked(real lat, real lon)

    // --- internal / derived state ---
    readonly property int tileSize: 256
    readonly property real worldSize: tileSize * Math.pow(2, zoom)
    // top-left corner of the viewport, in world pixels
    readonly property real originX: lonToWorldX(centerLon) - width / 2
    readonly property real originY: latToWorldY(centerLat) - height / 2
    property var tileModel: []

    // --- this object's own configuration ---
    clip: true

    // --- functions ---
    // Web Mercator projection helpers (degrees <-> world pixels)
    function lonToWorldX(lon) {
        return (lon + 180) / 360 * worldSize;
    }
    function latToWorldY(lat) {
        var s = Math.sin(lat * Math.PI / 180);
        return (0.5 - Math.log((1 + s) / (1 - s)) / (4 * Math.PI)) * worldSize;
    }
    function worldXToLon(x) {
        return x / worldSize * 360 - 180;
    }
    function worldYToLat(y) {
        var n = Math.PI - 2 * Math.PI * y / worldSize;
        return 180 / Math.PI * Math.atan(0.5 * (Math.exp(n) - Math.exp(-n)));
    }

    function rebuildTiles() {
        var n = Math.pow(2, zoom);
        var tiles = [];
        if (width <= 0 || height <= 0)
            return tiles;
        var firstX = Math.floor(originX / tileSize);
        var lastX = Math.floor((originX + width) / tileSize);
        var firstY = Math.floor(originY / tileSize);
        var lastY = Math.floor((originY + height) / tileSize);
        for (var tx = firstX; tx <= lastX; ++tx) {
            for (var ty = firstY; ty <= lastY; ++ty) {
                if (ty < 0 || ty >= n)
                    continue;                 // no wrap vertically
                var wx = ((tx % n) + n) % n;  // wrap horizontally
                tiles.push({
                    "sx": tx * tileSize - originX,
                    "sy": ty * tileSize - originY,
                    "url": tileServer + "/" + zoom + "/" + wx + "/" + ty + ".png"
                });
            }
        }
        return tiles;
    }

    function refresh() {
        tileModel = rebuildTiles();
    }
    // A pan updates centerLat and centerLon back-to-back; routing both through
    // Qt.callLater coalesces them (and any same-frame size/zoom changes) into a
    // single rebuild at the end of the event-loop pass, instead of rebuilding
    // the whole tile model twice per drag step.
    function scheduleRefresh() {
        Qt.callLater(refresh);
    }

    // --- signal handlers ---
    onWidthChanged: scheduleRefresh()
    onHeightChanged: scheduleRefresh()
    onCenterLatChanged: scheduleRefresh()
    onCenterLonChanged: scheduleRefresh()
    onZoomChanged: scheduleRefresh()
    Component.onCompleted: refresh()

    // --- child objects (back-to-front stacking order) ---
    Rectangle {
        anchors.fill: parent
        color: Theme.mapWater // ocean background while tiles load
    }

    Repeater {
        model: root.tileModel
        delegate: Image {
            x: modelData.sx
            y: modelData.sy
            width: root.tileSize
            height: root.tileSize
            source: modelData.url
            asynchronous: true
            cache: true
            fillMode: Image.Pad
        }
    }

    // marker pinned to the clicked geographic point
    Rectangle {
        visible: root.hasMarker
        width: 16
        height: 16
        radius: 8
        color: Theme.highlight
        border.color: Theme.highlightText
        border.width: 2
        x: root.lonToWorldX(root.markerLon) - root.originX - width / 2
        y: root.latToWorldY(root.markerLat) - root.originY - height / 2
    }

    MouseArea {
        anchors.fill: parent
        property real pressX: 0
        property real pressY: 0
        property real startLat: 0
        property real startLon: 0
        property bool dragging: false

        onPressed: function (m) {
            pressX = m.x;
            pressY = m.y;
            startLat = root.centerLat;
            startLon = root.centerLon;
            dragging = false;
        }
        onPositionChanged: function (m) {
            var dx = m.x - pressX;
            var dy = m.y - pressY;
            if (Math.abs(dx) > 4 || Math.abs(dy) > 4)
                dragging = true;
            if (dragging) {
                var cx = root.lonToWorldX(startLon) - dx;
                var cy = root.latToWorldY(startLat) - dy;
                root.centerLon = root.worldXToLon(cx);
                root.centerLat = root.worldYToLat(cy);
            }
        }
        onReleased: function (m) {
            if (dragging)
                return;
            var lat = root.worldYToLat(root.originY + m.y);
            var lon = root.worldXToLon(root.originX + m.x);
            root.markerLat = lat;
            root.markerLon = lon;
            root.hasMarker = true;
            root.mapClicked(lat, lon);
        }
        onWheel: function (w) {
            var nz = root.zoom + (w.angleDelta.y > 0 ? 1 : -1);
            nz = Math.max(2, Math.min(18, nz));
            if (nz === root.zoom)
                return;
            // keep the geo point under the cursor fixed across the zoom change
            var lat = root.worldYToLat(root.originY + w.y);
            var lon = root.worldXToLon(root.originX + w.x);
            root.zoom = nz; // worldSize/origin recompute on this assignment
            var newWorld = root.tileSize * Math.pow(2, nz);
            var newWx = (lon + 180) / 360 * newWorld;
            var s = Math.sin(lat * Math.PI / 180);
            var newWy = (0.5 - Math.log((1 + s) / (1 - s)) / (4 * Math.PI)) * newWorld;
            root.centerLon = root.worldXToLon(newWx - w.x + root.width / 2);
            root.centerLat = root.worldYToLat(newWy - w.y + root.height / 2);
        }
    }

    // OSM attribution (required by the tile usage policy)
    Text {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 4
        text: "© OpenStreetMap contributors"
        font.pixelSize: 10
        color: Theme.attribution
        style: Text.Outline
        styleColor: Theme.attributionOutline
    }
}
