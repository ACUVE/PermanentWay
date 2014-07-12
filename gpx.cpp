#include "include.hpp"
#include "gpx.hpp"
#include <rapidxml.h>

namespace x = rapidxml;
namespace b = boost;

// please see http://www.topografix.com/GPX/1/1/

static
inline bool isNumber(char ch){
    return '0' <= ch && ch <= '9';
}
static
inline unsigned int atoui(char const *&str){
    unsigned int num = 0;
    while(isNumber(*str)){
        num = num * 10 + (*str++ - '0');
    }
    return num;
}

static
inline unsigned int numofdate(unsigned int year, unsigned int month){
    // 紀元前のうるう年とかどういうふうなんだろうか
    // そもそもそんな時間の GPS データが存在するはずがないが
    static unsigned int const dateofmonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned int const dateofthismonth = dateofmonth[month - 1] + (month == 2 && DateTime::isLeapYear(year));
    
    return dateofthismonth;
}

// str は null terminated な文字列
static
b::optional<DateTime> parse_dateTime(char const *str){
    DateTime d;
    if(*str == '-'){
        d.isBC = true; ++str;
    }else{
        d.isBC = false;
    }
    
    // 年のパース
    char const * const ystr = str;
    d.year = atoui(str);
    if(*ystr == '0'){
        if(str - ystr != 4) return b::none;
    }else{
        if(str - ystr < 4) return b::none;
    }
    if(*str++ != '-') return b::none;
    if(d.year == 0) return b::none;
    
    // 月のパース
    char const * const mstr = str;
    d.month = atoui(str);
    if(str - mstr != 2) return b::none;
    if(!(1 <= d.month && d.month <= 12)) return b::none;
    if(*str++ != '-') return b::none;
    
    // 日のパース
    char const * const dstr = str;
    d.date = atoui(str);
    if(str - dstr != 2) return b::none;
    if(!(1 <= d.date && d.date <= numofdate(d.year, d.month))) return b::none;
    if(*str++ != 'T') return b::none;
    
    // 時のパース
    char const * const hstr = str;
    d.hour = atoui(str);
    if(str - hstr != 2) return b::none;
    if(!(0 <= d.hour && d.hour <= 24)) return b::none;
    if(*str++ != ':') return b::none;
    
    // 分のパース
    char const * const mistr = str;
    d.minute = atoui(str);
    if(str - mistr != 2) return b::none;
    if(!(0 <= d.minute && d.minute <= 59)) return b::none;
    if(*str++ != ':') return b::none;
    
    // 秒のパース（うるう秒の扱いが不明）
    char const * const sstr = str;
    d.second = atoui(str);
    if(str - sstr != 2) return b::none;
    if(!(0 <= d.second && d.second <= 60)) return b::none;
    
    // 時が 24 の時は分、秒は 0 である
    if(d.hour == 24){
        if(d.minute != 0 || d.second != 0) return b::none;
        
        // 24時の扱いが面倒なので翌日の 0 時にしておく
        d.hour = 0; ++d.date;
        if(d.date > numofdate(d.year, d.month)){
            ++d.month;
            if(d.month > 12){
                d.month = 1;
                if(d.isBC){
                    --d.year;
                    if(d.year == 0){
                        d.year = 1;
                        d.isBC = false;
                    }
                }else{
                    ++d.year;
                }
            }
        }
    }
    
    d.nanosecond = 0;
    
    // 大抵は秒未満はなくて UTC で書かれていると思われる
    if(!*str || (*str == 'Z' && !*(str + 1))) return d;
    
    if(*str == '.'){
        // 小数点以下がある（10桁目からは捨てる）
        ++str;
        unsigned int i = 0;
        for(; i < 9 && isNumber(*str); ++i){
            d.nanosecond = d.nanosecond * 10 + (*str - '0');
            ++str;
        }
        if(i == 0) return b::none;
        for(; i < 9; ++i) d.nanosecond *= 10;
        while(isNumber(*str)) ++str;
        
        // タイムゾーンが UTC で書かれているものはここで終わり
        if(!*str || (*str == 'Z' && !*(str + 1))) return d;
    }
    
    // タイムゾーンのパース
    if(*str != '+' && *str != '-') return b::none;
    bool tzplus = *str++ == '+';
    unsigned int tzhour = 0, tzminute = 0;
    
    char const * const tzhstr = str;
    tzhour = atoui(str);
    if(str - tzhstr != 2) return b::none;
    char const * const tzmstr = str;
    tzminute = atoui(str);
    if(str - tzmstr != 2) return b::none;
    
    if(!(0 <= tzhour && tzhour <= 14)) return b::none;
    if(!(0 <= tzminute && tzminute <= 59)) return b::none;
    if(tzhour == 14 && tzminute != 0) return b::none;
    
    // やる気がないソースコード
    if(tzplus){
        // + の時は引き算
        d.minute -= tzminute;
        d.hour -= tzhour;
        if(d.minute < 0){
            d.minute += 60;
            --d.hour;
        }
        if(d.hour < 0){
            d.hour += 24;
            --d.date;
            if(d.date < 1){
                --d.month;
                if(d.month < 1){
                    d.month = 12;
                    if(d.isBC){
                        ++d.year;
                    }else{
                        --d.year;
                        if(d.year == 0){
                            d.year = 1;
                            d.isBC = true;
                        }
                    }
                }
                d.date = numofdate(d.year, d.month);
            }
        }
    }else{
        // - の時は足し算
        d.minute += tzminute;
        d.hour += tzhour;
        if(d.minute >= 60){
            d.minute -= 60;
            ++d.hour;
        }
        if(d.hour >= 24){
            d.hour -= 24;
            ++d.date;
            if(d.date > numofdate(d.year, d.month)){
                d.date = 1;
                ++d.month;
                if(d.month > 12){
                    d.month = 1;
                    if(d.isBC){
                        --d.year;
                        if(d.year == 0){
                            d.year = 1;
                            d.isBC = false;
                        }
                    }else{
                        ++d.year;
                    }
                }
            }
        }
    }
    
    return d;
}

static
b::optional<Point> parse_wptType(x::xml_node<> *wpt){
    Point p;
    bool hasLat = false, hasLon = false;
    for(x::xml_attribute<> *attr = wpt->first_attribute(); attr; attr = attr->next_attribute()){
        char const *name = attr->name();
        if(s::strcmp(name, "lat") == 0){
            hasLat = true;
            char const *val = attr->value();
            char *next;
            p.lat = s::strtod(val, &next);
            if(val == next || *next) return b::none;
        }else if(s::strcmp(name, "lon") == 0){
            hasLon = true;
            char const *val = attr->value();
            char *next;
            p.lon = s::strtod(val, &next);
            if(val == next || *next) return b::none;
        }
    }
    if(!(hasLat && hasLon)) return b::none;
    
    enum class State{
        ELE, TIME, MAGVAR, GEOIDHEIGHT, NAME, CMT, DESC, SRC, LINK, SYM, TYPE, FIX, SAT, HDOP, VDOP, PDOP, AGEOFGPSDATA, DGPSID, EXTENSIONS, END
    } s = State::ELE;
    for(x::xml_node<> *child = wpt->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "ele") == 0){
            if(State::ELE < s) return b::none;
            s = State::TIME;
            char const *val = child->value();
            char *next;
            p.ele = s::strtod(val, &next);
            if(val == next || *next) return b::none;
        }else if(s::strcmp(name, "time") == 0){
            if(State::TIME < s) return b::none;
            s = State::MAGVAR;
            if(b::optional<DateTime> dt = parse_dateTime(child->value())){
                p.time = s::move(dt.get());
            }else{
                return b::none;
            }
        }else if(s::strcmp(name, "magvar") == 0){
            if(State::MAGVAR < s) return b::none;
            s = State::GEOIDHEIGHT;
        }else if(s::strcmp(name, "geoidheight") == 0){
            if(State::GEOIDHEIGHT < s) return b::none;
            s = State::NAME;
        }else if(s::strcmp(name, "name") == 0){
            if(State::NAME < s) return b::none;
            s = State::CMT;
        }else if(s::strcmp(name, "cmt") == 0){
            if(State::CMT < s) return b::none;
            s = State::DESC;
        }else if(s::strcmp(name, "desc") == 0){
            if(State::DESC < s) return b::none;
            s = State::SRC;
        }else if(s::strcmp(name, "src") == 0){
            if(State::SRC < s) return b::none;
            s = State::LINK;
        }else if(s::strcmp(name, "link") == 0){
            if(State::LINK < s) return b::none;
            s = State::LINK;
        }else if(s::strcmp(name, "sym") == 0){
            if(State::SYM < s) return b::none;
            s = State::TYPE;
        }else if(s::strcmp(name, "type") == 0){
            if(State::TYPE < s) return b::none;
            s = State::FIX;
        }else if(s::strcmp(name, "fix") == 0){
            if(State::FIX < s) return b::none;
            s = State::SAT;
        }else if(s::strcmp(name, "sat") == 0){
            if(State::SAT < s) return b::none;
            s = State::HDOP;
        }else if(s::strcmp(name, "hdop") == 0){
            if(State::HDOP < s) return b::none;
            s = State::VDOP;
        }else if(s::strcmp(name, "vdop") == 0){
            if(State::VDOP < s) return b::none;
            s = State::PDOP;
        }else if(s::strcmp(name, "pdop") == 0){
            if(State::PDOP < s) return b::none;
            s = State::AGEOFGPSDATA;
        }else if(s::strcmp(name, "ageofgpsdata") == 0){
            if(State::AGEOFGPSDATA < s) return b::none;
            s = State::DGPSID;
        }else if(s::strcmp(name, "dgpsid") == 0){
            if(State::DGPSID < s) return b::none;
            s = State::EXTENSIONS;
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return b::none;
            s = State::END;
        }else{
            return b::none;
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
            if(State::TRKPT < s) return b::none;
            s = State::TRKPT;
            if(b::optional<Point> p = parse_wptType(child)){
                ts.emplace_back(s::move(p.get()));
            }else{
                return b::none;
            }
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return b::none;
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
            if(State::NAME < s) return b::none;
            s = State::CMT;
            t.name = child->value();
        }else if(s::strcmp(name, "cmt") == 0){
            if(State::CMT < s) return b::none;
            s = State::DESC;
        }else if(s::strcmp(name, "desc") == 0){
            if(State::DESC < s) return b::none;
            s = State::SRC;
        }else if(s::strcmp(name, "src") == 0){
            if(State::SRC < s) return b::none;
            s = State::LINK;
        }else if(s::strcmp(name, "link") == 0){
            if(State::LINK < s) return b::none;
            s = State::NUMBER;
        }else if(s::strcmp(name, "number") == 0){
            if(State::NUMBER < s) return b::none;
            s = State::TYPE;
        }else if(s::strcmp(name, "type") == 0){
            if(State::TYPE < s) return b::none;
            s = State::EXTENSIONS;
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return b::none;
            s = State::TRKSEG;
        }else if(s::strcmp(name, "trkseg") == 0){
            if(State::TRKSEG < s) return b::none;
            s = State::TRKSEG;
            
            if(b::optional<TrackSegment> ts = parse_trksegType(child)){
                TrackSegment &tsd = ts.get();
                t.points.insert(t.points.end(), tsd.begin(), tsd.end());
            }else{
                return b::none;
            }
        }else{
            return b::none;
        }
    }
    return s::move(t);
}

static
b::optional<TrackData> parse_gpxType(x::xml_node<> *gpx){
    TrackData td;
    enum class State{
        META, WPT, RTE, TRK, EXTENSIONS, END
    } s = State::META;
    for(x::xml_node<> *child = gpx->first_node(); child; child = child->next_sibling()){
        char const *name = child->name();
        if(s::strcmp(name, "metadata") == 0){
            if(State::META < s) return b::none;
            s = State::WPT;
            parse_metadataType(child);
        }else if(s::strcmp(name, "wpt") == 0){
            if(State::WPT < s) return b::none;
            s = State::WPT;
            parse_wptType(child);   // stub
        }else if(s::strcmp(name, "rte") == 0){
            if(State::RTE < s) return b::none;
            s = State::RTE;
            parse_rteType(child);   // stub
        }else if(s::strcmp(name, "trk") == 0){
            if(State::TRK < s) return b::none;
            s = State::TRK;
            if(b::optional<Track> t = parse_trkType(child)){
                td.tracks.emplace_back(s::move(t.get()));
            }else{
                return b::none;
            }
        }else if(s::strcmp(name, "extensions") == 0){
            if(State::EXTENSIONS < s) return b::none;
            s = State::END;
        }else{
            return b::none;
        }
    }
    
    return s::move(td);
}

b::optional<TrackData> parse_GPX(s::unique_ptr<char[]> buff){
    x::xml_document<> doc;
    try{
        doc.parse<0>(buff.get());
    }catch(...){
        return b::none;
    }
    
    x::xml_node<> *root = doc.first_node("gpx");
    if(!root) return b::none;
    
    return parse_gpxType(root);
}
