#include "parse_file.hpp"
#include "option.hpp"
#include "stopwatch.hpp"

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
