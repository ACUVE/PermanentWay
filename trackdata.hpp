#ifndef ACUVE_TRACKDATA_HPP
#define ACUVE_TRACKDATA_HPP

#include "include.hpp"
#include <vector>

struct Point{
    // 緯度,経度
    double lat = 0.0, lon = 0.0;
    // 高さ
    double ele = s::numeric_limits<double>::quiet_NaN();
};

struct WayPoint{
    // 名前
    s::string name;
    // 位置
    Point point;
};

using TrackSegment = s::vector<Point>;

struct Track{
    // 名前
    s::string name;
    // 点列
    TrackSegment points;
};

struct TrackData{
    // 名前
    s::string name;
    // 重要な点集合
    s::vector<WayPoint> waypoints;
    // トラック（複数）
    s::vector<Track> tracks;
};

#endif
