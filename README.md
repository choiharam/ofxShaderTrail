ofxShaderTrail
=====================================


Introduction
------------
**ofxShaderTrail** is a simple header-only addon to be used with [openframeworks](https://openframeworks.cc). It uses two discrete fbos in combination to ping-pong between them and will store/overlap the color from previous frames on top of the current frame. As result, it creates a so-called trail effect.

The example shows a white circle moving around the screen with the trail effect applied.

By default, the mix-ratio between the current and the previous frames is set to `0.044`. But it can be changed by calling `.setAmount(float)`.

Licence
-------
Distributed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License), see the file 'license.md'.

Installation
------------
clone this repo into your `<OF_ROOT>/addons/`.

Dependencies
------------
None

Compatibility
------------
- Created with openFrameworks 0.11.2.

Known issues
------------


Version history
------------

- Version 0.1 (2023.Feb.05):

	First Release.


