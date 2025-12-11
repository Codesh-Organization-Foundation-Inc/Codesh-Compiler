#include "command_parser.h"

#include "blasphemies/blasphemy_collector.h"

#include <stdexcept>

codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result {};

    if (argc < 2)
    {
        error::get_blasphemy_collector().add_blasphemy("אַרְגוּמֶנְטִים חֲסֵרִים: נְתִיבֵי קֶלֶט וּפָלַט נִּדְרָשִׁים", error::blasphemy_type::UNKNOWN,std::nullopt, true);
    }

    result.src_path = argv[1];
    result.dest_path = argv[2];

    return result;
}
