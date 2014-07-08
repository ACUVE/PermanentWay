#include "include.hpp"
#include "gpx.hpp"
#include <rapidxml.h>

namespace x = rapidxml;
namespace b = boost;

static
b::optional<Point> parse_wptType(x::xml_node<> *wpt){
    Point p;
    bool hasLat = false, hasLon = false;
    for(x::xml_attribute<> *attr = wpt->first_attribute(); attr; attr = attr->next_attribute()){
        char const *name = attr->name();
        if(s::strcmp(name, "lat") == 0){
            hasLat = true;
            p.lat = s::strtod(attr->value(), nullptr);
        }else if(s::strcmp(name, "lon") == 0){
            hasLon = true;
            p.lon = s::strtod(attr->value(), nullptr);
        }
    }
    if(!(hasLat && hasLon)) return {};
    
    enum class State{
        ELE, TIME, MAGVAR, GEOIDHEIGHT, NAME, CMT, DESC, SRC, LINK, SYM, TYPE, FIX, SAT, HDOP, VDOP, PDOP, AGEOFGPSDATA, DGPSID, EXTENSIONS, END
    } s = State::ELE;
    for(x::xml_node<> *child = wpt->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "ele") == 0){
            if(State::ELE < s) return {};
            s = State::TIME;
            p.ele = s::strtod(child->value(), nullptr);
        }else if(s::strcmp(name, "time") == 0){
            if(State::TIME < s) return {};
            s = State::MAGVAR;
        }else if(s::strcmp(name, "magvar") == 0){
            if(State::MAGVAR < s) return {};
            s = State::GEOIDHEIGHT;
        }else if(s::strcmp(name, "geoidheight") == 0){
            if(State::GEOIDHEIGHT < s) return {};
            s = State::NAME;
        }else if(s::strcmp(name, "name") == 0){
            if(State::NAME < s) return {};
            s = State::CMT;
        }else if(s::strcmp(name, "cmt") == 0){
            if(State::CMT < s) return {};
            s = State::DESC;
        }else if(s::strcmp(name, "desc") == 0){
            if(State::DESC < s) return {};
            s = State::SRC;
        }else if(s::strcmp(name, "src") == 0){
            if(State::SRC < s) return {};
            s = State::LINK;
        }else if(s::strcmp(name, "link") == 0){
            if(State::LINK < s) return {};
            s = State::LINK;
        }else if(s::strcmp(name, "sym") == 0){
            if(State::SYM < s) return {};
            s = State::TYPE;
        }else if(s::strcmp(name, "type") == 0){
            if(State::TYPE < s) return {};
            s = State::FIX;
        }else if(s::strcmp(name, "fix") == 0){
            if(State::FIX < s) return {};
            s = State::SAT;
        }else if(s::strcmp(name, "sat") == 0){
            if(State::SAT < s) return {};
            s = State::HDOP;
        }else if(s::strcmp(name, "hdop") == 0){
            if(State::HDOP < s) return {};
            s = State::VDOP;
        }else if(s::strcmp(name, "vdop") == 0){
            if(State::VDOP < s) return {};
            s = State::PDOP;
        }else if(s::strcmp(name, "pdop") == 0){
            if(State::PDOP < s) return {};
            s = State::AGEOFGPSDATA;
        }else if(s::strcmp(name, "ageofgpsdata") == 0){
            if(State::AGEOFGPSDATA < s) return {};
            s = State::DGPSID;
        }else if(s::strcmp(name, "dgpsid") == 0){
            if(State::DGPSID < s) return {};
            s = State::EXTENSIONS;
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return {};
            s = State::END;
        }else{
            return {};
        }
    }
    
    return s::move(p);
}

static
b::optional<TrackSegment> parse_trksegType(x::xml_node<> *trkseg){
    TrackSegment ts;
    ts.reserve(10000);  // 適当にreserve
    enum class State{
        TRKPT, EXTENSIONS, END
    } s = State::TRKPT;
    for(x::xml_node<> *child = trkseg->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "trkpt") == 0){
            if(State::TRKPT < s) return {};
            s = State::TRKPT;
            if(b::optional<Point> p = parse_wptType(child)){
                ts.emplace_back(s::move(p.get()));
            }else{
                return {};
            }
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return {};
            s = State::END;
        }
    }
    return s::move(ts);
}

static
void parse_metadataType(x::xml_node<> *metadata){
}

static
bool parse_rteType(x::xml_node<> *rte){
    return true;
}

static
b::optional<Track> parse_trkType(x::xml_node<> *trk){
    Track t;
    enum class State{
        NAME, CMT, DESC, SRC, LINK, NUMBER, TYPE, EXTENSIONS, TRKSEG, END
    } s = State::NAME;
    for(x::xml_node<> *child = trk->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "name") == 0){
            if(State::NAME < s) return {};
            s = State::CMT;
        }else if(s::strcmp(name, "cmt") == 0){
            if(State::CMT < s) return {};
            s = State::DESC;
        }else if(s::strcmp(name, "desc") == 0){
            if(State::DESC < s) return {};
            s = State::SRC;
        }else if(s::strcmp(name, "src") == 0){
            if(State::SRC < s) return {};
            s = State::LINK;
        }else if(s::strcmp(name, "link") == 0){
            if(State::LINK < s) return {};
            s = State::NUMBER;
        }else if(s::strcmp(name, "number") == 0){
            if(State::NUMBER < s) return {};
            s = State::TYPE;
        }else if(s::strcmp(name, "type") == 0){
            if(State::TYPE < s) return {};
            s = State::EXTENSIONS;
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return {};
            s = State::TRKSEG;
        }else if(s::strcmp(name, "trkseg") == 0){
            if(State::TRKSEG < s) return {};
            s = State::TRKSEG;
            
            if(b::optional<TrackSegment> ts = parse_trksegType(child)){
                TrackSegment &tsd = ts.get();
                t.points.insert(t.points.end(), tsd.begin(), tsd.end());
            }else{
                return {};
            }
        }else{
            return {};
        }
    }
    return s::move(t);
}

b::optional<TrackData> parse_GPX(s::unique_ptr<char[]> buff){
    x::xml_document<> doc;
    try{
        doc.parse<0>(buff.get());
    }catch(...){
        return {};
    }
    
    //
    // http://www.topografix.com/GPX/1/1/
    //
    x::xml_node<> *root = doc.first_node("gpx");
    if(!root) return {};
    
    TrackData td;
    
    enum class State{
        META, WPT, RTE, TRK, EXTENSIONS, END
    } s = State::META;
    for(x::xml_node<> *child = root->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "metadata") == 0){
            if(State::META < s) return {};
            s = State::WPT;
            parse_metadataType(child);
        }else if(s::strcmp(name, "wpt") == 0){
            if(State::WPT < s) return {};
            s = State::WPT;
            parse_wptType(child);   // stub
        }else if(s::strcmp(name, "rte") == 0){
            if(State::RTE < s) return {};
            s = State::RTE;
            parse_rteType(child);   // stub
        }else if(s::strcmp(name, "trk") == 0){
            if(State::TRK < s) return {};
            s = State::TRK;
            
            if(b::optional<Track> t = parse_trkType(child)){
                td.tracks.emplace_back(s::move(t.get()));
            }else{
                return {};
            }
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return {};
            s = State::END;
        }else{
            return {};
        }
    }
    
    return s::move(td);
}
