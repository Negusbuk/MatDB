#!/bin/bash

source=MatDB
title=MatDB

qmake
make clean
rm -r MatDB.app
make
iconutil --convert icns MatDB.iconset
macdeployqt MatDB.app

rm -rf MatDB
rm -f MatDB.dmg
rm -f temp.dmg

mkdir MatDB
cp -r MatDB.app MatDB
mkdir MatDB/.background
cp MatDBBG.png MatDB/.background/MatDBBG.png

hdiutil create -srcfolder "${source}" -volname "${title}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size 50M temp.dmg
device=$(hdiutil attach -readwrite -noverify -noautoopen "temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

echo '
   tell application "Finder"
     tell disk "'${title}'"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {200, 100, 680, 350}
           set theViewOptions to the icon view options of container window 
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 128
           set background picture of theViewOptions to file ".background:MatDBBG.png"
           make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
           set position of item "MatDB.app" of container window to {100, 100}
           set position of item "Applications" of container window to {380, 100}
           close
           open
           set the bounds of container window to {200, 100, 680, 350}
           update without registering applications
           delay 5
           eject
     end tell
   end tell
' | osascript

hdiutil detach ${device}
hdiutil convert "temp.dmg" -format UDZO -imagekey zlib-level=9 -o "${title}.dmg"
rm -f temp.dmg
rm -rf MatDB

