#ifndef ACUVE_TRACKDATA_HPP
#define ACUVE_TRACKDATA_HPP

#include "include.hpp"
#include <vector>

struct DateTime{
    // 紀元前か否か
    bool isBC;
    // 年、月、日（4294967296年以降は知らん）
    // 時、分、秒
    // ナノ秒
    unsigned int year, month, date;
    unsigned int hour, minute, second;
    unsigned int nanosecond;
};

struct Point{
    // 緯度,経度（度数法）
    double lat = 0.0, lon = 0.0;
    // 高さ
    double ele = s::numeric_limits<double>::quiet_NaN();
    // 時間
    DateTime time;
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
