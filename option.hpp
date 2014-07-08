#ifndef ACUVE_OPTION_HPP
#define ACUVE_OPTION_HPP

#include "include.hpp"

struct Option{
    struct File{
        s::string filename;
    };
    
    File global;
    s::vector<File> files;
};

b::optional<Option> parse_option(int argc, char **argv);

#endif
