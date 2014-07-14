#ifndef ACUVE_UTILITY_HPP
#define ACUVE_UTILITY_HPP

#include "include.hpp"
#include "trackdata.hpp"

double distance(double lat1, double lng1, double lat2, double lng2);
double distance(double lat1, double lng1, double ele1, double lat2, double lng2, double ele2);
s::vector<double> distance(TrackSegment const &track);
double sum(s::vector<double> vec);
s::vector<double> speed(TrackSegment const &track, s::vector<double> const &distances);
b::optional<s::tuple<double, double>> minmax(s::vector<double> ran);

#endif // ACUVE_UTILITY_HPP
