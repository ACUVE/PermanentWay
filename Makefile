PROG = permanentway

CC = gcc
CXX = g++
CFLAGS = -Wall -O3
CPPFLAGS = $(CFLAGS) --std=c++1y
LINKFLAG = -lstdc++ -lboost_filesystem -lboost_system
SRC = main.cpp gpx.cpp option.cpp parse_file.cpp
OBJ = $(SRC:.cpp=.o)

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LINKFLAG)
	
clean:
	rm -vf *.o $(PROG)

gpx.o: include.hpp gpx.hpp trackdata.hpp
main.o: include.hpp option.hpp parse_file.hpp
option.o: include.hpp option.hpp
parse_file.o: include.hpp parse_file.hpp make_unique.hpp gpx.hpp
gpx.hpp: include.hpp trackdata.hpp
parse_file.hpp: include.hpp trackdata.hpp
