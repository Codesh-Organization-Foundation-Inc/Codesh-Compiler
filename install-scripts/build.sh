#!/bin/bash
set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <out_dir>"
    exit 1
fi

OUT_DIR="$1"

# Compile codeshc
mkdir -p "$OUT_DIR"
cmake -B "$OUT_DIR" -S . -DCMAKE_BUILD_TYPE=Release
cmake --build "$OUT_DIR"

# Build Talmud Codesh as JAR
# --unholy because we are MAKING the Talmud Codesh and do not rely on it
"$OUT_DIR/codeshc" --src ./resources/lib-src/ --dest "$OUT_DIR/תלמוד־קודש.jar" --sinful --unholy

echo "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
