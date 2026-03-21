#include "class_loader.h"

#include "class_directory_loader.h"
#include "jar_loader.h"
#include "jimage_loader.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"

#include <fmt/format.h>

codesh::external::class_loader::class_loader() : _is_loaded(true)
{
}

std::unique_ptr<codesh::external::class_loader> codesh::external::class_loader::create(
    const std::filesystem::path &classpath)
{
    std::unique_ptr<class_loader> result;

    if (std::filesystem::is_directory(classpath))
    {
        result = std::make_unique<class_directory_loader>(classpath);
    }
    else if (is_jimage(classpath))
    {
        result = std::make_unique<jimage_loader>(classpath);
    }
    else if (is_jar(classpath))
    {
        result = std::make_unique<jar_loader>(classpath);
    }


    if (result == nullptr)
    {
        blasphemy::get_blasphemy_collector().add_warning(
            fmt::format(blasphemy::details::CLASSPATH_NOT_SUPPORTED, classpath.string()),
            blasphemy::blasphemy_type::EXTERNAL,
            lexer::NO_CODE_POS
        );
        return nullptr;
    }

    if (!result->is_loaded())
    {
        blasphemy::get_blasphemy_collector().add_warning(
            fmt::format(blasphemy::details::CLASSPATH_OPEN_ERROR, classpath.string()),
            blasphemy::blasphemy_type::EXTERNAL,
            lexer::NO_CODE_POS
        );
        return nullptr;
    }

    return result;
}

codesh::external::class_loader::~class_loader() = default;

bool codesh::external::class_loader::is_loaded() const
{
    return _is_loaded;
}