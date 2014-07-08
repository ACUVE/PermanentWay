#include <fstream>
#include <cstdint>
#include <boost/filesystem.hpp>

#include "make_unique.hpp"
#include "trackdata.hpp"
#include "gpx.hpp"
#include "option.hpp"

#include "stopwatch.hpp"

namespace bf = b::filesystem;

bool parse_file(s::string const &path){
    if(!bf::is_regular_file(path)) return false;
    
    s::uintmax_t size = bf::file_size(path);
    s::ifstream ifs(path);
    
    s::unique_ptr<char[]> buff = make_unique<char[]>(size + 1);
    ifs.read(buff.get(), size + 1);
    
    return parse_GPX(s::move(buff));
}

int main(int argc, char **argv){

    if(b::optional<Option> opt = parse_option(argc, argv)){
        for(auto &&f : opt.get().files){
            s::cout << f.filename << s::endl;

            StopWatch sw;
            parse_file(f.filename);
            sw.stop();
            s::cout << (long double)sw.duration() / 1e9L << s::endl;
        }
    }
}
