@echo off

if "%QTDIR%" EQU "" (
  set QT=C:\Qt\5.8.0-desktop-vs2013-x64
) else (
  set QT=%QTDIR%
)
set PATH=%QT%\bin;%PATH%

set _IsNativeEnvironment=true
call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x64

set BUILD="%~dp0build"

mkdir "%BUILD%" 2>nul
pushd "%BUILD%"

cmake -G "Visual Studio 12 Win64" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ..
if %ERRORLEVEL% neq 0 (
  popd
  exit /b 1
)
devenv /useenv rclone-browser.sln

popd
