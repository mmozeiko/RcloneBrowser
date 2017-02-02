@echo off
setlocal enabledelayedexpansion

if "%1" == "" (
  echo No architecture x86 or x64 specified in cmdline!
  goto :eof
)

if "%2" == "" (
  echo No version specified in cmdline!
  goto :eof
)


set ARCH=%1
set VERSION=%2

where /q git.exe
if "%ERRORLEVEL%" equ "0" (
  for /f "tokens=*" %%t in ('git.exe rev-parse --short HEAD') do (
    set COMMIT=%%t
  )
  set VERSION=%VERSION%-!COMMIT!
)

set QT=C:\Qt\5.8.0-desktop-vs2013-%ARCH%
set PATH=%QT%\bin;%PATH%

call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" %ARCH%

set ROOT="%~dp0.."
set BUILD="%~dp0..\build\build\Release"
if "%ARCH%" == "x86" (
  set TARGET="%~dp0rclone-browser-%VERSION%-win32"
  set CMAKEGEN="Visual Studio 12"
) else (
  set TARGET="%~dp0rclone-browser-%VERSION%-win64"
  set CMAKEGEN="Visual Studio 12 Win64"
)

pushd "%ROOT%"
if exist build rd /s /q build
mkdir build
cd build

echo cmake .. -G %CMAKEGEN% -DCMAKE_CONFIGURATION_TYPES="Release" -DRCLONE_BROWSER_VERSION=%VERSION%
cmake .. -G %CMAKEGEN% -DCMAKE_CONFIGURATION_TYPES="Release" -DRCLONE_BROWSER_VERSION=%VERSION%
cmake --build . --config Release
popd

mkdir "%TARGET%" 2>nul

copy "%ROOT%\README.md" "%TARGET%\Readme.txt"
copy "%ROOT%\CHANGELOG.md" "%TARGET%\Changelog.txt"
copy "%ROOT%\LICENSE" "%TARGET%\License.txt"
copy "%BUILD%\RcloneBrowser.exe" "%TARGET%"

windeployqt.exe --no-translations --no-compiler-runtime --no-svg "%TARGET%\RcloneBrowser.exe"
rd /s /q "%TARGET%\imageformats"

copy "%VS120COMNTOOLS%..\..\VC\redist\%ARCH%\Microsoft.VC120.CRT\msvcp120.dll" "%TARGET%"
copy "%VS120COMNTOOLS%..\..\VC\redist\%ARCH%\Microsoft.VC120.CRT\msvcr120.dll" "%TARGET%"

(
echo [Paths]
echo Prefix = .
echo LibraryExecutables = .
echo Plugins = .
)>"%TARGET%\qt.conf"
7za.exe a -mx=9 -r -tzip "%TARGET%.zip" "%TARGET%"
rd /s /q "%TARGET%"
