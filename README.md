RcloneBrowser
=============

[![Travis CI Build Status](https://api.travis-ci.org/mmozeiko/RcloneBrowser.svg?branch=master)](https://travis-ci.org/mmozeiko/RcloneBrowser/)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/7s24ixolrk3ueggm/branch/master?svg=true)](https://ci.appveyor.com/project/mmozeiko/RcloneBrowser)
[![Downloads](https://img.shields.io/github/downloads/mmozeiko/RcloneBrowser/total.svg?maxAge=86400)](https://github.com/mmozeiko/RcloneBrowser/releases)
[![Release](https://img.shields.io/github/release/mmozeiko/RcloneBrowser.svg?maxAge=86400)](https://github.com/mmozeiko/RcloneBrowser/releases/latest)
[![License](https://img.shields.io/github/license/mmozeiko/RcloneBrowser.svg?maxAge=2592000)](https://github.com/mmozeiko/RcloneBrowser/blob/master/LICENSE)

Simple cross platfrom GUI for rclone command line tool.
Supports Windows, OSX and GNU/Linux.

Features
--------

* Allows to browse and modify any rclone remote, including encrypted ones
* Uses same configuration file as rclone, no extra configuration required
* Simultaneously navigate multiple repositories in separate tabs
* Lists files hierarchically with file name, size and modify date
* All rclone commands are executed asynchronously, no freezing GUI
* File hierarchy is lazily cached in memory, for faster traversal of folders
* Allows to upload, download, create new folders, rename or delete files and folders
* Can process multiple upload or download jobs in background
* Drag & drop support for dragging files from local file explorer for uploading
* Streaming media files for playback in player like [mpv](https://mpv.io/) or similar
* Mount and unmount folders on OSX and GNU/Linux
* Optionally minimizes to tray, with notifications when upload/download finishes

Download
--------

Get 64-bit Windows and OSX binary on [releases](https://github.com/mmozeiko/RcloneBrowser/releases) page.
GNU/Linux users will need to build from source. ArchLinux users can install latest release from AUR repository [rclone-browser](https://aur.archlinux.org/packages/rclone-browser).

Screenshots
-----------

![screenshot1.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot1.png)
![screenshot2.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot2.png)
![screenshot3.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot3.png)
![screenshot4.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot4.png)
![screenshot5.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot5.png)
![screenshot6.png](https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot6.png)

Build instructions for Windows
------------------------------

1. Get Visual Studio 2013 - https://www.visualstudio.com/en-us/news/releasenotes/vs2013-community-vs
2. Install CMake - http://www.cmake.org/
3. Install or build from source Qt v5 (64-bit) - https://www.qt.io/download-open-source/
4. Set `QTDIR` environment variable to Qt installation, or adjust path to Qt in `bootstrap.cmd` file
5. Run `bootstrap.cmd`, it will generate Visual Studio 2013 solution in `build` folder

Build instructions for GNU/Linux and OSX
----------------------------------------

1. Make sure you have working compiler and cmake installed
2. Install Qt v5 with package manager or from Qt website - https://www.qt.io/download-open-source/
3. Create new `build` folder next to `src` folder
4. Run `cmake ..` from `build` folder to create makefile
5. Run `cmake --build .` from `build` folder to create binary

License
---------

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.
