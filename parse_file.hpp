#ifndef ACUVE_PARSE_FILE_HPP
#define ACUVE_PARSE_FILE_HPP

#include "include.hpp"
#include "trackdata.hpp"

b::optional<TrackData> parse_file(s::string const &filename);

#endif // ACUVE_PARSE_FILE_HPP
