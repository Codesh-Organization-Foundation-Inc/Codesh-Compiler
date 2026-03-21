#include "details.h"

using rfs_t = fmt::runtime_format_string<>;
static rfs_t rfs(const std::string_view str)
{
    return fmt::runtime(str);
}


// INIT
const std::string codesh::blasphemy::details::SRC_NOT_PROVIDED =
    "סֵפֶר לֹא הֱיֵה: לֹא סֻפַּק סֵפֶר הַמּוֹצָא";
const std::string codesh::blasphemy::details::DEST_NOT_PROVIDED =
    "סֵפֶר לֹא הֱיֵה: לֹא סֻפַּק סֵפֶר הַיַּעַד";
const rfs_t codesh::blasphemy::details::SOURCE_FILE_OPEN_ERROR = rfs(
    "סֵפֶר אָטוּם הֱיֵה: לֹא נִתַּן לִפְתֹּחַ אֶת סֵפֶר הַמּוֹצָא {}"
);
const rfs_t codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR = rfs(
    "סֵפֶר אָטוּם הֱיֵה: לֹא נִתַּן לִפְתֹּחַ אֶת סֵפֶר הַיַּעַד {}"
);
const rfs_t codesh::blasphemy::details::DEST_PATH_NOT_DIRECTORY = rfs(
    "דֶּרֶךְ תּוֹעָה הֱיֵה: נְתִיב הַיַּעַד {} אֵינוֹ אֶרֶץ"
);
// TODO: Change argument error messages
const rfs_t codesh::blasphemy::details::INVALID_ARG = rfs(
    "דֶּרֶךְ תּוֹעָה הֱיֵה: הֵעָרֵךְ {} אֵינוֹ מַתְאִים"
);
const std::string codesh::blasphemy::details::NO_MAIN_ARGS =
    "מִנְחוֹת אֵינָם: נְתִיבֵי מוֹצָא וְיַעַד נִּדְרָשִׁים";
const std::string codesh::blasphemy::details::NO_ARG =
    "מִנְחוֹת אֵינָם: נִדְרָשׁ עֵרֶךְ הַכְנָסָה לַדֶּגֶל";
const std::string codesh::blasphemy::details::UNKNOWN_FLAG =
    "דֶּרֶךְ תּוֹעָה הֱיֵה: דֶּגֶל לֹא יָדוּעַ";

// LEXICAL
const std::string codesh::blasphemy::details::NO_BASAD =
    "בְּרָכָה חֲסֵרָה: בַּסַּ\"ד אֵינוֹ";
const std::string codesh::blasphemy::details::TOKEN_DOESNT_EXIST =
    "מִלָּה זָרָה תִּהְיֶה: מִלָּה אֵינָהּ קַיֶּמֶת אוֹ שְׁאוּלָה מִלּוֹעֲזִית";

// SYNTAX
const std::string codesh::blasphemy::details::NO_IDENTIFIER =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה מִלָּה חָפְשִׁית";
const std::string codesh::blasphemy::details::NO_IMPORT_TYPE =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה סוּג יְבוּא";
const std::string codesh::blasphemy::details::NO_KEYWORD_LET =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ויהי";
const std::string codesh::blasphemy::details::NO_KEYWORD_NAME =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ושמו";
const std::string codesh::blasphemy::details::NO_KEYWORD_NAME_OR_CONSTRUCTOR =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ושמו אוֹ בריאה";
const std::string codesh::blasphemy::details::NO_CLOSE_MULTI_COMMENT =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה סְגִירַת הָעֵרָה מְרוּבַּת־פְּסוּקִים";
const std::string codesh::blasphemy::details::NO_CLOSE_PARENTHESIS =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה כי־טוב";
const std::string codesh::blasphemy::details::NO_SCOPE_BEGIN =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ויאמר";
const std::string codesh::blasphemy::details::NO_SCOPE_END =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ויתם";
const std::string codesh::blasphemy::details::NO_KEYWORD_SHALL_BE =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה היה";
const std::string codesh::blasphemy::details::NO_KEYWORD_PUNC_EQUAL =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה ב־";
const std::string codesh::blasphemy::details::NO_KEYWORD_BY =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה פי־";
const std::string codesh::blasphemy::details::NO_KEYWORD_THE =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה את";
const std::string codesh::blasphemy::details::NO_KEYWORD_TO =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה עד";
const std::string codesh::blasphemy::details::NO_KEYWORD_FROM =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה מן";
const std::string codesh::blasphemy::details::NO_KEYWORD_SKIP =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה וידלג";
const std::string codesh::blasphemy::details::NO_KEYWORD_AGAINST =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה לנגד";
const std::string codesh::blasphemy::details::NO_PUNCTUATION_END_OP =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻפָּה סִיֹּמֶת שׁוּרָה";
const std::string codesh::blasphemy::details::UNEXPECTED_TOKEN =
    "נָבוֹא שְׁקָרַי הֱיֵה: מִלָּה לֹא צְפוּיָה";
const std::string codesh::blasphemy::details::EXPECTED_VARIABLE =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה משתנה";
const std::string codesh::blasphemy::details::NO_TYPE =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה סוּג מִשְׁתַּנֶּה";
const std::string codesh::blasphemy::details::UNEXPECTED_DECLARATION =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה עצם, צלם, מניין אוֹ חותם";
const std::string codesh::blasphemy::details::INVALID_TYPE_FOR_NEW =
    "נָבוֹא שְׁקָרַי הֱיֵה: צֻּפָּה עצם";

// SEMANTIC
const std::string codesh::blasphemy::details::UNRECOGNIZED_TYPE =
    "עֶצֶם בִּלְתִּי־מְזֹהֶה הֱיֵה: מִלָּה אֵינָהּ יְדוּעָה";
const std::string codesh::blasphemy::details::CONDITION_NOT_BOOLEAN =
    "סוּג אֵינוֹ כָּשֵׁר: תְּנַאי חַיָּב לִהְיוֹת מִסּוּג דְּבַר־מָה";
const std::string codesh::blasphemy::details::ITERATOR_NOT_COLLECTION =
    "סוּג אֵינוֹ כָּשֵׁר: מוֹנֶה חַיָּב לִהְיוֹת מִסּוּג אוסף";
const rfs_t codesh::blasphemy::details::NOT_AN_OBJECT = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: {} אֵינוֹ עֶצֶם"
);
const rfs_t codesh::blasphemy::details::UNARY_TYPE_MISMATCH = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: הַסּוּג {} אֵינוֹ תּוֹאֵם לִפְעֻלַּת {}"
);
const rfs_t codesh::blasphemy::details::BINARY_TYPE_MISMATCH = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: הַסּוּגִים {} וְ{} אֵינָם תּוֹאֲמִים לִפְעֻלַּת {}"
);
const rfs_t codesh::blasphemy::details::VARIABLE_REFERENCED_BEFORE_CREATION = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: אוּזְכַּר הַמִּשְׁתַּנֶּה {} אֲשֶׁר טֶּרֶם נוֹצַר"
);
const rfs_t codesh::blasphemy::details::NOT_A_VARIABLE = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: דְּבַר־{} אֵינוֹ מִשְׁתַּנֶּה"
);
const rfs_t codesh::blasphemy::details::SYMBOL_NOT_FOUND = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: דְּבַר־{} אֵינוֹ נִמְצָא"
);
const rfs_t codesh::blasphemy::details::TYPE_DOES_NOT_EXIST = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: דְּבַר־{} אֵינוֹ קַיָּם"
);
const rfs_t codesh::blasphemy::details::METHOD_NOT_FOUND = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: דְּבַר־{} אֵינוֹ נִמְצָא"
);
const rfs_t codesh::blasphemy::details::NOT_A_METHOD = rfs(
    "הַפְנָיָה פְּסוּלָה הֱיֵה: דְּבַר־{} אֵינוֹ מַעֲשֶׂה"
);
const rfs_t codesh::blasphemy::details::UNEXPECTED_END_OF_NUMBER = rfs(
    "סוֹף לֹא צָפוּי הֱיֵה: מִּסְפָּר נִגְמַר בְּלֹא עֵרֶךְ עֲבוּר {}"
);
const rfs_t codesh::blasphemy::details::INVALID_NUMBER_FORMAT_ASCENDING = rfs(
    "מִסְפָּר שִׁקְרִי הֱיֵה: סֵדֶר הַגְּדָלָה לֹא נִשְׁמַר בְּעֵת {}"
);
const rfs_t codesh::blasphemy::details::INVALID_MULTIPLICATION_FACTOR = rfs(
    "מִסְפָּר שִׁקְרִי הֱיֵה: הַכֶּפֶל אֵינוֹ בְּעָשׂוֹר עֲבוּר {}"
);
const rfs_t codesh::blasphemy::details::INVALID_NUMBER_FORMAT_DESCENDING = rfs(
    "מִסְפָּר שִׁקְרִי הֱיֵה: הַמִּסְפָּר הַבָּא ({}) גָּדוֹל (מְ־{}) בְּקֶרֶב אוֹתוֹ הַמַּחֲנֶה"
);
const rfs_t codesh::blasphemy::details::UNEXPECTED_ADDITION_OPERATOR = rfs(
    "מִסְפָּר שִׁקְרִי הֱיֵה: צֻפָּה פְּעֻלַּת חִבּוּר עֲבוּר {}"
);
const std::string codesh::blasphemy::details::UNEXPECTED_PERIOD_OPERATOR =
    "מִסְפָּר שִׁקְרִי הֱיֵה: צֻפָּה מִסְפָּר";
const std::string codesh::blasphemy::details::MULTIPLE_PERIOD_OPERATORS =
    "נַאִיפַת נְקֻדּוֹת: מִסְפַּר נְקֻדּוֹת בְּאוֹתוֹ הַמִּסְפָּר שֶׁלֹּא לְצֹרֵךְ";
const rfs_t codesh::blasphemy::details::ARGUMENT_TYPE_MISMATCH = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: סוּגֵי הַמִּנְחוֹת {} אֵינָם תּוֹאֲמִים לְאַף מַעֲשֶׂה {}"
);
const rfs_t codesh::blasphemy::details::RETURN_TYPE_MISMATCH = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: סוּג הַהַשָּׁבָה {} אֵינוֹ תּוֹאֵם לְחוֹתַם הַמַּעֲשֶׂה {}"
);
const rfs_t codesh::blasphemy::details::DUPLICATE_TYPE_DECLARATION = rfs(
    "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד"
);
const rfs_t codesh::blasphemy::details::DUPLICATE_METHOD_DECLARATION = rfs(
    "נֵאִיפַה תִּהְיֶה: הֻכְרַז מַעֲשֶׂה כָּפוּל: {}"
);
const rfs_t codesh::blasphemy::details::NON_STATIC_CALL_FROM_STATIC_CONTEXT = rfs(
    "קְרִיאָה אֵינָהּ כְּשֵׁרָה: וָלֹא־תְּבַשֵּׁל מַעֲשֶׂה כְּלָלִי {} בָּאֵנוּ כְּלָלֵי"
);

const rfs_t codesh::blasphemy::details::NOT_AN_ARRAY = rfs(
    "סוּג אֵינוֹ כָּשֵׁר: לֹא נִתַּן לְהִשְׁתַּמֵּשׁ בְּ{} כְּאוֹסֶף"
);
const std::string codesh::blasphemy::details::ARRAY_INDEX_NOT_INTEGER =
    "סוּג אֵינוֹ כָּשֵׁר: מַפְתֵּחַ הָאוֹסֶף חַיָּב לִהְיוֹת מִסּוּג שָׁלֵם";

// EXTERNAL
const rfs_t codesh::blasphemy::details::CLASSPATH_NOT_SUPPORTED = rfs(
    "סֵפֶר אָטוּם הֱיֵה: סוּג הַנְּתִיב {} אֵינוֹ נִתְמָךְ"
);
const rfs_t codesh::blasphemy::details::CLASSPATH_OPEN_ERROR = rfs(
    "סֵפֶר אָטוּם הֱיֵה: לֹא נִתַּן לִפְתֹּחַ אֶת נְתִיב {}"
);

// OUTPUT
const std::string codesh::blasphemy::details::CONSTANT_POOL_TOO_BIG =
    "גְּבוּל נֶעֱבַר: אָחִי, תִּרְגַּע... לֹא צָרִיךְ כָּל־כָּךְ הַרְבֵּה קְּבוּעִים. הַגְּבוּל הוּא חָמֵשׁ וּשְׁלֹשִׁים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה וְשִׁשִּׁים אָלֶף";
const std::string codesh::blasphemy::details::METHOD_TOO_BIG =
    "גְּבוּל נֶעֱבַר: נוּ, מַה זֶּה? מַעֲשֶׂה אוֹ הַתָּנ״ךְ עַצְמוֹ? קַצֵּר!";
const std::string codesh::blasphemy::details::STRING_TOO_BIG =
    "גְּבוּל נֶעֱבַר: אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ תָּוִים וּשְׁלֹשִׁים תָּוִים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה תָּוִים וְשִׁשִּׁים אָלֶף תָּוִים בִּכְתֻבִים";
const std::string codesh::blasphemy::details::NUMBER_TOO_BIG =
    "גְּבוּל נֶעֱבַר: אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ סְפָרוֹת וּשְׁלֹשִׁים סְפָרוֹת וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה סְפָרוֹת וְשִׁשִּׁים אָלֶף סְפָרוֹת בִּכְתֻבִים"; //TODO: לדוד לא היה כל כך הרבה נשים
const rfs_t codesh::blasphemy::details::TOO_MANY_LOCAL_VARIABLES = rfs(
    "גְּבוּל נֶעֱבַר: יוֹתֵר מִדַּי מִשְׁתַּנִּים בְּמַעֲשֶׂה {}"
);

// WARNINGS
const rfs_t codesh::blasphemy::details::NON_BIBLICAL_NUMBER = rfs(
    "וָלֹא תֹּאכַל גְּדִי בַּחֲלֵב אִמּוֹ: הַמְּסַפֵּר {} אֵינוֹ כָּשֵׁר לִמְהַדְּרִין"
);
const rfs_t codesh::blasphemy::details::INTEROP_REPLACEMENT_EXISTS = rfs(
    "וָלֹא תֹּאכַל גְּדִי בַּחֲלֵב אִמּוֹ: נִקְרָא הַמַּעֲשֶׂה {} וְגוֹי גָּדוֹל יִהְיֶה, כִּי קַיֶּמֶת תְּמוּרָה כְּשֵׁרָה בְּתַּלְמוּד קוֹדֶשׁ: {}"
);
const rfs_t codesh::blasphemy::details::ILLEGAL_IDENTIFIER_CONTENT_FOR_NON_KOSHER_LANGUAGES = rfs(
    "אוֹר לַגּוֹיִים: אֵין בִּיכֹלֶת מַרְבִּית הַלָּשׁוֹן הַזָּרָה לְהִשְׁתַּמֵּשׁ בְּשֵׁם הַמֵּכִיל {}"
);
