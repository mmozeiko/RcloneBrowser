**WARNING: This project is not longer active or maintaned.**

**Initially I created it only because rclone mount did not work in the beginning. Now mount on Windows works fine, so this project is not useful for me anymore.**

**I suggest to try out rclone built-in [web based GUI](https://rclone.org/gui/) instead**

RcloneBrowser
=============

[![Travis CI Build Status][img1]][1] [![AppVeyor Build Status][img2]][2] [![Downloads][img3]][3] [![Release][img4]][4] [![License][img5]][5]

Simple cross platfrom GUI for rclone command line tool.
Supports Windows, macOS and GNU/Linux.

Features
--------

* Allows to browse and modify any rclone remote, including encrypted ones
* Uses same configuration file as rclone, no extra configuration required
* Supports custom location and encryption for `.rclone.conf` configuration file
* Simultaneously navigate multiple repositories in separate tabs
* Lists files hierarchically with file name, size and modify date
* All rclone commands are executed asynchronously, no freezing GUI
* File hierarchy is lazily cached in memory, for faster traversal of folders
* Allows to upload, download, create new folders, rename or delete files and folders
* Allows to calculate size of folder, export list of files and copy rclone copmmand to clipboard
* Can process multiple upload or download jobs in background
* Drag & drop support for dragging files from local file explorer for uploading
* Streaming media files for playback in player like [mpv][6] or similar
* Mount and unmount folders on macOS and GNU/Linux
* Optionally minimizes to tray, with notifications when upload/download finishes
* Supports portable mode (create .ini file next to executable with same name), rclone and .rclone.conf path now can be relative to executable

Download
--------

Get Windows, macOS and Ubuntu package on [releases][3] page.

For Ubuntu you can also install it from Launchpad: [Rclone Browser][launchpad].

ArchLinux users can install latest release from AUR repository: [rclone-browser][7].

A Debian package is available here until it lands in official: http://phd-sid.ethz.ch/debian/rclone-browser/

Other GNU/Linux users will need to build from source.

Screenshots
-----------

### Windows

![screenshot1.png][screenshot1]
![screenshot2.png][screenshot2]
![screenshot3.png][screenshot3]
![screenshot4.png][screenshot4]

### Ubuntu

![screenshot5.png][screenshot5]

### macOS

![screenshot6.png][screenshot6]

Build instructions for Windows
------------------------------

1. Get [Visual Studio 2013][8]
2. Install [CMake][9]
3. Install or build from source Qt v5 (64-bit) from [Qt website][10]
4. Set `QTDIR` environment variable to Qt installation, or adjust path to Qt in `bootstrap.cmd` file
5. Run `bootstrap.cmd`, it will generate Visual Studio 2013 solution in `build` folder

Build instructions for GNU/Linux and macOS
------------------------------------------

1. Make sure you have working compiler and [cmake][9] installed
2. Install Qt v5 with package manager or from [Qt website][10]
3. Create new `build` folder next to `src` folder
4. Run `cmake ..` from `build` folder to create makefile
   - if cmake doesn't find Qt, add `-DCMAKE_PREFIX_PATH=path/to/Qt` to previous command
5. Run `cmake --build .` from `build` folder to create binary

License
-------

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

[1]: https://travis-ci.org/mmozeiko/RcloneBrowser/
[2]: https://ci.appveyor.com/project/mmozeiko/RcloneBrowser
[3]: https://github.com/mmozeiko/RcloneBrowser/releases
[4]: https://github.com/mmozeiko/RcloneBrowser/releases/latest
[5]: https://github.com/mmozeiko/RcloneBrowser/blob/master/LICENSE
[6]: https://mpv.io/
[7]: https://aur.archlinux.org/packages/rclone-browser
[8]: https://www.visualstudio.com/en-us/news/releasenotes/vs2013-community-vs
[9]: http://www.cmake.org/
[10]: https://www.qt.io/download-open-source/
[img1]: https://api.travis-ci.org/mmozeiko/RcloneBrowser.svg?branch=master
[img2]: https://ci.appveyor.com/api/projects/status/7s24ixolrk3ueggm/branch/master?svg=true
[img3]: https://img.shields.io/github/downloads/mmozeiko/RcloneBrowser/total.svg?maxAge=3600
[img4]: https://img.shields.io/github/release/mmozeiko/RcloneBrowser.svg?maxAge=3600
[img5]: https://img.shields.io/github/license/mmozeiko/RcloneBrowser.svg?maxAge=2592000
[screenshot1]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot1.png
[screenshot2]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot2.png
[screenshot3]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot3.png
[screenshot4]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot4.png
[screenshot5]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot5.png
[screenshot6]: https://raw.githubusercontent.com/wiki/mmozeiko/RcloneBrowser/screenshot6.png
[launchpad]: https://launchpad.net/~mmozeiko/+archive/ubuntu/rclone-browser
