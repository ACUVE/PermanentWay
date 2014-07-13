// PermanentWay
// Copyright (C) 2014  ACUVE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "include.hpp"
#include "parse_file.hpp"
#include "option.hpp"
#include "stopwatch.hpp"
#include "scope_exit.hpp"

#include "kml.hpp"

#include <future>

static
bool do_main(int argc, char **argv){
    b::optional<Option> optopt = parse_option(argc, argv);
    if(!optopt){
        s::cerr << "Err: parse_option error" << s::endl;
        return false;
    }
    Option const &opt = optopt.get();
    
    StopWatch sw;
    s::vector<TrackData> data;
    if(opt.files.size() > 1){
        s::vector<s::future<b::optional<TrackData>>> futures;
        for(auto &&f : opt.files){
            s::cout << f.filename << s::endl;
            
            futures.emplace_back(s::async(s::launch::async, [f = f.filename](){
                StopWatch sw;
                scope_exit{sw.stop(); sw.show(s::string("parse_file(\"") + f + "\")");};
                return parse_file(f);
            }));
        }
        for(auto &&f : futures){
            b::optional<TrackData> tr = f.get();
            if(!tr){
                s::cerr << "Err: parse_file() error" << s::endl;
                return false;
            }
            data.emplace_back(s::move(tr.get()));
        }
    }else{
        StopWatch sw;
        b::optional<TrackData> tr = parse_file(opt.files.front().filename);
        scope_exit{sw.stop(); sw.show(s::string("parse_file(\"") + opt.files.front().filename + "\")");};
        if(!tr){
            s::cerr << "Err: parse_file() error" << s::endl;
        }
        data.emplace_back(s::move(tr.get()));
    }
    sw.stop(); sw.show("load_time");
    
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
