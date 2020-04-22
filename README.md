Liri Text
=========

[![License](https://img.shields.io/github/license/lirios/text.svg)](https://github.com/lirios/text/blob/develop/LICENSE)
[![GitHub release](https://img.shields.io/github/release/lirios/text.svg)](https://github.com/lirios/text/releases)
[![CI](https://github.com/lirios/text/workflows/CI/badge.svg?branch=develop&event=push)](https://github.com/lirios/text/actions?query=workflow%3ACI)
[![Snap Status](https://build.snapcraft.io/badge/lirios/text.svg)](https://build.snapcraft.io/user/lirios/text)
[![GitHub issues](https://img.shields.io/github/issues/lirios/text.svg)](https://github.com/lirios/text/issues)

Liri Text is a cross-platform text editor made in accordance with Material Design.

![Screenshot](https://raw.githubusercontent.com/lirios/text/develop/.project/screenshots/text1.png)

## Dependencies

Qt >= 5.10.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

The following modules and their dependencies are required:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [fluid](https://github.com/lirios/fluid.git) >= 1.0.0
 * [SQLite](https://www.sqlite.org/) >= 3.7.15

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

You can also append the following options to the `cmake` command:

 * `-DTEXT_WITH_FLUID:BOOL=ON`: Build with a local copy of the Fluid sources.

## Credits

Many thanks to ...

 * [Corbin Crutchley](https://github.com/crutchcorn) for creating the application icon
 * Everyone who contributed to the project directly via Git, listed in [AUTHORS.md](AUTHORS.md)

## License

This application is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
