import QtQuick 6.8
import QtQuick.Controls 6.8
import QtQuick.Controls.Material 6.8
import QtQuick.Layouts 6.8
import QtCharts 6.8
import App

ApplicationWindow {
    id: win
    width: 1200
    height: 760
    visible: true
    title: qsTr("OpenMeteo — click the map to load a temperature forecast")

    // Material style, colored from the Theme singleton. Set here on the root
    // window, these cascade to every child control; override per-control when
    // a specific element needs a different color.
    Material.theme: Theme.dark ? Material.Dark : Material.Light
    Material.primary: Theme.primary
    Material.accent: Theme.accent
    Material.foreground: Theme.foreground
    Material.background: Theme.background

    readonly property var unitCodes: ["C", "F", "K"]

    // Open-Meteo forecast API serves roughly the last 92 days .. +16 days ahead.
    readonly property date minSelectable: {
        var d = new Date();
        d.setDate(d.getDate() - 92);
        return d;
    }
    readonly property date maxSelectable: {
        var d = new Date();
        d.setDate(d.getDate() + 16);
        return d;
    }

    function log(msg) {
        if (appVerbose)
            console.log("[qml] " + msg);
    }

    WeatherService {
        id: weather
        onDataChanged: win.rebuildChart()
        onErrorOccurred: function (msg) {
            status.text = "Error: " + msg;
            win.log("error: " + msg);
        }
    }

    // Re-fetch using the current map marker and the selected date range.
    function loadData() {
        if (!map.hasMarker) {
            status.text = "Click a point on the map first";
            return;
        }
        win.log("fetch lat=" + map.markerLat.toFixed(4) + " lon=" + map.markerLon.toFixed(4)
                + " from=" + fromField.iso + " till=" + tillField.iso);
        weather.fetch(map.markerLat, map.markerLon, fromField.iso, tillField.iso);
    }

    function fillSeries(s) {
        s.clear();
        for (var i = 0; i < weather.count; ++i)
            s.append(weather.timeMsAt(i), weather.temperatureAt(i));
    }

    function rebuildChart() {
        fillSeries(lineSeries);
        fillSeries(splineSeries);
        fillSeries(scatterSeries);

        if (weather.count > 0) {
            axisX.min = new Date(weather.timeMsAt(0));
            axisX.max = new Date(weather.timeMsAt(weather.count - 1));
            axisY.min = weather.minTemp - 1;
            axisY.max = weather.maxTemp + 1;
            status.text = weather.locationLabel + "   —   " + weather.count + " hourly points";
        }
        axisY.titleText = weather.unitSuffix;
        chart.title = "Temperature (" + weather.unitSuffix + ")";

        win.log("rebuildChart: count=" + weather.count + " unit=" + weather.unit
                + " yMin=" + axisY.min.toFixed(1) + " yMax=" + axisY.max.toFixed(1));
    }

    function showTip(point, state, series) {
        if (state) {
            var d = new Date(point.x);
            tipLabel.text = Qt.formatDateTime(d, "ddd MMM d, HH:mm") + "    "
                    + point.y.toFixed(1) + " " + weather.unitSuffix;
            var pos = chart.mapToPosition(point, series);
            tip.x = Math.max(0, Math.min(pos.x + 14, chart.width - tip.width));
            tip.y = Math.max(0, pos.y - tip.height - 10);
            tip.visible = true;
        } else {
            tip.visible = false;
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            spacing: 12

            Label {
                text: "From"
            }
            DateField {
                id: fromField
                selected: new Date()
                minDate: win.minSelectable
                maxDate: win.maxSelectable
            }

            Label {
                text: "Till"
            }
            DateField {
                id: tillField
                minDate: win.minSelectable
                maxDate: win.maxSelectable
                selected: {
                    var d = new Date();
                    d.setDate(d.getDate() + 7);
                    return d;
                }
            }

            ToolSeparator {}

            Label {
                text: "Units"
            }
            ComboBox {
                id: unitBox
                model: ["°C", "°F", "K"]
                implicitWidth: 90
                onActivated: weather.unit = win.unitCodes[currentIndex]
            }

            Label {
                text: "Graph"
            }
            ComboBox {
                id: graphBox
                model: ["Line", "Spline", "Scatter"]
                implicitWidth: 110
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "Apply range"
                onClicked: win.loadData()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 1

        TileMap {
            id: map
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: win.width * 0.55
            onMapClicked: function (lat, lon) {
                win.loadData();
            }
        }

        ChartView {
            id: chart
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: win.width * 0.45
            title: "Temperature (°C)"
            antialiasing: true
            legend.visible: false

            DateTimeAxis {
                id: axisX
                format: "MMM d, HH:mm"
                tickCount: 7
                labelsAngle: -45
            }
            ValueAxis {
                id: axisY
                titleText: "°C"
                min: -10
                max: 40
            }

            // Three series share the axes; only the selected one is shown, so
            // switching graph type is instant (no refetch / repopulate).
            LineSeries {
                id: lineSeries
                name: "Temperature"
                axisX: axisX
                axisY: axisY
                color: Theme.highlight
                width: 2
                visible: graphBox.currentIndex === 0
                onHovered: function (point, state) {
                    win.showTip(point, state, lineSeries);
                }
            }
            SplineSeries {
                id: splineSeries
                name: "Temperature"
                axisX: axisX
                axisY: axisY
                color: Theme.highlight
                width: 2
                visible: graphBox.currentIndex === 1
                onHovered: function (point, state) {
                    win.showTip(point, state, splineSeries);
                }
            }
            ScatterSeries {
                id: scatterSeries
                name: "Temperature"
                axisX: axisX
                axisY: axisY
                color: Theme.highlight
                markerSize: 8
                borderColor: Theme.highlight
                visible: graphBox.currentIndex === 2
                onHovered: function (point, state) {
                    win.showTip(point, state, scatterSeries);
                }
            }

            // Hover tooltip
            Rectangle {
                id: tip
                visible: false
                z: 50
                radius: 4
                color: Theme.tooltipBg
                opacity: 0.93
                width: tipLabel.implicitWidth + 14
                height: tipLabel.implicitHeight + 8
                Label {
                    id: tipLabel
                    anchors.centerIn: parent
                    color: Theme.tooltipText
                    font.pixelSize: 12
                }
            }
        }
    }

    footer: Label {
        id: status
        text: "Click anywhere on the map to load its weather"
        padding: 8
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: weather.loading
        visible: weather.loading
    }
}
