#!/bin/bash
set -e

if (( EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' לְהָרִץ אֶת־הַסְּקְרִיפְּט בְּסַמְכוּת שֹׁרֶשׁ"
    exit 1
fi

if [ $# -lt 1 ]; then
    echo "Usage: $0 <build_dir>"
    echo "  build_dir: Output directory from build.sh"
    exit 1
fi

BUILD_DIR="$1"

if [ ! -f "$BUILD_DIR/codeshc" ]; then
    echo "codeshc not found in $BUILD_DIR"
    echo "Run build.sh first"
    exit 1
fi

CODESH_PATH="/usr/lib/קודש"

mkdir -p "$CODESH_PATH"
cp -r "$BUILD_DIR"/. "$CODESH_PATH/"
rm -f "$CODESH_PATH/install-global.sh" "$CODESH_PATH/install-global.ps1"

ln -sf "$CODESH_PATH/codeshc" /usr/local/bin/codeshc

echo "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
