@echo off

if "%QTDIR%" EQU "" (
  set QT=C:\Qt\5.10.0\msvc2017_64
) else (
  set QT=%QTDIR%
)
set Qt5Core_DIR=%QT%
set Qt5Gui_DIR=%QT%
set Qt5Widgets_DIR=%QT%
set Qt5WinExtras_DIR=%QT%
set PATH=%QT%\bin;%PATH%

set _IsNativeEnvironment=true
call "%VS150COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x64

set BUILD="%~dp0build"

mkdir "%BUILD%" 2>nul
pushd "%BUILD%"

cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ..
if %ERRORLEVEL% neq 0 (
  popd
  exit /b 1
)
devenv /useenv rclone-browser.sln

popd
