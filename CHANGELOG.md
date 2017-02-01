# Change Log

## [1.1] - 2017-01-31
- Added `--transfer` option in UI, issue #1
- Supports encrypted `.rclone.conf` configuration file, issue #2
- Fixed crash when canceling active stream
- Added ETA tooltip for transfer progress bars
- Allow to specify extra arguments for rclone, issue #7
- Fix for browsing Hubic remotes, issue #10
- Support high-dpi mode for macOS

## [1.0.0] - 2017-01-29
- Allows to browse and modify any rclone remote, including encrypted ones
- Uses same configuration file as rclone, no extra configuration required
- Simultaneously navigate multiple repositories in separate tabs
- Lists files hierarchically with file name, size and modify date
- All rclone commands are executed asynchronously, no freezing GUI
- File hierarchy is lazily cached in memory, for faster traversal of folders
- Allows to upload, download, create new folders, rename or delete files and folders
- Can process multiple upload or download jobs in background
- Drag & drop support for dragging files from local file explorer for uploading
- Streaming media files for playback in player like mpv or similar
- Mount and unmount folders on macOS and GNU/Linux
- Optionally minimizes to tray, with notifications when upload/download finishes

[1.1]: https://github.com/mmozeiko/RcloneBrowser/releases/tag/1.1
[1.0.0]: https://github.com/mmozeiko/RcloneBrowser/releases/tag/1.0.0
