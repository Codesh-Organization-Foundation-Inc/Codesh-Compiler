#!/bin/bash
set -e

if (( EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' לְהָרִץ אֶת־הַסְּקְרִיפְּט בְּסַמְכוּת שֹׁרֶשׁ"
    exit 1
fi

# sudo strips environment.
# Recover VCPKG_ROOT from the invoking user's home
if [ -z "$VCPKG_ROOT" ]; then
    VCPKG_ROOT="/home/$SUDO_USER/vcpkg"
fi
export VCPKG_ROOT

CODESH_PATH="/usr/lib/קודש"

"$(dirname "$0")/build.sh" "$CODESH_PATH"

ln -sf "$CODESH_PATH/codeshc" /usr/local/bin/codeshc
