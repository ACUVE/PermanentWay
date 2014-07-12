#ifndef ACUVE_GPX_HPP
#define ACUVE_GPX_HPP

#include "include.hpp"
#include "trackdata.hpp"

b::optional<TrackData> parse_GPX(s::unique_ptr<char[]>);

#endif // ACUVE_GPX_HPP
