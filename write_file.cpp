#include "include.hpp"
#include "stopwatch.hpp"

#include "kml.hpp"
#include "html.hpp"

bool write_file(Option opt, s::vector<TrackData> data){
    // {
        // StopWatch sw;
        // write_kml(opt, data);
        // sw.stop();
        // sw.show("write_kml");
    // }
    
    {
        StopWatch sw;
        write_html(opt, data);
        sw.show("write_html");
    }
    
    // stub
    return true;
}
