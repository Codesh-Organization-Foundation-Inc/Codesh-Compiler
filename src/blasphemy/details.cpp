#include "details.h"

#define RFS_T fmt::runtime_format_string<>
static RFS_T rfs(const std::string_view str)
{
    return fmt::runtime(str);
}


// INIT
const RFS_T codesh::blasphemy::details::SOURCE_FILE_OPEN_ERROR = rfs(
    "לֹא נִתַּן לִפְתֹּחַ אֶת סֵפֶר הַמּוֹצָא {}"
);
const RFS_T codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR = rfs(
    "לֹא נִתַּן לִפְתֹּחַ אֶת סֵפֶר הַיַּעַד {}"
);
const RFS_T codesh::blasphemy::details::DEST_PATH_NOT_DIRECTORY = rfs(
    "נְתִיב הַיַּעַד {} אֵינוֹ אֶרֶץ"
);
const std::string codesh::blasphemy::details::NO_MAIN_ARGS =
    "מִנְחוֹת אֵינָם: נְתִיבֵי מוֹצָא וְיַעַד נִּדְרָשִׁים";

// LEXICAL
const std::string codesh::blasphemy::details::NO_BASAD =
    "בַּסַּ\"ד אֵינוֹ";
const std::string codesh::blasphemy::details::TOKEN_DOESNT_EXIST =
    "מִלָּה אֵינָהּ קַיֶּמֶת אוֹ שְׁאוּלָה מִלּוֹעֲזִית";

// SYNTAX
const std::string codesh::blasphemy::details::NO_IDENTIFIER =
    "נָבוֹא שְׁקָרַי: צֻּפָּה מִלָּה חָפְשִׁית";
const std::string codesh::blasphemy::details::NO_IMPORT_TYPE =
    "נָבוֹא שְׁקָרַי: צֻּפָּה סוּג יְבוּא";
const std::string codesh::blasphemy::details::NO_KEYWORD_LET =
    "נָבוֹא שְׁקָרַי: צֻּפָּה ויהי";
const std::string codesh::blasphemy::details::NO_KEYWORD_NAME =
    "נָבוֹא שְׁקָרַי: צֻּפָּה ושמו";
const std::string codesh::blasphemy::details::NO_CLOSE_MULTI_COMMENT =
    "נָבוֹא שְׁקָרַי: צֻּפָּה סְגִירַת הָעֵרָה מְרוּבַּת־פְּסוּקִים";
const std::string codesh::blasphemy::details::NO_CLOSE_PARENTHESIS =
    "נָבוֹא שְׁקָרַי: צֻּפָּה כי־טוב";
const std::string codesh::blasphemy::details::NO_SCOPE_BEGIN =
    "נָבוֹא שְׁקָרַי: צֻּפָּה ויאמר";
const std::string codesh::blasphemy::details::NO_SCOPE_END =
    "נָבוֹא שְׁקָרַי: צֻּפָּה ויתם";
const std::string codesh::blasphemy::details::NO_KEYWORD_SHALL_BE =
    "נָבוֹא שְׁקָרַי: צֻּפָּה היה";
const std::string codesh::blasphemy::details::NO_KEYWORD_BY =
    "נָבוֹא שְׁקָרַי: צֻּפָּה פי";
const std::string codesh::blasphemy::details::NO_KEYWORD_TO =
    "נָבוֹא שְׁקָרַי: צֻּפָּה עד";
const std::string codesh::blasphemy::details::NO_KEYWORD_FROM =
    "נָבוֹא שְׁקָרַי: צֻּפָּה מן";
const std::string codesh::blasphemy::details::NO_KEYWORD_SKIP =
    "נָבוֹא שְׁקָרַי: צֻּפָּה וידלג";
const std::string codesh::blasphemy::details::NO_KEYWORD_AGAINST =
    "נָבוֹא שְׁקָרַי: צֻּפָּה לנגד";
const std::string codesh::blasphemy::details::NO_PUNCTUATION_END_OP =
    "נָבוֹא שְׁקָרַי: צֻפָּה סִיֹּמֶת שׁוּרָה";
const std::string codesh::blasphemy::details::UNEXPECTED_TOKEN =
    "נָבוֹא שְׁקָרַי: מִלָּה לֹא צְפוּיָה";
const std::string codesh::blasphemy::details::EXPECTED_VARIABLE =
    "נָבוֹא שְׁקָרַי: צֻּפָּה משתנה";
const std::string codesh::blasphemy::details::NO_TYPE =
    "נָבוֹא שְׁקָרַי: צֻּפָּה סוּג מִשְׁתַּנֶּה";
const std::string codesh::blasphemy::details::UNEXPECTED_DECLARATION =
    "נָבוֹא שְׁקָרַי: צֻּפָּה עצם, צלם, מניין אוֹ חותם";

// SEMANTIC
const std::string codesh::blasphemy::details::UNRECOGNIZED_TYPE =
    "עֶצֶם בִּלְתִּי־מְזֹהֶה: מִלָּה אֵינָהּ יְדוּעָה";
const std::string codesh::blasphemy::details::CONDITION_NOT_BOOLEAN =
    "תְּנַאי חַיָּב לִהְיוֹת מִסּוּג דְּבַר־מָה";
const std::string codesh::blasphemy::details::ITERATOR_NOT_COLLECTION =
    "מוֹנֶה חַיָּב לִהְיוֹת מִסּוּג אוסף";
const RFS_T codesh::blasphemy::details::NOT_AN_OBJECT = rfs(
    "{} אֵינוֹ עֶצֶם"
);
const RFS_T codesh::blasphemy::details::UNARY_TYPE_MISMATCH = rfs(
    "הַסּוּג {} אֵינוֹ תּוֹאֵם לִפְעֻלַּת {}"
);
const RFS_T codesh::blasphemy::details::BINARY_TYPE_MISMATCH = rfs(
    "הַסּוּגִים {} וְ{} אֵינָם תּוֹאֲמִים לִפְעֻלַּת {}"
);
const RFS_T codesh::blasphemy::details::VARIABLE_REFERENCED_BEFORE_CREATION = rfs(
    "אוּזְכַּר הַמִּשְׁתַּנֶּה {} אֲשֶׁר טֶּרֶם נוֹצַר"
);
const RFS_T codesh::blasphemy::details::NOT_A_VARIABLE = rfs(
    "דְּבַר־{} אֵינוֹ מִשְׁתַּנֶּה"
);
const RFS_T codesh::blasphemy::details::SYMBOL_NOT_FOUND = rfs(
    "דְּבַר־{} אֵינוֹ נִמְצָא"
);
const RFS_T codesh::blasphemy::details::TYPE_DOES_NOT_EXIST = rfs(
    "דְּבַר־{} אֵינוֹ קַיָּם"
);
const RFS_T codesh::blasphemy::details::METHOD_NOT_FOUND = rfs(
    "דְּבַר־{} אֵינוֹ נִמְצָא"
);
const RFS_T codesh::blasphemy::details::NOT_A_METHOD = rfs(
    "דְּבַר־{} אֵינוֹ מַעֲשֶׂה"
);
const std::string codesh::blasphemy::details::ARGUMENT_TYPE_MISMATCH =
    "סוּג הַמִּנְחוֹת אֵינוֹ תּוֹאֵם לְחוֹתַם הַמַּעֲשֶׂה";
const RFS_T codesh::blasphemy::details::DUPLICATE_TYPE_DECLARATION = rfs(
    "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד"
);
const RFS_T codesh::blasphemy::details::DUPLICATE_METHOD_DECLARATION = rfs(
    "נֵאִיפַה: הֻכְרַז מַעֲשֶׂה כָּפוּל: {}"
);

// OUTPUT
const std::string codesh::blasphemy::details::CONSTANT_POOL_TOO_BIG =
    "אָחִי, תִּרְגַּע... לֹא צָרִיךְ כָּל־כָּךְ הַרְבֵּה קְּבוּעִים. הַגְּבוּל הוּא חָמֵשׁ וּשְׁלֹשִׁים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה וְשִׁשִּׁים אָלֶף";
const std::string codesh::blasphemy::details::METHOD_TOO_BIG =
    "נוּ, מַה זֶּה? מַעֲשֶׂה אוֹ הַתָּנ״ךְ עַצְמוֹ? קַצֵּר!";
const std::string codesh::blasphemy::details::STRING_TOO_BIG =
    "אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ תָּוִים וּשְׁלֹשִׁים תָּוִים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה תָּוִים וְשִׁשִּׁים אָלֶף תָּוִים בִּכְתֻבִים";
const std::string codesh::blasphemy::details::NUMBER_TOO_BIG =
    "אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ סְפָרוֹת וּשְׁלֹשִׁים סְפָרוֹת וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה סְפָרוֹת וְשִׁשִּׁים אָלֶף סְפָרוֹת בִּכְתֻבִים"; //TODO: לדוד לא היה כל כך הרבה נשים
const RFS_T codesh::blasphemy::details::TOO_MANY_LOCAL_VARIABLES = rfs(
    "יוֹתֵר מִדַּי מִשְׁתַּנִּים בְּמַעֲשֶׂה {}"
);
