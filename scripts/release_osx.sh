#!/bin/sh

set -e

QTDIR=~/Qt/5.8.0-desktop

if [ -z "$1" ]
then
  echo "No version specified in cmdline!"
  exit 1
fi

VERSION=$1-`git rev-parse --short HEAD`

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/..
BUILD="$ROOT"/build
TARGET=rclone-browser-$VERSION-osx
APP="$TARGET"/"Rclone Browser.app"

rm -rf "$BUILD"
mkdir -p "$BUILD"
cd "$BUILD"
cmake .. -DCMAKE_PREFIX_PATH="$QTDIR" -DCMAKE_BUILD_TYPE=Release -DRCLONE_BROWSER_VERSION=$VERSION
make -j2
cd ..

rm -rf "$TARGET"
mkdir "$TARGET"
cp "$ROOT"/README.md "$TARGET"/Readme.txt
cp "$ROOT"/LICENSE "$TARGET"/License.txt
cp -R "$BUILD"/build/rclone-browser.app "$APP"
mv "$APP"/Contents/MacOS/rclone-browser "$APP"/Contents/MacOS/"Rclone Browser"

sed -i .bak 's/rclone-browser/Rclone Browser/g' "$APP"/Contents/Info.plist
rm "$APP"/Contents/*.bak

FRAMEWORKS=("Core" "Gui" "Widgets" "PrintSupport" "MacExtras")

mkdir "$APP"/Contents/Frameworks
for FX in "${FRAMEWORKS[@]}"
do
  cp -R "$QTDIR"/lib/Qt${FX}.framework "$APP"/Contents/Frameworks/
  FXPATH="$APP"/Contents/Frameworks/Qt${FX}.framework
  install_name_tool -id @executable_path/../Frameworks/Qt${FX}.framework/Versions/5/Qt${FX} "${FXPATH}"/Versions/5/Qt${FX}
  rm -rf "$FXPATH"/Headers
  rm -rf "$FXPATH"/*.prl
  rm -rf "$FXPATH"/*_debug
  rm -rf "$FXPATH"/Versions/5/Headers
  rm -rf "$FXPATH"/Versions/5/*_debug
done

mkdir -p "$APP"/Contents/Plugins/platforms
cp "$QTDIR"/plugins/platforms/libqcocoa.dylib "$APP"/Contents/Plugins/platforms

change()
{
  for x in $2
  do
    install_name_tool -change @rpath/Qt$x.framework/Versions/5/Qt$x @executable_path/../Frameworks/Qt$x.framework/Versions/5/Qt$x "$1"
  done
}

change "$APP"/Contents/MacOS/"Rclone Browser" "Core Gui Widgets MacExtras"
change "$APP"/Contents/Frameworks/QtGui.framework/QtGui "Core"
change "$APP"/Contents/Frameworks/QtWidgets.framework/QtWidgets "Core Gui"
change "$APP"/Contents/Frameworks/QtMacExtras.framework/QtMacExtras "Core Gui Widgets"
change "$APP"/Contents/Frameworks/QtPrintSupport.framework/QtPrintSupport "Core Gui Widgets"
change "$APP"/Contents/Plugins/platforms/libqcocoa.dylib "Core Gui Widgets PrintSupport"

zip -q -r9 "$TARGET".zip "$TARGET"

