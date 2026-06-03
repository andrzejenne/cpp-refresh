# QML Component Style Guide

Conventions for ordering and structuring members inside a QML component,
based on the official [Qt QML Coding Conventions](https://doc.qt.io/qt-6/qml-codingconventions.html)
and pragmatic refinements used in this project.

## Member ordering

Order members top-to-bottom inside an object block as follows:

1. **`id`** — always the first line.
2. **Public properties** — state a parent configures or observes.
3. **Signal declarations.**
4. **Internal / derived properties** — `readonly` and "private-ish" state,
   visually separated from the public block.
5. **This object's own configuration** — direct property assignments such as
   `clip`, `anchors`, `width`/`height`, grouped properties (`font`, `anchors`).
6. **JavaScript functions.**
7. **Signal handlers** (`onXxxChanged`, `Component.onCompleted`, …).
8. **Child objects** — visual children, `Repeater`, `MouseArea`, etc.,
   in back-to-front stacking order.
9. **States and transitions** — typically last.

## Principles

- **Public before private.** Properties a parent sets belong at the top.
  Push `readonly`/derived values into a clearly separated "internal" block so
  the public API is easy to find at a glance.
- **Keep functions together.** Don't interleave functions with properties or
  handlers — one contiguous block is easier to scan.
- **Group change handlers near the functions they call.** When several
  `onXxxChanged` handlers all call the same function (e.g. `refresh()`), keep
  them together right after that function.
- **Child objects in stacking order.** List visual children back-to-front so
  the source order matches the visual z-order (background first, overlays last).
- **Use section comments.** Lightweight headers like `// --- public API ---`
  make the structure obvious without tooling.

## Skeleton

```qml
import QtQuick

Item {
    id: root

    // --- public API ---
    property int  value: 0
    property string label: ""

    signal activated(int value)

    // --- internal / derived state ---
    readonly property int  doubled: value * 2
    property var cache: []

    // --- this object's own configuration ---
    clip: true
    anchors.fill: parent

    // --- functions ---
    function refresh() { cache = rebuild(); }
    function rebuild()  { /* ... */ }

    // --- signal handlers ---
    onValueChanged: refresh()
    Component.onCompleted: refresh()

    // --- child objects (back-to-front) ---
    Rectangle { /* background */ }
    Repeater  { /* content    */ }
    MouseArea { /* input       */ }
    Text      { /* overlay     */ }
}
```

## Notes

- The Qt convention for signal handler placement is loose. Two common styles:
  group all handlers in one block (used here), or place each handler next to
  the property it relates to. Pick one and stay consistent within a file.
- Prefer `readonly property` for derived values so their computed-only intent
  is enforced by the engine.
