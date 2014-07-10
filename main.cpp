#include "include.hpp"
#include "parse_file.hpp"
#include "option.hpp"
#include "stopwatch.hpp"
#include "scope_exit.hpp"

#include "kml.hpp"

#include <thread>

static
bool do_main(int argc, char **argv){
    b::optional<Option> optopt = parse_option(argc, argv);
    if(!optopt){
        s::cerr << "Err: parse_option error" << s::endl;
        return false;
    }
    Option const &opt = optopt.get();
    
    s::vector<TrackData> data;
    for(auto &&f : opt.files){
        s::cout << f.filename << s::endl;
        
        StopWatch sw;
        b::optional<TrackData> tr = parse_file(f.filename);
        sw.stop(); sw.show(s::string("parse_file(\"") + f.filename + "\")");
        if(!tr){
            s::cerr << "Err: parse_file(\"" << f.filename << "\") error" << s::endl;
            return false;
        }
        
        data.emplace_back(s::move(tr.get()));
    }
    
    // stub: 取り敢えず、某GPUロガー互換のログを吐き出してみる
    {
        StopWatch sw;
        write_kml(opt, data);
        sw.stop();
        sw.show("write_kml");
    }
    
    return true;
}

int main(int argc, char **argv){
    StopWatch sw;
    scope_exit{sw.stop(); sw.show("main");};
    
    if(!do_main(argc, argv)) return -1;
    return 0;
}
