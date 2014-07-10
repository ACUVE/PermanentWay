#include "include.hpp"
#include "option.hpp"
#include "trackdata.hpp"
#include "kml.hpp"
#include "utility.hpp"

#include <fstream>

bool format_kml(s::ostream &os, Option const &opt, s::vector<TrackData> const &data){
    for(auto &&td : data){
        for(auto &&t : td.tracks){
            s::vector<double> dis = distance(t.points);
            for(unsigned int i = 0, size = t.points.size(); i < size; ++i){
                if(i != 0){
                    os << t.points[i].lat << "," << t.points[i].lon << "," << dis[i - 1] * 3.6 << "\n";
                }else{
                    os << t.points[i].lat << "," << t.points[i].lon << "\n";
                }
            }
            os << s::flush;
        }
    }
    return false;
}
bool write_kml(Option const &opt, s::vector<TrackData> const &data){
    // stub: 適当にファイル名を決め打ち
    s::ofstream ofs("out.kml");
    return format_kml(ofs, opt, data);
}
