import QtQuick 6.8
import QtQuick.Controls 6.8
import QtQuick.Layouts 6.8
import App

// A date field that opens a MonthGrid calendar popup. Days outside
// [minDate, maxDate] (and days bleeding in from adjacent months) are dimmed
// and non-selectable, so the user can only pick a date the API will accept.
Item {
    id: control

    property date selected: new Date()
    property date minDate: new Date(1970, 0, 1)
    property date maxDate: new Date(2100, 11, 31)
    readonly property string iso: Qt.formatDate(selected, "yyyy-MM-dd")

    implicitWidth: field.implicitWidth
    implicitHeight: field.implicitHeight

    // currently displayed calendar page
    property int pageMonth: selected.getMonth()
    property int pageYear: selected.getFullYear()

    function _mid(d) {
        return new Date(d.getFullYear(), d.getMonth(), d.getDate()).getTime();
    }
    function inRange(d) {
        return _mid(d) >= _mid(minDate) && _mid(d) <= _mid(maxDate);
    }
    function sameDay(a, b) {
        return _mid(a) === _mid(b);
    }
    function showMonthOf(d) {
        pageYear = d.getFullYear();
        pageMonth = d.getMonth();
    }
    function shiftMonth(delta) {
        showMonthOf(new Date(pageYear, pageMonth + delta, 1));
    }

    Button {
        id: field
        text: control.iso
        implicitWidth: 130
        onClicked: {
            control.showMonthOf(control.selected);
            popup.open();
        }
    }

    Popup {
        id: popup
        parent: field
        y: field.height + 4
        modal: true
        focus: true
        padding: 10

        ColumnLayout {
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                ToolButton {
                    text: "‹"
                    onClicked: control.shiftMonth(-1)
                }
                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    text: Qt.locale().standaloneMonthName(control.pageMonth) + " " + control.pageYear
                }
                ToolButton {
                    text: "›"
                    onClicked: control.shiftMonth(1)
                }
            }

            DayOfWeekRow {
                Layout.fillWidth: true
                locale: grid.locale
            }

            MonthGrid {
                id: grid
                Layout.fillWidth: true
                month: control.pageMonth
                year: control.pageYear
                locale: Qt.locale()

                delegate: Item {
                    id: cell
                    required property var model
                    implicitWidth: 36
                    implicitHeight: 30
                    readonly property bool selectable: model.month === grid.month
                                                       && control.inRange(model.date)
                    readonly property bool isSelected: cell.selectable
                                                       && control.sameDay(model.date, control.selected)

                    Rectangle {
                        anchors.centerIn: parent
                        width: 28
                        height: 26
                        radius: 4
                        visible: cell.isSelected
                        color: Theme.highlight
                    }
                    Text {
                        anchors.centerIn: parent
                        text: model.day
                        color: cell.isSelected ? Theme.highlightText
                                               : (cell.selectable ? Theme.foreground : Theme.dayTextDisabled)
                    }
                }

                onClicked: function (date) {
                    if (date.getMonth() === grid.month && control.inRange(date)) {
                        control.selected = date;
                        popup.close();
                    }
                }
            }
        }
    }
}
