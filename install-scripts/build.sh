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
CODESH_EXECUTABLE=$CODESH_PATH/codeshc


# Compile codeshc
cmake -B ./cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build ./cmake-build-release

# Deploy to CODESH_PATH
mkdir -p $CODESH_PATH
cp ./cmake-build-release/codeshc $CODESH_PATH/

# Link to the compiler in bin
ln -sf $CODESH_EXECUTABLE /usr/local/bin/codeshc


# Build Talmud Codesh as JAR
# --unholy because we are MAKING the Talmud Codesh and do not rely on it
$CODESH_EXECUTABLE --src ./resources/lib-src/ --dest $CODESH_PATH/תלמוד־קודש.jar --sinful --unholy

echo "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
