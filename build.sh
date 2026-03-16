#!/bin/bash

#TODO: Replace with build-jar once compiler support for JARs exist

if (( EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' לְהָרִץ אֶת־הַסְּקְרִיפְּט בְּסַמְכוּת שֹׁרֶשׁ"
    exit 1
fi

CODESH_PATH="/usr/lib/קודש"
CODESH_EXECUTABLE=$CODESH_PATH/codeshc
TALMUD_CODESH_PATH="$CODESH_PATH/תלמוד־קודש"


# Compile codeshc
cmake -B ./cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build ./cmake-build-release

# Deploy to CODESH_PATH
mkdir -p $CODESH_PATH
cp ./cmake-build-release/codeshc $CODESH_PATH/

# Link to the compiler in bin
ln -sf $CODESH_EXECUTABLE /usr/local/bin/codeshc


# Build Talmud Codesh
mkdir -p $TALMUD_CODESH_PATH
$CODESH_EXECUTABLE --src ./resources/lib-src/ --dest $TALMUD_CODESH_PATH --sinful

echo "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
