#include <getopt.h>
#include <boost/filesystem.hpp>

#include "include.hpp"
#include "option.hpp"

namespace bf = b::filesystem;

b::optional<Option> parse_option(int argc, char **argv){
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
            if(bf::is_regular_file(optarg)){
                Option::File f;
                f.filename = optarg;
                ret.files.push_back(f);
                
                crr = &ret.files.back();
            }else{
                s::cerr << "Err: " << optarg << " doesn't exist" << s::endl;
                return {};
            }
            break;
        }
    }
    
    (void)crr;
    
    return ret;
}
