#!/bin/bash
if (( $EUID != 0 )); then
    echo "וִיבַקֵּשׁ ה' לְהָרִץ אֶת־הַסְּקְרִיפְּט בְּסַמְכוּת שֹׁרֶשׁ"
    exit 1
fi

CODESH_PATH="/usr/lib/קודש"

rm -f /usr/local/bin/codeshc
rm -rf $CODESH_PATH
rm -rf ./build

echo "וְיִגְעַר ה' בַּיּוֹצֵר כִּי־בָּחַר בַּחֹשֶׁךְ וְלֹא בָאוֹר וַיִּתְעַצֵּב מְאֹד מְאֹד וַיִּתֹּם:"
