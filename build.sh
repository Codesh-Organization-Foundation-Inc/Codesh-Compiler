if (( EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' כִּי יָרִיץ אֶת־הַסְּקָרִיפְּט כְּרוּת"
    exit 1
fi

CODESH_PATH=/usr/lib/codesh
mkdir -p $CODESH_PATH

#TODO: Install Codesh in /usr/lib/codeshc and put it in PATH
CODESH_EXECUTABLE=./cmake-build-debug/codeshc


#TODO: Compress Talmud Codesh into JAR file after JAR loading is implemented

TALMUD_CODESH_PATH=$CODESH_PATH/talmud-codesh

mkdir -p $TALMUD_CODESH_PATH
$CODESH_EXECUTABLE ./resources/lib-src/ $TALMUD_CODESH_PATH --sinful

echo "וְיִשְׂמַח ה' כִּי־הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וִיאַחֵל לְיוֹצֵר קִדּוּד נָעִים וַיִּתֹּם"