#include "util.h"

void codesh::util::replace_all(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;

    size_t pos = str.find(from);

    while (pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos = str.find(from, pos + to.length());
    }
}

void codesh::util::put_bytes(unsigned char arr[], const std::vector<unsigned char> &contents)
{
    for (size_t i = 0; i < contents.size(); i++)
    {
        arr[i] = contents[i];
    }
}

void codesh::util::put_int_bytes(unsigned char arr[], const size_t width, const int num)
{
    for (int i = 0; i < width; i++)
    {
        arr[width - 1 - i] = static_cast<unsigned char>(num >> (8 * i) & 0xFF);
    }
}