#include <boost/filesystem.hpp>
#include <fstream>

#include "include.hpp"
#include "parse_file.hpp"
#include "make_unique.hpp"
#include "gpx.hpp"

namespace bf = b::filesystem;

b::optional<TrackData> parse_file(s::string const &filename){
    bf::path path(filename);
    if(!bf::is_regular_file(path)) return b::none;
    
    s::uintmax_t size = bf::file_size(filename);
    s::ifstream ifs(filename);
    if(ifs){
        s::unique_ptr<char[]> buff = make_unique<char[]>(size + 1);
        ifs.read(buff.get(), size + 1);
        
        b::optional<TrackData> trackdata = parse_GPX(s::move(buff));
        if(!trackdata) return b::none;
        if(trackdata.get().name.empty()){
            trackdata.get().name = path.filename().string();
        }
        
        return std::move(trackdata);
    }
    return b::none;
}
