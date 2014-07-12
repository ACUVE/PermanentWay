#ifndef ACUVE_TRACKDATA_HPP
#define ACUVE_TRACKDATA_HPP

#include "include.hpp"

struct DateTime{
    // 紀元前か否か
    bool isBC;
    // 年、月、日（4294967296年以降は知らん）
    // 時、分、秒
    // ナノ秒
    unsigned int year, month, date;
    unsigned int hour, minute, second;
    unsigned int nanosecond;
    
    bool operator==(DateTime const &r) const{
        return s::tie(isBC, year, month, date, hour, minute, second, nanosecond) == s::tie(r.isBC, r.year, r.month, r.date, r.hour, r.minute, r.second, r.nanosecond);
    }
    bool operator!=(DateTime const &r) const{
        return !(*this == r);
    }
    bool operator<(DateTime const &r) const{
        if(isBC == r.isBC){
            return s::tie(year, month, date, hour, minute, second, nanosecond) < s::tie(r.year, r.month, r.date, r.hour, r.minute, r.second, r.nanosecond);
        }else{
            return isBC;
        }
    }
    bool operator<=(DateTime const &r) const{
        return !(r < *this);
    }
    bool operator>(DateTime const &r) const{
        return r < *this;
    }
    bool operator>=(DateTime const &r) const{
        return !(*this < r);
    }
    
    // 秒単位で2つの時間の差を計算
    // 単純に言ってしまえば、 ret = a - b;
    static
    double interval(DateTime const &a, DateTime const &b){
        double ret;
        ret  = ((int)a.nanosecond - (int)b.nanosecond) / 1e9;
        ret += ((int)a.second - (int)b.second);
        ret += ((int)a.minute - (int)b.minute) * 60;
        ret += ((int)a.hour - (int)b.hour) * (60 * 60);
        ret += ((int)a.date - (int)b.date) * (60 * 60 * 24);
        if(a.month != b.month){ // 大体 a.month == b.month だと思う
            static const int sumdate[] = {
                0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
            };
            int ad = sumdate[a.month - 1];
            if(a.month > 2 && DateTime::isLeapYear(a.year)) ++ad;
            int bd = sumdate[b.month - 1];
            if(b.month > 2 && DateTime::isLeapYear(b.year)) ++bd;
            ret += (ad - bd) * (60 * 60 * 24);
        }
        if(a.year != b.year || a.isBC != b.isBC){   // 大体 a.year == b.year && a.isBC == b.isBC だと思う
            static const int dayslist[400] = {
                #include "dayslist.txt"
            };
            int const ab = a.year / 400, bb = b.year / 400;
            int const ar = a.year % 400, br = b.year % 400;
            
            // 146097 = 400年分の日数
            if(a.isBC == b.isBC){
                long long int intday = (ab - bb) * 146097LL;
                intday += dayslist[ar] - dayslist[br];
                double pret = intday * (60 * 60 * 24);
                if(a.isBC){ // a.isBC == b.isBC だからこれで十分
                    ret -= pret;
                }else{
                    ret += pret;
                }
            }else{
                long long int intday = (ab + bb) * 146097LL;
                intday += dayslist[ar] + dayslist[br] - dayslist[1] - dayslist[1];  // 0年は存在しないので1年分引く
                double pret = intday * (60 * 60 * 24);
                if(a.isBC){
                    ret -= pret;
                }else{
                    ret += pret;
                }
            }
        }
        
        return ret;
    }
    
    static
    bool isLeapYear(unsigned int year){
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }
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
