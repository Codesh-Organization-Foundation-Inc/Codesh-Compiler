#include "blasphemy_details.h"

// INIT
const std::string codesh::error::blasphemy_details::CANT_OPEN_SOURCE_FILE = "לֹא נִתַּן לִפְתֹּחַ אֶת קּוֹבֶץ הַמּוֹצָא";
const std::string codesh::error::blasphemy_details::CANT_OPEN_FILE = "לֹא נִתַּן לִפְתֹּחַ אֶת הַקּוֹבֶץ: ";
const std::string codesh::error::blasphemy_details::NO_MAIN_ARGS = "מִנְחוֹת אֵינָם: נְתִיבֵי מוֹצָא וְיַעַד נִּדְרָשִׁים";

// LEXICAL
const std::string codesh::error::blasphemy_details::NO_BASAD = "בַּסַּ\"ד אֵינוֹ";
const std::string codesh::error::blasphemy_details::TOKEN_DOESNT_EXIST = "מִלָּה אֵינָהּ קַיֶּמֶת אוֹ שְׁאוּלָה מִלּוֹעֲזִית";

// SYNTAX
const std::string codesh::error::blasphemy_details::NO_IDENTIFIER = "נָבוֹא שְׁקָרַי: צֻּפָּה מִלָּה חָפְשִׁית";
const std::string codesh::error::blasphemy_details::NO_IMPORT_TYPE = "נָבוֹא שְׁקָרַי: צֻּפָּה סוּג יְבוּא";
const std::string codesh::error::blasphemy_details::NO_KEYWORD_LET = "נָבוֹא שְׁקָרַי: צֻּפָּה ויהי";
const std::string codesh::error::blasphemy_details::NO_KEYWORD_NAME = "נָבוֹא שְׁקָרַי: צֻּפָּה ושמו";
const std::string codesh::error::blasphemy_details::NO_CLOSE_MULTI_COMMENT = "נָבוֹא שְׁקָרַי: צֻּפָּה סְגִירַת הָעֵרָה מְרוּבַּת־פְּסוּקִים";
const std::string codesh::error::blasphemy_details::NO_SCOPE_BEGIN = "נָבוֹא שְׁקָרַי: צֻּפָּה ויאמר";
const std::string codesh::error::blasphemy_details::NO_SCOPE_END = "נָבוֹא שְׁקָרַי: צֻּפָּה ויתם";
const std::string codesh::error::blasphemy_details::NO_KEYWORD_SHALL_BE = "נָבוֹא שְׁקָרַי: צֻּפָּה היה";
const std::string codesh::error::blasphemy_details::NO_PUNCTUATION_END_OP = "נָבוֹא שְׁקָרַי: צֻפָּה סִיֹּמֶת שׁוּרָה";
const std::string codesh::error::blasphemy_details::UNEXPECTED_TOKEN = "נָבוֹא שְׁקָרַי: מִלָּה לֹא צְפוּיָה";
const std::string codesh::error::blasphemy_details::NO_TYPE = "נָבוֹא שְׁקָרַי: צֻּפָּה סוּג מִשְׁתַּנֶּה";
const std::string codesh::error::blasphemy_details::UNEXPECTED_DECLARATION = "נָבוֹא שְׁקָרַי: צֻּפָּה עצם, צלם, מניין אוֹ חותם";

// SEMANTIC
//TODO: Throw when interoperability & type checks exist
const std::string codesh::error::blasphemy_details::UNRECOGNIZED_TYPE = "עֶצֶם בִּלְתִּי־מְזֹהֶה: מִלָּה אֵינָהּ יְדוּעָה";

// OUTPUT
const std::string codesh::error::blasphemy_details::CONSTANT_POOL_TOO_BIG = "אָחִי, תִּרְגַּע... לֹא צָרִיךְ כָּל־כָּךְ הַרְבֵּה קְּבוּעִים. הַגְּבוּל הוּא חָמֵשׁ וּשְׁלֹשִׁים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה וְשִׁשִּׁים אָלֶף";
const std::string codesh::error::blasphemy_details::METHOD_TOO_BIG = "נוּ, מַה זֶּה? מַעֲשֶׂה אוֹ הַתָּנ״ךְ עַצְמוֹ? קַצֵּר!";
const std::string codesh::error::blasphemy_details::STRING_TOO_BIG = "אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ תָּוִים וּשְׁלֹשִׁים תָּוִים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה תָּוִים וְשִׁשִּׁים אָלֶף תָּוִים בִּכְתֻבִים";
