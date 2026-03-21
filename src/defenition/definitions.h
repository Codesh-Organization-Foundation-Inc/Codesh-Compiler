#pragma once

#include "fmt/base.h"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace codesh::external
{
class class_loader;
}

namespace codesh::definition
{
inline const std::string JVM_CONSTRUCTOR_NAME = "<init>";

inline const std::string SOURCE_FILE_EXTENSION = ".אמן";

/**
 * The identifier used when there was an error parsing an identifier
 */
inline const std::string ERROR_IDENTIFIER_CONTENT = "סְדוֹם וַעֲמֹרָה";

inline const fmt::runtime_format_string<> CONSTRUCTOR_PRETTY_STRING = fmt::runtime("בְּרִיאַת {}");
inline const fmt::runtime_format_string<> METHOD_PRETTY_STRING = fmt::runtime("וּשְׁמוֹ {}");

using class_loaders = std::unordered_map<std::filesystem::path, std::unique_ptr<external::class_loader>>;
}
