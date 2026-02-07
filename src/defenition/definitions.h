#pragma once

#include <string>

namespace codesh::definition
{
const std::string JVM_CONSTRUCTOR_NAME = "<init>";

const std::string SOURCE_FILE_EXTENSION = ".אמן";

/**
 * The identifier used when there was an error parsing an identifier
 */
const std::string ERROR_IDENTIFIER_CONTENT = "סְדוֹם וַעֲמֹרָה";

const fmt::runtime_format_string<> CONSTRUCTOR_PRETTY_STRING = fmt::runtime("בְּרִיאַת {}");
const fmt::runtime_format_string<> METHOD_PRETTY_STRING = fmt::runtime("וּשְׁמוֹ {}");
}
