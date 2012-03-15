CXX=clang++
INTELIB=../intelib
CXXFLAGS=-O0 -g -W -Wall -Wno-unused-parameter -I$(INTELIB) -L$(INTELIB)/build/intelib
MODULES=reversi.o matrix.o gamefield.o
LIBRARIES=-lSDL -lSDL_image -lSDL_gfx -lintelib

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

reversi.cpp: gamefield.hpp

clean:
	rm -f reversi $(MODULES)
