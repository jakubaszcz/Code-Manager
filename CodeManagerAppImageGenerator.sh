#!/usr/bin/env bash
set -euo pipefail
APP=CodeManager
VER=${CODEMANAGER_VERSION:-0.1.0}

# Build binaire Linux
make -j

# Construire AppDir
rm -rf AppDir
mkdir -p AppDir/usr/{bin,lib,share/applications,share/icons/hicolor/256x256/apps}
cp "$APP" AppDir/usr/bin/

# Desktop entry + icône (adaptez les chemins/icône)
cat > AppDir/${APP}.desktop <<EOF
[Desktop Entry]
Type=Application
Name=${APP}
Exec=${APP}
Icon=${APP}
Categories=Development;
EOF
# Facultatif: copier une icône 256x256
# cp assets/${APP}.png AppDir/usr/share/icons/hicolor/256x256/apps/${APP}.png
cp AppDir/usr/share/icons/hicolor/256x256/apps/${APP}.png AppDir/${APP}.png || true

# Optionnel: embarquer libs non-systèmes dans AppDir/usr/lib
# ldd $APP | awk '/=> \// {print $3}' | xargs -I{} cp -n {} AppDir/usr/lib/ || true

appimagetool AppDir ${APP}-${VER}-x86_64.AppImage
echo "AppImage générée: ${APP}-${VER}-x86_64.AppImage"
