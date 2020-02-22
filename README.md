# atsamd21-poe-dmx-controller
This project is a DMX controller that generates RGB light patterns. It has a browser-based user interface for controlling the patterns.

Two hardware platforms are supported:

For quick prototyping, the hardware can be built using a Particle Ethernet Feather Wing, an Adafruit Feather M0 Basic Proto, and the DMX Feather Wing from the boards directory in this repository. These boards all plug together with no additional wiring required.

For more permanent installations, the hardware can be built using a custom board, surface mount components, and a Hammond 1455C801 extruded aluminum enclosure.

The software runs on both platforms without any modifications. The effects.ino file can be modified to add new effects to the controller. The html.ino and style.ino files can be modified to update the user interface.
