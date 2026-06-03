pragma Singleton

import QtQuick 6.8

// Single source of truth for app colors. The control-facing values are fed
// into the Material style on the root window (see Main.qml); the rest cover
// painting that the style does not reach (chart, map, calendar, tooltip).
QtObject {
    // --- Material theme selection ---
    readonly property bool dark: false

    // --- control theming (consumed by Material.* on the window) ---
    readonly property color primary:    "#1565c0"  // app/primary surfaces
    readonly property color accent:     "#1565c0"  // highlights, focus, indicators
    readonly property color foreground: "#10243a"  // primary text/icon color
    readonly property color background: "#ffffff"  // control backgrounds

    // --- shared accent for data, marker and selection ---
    readonly property color highlight:     "#e53935"  // chart series, map marker, selected day
    readonly property color highlightText: "white"    // text/border drawn over `highlight`

    // --- map ---
    readonly property color mapWater:           "#a9d3ff"  // ocean shown while tiles load
    readonly property color attribution:        "#333333"  // OSM attribution text
    readonly property color attributionOutline: "#ffffff"  // its outline, for contrast

    // --- calendar day text ---
    readonly property color dayTextDisabled: "#c2c2c2"  // out-of-range / adjacent month

    // --- tooltip ---
    readonly property color tooltipBg:   "#333333"
    readonly property color tooltipText: "white"
}
