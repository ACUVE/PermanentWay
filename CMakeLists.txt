cmake_minimum_required(VERSION 2.6)
project(PermanentWay)

set (CMAKE_CXX_FLAGS "-Wall")
set (CMAKE_CXX_FLAGS_DEBUG "-g")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(--std=c++11)
endif()

find_package(Boost COMPONENTS filesystem system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(permanentway main.cpp gpx.cpp kml.cpp option.cpp parse_file.cpp utility.cpp)
    target_link_libraries(permanentway m pthread ${Boost_LIBRARIES})
endif()
