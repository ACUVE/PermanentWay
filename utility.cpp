#include "include.hpp"
#include "utility.hpp"

#include <thread>
#include <future>

static
inline double deg2rad(double deg){
    return deg / 180.0 * M_PI;
}
static
inline double rad2deg(double rad){
    return rad / M_PI * 180.0;
}

// 地球上の2点の距離計算
// http://yamadarake.jp/trdi/report000001.html を参考にした
// ラジアンで渡すこと
double distance(double lat1, double lng1, double lat2, double lng2){
    constexpr double A = 6378137.000000;        // 長半径
    constexpr double E2 = 0.00669437999019758;  // 第一離心率^2
    constexpr double MNUM = 6335439.32729246;   // Mの分子

    double const my = (lat1 + lat2) / 2.0;
    double const dx = lng1 - lng2;
    double const dy = lat1 - lat2;
    double const sin = s::sin(my);
    
    double const w = s::sqrt(1.0 - E2 * sin * sin);
    double const m = MNUM / (w * w * w);
    double const n = A / w;
    
    double const dym = dy * m;
    double const dxncos = dx * n * s::cos(my);
    
    return s::sqrt(dym * dym + dxncos * dxncos);
}

// vector<double>に含まれる値の和（正数を仮定）
double sum(s::vector<double> vec){
    s::size_t size = vec.size();
    if(size == 0) return 0.0;
    
    s::sort(vec.begin(), vec.end());
    
    // 和の計算は項数がかなり多くないと早くならないので、並列化しない
    while(size > 1){
        unsigned int i = size & 1;
        unsigned int j = i;
        for(; j < size; ++i, j += 2){
            vec[i] = vec[j] + vec[j + 1];
        }
        size = (size + 1) / 2;
    }
    
    return vec[0];
}

// TrackSegmentの距離
s::vector<double> distance(TrackSegment const &track){
    s::size_t const size = track.size(), calc_size = size - 1;
    if(size == 0 || size == 1) return {};
    
    s::vector<double> distances(calc_size);
    unsigned int hwc;
    // if(calc_size < 10000 || (hwc = s::thread::hardware_concurrency()) <= 1){
    if(true){
        // 並列化しない
        double lat_old = deg2rad(track[0].lat), lng_old = deg2rad(track[0].lon);
        for(s::size_t i = 1; i < size; ++i){
            double lat = deg2rad(track[i].lat), lng = deg2rad(track[i].lon);
            distances[i - 1] = distance(lat_old, lng_old, lat, lng);
            lat_old = lat; lng_old = lng;
        }
    }else{
        // 並列化
        s::vector<s::future<void>> futures;
        for(unsigned int no = 0; no < hwc; ++no){
            futures.emplace_back(s::async(s::launch::async, [&, no](){
                unsigned int const div = calc_size / hwc;
                unsigned int const rest = calc_size % hwc;
                unsigned int const this_calc_size = div + (no < rest);
                unsigned int const start_index = div * no + std::min(no, rest);
                unsigned int const end_index = start_index + this_calc_size;
                
                double lat_old = deg2rad(track[start_index].lat), lng_old = deg2rad(track[start_index].lon);
                for(unsigned int i = start_index + 1; i <= end_index; ++i){
                    double lat = deg2rad(track[i].lat), lng = deg2rad(track[i].lon);
                    distances[i - 1] = distance(lat_old, lng_old, lat, lng);
                    lat_old = lat; lng_old = lng;
                }
            }));
        }
        for(auto &&f : futures) f.get();
    }
    
    return s::move(distances);
}
