#ifndef ACUVE_UTILITY_HPP
#define ACUVE_UTILITY_HPP

#include "include.hpp"
#include "trackdata.hpp"

double distance(double lat1, double lng1, double lat2, double lng2);
double sum_vector(s::vector<double> vec);
s::vector<double> distance(TrackSegment const &track);

#endif // ACUVE_UTILITY_HPP
