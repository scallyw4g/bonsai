# Hotkeys and Controls

All hotkey bindings in Bonsai are customizable through the `hotkeys.init` file
in your project root. This file defines every keyboard shortcut that can be
customized in the engine.

NOTE(Jesse): There interactions which are hard-coded, and not discoverable.  I
intend on adding to the hotkey system over time as I flesh out the editor and
engine interactions.

### Stamp Basis

The stamp tool's Tools window has a Stamp Basis control. Its three values are
clamped to 0-1 across the combined bounds of the selected edits. The default
`(0, 0, 0)` places their minimum corner at the clicked point; `(0.5, 0.5, 0)`
centers them horizontally with their bottom at the clicked point.

### How to Customize Hotkeys

1. Open `hotkeys.init` in your text editor
2. Find the hotkey or chord you want to change
3. Modify the key assignment (e.g., change `hotkey Forward = W` to `hotkey Forward = Up`)
4. Save the file
5. Restart the engine to apply the changes
