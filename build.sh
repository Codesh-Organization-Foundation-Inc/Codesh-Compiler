if (( EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' כִּי יָרִיץ אֶת־הַסְּקָרִיפְּט כְּרוּת"
    exit 1
fi

CODESH_PATH="/usr/lib/קודש"
CODESH_EXECUTABLE=$CODESH_PATH/codeshc
TALMUD_CODESH_PATH="$CODESH_PATH/תלמוד־קודש"


# Compile codeshc
cmake --build ./build

# Deploy to CODESH_PATH
mkdir -p $CODESH_PATH
cmake --install ./build --prefix $CODESH_PATH

# Link to the compiler in bin
ln -sf $CODESH_EXECUTABLE /usr/local/bin/codeshc


#TODO: Compress Talmud Codesh into JAR file after JAR loading is implemented

mkdir -p $TALMUD_CODESH_PATH
$CODESH_EXECUTABLE ./resources/lib-src/ $TALMUD_CODESH_PATH --sinful

echo "וְיִשְׂמַח ה' כִּי־הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וִיאַחֵל לְיוֹצֵר קִדּוּד נָעִים וְיִתַּם"
