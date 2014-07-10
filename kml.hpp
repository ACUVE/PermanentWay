#ifndef ACUVE_KML_HPP
#define ACUVE_KML_HPP

#include "include.hpp"
#include "option.hpp"
#include "trackdata.hpp"

bool format_kml(s::ostream &os, Option const &opt, s::vector<TrackData> const &data);
bool write_kml(Option const &opt, s::vector<TrackData> const &data);

#endif
