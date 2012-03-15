CXX=clang++
INTELIB=../intelib
CXXFLAGS=-W -Wall -Wno-unused-parameter -O0 -g -I$(INTELIB) -L$(INTELIB)/build/intelib
MODULES=matrix.o gamefield.o
LIBRARIES=-lSDL -lSDL_image -lintelib

.PHONY: build clean default

default: build

build: reversi

reversi: reversi.cpp $(MODULES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

$(MODULES): %.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

gamefield.hpp: matrix.hpp

clean:
	rm -f reversi $(MODULES)
