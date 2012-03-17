CXX=g++
INTELIB=../intelib
CXXFLAGS=-O0 -g -std=c++0x -DDEBUG -W -Wall -Wno-unused-parameter -I$(INTELIB) -L$(INTELIB)/build/intelib
MODULES=reversi.o matrix.o gamefield.o player.o randbot.o
LIBRARIES=-lSDL -lSDL_gfx -lintelib

.PHONY: build clean default

default: build

build: reversi

run: build
	./reversi

reversi: $(MODULES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

$(MODULES): %.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

gamefield.hpp: matrix.hpp reversi.hpp

reversi.cpp: gamefield.hpp player.hpp randbot.hpp

player.hpp: gamefield.hpp

player.cpp: reversi.hpp

randbot.hpp: player.hpp gamefield.hpp

clean:
	rm -f reversi $(MODULES)
