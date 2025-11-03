#include "util.h"

void codesh::util::replaceAll(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;

    size_t pos = str.find(from);

    while (pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos = str.find(from, pos + to.length());
    }
}
