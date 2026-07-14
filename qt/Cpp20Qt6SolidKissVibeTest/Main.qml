import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Weather

ApplicationWindow {
    id: root
    width: 420
    height: 560
    visible: true
    title: qsTr("Weather")

    // Display concern lives in the view: map WMO codes to text/emoji.
    function describe(code) {
        if (code === 0) return qsTr("Clear");
        if (code <= 3) return qsTr("Partly cloudy");
        if (code <= 48) return qsTr("Fog");
        if (code <= 67) return qsTr("Rain");
        if (code <= 77) return qsTr("Snow");
        if (code <= 82) return qsTr("Showers");
        return qsTr("Thunderstorm");
    }
    function glyph(code) {
        if (code === 0) return "☀️";
        if (code <= 3) return "⛅";
        if (code <= 48) return "🌫️";
        if (code <= 67) return "🌧️";
        if (code <= 77) return "❄️";
        if (code <= 82) return "🌦️";
        return "⛈️";
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: cityField
                Layout.fillWidth: true
                placeholderText: qsTr("Search a city…")
                enabled: !WeatherController.busy
                onAccepted: WeatherController.search(text)
            }
            Button {
                text: qsTr("Go")
                enabled: !WeatherController.busy && cityField.text.trim().length > 0
                onClicked: WeatherController.search(cityField.text)
            }
        }

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            running: WeatherController.busy
            visible: running
        }

        Label {
            Layout.fillWidth: true
            visible: WeatherController.errorString.length > 0
            text: WeatherController.errorString
            color: "#c0392b"
            wrapMode: Text.WordWrap
        }

        // Current conditions — declarative bindings; no imperative updates.
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            visible: WeatherController.city.length > 0 && !WeatherController.busy

            Label {
                text: WeatherController.city
                font.pixelSize: 24
                font.bold: true
            }
            Label {
                text: root.glyph(WeatherController.weatherCode) + " "
                      + Math.round(WeatherController.temperature) + "°C · "
                      + root.describe(WeatherController.weatherCode)
                font.pixelSize: 18
                opacity: 0.85
            }
        }

        // Forecast — a real model with role-based delegates.
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 4
            model: WeatherController.forecast

            delegate: ItemDelegate {
                required property string day
                required property double high
                required property double low
                required property int code

                width: ListView.view.width
                contentItem: RowLayout {
                    spacing: 12
                    Label { text: day; Layout.preferredWidth: 44 }
                    Label { text: root.glyph(code); Layout.preferredWidth: 28 }
                    Label {
                        text: root.describe(code)
                        Layout.fillWidth: true
                        opacity: 0.85
                    }
                    Label {
                        text: Math.round(high) + "° / " + Math.round(low) + "°"
                        font.bold: true
                    }
                }
            }
        }
    }
}
