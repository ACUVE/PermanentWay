PROG = permanentway

CC = gcc
CXX = g++
CFLAGS = -Wall
CPPFLAGS = $(CFLAGS) --std=c++1y
LINKFLAG = -lstdc++ -lboost_filesystem -lboost_system -lm -pthread
SRC = main.cpp gpx.cpp option.cpp parse_file.cpp kml.cpp utility.cpp
OBJ = $(SRC:.cpp=.o)

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LINKFLAG)
	
clean:
	rm -vf *.o $(PROG)

gpx.o: include.hpp gpx.hpp trackdata.hpp
main.o: include.hpp option.hpp parse_file.hpp kml.hpp
option.o: include.hpp option.hpp
parse_file.o: include.hpp parse_file.hpp make_unique.hpp gpx.hpp
utility.o: include.hpp utility.hpp
kml.o: include.hpp kml.hpp trackdata.hpp option.hpp
gpx.hpp: include.hpp trackdata.hpp
parse_file.hpp: include.hpp trackdata.hpp
kml.hpp: include.hpp option.hpp trackdata.hpp
utility.hpp: trackdata.hpp
