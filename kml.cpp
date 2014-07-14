#include "include.hpp"
#include "option.hpp"
#include "trackdata.hpp"
#include "utility.hpp"
#include "kml.hpp"
#include "kml_template.hpp"

#include "stopwatch.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

#include <boost/geometry.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>

TrackSegment simple(TrackSegment const &tr){
    namespace bg = b::geometry;
    // typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree>> point;
    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef bg::model::linestring<point> linestring;
    
    linestring line;
    for(auto &&p : tr){
        line.push_back(point(p.lat, p.lon));
    }
    
    linestring result;
    bg::simplify(line, result, 0.0002);
    
    TrackSegment ts;
    for(auto &&p : result){
        Point po;
        po.lat = p.template get<0>();
        po.lon = p.template get<1>();
        
        ts.push_back(po);
    }
    return ts;
}

bool format_kml(s::ostream &os, Option const &opt, s::vector<TrackData> const &data){
    os.precision(10);
    os << s_kml_head;
    os << s_kml_speedbar;
    os << s_kml_folder_head;
                    
    for(auto &&td : data){
        for(auto &&t : td.tracks){
            os << s_kml_track_head;
            
            StopWatch sw;
            s::vector<double> dis = distance(t.points);
            s::vector<double> spe = speed(t.points, dis);
            b::optional<s::tuple<double, double>> speminmax = minmax(spe);
            double const sumofdis = sum(dis);
            sw.stop(); sw.show("calcparm");
            s::clog << "sumofdis: " << sumofdis / 1e3 << "km" << s::endl;
            s::clog << "maxspeed: " << s::get<1>(speminmax.get()) << "km/s" << s::endl;
            
            sw.restart();
            auto sts = simple(t.points);
            sw.stop(); sw.show("simple");
            
            unsigned int i = 0;
            // for(auto &&p : t.points){
            for(auto &&p : sts){
                if(i > 1){
                    os << p.lon << "," << p.lat << "," << p.ele << "\n";
                    os << s_kml_track_foot << s_kml_track_head;
                    i = 0;
                }
                os << p.lon << "," << p.lat << "," << p.ele << "\n";
                ++i;
            }
            
            os << s_kml_track_foot;
        }
    }
    
    os << s_kml_folder_foot;
    os << s_kml_foot;
    
    return true;
    // for(auto &&td : data){
        // s::clog << "----- " << td.name << " -----" << s::endl;
        // for(auto &&t : td.tracks){
            // StopWatch sw;
            // s::vector<double> dis = distance(t.points);
            // sw.stop(); sw.show("distance");
            
            // sw.restart();
            // double const sumofdis = sum(dis);
            // sw.stop(); sw.show("sum");
            // s::clog << "sumofdis: " << sumofdis / 1e3 << "km" << s::endl;
            
            // os.precision(10);
            
            // TrackSegment const &ts = t.points;
            // for(unsigned int i = 0, size = ts.size(); i < size; ++i){
                // Point const &p = ts[i];
                // DateTime const &dt = p.time;
                
                // if(i != 0){
                    // double const in = DateTime::interval(t.points[i].time, t.points[i - 1].time);
                    // os << dt.year << "/" << dt.month << "/" << dt.date << " " << dt.hour << ":" << dt.minute << ":" << dt.second << ", ";
                    // os << "(" << p.lat << ", " << p.lon << "), ";
                    // os << dis[i - 1] / in * 3.6<< "\n";
                // }else{
                    // os << dt.year << "/" << dt.month << "/" << dt.date << " " << dt.hour << ":" << dt.minute << ":" << dt.second << ", ";
                    // os << "(" << p.lat << ", " << p.lon << "), ";
                    // os << "\n";
                // }
            // }
        // }
    // }
    // return false;
}
bool write_kml(Option const &opt, s::vector<TrackData> const &data){
    // stub: 適当にファイル名を決め打ち
    s::ostringstream oss;
    bool ret = format_kml(oss, opt, data);
    s::ofstream ofs("a.kml");
    ofs << oss.str();
    return ret;
}
