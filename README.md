Liri Text
=========

[![license](https://img.shields.io/github/license/lirios/text.svg)](https://github.com/lirios/text/blob/develop/LICENSE)
[![GitHub release](https://img.shields.io/github/release/lirios/text.svg)](https://github.com/lirios/text/releases)
[![GitHub issues](https://img.shields.io/github/issues/lirios/text.svg)](https://github.com/lirios/text/issues)
[![Maintenance](https://img.shields.io/maintenance/yes/2017.svg)](https://github.com/lirios/text/commits/develop)

Liri Text is a cross-platform text editor made in accordance with Material Design.

## Dependencies
* Qt >= 5.8.0 with at least the following modules is required:
 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
* [Fluid](https://github.com/lirios/fluid) from develop branch
* [SQLite](https://www.sqlite.org/) >= 3.7.15

## Build
  ```sh
  git clone --recursive https://github.com/lirios/text.git
  cd text
  mkdir build; cd build
  qmake ..
  make
  make install   # optional; use sudo if needed
  ```

## License
This application is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
