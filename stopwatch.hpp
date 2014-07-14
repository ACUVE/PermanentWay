#ifndef ACUVE_STOPWATCH_HPP
#define ACUVE_STOPWATCH_HPP

#include "include.hpp"
#include <time.h>

class StopWatch{
    timespec start = {}, end = {};
    
public:
    StopWatch(){
        restart();
    }
    
    void restart(){
        clock_gettime(CLOCK_REALTIME, &start);
    }
    
    void stop(){
        clock_gettime(CLOCK_REALTIME, &end);
    }
    
    unsigned long long int duration(){
        return (end.tv_sec - start.tv_sec) * 1000000000LL + end.tv_nsec - start.tv_nsec;
    }
    
    void show(s::string const &str = "", s::ostream &oss = s::clog){
        if(!str.empty()){
            oss << str << ": ";
        }
        auto ss  = oss.precision(6);
        auto f = oss.setf(s::ios_base::fixed, s::ios_base::floatfield);
        oss << (long double)duration() / 1e6L << "ms" << s::endl;
        oss.precision(ss);
        oss.setf(f, s::ios_base::floatfield);
    }
};

#endif
