CXX=clang++
CXXFLAGS=-W -Wall -O0 -g

reversi: reversi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lSDL -lSDL_image
