#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <getopt.h>
#include <memory>
#include <rapidxml.h>
#include <boost/filesystem.hpp>

#include "make_unique.hpp"
#include "trackdata.hpp"
#include "gpx.hpp"

#include "stopwatch.hpp"

namespace s = std;
namespace x = rapidxml;
namespace bf = boost::filesystem;

struct Option{
    struct File{
        s::string filename;
    };
    
    File global;
    s::vector<File> files;
};

Option parse_option(int argc, char **argv){
    Option ret;
    Option::File *crr = &ret.global;
    
    static option const opt[] = {
        {0,     0,                  0, 0},
    };
    optind = 0;
    while(true){
        int longindex;
        int c = getopt_long_only(argc, argv, "f:", opt, &longindex);
        if(c == -1) break;
        
        switch(c){
        case 'f':
            Option::File f;
            f.filename = optarg;
            ret.files.push_back(f);
            crr = &ret.files.back();
            break;
        }
    }
    
    (void)crr;
    
    return ret;
}

bool parse_file(s::string const &path){
    if(!bf::is_regular_file(path)) return false;
    
    s::uintmax_t size = bf::file_size(path);
    s::ifstream ifs(path);
    
    s::unique_ptr<char[]> buff = make_unique<char[]>(size + 1);
    ifs.read(buff.get(), size + 1);
    
    return parse_GPX(s::move(buff));
}


int main(int argc, char **argv){
    Option opt = parse_option(argc, argv);
    
    for(auto &&f : opt.files){
        s::cout << f.filename << s::endl;
        
        StopWatch sw;
        parse_file(f.filename);
        sw.stop();
        s::cout << sw.duration() << s::endl;
    }
}
