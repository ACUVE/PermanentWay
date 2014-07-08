PROG = PermanentWay

CC = gcc
CXX = g++
CFLAGS = -Wall -O3
CPPFLAGS = $(CFLAGS) --std=c++1y
LINKFLAG = -lstdc++ -lboost_filesystem -lboost_system
SRC = main.cpp gpx.cpp
OBJ = $(SRC:.cpp=.o)

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LINKFLAG)
	
clean:
	rm -vf *.o $(PROG)

gpx.o: gpx.hpp include.hpp trackdata.hpp
main.o: make_unique.hpp include.hpp gpx.hpp trackdata.hpp
gpx.hpp: trackdata.hpp
